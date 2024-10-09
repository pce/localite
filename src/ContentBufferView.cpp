#include <iostream>

#include <cstdlib>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "ContentBufferView.hpp"

void ContentBufferView::Draw(std::string_view label)
{

    constexpr static auto window_flags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    // window size to match the screen width and height
    ImGui::SetNextWindowSize(screenSize);

    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    ImGui::BeginChild("Sidebar", ImVec2(320, 0), true);

    if (ImGui::Button("Go Up"))
    {
        if (currentPath.has_parent_path())
        {
            currentPath = currentPath.parent_path();
        }
    }
    ImGui::SameLine();
    ImGui::Text("Path: %s", currentPath.string().c_str());

    ImGui::Separator();

    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        const auto is_selected = entry.path() == selectedEntry;
        const auto is_directory = entry.is_directory();
        const auto is_file = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_directory)
        {
            entry_name = "\ue067 " + entry_name;
        }
        if (is_file)
        {
            entry_name = "\ue061 " + entry_name;
        }

        if (ImGui::Selectable(entry_name.c_str(), is_selected))
        {
            if (is_directory)
            {
                currentPath /= entry.path().filename();
            }
            selectedEntry = entry.path();
        }
    }
    ImGui::Separator();

    if (!selectedEntry.empty())
    {
        if (fs::is_directory(selectedEntry))
        {
            ImGui::Text("Selected dir: %s", selectedEntry.string().c_str());
        }
        else
        {
            ImGui::Text("Selected file: %s", selectedEntry.string().c_str());
        }

        if (fs::is_regular_file(selectedEntry))
        {
            if (ImGui::Button("Open"))
            {
                fileUtils.OpenFile(selectedEntry);
            }
            ImGui::SameLine();

            if (ImGui::Button("Rename"))
            {
                renameDialogOpen = true;
            }
            ImGui::SameLine();

            if (ImGui::Button("Delete"))
            {
                deleteDialogOpen = true;
            }
            // ImGui::SameLine();

            if (renameDialogOpen)
            {
                ImGui::OpenPopup("Rename File");
            }

            if (deleteDialogOpen)
            {
                ImGui::OpenPopup("Delete File");
            }

            // ********** rename popup
            if (ImGui::BeginPopupModal("Rename File", &renameDialogOpen))
            {
                // auto bufferName = std::string{};
                static char bufferName[512] = "";
                // strncpy(bufferName, selectedEntry.filename().c_str(), sizeof(bufferName));
                ImGui::Text("New name: ");
                ImGui::InputText("###NEWNAME",
                                 bufferName,
                                 IM_ARRAYSIZE(bufferName));

                if (ImGui::Button("Rename"))
                {
                    auto targetPath = selectedEntry.parent_path() / bufferName;
                    if (fileUtils.MoveFile(selectedEntry, targetPath))
                    {
                        renameDialogOpen = false;
                        selectedEntry = targetPath;
                        std::memset(bufferName, 0, sizeof(bufferName));
                    }
                }
                ImGui::EndPopup();
            }
            // ************* delete popup
            if (ImGui::BeginPopupModal("Delete File", &deleteDialogOpen))
            {
                ImGui::Text("Delete %s?",
                            selectedEntry.filename().string().c_str());
                if (ImGui::Button("Yes"))
                {
                    std::cout << "delete: " << selectedEntry.filename().string();

                    if (fileUtils.UnlinkFile(selectedEntry))
                    {
                        selectedEntry.clear();
                    }
                    deleteDialogOpen = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("No"))
                {
                    deleteDialogOpen = false;
                }

                ImGui::EndPopup();
            }
        }
    }

    ImGui::EndChild(); // end Sidebar

    // Viewport
    ImGui::SameLine(); //  place the viewport right next to the sidebar
    ImGui::BeginChild("Viewport", ImVec2(0, 0), true);

    mediaPreview.Initialize(selectedEntry);

    const ImVec2 viewportSize = ImGui::GetWindowSize();
    const int viewportWidth = static_cast<int>(viewportSize.x);
    const int viewportHeight = static_cast<int>(viewportSize.y);

    mediaPreview.RenderMedia(viewportWidth, viewportHeight);

    ImGui::EndChild();

    ImGui::End();
}


void ContentBufferView::SetProjectDirectory(fs::path projectDir)
{
    projectDirectory = projectDir;
}
