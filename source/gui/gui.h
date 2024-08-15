//
// Created by Vlad on 14.08.2024.
//

#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <optional>


namespace valencia::gui
{
    class Menu
    {
    public:
        Menu();
        ~Menu();
        void Run();

    private:
        void Init();
        void MainLoop();
        void Cleanup() const;

        GLFWwindow* window{};
        const char* glsl_version = "#version 130";

        std::optional<std::string> m_pathToApex;
    };
}