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
    void initialize(Config *config, Systems *systems);

    template <typename T>
    void executeRoutine(IBTNode<T> &rootNode, T ctx)
    {
        fsrPhone->read();
        if (fsrPhone->getValue() > m_config->getFSRPhoneThreshold())
        {
            // TODO
        }

        if (status != BTNodeStatus::Running)
        {
            rootNode.initialize(ctx);
        }

        status = rootNode.tick(ctx);
    }

    bool isPhoneSet();

public:
    struct Events
    {
        // TODO
    } events;

private:
    void registerEvents();
    void doHelloWorld();

private:
    // These will live until the lifetime of the Robot
    Config *m_config;
    Systems *m_systems;

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

    BTNodeStatus status{BTNodeStatus::Success};
};

#endif