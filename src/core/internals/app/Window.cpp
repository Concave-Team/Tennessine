#include "Window.h"

Window::~Window()
{
	glfwDestroyWindow(this->wHnd);
}

void Window::Init(std::string title, Vector2 size)
{
	if (!glfwInit())
	{
		const char* desc;
		printf("[-]: Couldn't initialize GLFW: %s", glfwGetError(&desc));
	}
	else
	{
		glfwWindowHint(GLFW_SAMPLES, 4);
		#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		#endif
		this->wHnd = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(this->wHnd, this);

		glfwSetWindowSizeCallback(this->wHnd, resize_callback);
		glfwSetMouseButtonCallback(this->wHnd, mouse_button_callback);
		glfwSetDropCallback(this->wHnd, drop_callback);
		glfwSetKeyCallback(this->wHnd, key_callback);
		glfwSetCursorPosCallback(this->wHnd, cursor_position_callback);
	}
}

void Window::SetSize(Vector2 size)
{
	Size = size;
	glfwSetWindowSize(this->wHnd, Size.x, Size.y);
}

void Window::SetTitle(std::string title)
{
	Title = title;
	glfwSetWindowTitle(this->wHnd, title.c_str());
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(this->wHnd);
}

GLFWwindow* Window::GetWindowHandle()
{
	return this->wHnd;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::shared_ptr<KeyEventArgs> e = std::make_shared<KeyEventArgs>();
	e->window = window;
	e->key = key;
	e->scancode = scancode;
	e->action = action;
	e->mods = mods;

	EventHandler::FireEvent(EventType::APP_WINDOW_KEY_PRESS, e);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::shared_ptr<MouseEventArgs> e = std::make_shared<MouseEventArgs>();

	printf("A %.2d, B %.2d", (float)xpos, (float)ypos);

	e->window = window;
	e->Position = { (float)xpos, (float)ypos };

	EventHandler::FireEvent(EventType::APP_WINDOW_MOUSE_MOVE, e);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::shared_ptr<MouseEventArgs> e = std::make_shared<MouseEventArgs>();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	e->Position.x = xpos;
	e->Position.y = ypos;

	e->Button = button;
	e->Action = action;
	e->Mods = mods;

	EventHandler::FireEvent(EventType::APP_WINDOW_MOUSE_MOVE, e);
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	std::shared_ptr<DropEventArgs> e = std::make_shared<DropEventArgs>();

	e->count = count;
	e->paths = paths;
	e->window = window;

	EventHandler::FireEvent(EventType::APP_WINDOW_FILES_DRAGGED, e);
}

void resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}