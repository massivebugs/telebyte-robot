#ifndef UTILS_EVENTS
#define UTILS_EVENTS

#include <functional>
#include <vector>

/**
 * @file Events.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief A very simple Event implementation.
 *
 * This class provides functionality for subscribing to events with handlers
 * and publishing events with associated data.
 *
 * Usage:
 * struct AllEvents
 * {
 *     Event<InputData> onInput;
 *     Event<FooSensorData> onFooSensorRecv;
 * } events;
 *
 * // Subscribe to event
 * events.onInput.subscribe(
 *     [&](InputData *data){
 *         std::cout << "Event received: " << data->value << std::endl;
 *     }
 * );
 *
 * // Publish event
 * auto result = getInputData();
 * events.onInput.publish(InputData{result});
 *
 * @tparam T The type of data associated with the event.
 */
template <typename T>
class Event
{
public:
    /**
     * @brief Subscribes a handler function to the event.
     * @param handler The handler function to subscribe.
     */
    void subscribe(std::function<void(T *)> handler)
    {
        eventHandlers.push_back(handler);
    }

    /**
     * @brief Publishes an event with associated data.
     *
     * Invokes all subscribed handler functions with the provided data.
     *
     * @param data The data associated with the event.
     */
    void publish(T *data)
    {
        for (const auto &handler : eventHandlers)
        {
            handler(data);
        }
    }

private:
    std::vector<std::function<void(T *)>> eventHandlers;
};

#endif