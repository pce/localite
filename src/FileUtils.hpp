#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <filesystem>
#include <iostream>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;


class FileUtils {
public:
    // FileUtils();
    bool MoveFile(const fs::path &filePath, const fs::path &targetPath);
    bool UnlinkFile(const fs::path &filePath);
    void OpenFile(const fs::path &filePath);
    static bool CreatePath(const fs::path &path);
    static bool PathExists(const fs::path &path);
    // void CopyFile(const fs::path &filePath);
    // bool PasteFile(const fs::path &filePath, const fs::path &targetPath);
    std::optional<json> ReadMetadataFromFile(const std::string& filePath);
    bool WriteMetadataToFile(const std::string& filePath, const nlohmann::json& metadata);
};

#endif