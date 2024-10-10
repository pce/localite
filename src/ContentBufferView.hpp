#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>
#include <utility>
#include <vector>
// GLFW for OpenGL GLuint not #include <glad/glad.h>
#include <GLFW/glfw3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h> // Include OpenGL ES 2.0 headers
#endif

#include <nlohmann/json.hpp>
#include "MediaPreview.hpp"
#include "FileUtils.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;


class ContentBufferView
{
public:
    ContentBufferView() : currentPath(fs::current_path()){};
    explicit ContentBufferView(fs::path projectDirectory)
    {
        currentPath = std::move(projectDirectory);
        SetProjectDirectory(projectDirectory);
    };
    // set project dir
    void Draw(std::string_view label);
    void SetProjectDirectory(fs::path projectDir);

private:
    fs::path projectDirectory = "./";
    fs::path currentPath;
    fs::path selectedEntry;
    bool renameDialogOpen = false;
    bool deleteDialogOpen = false;

    MediaPreview mediaPreview;
    FileUtils fileUtils;

    void RenderMetadataEditor(std::optional<json> &metadata);

};
