#include "FileUtils.hpp"

// FileUtils::FileUtils() {}

bool FileUtils::MoveFile(const fs::path &filePath, const fs::path &targetPath)
{
    try
    {
        fs::rename(filePath, targetPath);
        std::cout << "moved: " << filePath << " to " << targetPath;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
    }
    return false;
}

bool FileUtils::UnlinkFile(const fs::path &filePath)
{
    try
    {
        fs::remove(filePath);
        std::cout << "removed: " << filePath;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
    }
    return false;
}

void FileUtils::OpenFile(const fs::path &filePath)
{
#ifdef _WIN32
    const auto command = "start \"\" \"" + filePath.string() + "\"";
#elif __APPLE__
    const auto command = "open \"" + filePath.string() + "\"";
#else
    const auto command = "xdg-open \"" + filePath.string() + "\"";
#endif

    std::system(command.c_str());
}

std::optional<json> FileUtils::ReadMetadataFromFile(const std::string &filePath)
{
    // metadata file path (e.g., document.txt.metadata)
    std::string metadataFilePath = filePath;
    std::ifstream file(metadataFilePath);
    if (!file.is_open())
    {
        // metadata file doesn't exist
        return std::nullopt;
    }
    // Parse JSON
    json metadata;
    file >> metadata;
    return metadata;
}


bool FileUtils::WriteMetadataToFile(const std::string &filePath, const nlohmann::json &metadata)
{
    try
    {
        // Open the file for writing
        std::string metadataFilePath = filePath;

        std::ofstream file(metadataFilePath, std::ios::out | std::ios::trunc);

        std::cout << metadataFilePath << std::endl;

        if (!file.is_open())
        {
            // Handle error: Unable to open the file for writing
            return false;
        }

        // Write the JSON metadata to the file
        file << metadata.dump(4); // Indented JSON with 4 spaces

        // Close the file
        file.close();

        return true;
    }
    catch (const std::exception &e)
    {
        // Handle error: An exception occurred while writing to the file
        return false;
    }

    return false;
}
