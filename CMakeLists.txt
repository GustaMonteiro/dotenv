cmake_minimum_required(VERSION 4.0)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)

project(dotenv VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------
# dotenv library (header-only)
# ------------------------
add_library(dotenv INTERFACE)
target_include_directories(dotenv INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(dotenv INTERFACE cxx_std_23)

# ------------------------
# example
# ------------------------
add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE dotenv)

# ------------------------
# tests
# ------------------------
if (PROJECT_IS_TOP_LEVEL)
    find_package(GTest REQUIRED)
    enable_testing()

    file(GLOB TEST_SOURCES tests/*.cpp)
    add_executable(dotenv_tests ${TEST_SOURCES})
    target_link_libraries(dotenv_tests PRIVATE dotenv GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(dotenv_tests)
endif ()

# ------------------------
# Installation for FetchContent / package usage
# ------------------------
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets
install(TARGETS dotenv EXPORT dotenvTargets)
install(EXPORT dotenvTargets
        FILE dotenvTargets.cmake
        NAMESPACE dotenv::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dotenv)

# Config file for FetchContent / find_package
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/dotenvConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dotenvConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/dotenvConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dotenv
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/dotenvConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/dotenvConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dotenv
)