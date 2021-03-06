#include "util/tools.h"

#include <gtest/gtest.h>

using namespace ::testing;

TEST(ToolsTest, secondsToHMS)
{
    EXPECT_EQ(secondsToHMS(0), "00:00:00");
    EXPECT_EQ(secondsToHMS(1), "00:00:01");
    EXPECT_EQ(secondsToHMS(62), "00:01:02");
    EXPECT_EQ(secondsToHMS(3662), "01:01:02");
}

TEST(ToolsTest, secondsToHMSOverflow)
{
    EXPECT_EQ(secondsToHMS(1000 * 3600 + 1), "999:00:01");
}

TEST(ToolsTest, readWriteBinaryFile)
{
    std::string source = "This is a test, \0, binary";
    auto test_file = fs::temp_directory_path() / "gerbera-binary-test";
    EXPECT_FALSE(fs::exists(test_file)) << "Can't test existing file";
    writeBinaryFile(test_file, reinterpret_cast<const std::byte*>(source.data()), source.size());
    auto result = readBinaryFile(test_file);
    EXPECT_TRUE(result.has_value());

    std::vector<std::byte> expected(source.size());
    std::transform(source.begin(), source.end(), expected.begin(),
        [](char c) { return std::byte(c); });

    EXPECT_EQ(*result, expected);
    fs::remove(test_file);
}

TEST(ToolsTest, readBinaryReturnsEmptyIfFileMissing)
{
    fs::path test_file("/some/unexisting/file");
    EXPECT_FALSE(fs::exists(test_file));
    auto res = readBinaryFile(test_file);
    EXPECT_FALSE(res.has_value());
}

TEST(ToolsTest, writeFileThrowsIfCantOpenFile)
{
    fs::path test_file("/some/unexisting/file");
    EXPECT_FALSE(fs::exists(test_file));

    std::vector<std::byte> data { std::byte { 1 } };

    EXPECT_THROW(writeBinaryFile(test_file, data.data(), data.size()), std::runtime_error);
}

TEST(ToolsTest, renderWebUriV4)
{
    EXPECT_EQ(renderWebUri("192.168.5.5", 7777), "192.168.5.5:7777");
}

TEST(ToolsTest, renderWebUriV6)
{
    EXPECT_EQ(renderWebUri("2001:0db8:85a3:0000:0000:8a2e:0370:7334", 7777), "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:7777");
}