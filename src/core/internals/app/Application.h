#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include <iostream>
#include <memory>
#include "EventHandler.h"
#include "vector/Vector2.h"
#include "AssetManager.h"
#include "graphics/Renderer.h"
#include "graphics/Material.h"
#include "ecs/ecs.h"
#include "graphics/Vertex.h"
#include "Components.h"
#include "ImGuiInterface.h"
#include "input/input.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EventArgs;

class Application
{
public:
	void Run(); // begins the main loop. Also fires the APP_PROCESS_START event.
	void Halt(int nExitcode = 0); // sends an interrupt to halt the program, optionally, with a specified exit code. Also fires the APP_PROCESS_HALT event.
	int Init(bool bInitGL = true); // Inits GLFW, and optionally glad(which can be later initialized with InitGL.)
	int InitGL(); // initializes GLAD. Fires the APP_WINDOW_GL_INIT event.
	Window window;
private:
	bool shouldHalt = false; // flag for the Halt function. It marks the shouldHalt as true, fires the event, and then the application should exit.
	bool glInitialized = false; // flag that tells if glad was already initialized or not.
	void DoGLFWLoop();
};

class AppStateEventArgs : public EventArgs {
public:
	Application* app;
};
#endif