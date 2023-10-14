#pragma once
#include <iostream>
#include "Application.h"

class Input
{
public:
	GLFWwindow* procWnd;
	Vector2 MousePosition;
    int CurrentKey;
    int CurrentButton;

    static Input& GetInstance() {
        static Input instance; // This ensures it's created only once.
        return instance;
    }

    bool IsKeyDown(int key);
    bool IsMouseButtonDown(int button);

    // Sets up all of the event handlers.
    void RegisterInput();
};