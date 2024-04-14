#ifndef TELEBYTE_CONTROLLER
#define TELEBYTE_CONTROLLER

#include "all.h"
#include "Systems.h"
#include "Robot.h"
#include "BehaviorTree.h"
#include "Animations.h"
#include "Arm.h"

struct BehaviorContext
{
    bool isActive{false};
};

class Controller
{
public:
    void setup();
    void execute(std::uint16_t delayMs);

private:
    void setupAPIEndpoints();
    void setupBehaviors();
    void handleDebugCommand(CLIInput &command);

private:
    Config config{};
    Systems systems{};
    Robot robot{};
    BehaviorContext ctx;
    BTSequenceNode<BehaviorContext *> rootNode{};
};

// ##### Custom leaf node definitions ##### //

class RobotHelloWorldNode : public BTLeafNode<BehaviorContext *>
{
public:
    RobotHelloWorldNode(Timer *timer, Robot *robot);

private:
    Timer *m_timer;

    // ----- Animation Sequences ----- //
    // To position
    AnimationRotateArm m_animationLeftArmReady;
    AnimationRotateArm m_animationRightArmReady;
    // Wave hand
    AnimationRotateArm m_animationLeftArmWave;
    AnimationRotateArm m_animationRightArmWave;
};

class RobotResetPositionNode : public BTLeafNode<BehaviorContext *>
{
public:
    RobotResetPositionNode(std::uint16_t totalDurationMs, Timer *timer, Robot *robot);

private:
    Timer *m_timer;
    AnimationRotateArm m_animationLeftArm;
    AnimationRotateArm m_animationRightArm;
};

#endif