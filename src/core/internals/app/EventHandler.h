#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>

enum class EventType
{
	EVENT_NONE = 0,
	ERROR_GOT, // used in various classes.
	APP_PROCESS_START, // used in Application
	APP_PROCESS_HALT, // used in Application
	APP_WINDOW_GL_INIT, // used in Application
	APP_WINDOW_SET_CONTEXT, // used in Application
	APP_WINDOW_MOUSE_CLICK,
	APP_WINDOW_KEY_PRESS,
	APP_WINDOW_MOUSE_MOVE,
	APP_WINDOW_FILES_DRAGGED,
	ASSET_LOADED, // used in AssetManager
	ASSET_DESTROYED, // used in AssetManager
	ASSET_SHADER_LOAD, // used in AssetManager
	ENTITY_TRANSFORM_UPDATED, // used in Components
	MESHCP_MESH_ADDED,
	MESHCP_MAT_ADDED,
};

class EventArgs {
public:
	virtual ~EventArgs() = default;
}; // class to inherit from to make event argument classes.

class ErrorEventArgs : public EventArgs {
public:
	std::string message;
};

static class EventHandler
{
public:
	// Takes callback function, and then sets it to call when a certain event type's event is fired.
	static void SetEventCallback(EventType etType, std::function<void(EventArgs*)> callback);
	static void FireEvent(EventType etType, std::shared_ptr<EventArgs> args);
	static void CreateThread();
	static void JoinThread();
	static void WaitUntilFinished();
	static void Destruct();

private:
	struct QueuedEvent
	{
		EventType etType;
		std::shared_ptr<EventArgs> args;
	};

	static std::map<EventType, std::vector<std::function<void(EventArgs*)>>> eventCallbacks;
	static std::queue<QueuedEvent> eventQueue;
	static std::thread processThread;
	static std::mutex mutex;
	static bool shouldTerminate;

	static void ProcessEvents();
};