#include "Core/File.h"

#include <gtest/gtest.h>

using namespace Nexora;

TEST(FileTest, ShouldReadWrittenText)
{
    const auto path    = std::filesystem::temp_directory_path() / "NexoraFileTest.txt";
    const auto written = File::writeText(path, "Nexora");
    ASSERT_TRUE(written.has_value());

    const auto text = File::readText(path);

    ASSERT_TRUE(text.has_value());
    EXPECT_EQ(*text, "Nexora");
}

TEST(FileTest, ShouldReadWrittenBinary)
{
    const auto               path    = std::filesystem::temp_directory_path() / "NexoraFileTest.bin";
    const std::vector<uint8> bytes   = {0x00, 0xFF, 0x0A};
    const auto               written = File::writeBinary(path, bytes);
    ASSERT_TRUE(written.has_value());

    const auto readBytes = File::readBinary(path);

    ASSERT_TRUE(readBytes.has_value());
    EXPECT_EQ(*readBytes, bytes);
}
