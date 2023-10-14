#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector/Vector2.h"
#include "internals/app/EventHandler.h"

class KeyEventArgs : public EventArgs {
public: GLFWwindow* window; int key, scancode, action, mods;
};

class MouseEventArgs : public EventArgs
{
public: GLFWwindow* window; Vector2 Position; int Button, Action, Mods;
};

class DropEventArgs : public EventArgs
{
public:
	int count;
	const char** paths;
	GLFWwindow* window;
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drop_callback(GLFWwindow* window, int count, const char** paths);
void resize_callback(GLFWwindow* window, int width, int height);

class Window
{
private:
	GLFWwindow* wHnd;
public:
	Vector2 Size{ 800, 600 };
	Vector2 Position{ 0,0 };

	std::string Title = "Untitled Tennessine Window";

	void SetTitle(std::string title);
	void SetSize(Vector2 size);
	void MakeContextCurrent();

	GLFWwindow* GetWindowHandle();
	void Init(std::string title, Vector2 size);
	~Window();
};