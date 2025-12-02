#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "dotenv/dotenv.hpp"

namespace fs = std::filesystem;

class DotenvTest : public ::testing::Test {
protected:
    fs::path test_env_file;

    void SetUp() override {
        test_env_file = fs::temp_directory_path() / "test.env";
        std::ofstream ofs(test_env_file);
        ofs << "# Comment line\n";
        ofs << "PORT=8080\n";
        ofs << "HOST=\"localhost\"\n";
        ofs << "PI=3.1415\n";
        ofs << "EMPTY=\n";
        ofs << "SPACED=  some value  \n";
        ofs << "OVERRIDE=first\n";
        ofs << "REF1=$OVERRIDE\n";
        ofs << "REF2=${OVERRIDE}\n";
        ofs << "INVALID_INT=abc\n";
    }

    void TearDown() override { fs::remove(test_env_file); }
};

TEST_F(DotenvTest, LoadAndGetInt) {
    dotenv::load(test_env_file.string());
    const auto port = dotenv::get<int>("PORT");
    ASSERT_TRUE(port.has_value());
    EXPECT_EQ(*port, 8080);
}

TEST_F(DotenvTest, LoadAndGetString) {
    dotenv::load(test_env_file.string());

    auto host = dotenv::get<std::string>("HOST");
    ASSERT_TRUE(host.has_value());
    EXPECT_EQ(*host, "localhost");

    auto spaced = dotenv::get<std::string>("SPACED");
    ASSERT_TRUE(spaced.has_value());
    EXPECT_EQ(*spaced, "some value");

    auto empty = dotenv::get<std::string>("EMPTY");
    ASSERT_TRUE(empty.has_value());
    EXPECT_EQ(*empty, "");
}

TEST_F(DotenvTest, LoadAndGetFloat) {
    dotenv::load(test_env_file.string());
    const auto pi = dotenv::get<double>("PI");
    ASSERT_TRUE(pi.has_value());
    EXPECT_NEAR(*pi, 3.1415, 1e-5);
}

TEST_F(DotenvTest, MissingVariableReturnsNullopt) {
    dotenv::load(test_env_file.string());
    const auto missing = dotenv::get<int>("MISSING");
    EXPECT_FALSE(missing.has_value());
}

TEST_F(DotenvTest, VariableOverwriting) {
    dotenv::load(test_env_file.string());
    const auto override_val = dotenv::get<std::string>("OVERRIDE");
    ASSERT_TRUE(override_val.has_value());
    EXPECT_EQ(*override_val, "first");
}

TEST_F(DotenvTest, InvalidIntReturnsNullopt) {
    dotenv::load(test_env_file.string());
    const auto invalid = dotenv::get<int>("INVALID_INT");
    EXPECT_FALSE(invalid.has_value());
}

TEST_F(DotenvTest, EnvVariableSetInStdGetenv) {
    dotenv::load(test_env_file.string());
    const char* port_env = std::getenv("PORT");
    ASSERT_NE(port_env, nullptr);
    EXPECT_STREQ(port_env, "8080");

    const char* host_env = std::getenv("HOST");
    ASSERT_NE(host_env, nullptr);
    EXPECT_STREQ(host_env, "localhost");
}

TEST_F(DotenvTest, CommentsAndEmptyLinesIgnored) {
    dotenv::load(test_env_file.string());
    const auto comment = dotenv::get<std::string>("# Comment line");
    EXPECT_FALSE(comment.has_value());

    const auto non_exist = dotenv::get<std::string>("");  // empty key
    EXPECT_FALSE(non_exist.has_value());
}
