#include "MediaPreview.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace fs = std::filesystem;

void MediaPreview::Initialize(const fs::path &selectedEntry)
{
    if (fs::is_regular_file(selectedEntry) && (selectedEntry.extension() == ".png" || selectedEntry.extension() == ".jpg"))
    {
        isLoaded = LoadTextureFromFile(selectedEntry.string().c_str(), &textureID, &width, &height);
    }
    else
    {
        // other media types (videos, 3D models, etc.)
    }
}

void MediaPreview::RenderMedia(int viewportWidth, int viewportHeight)
{
    if (isLoaded)
    {
        float scaleFactor = 1.0f;

        if (width > 0 && height > 0)
        {
            // Calculate the scaling factor to fit the media within the viewport
            float widthScale = static_cast<float>(viewportWidth) / static_cast<float>(width);
            float heightScale = static_cast<float>(viewportHeight) / static_cast<float>(height);
            scaleFactor = std::min(widthScale, heightScale);
        }

        ImVec2 imageSize(static_cast<float>(width) * scaleFactor, static_cast<float>(height) * scaleFactor);
        ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(textureID)), imageSize);

        // ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), ImVec2(width, height));
    }
    else
    {
        // media cannot be loaded or rendered
    }
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool MediaPreview::LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}