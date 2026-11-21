#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "Core/Types.h"

namespace Nexora
{

enum class FileError : uint8
{
    Unknown,
    OpenFailed
};

class File final
{
public:
    // core
    static std::expected<std::string, FileError>        readText(const std::filesystem::path& path);
    static std::expected<std::vector<uint8>, FileError> readBinary(const std::filesystem::path& path);
    static std::expected<void, FileError>               writeText(const std::filesystem::path& path, std::string_view text);
    static std::expected<void, FileError>               writeBinary(const std::filesystem::path& path, const std::vector<uint8>& bytes);
};

}  // namespace Nexora
