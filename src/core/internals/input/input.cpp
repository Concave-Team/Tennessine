#include "input.h"

bool Input::IsKeyDown(int key)
{
	if (this->CurrentKey == key)
	{
		return true;
	}
	return false;
}

bool Input::IsMouseButtonDown(int button)
{
	if (this->CurrentButton == button)
		return true;
	return false;
}

void Input::RegisterInput()
{
	EventHandler::SetEventCallback(EventType::APP_WINDOW_KEY_PRESS, [](EventArgs* args) -> void {
		KeyEventArgs* eStateArgs = dynamic_cast<KeyEventArgs*>(args);
		if (eStateArgs) {
			Input& input = Input::GetInstance();
			input.procWnd = eStateArgs->window;
			input.CurrentKey = eStateArgs->key;
		}
		else
		{
			printf("E013: Cannot cast EventArgs to KeyEventArgs.");
		}
		});
	EventHandler::SetEventCallback(EventType::APP_WINDOW_MOUSE_CLICK, [](EventArgs* args) -> void {
		MouseEventArgs* eStateArgs = dynamic_cast<MouseEventArgs*>(args);
		if (eStateArgs) {
			Input& input = Input::GetInstance();
			input.procWnd = eStateArgs->window;
			input.CurrentButton = eStateArgs->Button;
		}
		else
		{
			printf("E013: Cannot cast EventArgs to MouseEventArgs.");
		}
		});
	EventHandler::SetEventCallback(EventType::APP_WINDOW_MOUSE_MOVE, [](EventArgs* args) -> void {
		MouseEventArgs* eStateArgs = dynamic_cast<MouseEventArgs*>(args);
		if (eStateArgs) {
			Input& input = Input::GetInstance();
			input.procWnd = eStateArgs->window;
			printf("XO: %d, YO: %d", eStateArgs->Position.x, eStateArgs->Position.y);
			input.MousePosition = eStateArgs->Position;
		}
		else
		{
			printf("E013: Cannot cast EventArgs to MouseEventArgs.");
		}
		});
}