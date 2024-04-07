#include "Robot.h"

void Robot::initialize(Config *config, Systems *systems)
{
    m_config = config;
    m_systems = systems;

    auto leftShoulder = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCALeftShoulderPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};
    auto leftElbow = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCALeftElbowPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};
    auto leftWrist = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCALeftWristPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};
    auto rightShoulder = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCARightShoulderPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};
    auto rightElbow = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCARightElbowPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};
    auto rightWrist = Servo<Adafruit_PWMServoDriver>{&systems->pwmDriver, config->getPCARightWristPin(), config->getSG90PWMMin(), config->getSG90PWMMax()};

    leftArm = std::unique_ptr<Arm>(new Arm(Arm::Side::LEFT, leftShoulder, leftElbow, leftWrist));
    rightArm = std::unique_ptr<Arm>(new Arm(Arm::Side::RIGHT, rightShoulder, rightElbow, rightWrist));

    fsrPhone = std::unique_ptr<FSR>(new FSR(config->getFSRPhonePin()));

    m_systems->logger->logn("Robot", "Starting initialization!");

    m_systems->logger->logn("Robot", "Registering events...");
    registerEvents();

    m_systems->logger->logn("Robot", "Initialization finished!");
}

void Robot::registerEvents()
{
    m_systems->events.helloWorld.subscribe([&](void *)
                                           { doHelloWorld(); });
}

void Robot::doHelloWorld()
{
    m_systems->logger->logn("Robot", "Hello, world!");
}