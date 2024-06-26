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

    // Body parts
    std::shared_ptr<Arm> leftArm;
    std::shared_ptr<Arm> rightArm;

    // Phone mount FSR
    std::unique_ptr<FSR> fsrPhone;

private:
    void registerEvents();
    void doHelloWorld();

private:
    // These will live until the lifetime of the Robot
    Config *m_config;
    Systems *m_systems;

    BTNodeStatus status{BTNodeStatus::Success};
};

#endif