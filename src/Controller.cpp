#include "Controller.h"

void Controller::setup()
{
    config.initialize();
    systems.initialize(config);
    robot.initialize(&config, &systems);

    setupAPIEndpoints();
    setupBehaviors();

    // Set system timer in Behavior Context
    ctx.timer = &systems.timer;

    // Reset to robot's initial position
    robot.leftArm->rotate(115, 15, 35);
    robot.rightArm->rotate(45, 135, 125);
}

void Controller::execute(std::uint16_t delayMs = 100)
{
    if (config.getDebug() && Serial.available() > 0)
    {
        auto command = Serial.readString();
        auto result = systems.cli.parse(command.c_str());
        handleDebugCommand(result);
    }

    robot.executeRoutine(rootNode, &ctx);

    delay(delayMs);
}

void Controller::handleDebugCommand(CLIInput &command)
{
    // Register command handlers here
    if (command.name == "ping")
    {
        systems.logger->logn("Controller", "ping");
        for (auto arg : command.args)
        {
            systems.logger->logn("Controller", "ping arg: " + arg);
        }
    }
    else if (command.name == "hello_world")
    {
        systems.events.helloWorld.publish(nullptr);
    }
    else if (command.name == "servo")
    {
        auto servo = command.args[0];
        auto val = command.args[1];
        int valn = std::stoi(val);

        if (servo == "leftShoulder")
        {
            robot.leftArm->rotateShoulder(valn);
        }
        if (servo == "leftElbow")
        {
            robot.leftArm->rotateElbow(valn);
        }
        if (servo == "leftWrist")
        {
            robot.leftArm->rotateWrist(valn);
        }
        if (servo == "rightShoulder")
        {
            robot.rightArm->rotateShoulder(valn);
        }
        if (servo == "rightElbow")
        {
            robot.rightArm->rotateElbow(valn);
        }
        if (servo == "rightWrist")
        {
            robot.rightArm->rotateWrist(valn);
        }
    }
}

void Controller::setupAPIEndpoints()
{
    // Ping
    systems.webServer->GET("/api/ping", [&](AsyncWebServerRequest *request)
                           {
                                String message = "OK";
                                if (request->hasArg("msg")) {
                                    message = request->arg("msg");
                                }

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"pong\":\"" + message + "\"}"); });

    // Command
    systems.webServer->POST("/api/command", [&](AsyncWebServerRequest *request, JsonVariant &json)
                            { 
                                std::string command = json["c"];
                                auto result = systems.cli.parse(command.c_str());

                                handleDebugCommand(result);

                                request->send(200, WebServer::CONTENT_TYPE_JSON, "{\"status\":\"OK\"}"); });

    // List config
    systems.webServer->GET("/api/config", [&](AsyncWebServerRequest *request)
                           { request->send(200, WebServer::CONTENT_TYPE_JSON, config.toJSON().c_str()); });
}

void Controller::setupBehaviors()
{
    // Selector:RootSelector
    auto selNode_RootSelector = std::unique_ptr<BTSelectorNode<BehaviorContext *>>{new BTSelectorNode<BehaviorContext *>()};
    // Condition:CheckPhoneFSR
    auto conNode_CheckPhoneFSR = std::unique_ptr<BTConditionNode<BehaviorContext *>>{new BTConditionNode<BehaviorContext *>(
        [&](BehaviorContext *ctx)
        {
            return robot.isPhoneSet();
        })};
    // Condition:PhoneFSRTimeThreshold
    auto conDelayNode_PhoneFSRTimeThreshold = std::unique_ptr<BTConditionalDelayNode<BehaviorContext *>>{new BTConditionalDelayNode<BehaviorContext *>(
        [&](BehaviorContext *ctx)
        {
            ctx->fsrThresholdPassedMs = ctx->timer->getElapsedMilliseconds();
        },
        [&](BehaviorContext *ctx)
        {
            if (!ctx->isActive)
            {
                systems.logger->logn("conNode_PhoneFSRTimeThreshold", "Waiting for FSR time threshold...");
                return ctx->timer->getElapsedMilliseconds() - ctx->fsrThresholdPassedMs > 2000;
            }
            return true;
        })};
    // Selector:InitializerSelector
    auto selNode_InitializerSelector = std::unique_ptr<BTSelectorNode<BehaviorContext *>>{new BTSelectorNode<BehaviorContext *>()};
    // Condition:IsInactive
    auto conNode_IsInactive = std::unique_ptr<BTConditionNode<BehaviorContext *>>{new BTConditionNode<BehaviorContext *>(
        [&](BehaviorContext *ctx)
        {
            return !ctx->isActive;
        })};
    // Sequence:StateTransition
    auto seqNode_StateTransitionSequence = std::unique_ptr<BTSequenceNode<BehaviorContext *>>{new BTSequenceNode<BehaviorContext *>()};
    // Leaf:SetActive
    auto leafNode_SetActive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            if (!ctx->isActive)
            {
                systems.logger->logn("leafNode_SetActive", "Setting to ACTIVE");
                ctx->isActive = true;
                return BTNodeStatus::Success;
            }
            return BTNodeStatus::Fail;
        })};
    // Leaf:DoHelloWorld
    auto leafNode_DoHelloWorld = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new RobotHelloWorldNode(&robot)};
    // Sequence:MainSequence
    auto seqNode_MainSequence = std::unique_ptr<BTSequenceNode<BehaviorContext *>>{new BTSequenceNode<BehaviorContext *>()};
    // Leaf:LofEndOfSequence
    auto leafNode_LogEndOfSequence = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            systems.logger->logn("leafNode_LogEndOfSequence", "End of main sequence");
            return BTNodeStatus::Success;
        })};
    // Leaf:SetInactive
    auto leafNode_SetInactive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            if (ctx->isActive)
            {
                systems.logger->logn("leafNode_SetInactive", "Setting to INACTIVE");
                ctx->isActive = false;
                return BTNodeStatus::Success;
            }
            return BTNodeStatus::Fail;
        })};
    // Sequence:MainInactiveSequence
    auto seqNode_MainInactiveSequence = std::unique_ptr<BTSequenceNode<BehaviorContext *>>{new BTSequenceNode<BehaviorContext *>()};
    // Leaf:ResetPosture
    auto leafNode_ResetPosture = std::unique_ptr<RobotResetPostureNode>{new RobotResetPostureNode(1200, &robot)};
    // Leaf:EmitEventInactive
    auto leafNode_EmitInactive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            systems.logger->logn("leafNode_EmitInactive", "IS INACTIVE");
            return BTNodeStatus::Success;
        })};

    seqNode_StateTransitionSequence->addChildNode(std::move(leafNode_SetActive));
    seqNode_StateTransitionSequence->addChildNode(std::move(leafNode_DoHelloWorld));
    conNode_IsInactive->setChildNode(std::move(seqNode_StateTransitionSequence));
    seqNode_MainSequence->addChildNode(std::move(leafNode_LogEndOfSequence));
    selNode_InitializerSelector->addChildNode(std::move(conNode_IsInactive));
    selNode_InitializerSelector->addChildNode(std::move(seqNode_MainSequence));
    conDelayNode_PhoneFSRTimeThreshold->setChildNode(std::move(selNode_InitializerSelector));
    conNode_CheckPhoneFSR->setChildNode(std::move(conDelayNode_PhoneFSRTimeThreshold));
    seqNode_MainInactiveSequence->addChildNode(std::move(leafNode_ResetPosture));
    seqNode_MainInactiveSequence->addChildNode(std::move(leafNode_EmitInactive));
    selNode_RootSelector->addChildNode(std::move(conNode_CheckPhoneFSR));
    selNode_RootSelector->addChildNode(std::move(leafNode_SetInactive));
    selNode_RootSelector->addChildNode(std::move(seqNode_MainInactiveSequence));
    rootNode.addChildNode(std::move(selNode_RootSelector));
}

