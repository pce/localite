#ifdef _WIN32
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>

// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_opengl3_loader.h"
#include <imgui.h>


#include <filesystem>
namespace fs = std::filesystem;



class MediaPreview
{
public:
    void Initialize(const fs::path &selectedEntry);
    void RenderMedia(int viewportWidth, int viewportHeight);
private:
    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

    GLuint textureID = 0;
    int width = 0;
    int height = 0;
    bool isLoaded = false;    


    // GLuint videoTexture = 0;
    // int videoWidth = 0;
    // int videoHeight = 0;
    // bool isVideoLoaded = false;

        // MediaPreview (image, video, etc.) UI elements here
    // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users
    // For video rendering, a video playback library, ie. FFmpeg 
    // initialize and play videos in your MediaPreview class, updating the texture as frames are decoded

};