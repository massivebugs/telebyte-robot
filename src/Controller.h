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
    Timer *timer;
    bool isActive{0};

    // for ConditionalDelay:PhoneFSRTimeThreshold
    long long fsrThresholdPassedMs{0};
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
class SequenceDelayNode : public BTLeafNode<BehaviorContext *>
{
public:
    SequenceDelayNode(long long seconds);

private:
    long long m_lengthMs;
    long long m_initMs{0};
};

class RobotHelloWorldNode : public BTLeafNode<BehaviorContext *>
{
public:
    RobotHelloWorldNode(Robot *robot);

private:
    // ----- Animation Sequences ----- //
    // To position
    AnimationRotateArm m_animationLeftArmReady;
    AnimationRotateArm m_animationRightArmReady;
    // Wave hand
    AnimationRotateArm m_animationLeftArmWave;
    AnimationRotateArm m_animationRightArmWave;
};

class RobotResetPostureNode : public BTLeafNode<BehaviorContext *>
{
public:
    RobotResetPostureNode(std::uint16_t totalDurationMs, Robot *robot);

private:
    AnimationRotateArm m_animationLeftArm;
    AnimationRotateArm m_animationRightArm;
};

#endif