#include <cstdint>
#include <filesystem>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui.h>
#include <implot.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "ContentBufferView.hpp"
#include "open_font_icons.hpp" 

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

constexpr auto WINDOW_WIDTH = std::uint32_t{1280};
constexpr auto WINDOW_HEIGHT = std::uint32_t{720};

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char *description);

void start_cycle()
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

void end_cycle(GLFWwindow *const window)
{
    const auto clear_color =
        ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void render(ContentBufferView &window_obj)
{
    window_obj.Draw("Content Browser");
}

int main(int argc, char **argv)
{
    fs::path projectdir = "./";
    // fs::path projectdir = fs::current_path();
    std::cout << projectdir.string() << std::endl;
    if (argc > 1) {
        fs::path arg1 = argv[1];
        if (FileUtils::PathExists(arg1)) {
            projectdir = arg1;
            std::cout << "Project directory set to: " << projectdir << std::endl;
        } else {
            std::cerr << "Provided path does not exist: " << arg1 << std::endl;
            return 1;
        }
    } else {
        std::cout << "No project directory provided." << std::endl;
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    auto *window = glfwCreateWindow(static_cast<std::int32_t>(WINDOW_WIDTH),
                                    static_cast<std::int32_t>(WINDOW_HEIGHT),
                                    "Gui",
                                    nullptr,
                                    nullptr);
    if (window == nullptr)
    {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    auto &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0, 1.0, 1.0, 1.0);

    // Setup Fonts
    ImGuiIO &io = ImGui::GetIO();
    float iconSize = 12.f;
    
    io.Fonts->AddFontDefault(); //  default font to cover regular characters
    // Load embedded font from memory and Map Unicode characters to icons (File, Folders Icons etc.)
    ImFontConfig icon_font_config;
    icon_font_config.MergeMode = true; // Merge the icon font with the default font
    static constexpr ImWchar icon_ranges[] = {0xE000, 0xF8FF, 0}; // Unicode range for icons
    io.Fonts->AddFontFromMemoryTTF(OpenFontIcons_ttf, OpenFontIcons_ttf_len, iconSize, &icon_font_config, icon_ranges);

    ContentBufferView window_obj{projectdir};

    ImPlot::CreateContext();

    while (!glfwWindowShouldClose(window))
    {
        start_cycle();

        ImGui::NewFrame();
        render(window_obj);
        ImGui::Render();

        end_cycle(window);
    }
    ImPlot::DestroyContext();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
