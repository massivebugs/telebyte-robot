#include "Robot.h"

void Robot::initialize(Config *config, Systems *systems)
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
