#ifndef TELEBYTE_ROBOT
#define TELEBYTE_ROBOT

#include "all.h"
#include "Systems.h"
#include "Arm.h"
#include "Servo.h"
#include "FSR.h"
#include "BehaviorTree.h"

class Robot
{
public:
    Robot(Systems *systems) : systems{systems},
                              leftShoulder{&systems->pwmDriver, systems->config.pcaUL1, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              leftElbow{&systems->pwmDriver, systems->config.pcaUL2, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              leftWrist{&systems->pwmDriver, systems->config.pcaUL3, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              rightShoulder{&systems->pwmDriver, systems->config.pcaUR1, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              rightElbow{&systems->pwmDriver, systems->config.pcaUR2, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              rightWrist{&systems->pwmDriver, systems->config.pcaUR3, systems->config.sg90PWMMin, systems->config.sg90PWMMax},
                              leftArm{Arm::Side::LEFT, &leftShoulder, &leftElbow, &leftWrist},
                              rightArm{Arm::Side::RIGHT, &rightShoulder, &rightElbow, &rightWrist},
                              fsrPhone{systems->config.fsrPhone}
    {
    }

    void initialize()
    {
        systems->logger.logn("Robot", "Starting initialization!");
        systems->logger.logn("Robot", "Registering events...");

        systems->events.helloWorld.subscribe([&](void *)
                                             { doHello(); });

        events.baz.subscribe([&](void *)
                             { systems->logger.logn("Robot", "bazbaz!"); });

        systems->logger.logn("Robot", "Registering behaviors...");

        // TODO: Implement control flow and behaviors
        // sequenceNode1.addChildNode(&foo1);
        // sequenceNode1.addChildNode(&foo2);
        // sequenceNode2.addChildNode(&bar1);
        // sequenceNode2.addChildNode(&bar2);
        // rootNode.addChildNode(&sequenceNode1);
        // rootNode.addChildNode(&sequenceNode2);

        systems->logger.logn("Robot", "Initialization finished!");
    }

    void executeRoutine()
    {
        fsrPhone.read();
        if (fsrPhone.getValue() > systems->config.fsrPhoneThreshold)
        {
            events.baz.publish(nullptr);
        }

        if (status != BTNodeStatus::Running)
        {
            rootNode.initialize(this);
        }

        status = rootNode.tick(this);
    }

    void doHello()
    {
        systems->logger.logn("Robot", "Hello, world!");
    }

    void logFoo()
    {
        systems->logger.logn("Robot", "Foo!");
    }

    void logBar()
    {
        systems->logger.logn("Robot", "Bar!");
    }

    void rotateLeftArm(std::uint16_t shoulderAngle, std::uint16_t elbowAngle, std::uint16_t wristAngle)
    {
        leftArm.rotate(shoulderAngle, elbowAngle, wristAngle);
    }

    void rotateRightArm(std::uint16_t shoulderAngle, std::uint16_t elbowAngle, std::uint16_t wristAngle)
    {
        rightArm.rotate(shoulderAngle, elbowAngle, wristAngle);
    }

public:
    bool getIsPhoneSet() const
    {
        return isPhoneSet;
    }

private:
    bool isPhoneSet{false};

private:
    Systems *systems;
    BTSequenceNode<Robot *> rootNode{};
    BTSequenceNode<Robot *> sequenceNode1{};
    BTSequenceNode<Robot *> sequenceNode2{};
    BTLeafNode<Robot *> foo1{
        [&](Robot *robot)
        {
            // robot->logFoo();
        },
        [&](Robot *robot)
        {
            systems->logger.logn("Robot", "Foo 1");
            return BTNodeStatus::Success;
        },
    };
    BTLeafNode<Robot *> foo2{
        [&](Robot *robot)
        {
            // robot->logFoo();
        },
        [&](Robot *robot)
        {
            systems->logger.logn("Robot", "Foo 2");
            return BTNodeStatus::Success;
        },
    };
    BTLeafNode<Robot *> bar1{
        [&](Robot *robot)
        {
            // robot->logFoo();
        },
        [&](Robot *robot)
        {
            systems->logger.logn("Robot", "Bar 1");
            return BTNodeStatus::Success;
        },
    };
    BTLeafNode<Robot *> bar2{
        [&](Robot *robot)
        {
            // robot->logFoo();
        },
        [&](Robot *robot)
        {
            systems->logger.logn("Robot", "Bar 2");
            return BTNodeStatus::Success;
        },
    };
    BTNodeStatus status{BTNodeStatus::Success};

    // Body parts
    Arm leftArm;
    Arm rightArm;

    // Sensors
    // Phone mount FSR
    FSR fsrPhone;

    // Actuators
    Servo<Adafruit_PWMServoDriver> leftShoulder;
    Servo<Adafruit_PWMServoDriver> leftElbow;
    Servo<Adafruit_PWMServoDriver> leftWrist;
    Servo<Adafruit_PWMServoDriver> rightShoulder;
    Servo<Adafruit_PWMServoDriver> rightElbow;
    Servo<Adafruit_PWMServoDriver> rightWrist;

    struct SystemEvents
    {
        Event<void> baz;
    } events;
};

#endif