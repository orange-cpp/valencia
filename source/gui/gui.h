//
// Created by Vlad on 14.08.2024.
//

#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <optional>
#include <functional>


namespace valencia::gui
{
    class Menu final
    {
    public:
        Menu();
        ~Menu();
        void Run();

    private:
        void Init();
        void MainLoop();

        std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> window{nullptr, [](auto p) {glfwDestroyWindow(p);}};

        std::optional<std::string> m_pathToApex;
    };
}