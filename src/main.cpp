#include "all.h"
#include "Controller.h"

constexpr std::uint16_t LOOP_DELAY = 20;
Controller controller{};

void setup(void)
{
    controller.setup();
}

void loop(void)
{
    controller.execute(LOOP_DELAY);
}
