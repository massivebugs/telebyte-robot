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
    void initialize(Config *config, Systems *systems)
    {
        m_config = config;
        m_systems = systems;

        leftShoulder = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUL1Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));
        leftElbow = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUL2Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));
        leftWrist = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUL3Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));
        rightShoulder = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUR1Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));
        rightElbow = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUR2Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));
        rightWrist = std::shared_ptr<Servo<Adafruit_PWMServoDriver>>(new Servo<Adafruit_PWMServoDriver>(&systems->pwmDriver, config->getPCAUR3Pin(), config->getSG90PWMMin(), config->getSG90PWMMax()));

        leftArm = std::unique_ptr<Arm>(new Arm(Arm::Side::LEFT, leftShoulder, leftElbow, leftWrist));
        rightArm = std::unique_ptr<Arm>(new Arm(Arm::Side::RIGHT, rightShoulder, rightElbow, rightWrist));

        fsrPhone = std::unique_ptr<FSR>(new FSR(config->getFSRPhonePin()));

        m_systems->logger->logn("Robot", "Starting initialization!");
        m_systems->logger->logn("Robot", "Registering events...");

        m_systems->events.helloWorld.subscribe([&](void *)
                                               { doHello(); });

        events.baz.subscribe([&](void *)
                             { m_systems->logger->logn("Robot", "bazbaz!"); });

        m_systems->logger->logn("Robot", "Registering behaviors...");

        // TODO: Implement control flow and behaviors
        // sequenceNode1.addChildNode(&foo1);
        // sequenceNode1.addChildNode(&foo2);
        // sequenceNode2.addChildNode(&bar1);
        // sequenceNode2.addChildNode(&bar2);
        // rootNode.addChildNode(&sequenceNode1);
        // rootNode.addChildNode(&sequenceNode2);

        m_systems->logger->logn("Robot", "Initialization finished!");
    }

    void executeRoutine()
    {
        fsrPhone->read();
        if (fsrPhone->getValue() > m_config->getFSRPhoneThreshold())
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
        m_systems->logger->logn("Robot", "Hello, world!");
    }

    void logFoo()
    {
        m_systems->logger->logn("Robot", "Foo!");
    }

    void logBar()
    {
        m_systems->logger->logn("Robot", "Bar!");
    }

    void rotateLeftArm(std::uint16_t shoulderAngle, std::uint16_t elbowAngle, std::uint16_t wristAngle)
    {
        leftArm->rotate(shoulderAngle, elbowAngle, wristAngle);
    }

    void rotateRightArm(std::uint16_t shoulderAngle, std::uint16_t elbowAngle, std::uint16_t wristAngle)
    {
        rightArm->rotate(shoulderAngle, elbowAngle, wristAngle);
    }

public:
    bool getIsPhoneSet() const
    {
        return isPhoneSet;
    }

private:
    bool isPhoneSet{false};

private:
    // These will live until the lifetime of the Robot
    Config *m_config;
    Systems *m_systems;

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
            m_systems->logger->logn("Robot", "Foo 1");
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
            m_systems->logger->logn("Robot", "Foo 2");
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
            m_systems->logger->logn("Robot", "Bar 1");
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
            m_systems->logger->logn("Robot", "Bar 2");
            return BTNodeStatus::Success;
        },
    };
    BTNodeStatus status{BTNodeStatus::Success};

    // Body parts
    std::unique_ptr<Arm> leftArm;
    std::unique_ptr<Arm> rightArm;

    // Phone mount FSR
    std::unique_ptr<FSR> fsrPhone;

    // Actuators
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> leftShoulder;
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> leftElbow;
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> leftWrist;
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> rightShoulder;
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> rightElbow;
    std::shared_ptr<Servo<Adafruit_PWMServoDriver>> rightWrist;

    struct SystemEvents
    {
        Event<void> baz;
    } events;
};

#endif