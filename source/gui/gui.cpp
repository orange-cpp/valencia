//
// Created by Vlad on 14.08.2024.
//
#include "gui.h"


#include <expected>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <sstream>


#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include <nfd.h>


#include "PortableExecutableHeaders.h"
#include "OffsetsDumper.h"


namespace valencia::gui
{
    [[nodiscard]] std::expected<std::vector<uint8_t>, std::string> GetCodeSegment(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::binary);

        if (!file.is_open()) [[unlikely]]
            return std::unexpected("Cannot open file, try to check if path is valid");

        IMAGE_DOS_HEADER dosHeader{};
        file.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));

        if (dosHeader.e_magic != 0x5A4D) [[unlikely]]
            return std::unexpected("File must be in PE32 format (.exe)");

        file.seekg(dosHeader.e_lfanew, std::ios::beg);

        IMAGE_NT_HEADERS32 ntHeaders{};
        file.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));

        if (ntHeaders.Signature != 0x00004550) [[unlikely]]
            return std::unexpected("File must be in PE32 format (.exe)");

        constexpr size_t sizeOfSignature = 4;
        const auto offsetToSegmentTable = dosHeader.e_lfanew + ntHeaders.FileHeader.SizeOfOptionalHeader
                                          + sizeof(IMAGE_FILE_HEADER) + sizeOfSignature;

        file.seekg(static_cast<std::ifstream::off_type>(offsetToSegmentTable), std::ios::beg);

        for (size_t i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++)
        {
            IMAGE_SECTION_HEADER currentSection{};
            file.read(reinterpret_cast<char*>(&currentSection), sizeof(IMAGE_SECTION_HEADER));

            if (std::string_view(currentSection.Name) != ".text")
                continue;

            std::vector<uint8_t> sectionData(currentSection.SizeOfRawData, 0);

            file.seekg(currentSection.PointerToRawData, std::ios::beg);
            file.read(reinterpret_cast<char*>(sectionData.data()), static_cast<std::streamsize>(sectionData.size()));

            return sectionData;
        }
        return std::unexpected("Executable does not contain .text segment!");
    }

    [[nodiscard]] std::string GetStringOfOffsets(const std::string& category,
                      const std::unordered_map<std::string, std::optional<uintptr_t>>& offsets)
    {
        std::stringstream ss;
        ss << "\n<===" << category << "===>\n";

        for (const auto& [name, offset] : offsets)
        {
            ss << name << " = 0x";

            if (offset.has_value())
                ss << std::hex << offset.value();
            else
                ss << "N/A";

            ss << '\n';
        }
        return ss.str();
    }

    void Menu::MainLoop() {
        while (!glfwWindowShouldClose(window.get())) {
            const auto startRenderTime = std::chrono::high_resolution_clock::now();
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            ImGui::Begin("xxxMainMenu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::SetWindowPos({0.f, 0.f});
                ImGui::SetWindowSize(ImGui::GetMainViewport()->Size);

                static std::string offsets = "Please choose apex binary";

                if (ImGui::Button("Select Exe"))
                {
                    nfdchar_t *outPath = nullptr;

                    if (const auto result = NFD_OpenDialog( "exe", nullptr, &outPath ); result == NFD_OKAY)
                        m_pathToApex = std::string(outPath);
                }

                ImGui::SameLine();
                if (ImGui::Button("Dump") && m_pathToApex)
                {
                    offsets.clear();
                    if (auto segment = GetCodeSegment(m_pathToApex.value()))
                    {
                        OffsetsDumper dumper(std::move(segment.value()));

                        offsets += GetStringOfOffsets("GLOBAL", dumper.GetOffsetsOfGlobalObjects());
                        offsets += GetStringOfOffsets("ENTITY", dumper.GetOffsetsOfBaseEntityFileds());
                        offsets += GetStringOfOffsets("WEAPON", dumper.GetOffsetsOfWeaponFileds());
                    }
                    else
                        offsets = "Failed to get .text segent from binary :(";
                }

                ImGui::SameLine();
                if (ImGui::Button("GitHub"))
                {
                    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                        std::system("start https://github.com/orange-cpp/valencia");
                    #elif __APPLE__
                        std::system("open https://github.com/orange-cpp/valencia");
                    #elif __linux__
                        std::system("xdg-open https://github.com/orange-cpp/valencia");
                    #else
                    #   error "Unknown compiler"
                    #endif
                }
                ImGui::TextColored(m_pathToApex ? ImColor(0, 255, 0) : ImColor(255, 0, 0), std::format("Path: {}", m_pathToApex.value_or("NONE")).c_str());

                constexpr auto floatMin = std::numeric_limits<float>::min();
                ImGui::InputTextMultiline("##textview", offsets.data(), offsets.size(),
                                          {-floatMin, -floatMin}, ImGuiInputTextFlags_ReadOnly);
                ImGui::End();
            }

            ImGui::Render();

            int display_w, display_h;

            glfwGetFramebufferSize(window.get(), &display_w, &display_h);

            glViewport(0, 0, display_w, display_h);
            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window.get());
            const auto endRenderTime = std::chrono::high_resolution_clock::now();

            const auto deltaMilliSeconds = std::chrono::duration_cast<std::chrono::microseconds>(endRenderTime-startRenderTime).count() / 1000.f;

            const auto sleepTime = static_cast<size_t>(1000.f / 35.f - deltaMilliSeconds);


            if (sleepTime > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }

    Menu::Menu()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
        glfwWindowHint(GLFW_RESIZABLE, false);

        window.reset(glfwCreateWindow(300, 500, "Valencia - by Orange", nullptr, nullptr));

        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1);


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        const ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();


        ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }


    Menu::~Menu()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
    }


    void Menu::Run()
    {
        MainLoop();
    }
}