// ##### Custom leaf node implementations ##### //

SequenceDelayNode::SequenceDelayNode(long long seconds)
    : m_lengthMs{seconds},
      BTLeafNode{
          [&](BehaviorContext *ctx)
          {
              m_initMs = 0;
          },
          [&](BehaviorContext *ctx)
          {
              auto elapsedMs = ctx->timer->getElapsedMilliseconds();
              if (m_initMs == 0)
              {
                  m_initMs = elapsedMs;
              }

              return elapsedMs - m_initMs > m_lengthMs
                         ? BTNodeStatus::Success
                         : BTNodeStatus::Running;
          },
      }
{
}

RobotHelloWorldNode::RobotHelloWorldNode(
    Robot *robot)
    : m_animationLeftArmReady{1000, robot->leftArm, Arm::Angles<std::int32_t>{140, 40, 70}},
      m_animationRightArmReady{1000, robot->rightArm, Arm::Angles<std::int32_t>{130, 40, 170}},
      m_animationLeftArmWave{1500, robot->leftArm, Arm::Angles<std::int32_t>{128, 28, 53}},
      m_animationRightArmWave{500, robot->rightArm, Arm::Angles<std::int32_t>{115, 20, 55}, std::uint8_t(2), true},
      BTLeafNode{
          [&](BehaviorContext *ctx)
          {
              m_animationLeftArmReady.reset();
              m_animationRightArmReady.reset();
              m_animationLeftArmWave.reset();
              m_animationRightArmWave.reset();
          },
          [&](BehaviorContext *ctx)
          {
              // ----- Perform sequence ----- //
              auto r1 = m_animationLeftArmReady.animate(ctx->timer->getElapsedMilliseconds());
              auto r2 = m_animationRightArmReady.animate(ctx->timer->getElapsedMilliseconds());

              if (!(r1 && r2))
              {
                  return BTNodeStatus::Running;
              }

              auto r3 = m_animationLeftArmWave.animate(ctx->timer->getElapsedMilliseconds());
              auto r4 = m_animationRightArmWave.animate(ctx->timer->getElapsedMilliseconds());

              return r3 && r4 ? BTNodeStatus::Success
                              : BTNodeStatus::Running;
          }}
{
}

RobotResetPostureNode::RobotResetPostureNode(
    std::uint16_t totalDurationMs,
    Robot *robot)
    : m_animationLeftArm{totalDurationMs, robot->leftArm, Arm::Angles<std::int32_t>{115, 15, 35}},
      m_animationRightArm{totalDurationMs, robot->rightArm, Arm::Angles<std::int32_t>{45, 135, 125}},
      BTLeafNode{
          [&](BehaviorContext *ctx)
          {
              m_animationLeftArm.reset();
              m_animationRightArm.reset();
          },
          [&](BehaviorContext *ctx)
          {
              auto r1 = m_animationLeftArm.animate(ctx->timer->getElapsedMilliseconds());
              auto r2 = m_animationRightArm.animate(ctx->timer->getElapsedMilliseconds());

              return r1 && r2 ? BTNodeStatus::Success
                              : BTNodeStatus::Running;
          }}
{
}