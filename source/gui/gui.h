//
// Created by Vlad on 14.08.2024.
//

#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Simple class to encapsulate the GUI app
class SimpleGuiApp {
public:
    SimpleGuiApp();
    ~SimpleGuiApp();
    void Run();

private:
    void Init();
    void MainLoop();
    void Cleanup();

    GLFWwindow* window;
    const char* glsl_version = "#version 130";
    const int window_width = 1280;
    const int window_height = 720;
};
