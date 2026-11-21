#include "Core/File.h"

#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogFile);

namespace
{

template <typename Container>
std::expected<Container, Nexora::FileError> readFile(const std::filesystem::path& path)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open())
    {
        NX_LOG(LogFile, Error, "Failed to open {} for reading", path.string());
        return std::unexpected(Nexora::FileError::OpenFailed);
    }

    return Container(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

template <typename Container>
std::expected<void, Nexora::FileError> writeFile(const std::filesystem::path& path, const Container& content)
{
    std::ofstream stream(path, std::ios::binary);
    if (!stream.is_open())
    {
        NX_LOG(LogFile, Error, "Failed to open {} for writing", path.string());
        return std::unexpected(Nexora::FileError::OpenFailed);
    }

    std::ranges::copy(content, std::ostreambuf_iterator<char>(stream));
    return {};
}

}  // namespace

namespace Nexora
{

std::expected<std::string, FileError> File::readText(const std::filesystem::path& path)
{
    return readFile<std::string>(path);
}

std::expected<std::vector<uint8>, FileError> File::readBinary(const std::filesystem::path& path)
{
    return readFile<std::vector<uint8>>(path);
}

std::expected<void, FileError> File::writeText(const std::filesystem::path& path, std::string_view text)
{
    return writeFile(path, text);
}

std::expected<void, FileError> File::writeBinary(const std::filesystem::path& path, const std::vector<uint8>& bytes)
{
    return writeFile(path, bytes);
}

}  // namespace Nexora
