#include "Controller.h"

void Controller::setupBehaviors()
{
    // ##### ACTIVE STATE BEHAVIOR ##### //
    auto conNode_CheckPhoneFSR = std::unique_ptr<BTConditionNode<BehaviorContext *>>{new BTConditionNode<BehaviorContext *>(
        [&](BehaviorContext *ctx)
        {
            return robot.isPhoneSet();
        })};

    auto leafNode_SetActive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            if (!ctx->isActive)
            {
                // TEST - Do hello animation
                systems.logger->logn("leafNode_SetActive", "Doing hello animation TEST");
                robot.leftArm->rotate(140, 40, 70);
                robot.rightArm->rotate(130, 40, 170);
                delay(1000);
            }

            ctx->isActive = true;
            return BTNodeStatus::Success;
        })};
    auto leafNode_LogEndOfSequence = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            return BTNodeStatus::Success;
        })};
    auto seqNode_MainSequence = std::unique_ptr<BTSequenceNode<BehaviorContext *>>{new BTSequenceNode<BehaviorContext *>()};

    seqNode_MainSequence->addChildNode(std::move(leafNode_SetActive));
    seqNode_MainSequence->addChildNode(std::move(leafNode_LogEndOfSequence));
    conNode_CheckPhoneFSR->setChildNode(std::move(seqNode_MainSequence));

    // ##### STATE TRANSITION BEHAVIOR ##### //
    auto leafNode_CheckActive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            if (ctx->isActive)
            {
                ctx->isActive = false;
                return BTNodeStatus::Fail;
            }
            return BTNodeStatus::Success;
        })};

    // ##### INACTIVE TRANSITION BEHAVIOR ##### //
    auto leafNode_ResetArms = std::unique_ptr<ResetRobotPositionNode>{new ResetRobotPositionNode(1000, &systems.timer, &robot)};

    auto leafNode_EmitInactive = std::unique_ptr<BTLeafNode<BehaviorContext *>>{new BTLeafNode<BehaviorContext *>(
        [&](BehaviorContext *ctx) {},
        [&](BehaviorContext *ctx)
        {
            systems.logger->logn("leafNode_EmitInactive", "IS INACTIVE");
            return BTNodeStatus::Success;
        })};

    auto seqNode_MainInactiveSequence = std::unique_ptr<BTSequenceNode<BehaviorContext *>>{new BTSequenceNode<BehaviorContext *>()};

    seqNode_MainInactiveSequence->addChildNode(std::move(leafNode_ResetArms));
    seqNode_MainInactiveSequence->addChildNode(std::move(leafNode_EmitInactive));

    // ##### REGISTER TO ROOT ##### //
    auto selNode_RootSelector = std::unique_ptr<BTSelectorNode<BehaviorContext *>>{new BTSelectorNode<BehaviorContext *>()};
    selNode_RootSelector->addChildNode(std::move(conNode_CheckPhoneFSR));
    selNode_RootSelector->addChildNode(std::move(leafNode_CheckActive));
    selNode_RootSelector->addChildNode(std::move(seqNode_MainInactiveSequence));

    rootNode.addChildNode(std::move(selNode_RootSelector));
}

ResetRobotPositionNode::ResetRobotPositionNode(
    std::uint16_t totalDurationMs,
    Timer *timer,
    Robot *robot)
    : m_timer{timer},
      m_animationLeftArm{totalDurationMs, robot->leftArm, Arm::Angles<std::int32_t>{115, 15, 35}},
      m_animationRightArm{totalDurationMs, robot->rightArm, Arm::Angles<std::int32_t>{45, 135, 125}},
      BTLeafNode{
          [&](BehaviorContext *ctx)
          {
              m_animationLeftArm.reset();
              m_animationRightArm.reset();
          },
          [&](BehaviorContext *ctx)
          {
              auto r1 = m_animationLeftArm.animate(m_timer->getElapsedMilliseconds());
              auto r2 = m_animationRightArm.animate(m_timer->getElapsedMilliseconds());

              return r1 && r2 ? BTNodeStatus::Success
                              : BTNodeStatus::Running;
          }}
{
}