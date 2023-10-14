#include "EventHandler.h"

std::map<EventType, std::vector<std::function<void(EventArgs*)>>> EventHandler::eventCallbacks;
std::queue<EventHandler::QueuedEvent> EventHandler::eventQueue;
std::thread EventHandler::processThread;
std::mutex EventHandler::mutex;
bool EventHandler::shouldTerminate;


void EventHandler::JoinThread()
{
	processThread.join();
}

void EventHandler::SetEventCallback(EventType etType, std::function<void(EventArgs*)> callback)
{
    auto it = eventCallbacks.find(etType);
    if (it != eventCallbacks.end())
    {
        it->second.push_back(callback);
    }
    else
    {
        eventCallbacks.insert({ etType, std::vector< std::function<void(EventArgs*)>>() });
        eventCallbacks[etType].push_back(callback);
        //std::cout << eventCallbacks[etType].empty() << std::endl;
    }
}

void EventHandler::FireEvent(EventType etType, std::shared_ptr<EventArgs> args)
{
    std::lock_guard<std::mutex> lock(mutex);
    eventQueue.push({ etType, args });
}

void EventHandler::WaitUntilFinished()
{
    while (true)
    {
        bool eventsPending;
        {
            std::lock_guard<std::mutex> lock(mutex);
            eventsPending = !eventQueue.empty();
        }

        if (!eventsPending)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void EventHandler::ProcessEvents()
{  
    while (!shouldTerminate)
    {
        {
            //std::cerr << "PROCESSING EVENTS." << std::endl;
            QueuedEvent event;
            event.etType = EventType::EVENT_NONE;
            event.args = nullptr;

            {
                std::lock_guard<std::mutex> lock(mutex);
                if (!eventQueue.empty()) {
                    //std::cerr << "EVENT Q CHK" << std::endl;
                    event = eventQueue.front();
                    eventQueue.pop();
                }
            }

            //std::cerr << &event << std::endl;

            for (const auto& callback : eventCallbacks[event.etType])
            {
                //std::cerr << "CALLBKS" << std::endl;
                //std::cerr << &callback << std::endl;
                callback(event.args.get());
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // sleep.
        }
    }
}

void EventHandler::CreateThread()
{
    processThread = std::thread(&EventHandler::ProcessEvents);
}

void EventHandler::Destruct()
{
    shouldTerminate = true;
    processThread.join();
}

