#!/bin/bash

set -e
set -o pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
BUILD_TYPE="Debug"

# Arg Parser
COMMAND=""
ARG=""
for arg in "$@"; do
    case $arg in
        --release)
            BUILD_TYPE="Release"
            BUILD_DIR="$ROOT_DIR/build-release"
            ;;
        all|test|clean|format|help|check_deps)
            COMMAND="$arg"
            ;;
        *)
            ARG="$arg"
            ;;
    esac
done

# Help
function show_help() {
    echo "Usage: ./build.sh [command] [options]"
    echo
    echo "Commands:"
    echo "  all                  Configure and build everything"
    echo "  test                 Build and run all tests"
    echo "  test <name>          Run a specific test executable"
    echo "  clean                Remove build directory"
    echo "  format               Run clang-format on all source files"
    echo "  check_deps           Run include-what-you-use on a single file or full project"
    echo "  help                 Show this page"
    echo
    echo "Options:"
    echo "  --release            Build in Release mode (default: Debug)"
    echo
    echo "Examples:"
    echo "  ./build.sh all"
    echo "  ./build.sh test"
    echo "  ./build.sh test DotenvTest"
}

# clean
function clean_build() {
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
}


# configure cmake
function configure() {
    echo "=== Configuring build ($BUILD_TYPE) ==="
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    cmake "$ROOT_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
}

# Build all
function build_all() {
    configure
    echo "=== Building project ($BUILD_TYPE) ==="
    cmake --build "$BUILD_DIR" -- -j"$(sysctl -n hw.ncpu)"
}

# Run tests
function run_tests() {
    build_all

    if [[ -z "$ARG" ]]; then
        echo "=== Running all tests ==="
        ctest --test-dir "$BUILD_DIR" --output-on-failure
    else
        TEST_EXEC="$BUILD_DIR/$ARG"
        if [[ ! -f "$TEST_EXEC" ]]; then
            echo "Test executable not found: $TEST_EXEC"
            exit 1
        fi
        echo "=== Running test: $ARG ==="
        "$TEST_EXEC"
    fi
}

# format
function format_code() {
    echo "=== Formatting C++ code ==="
    find "$ROOT_DIR" -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-format -i {} +
    echo "=== Done formatting C++ code ==="
}

function check_deps() {

    if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
        echo "=== No compile_commands.json found, configuring build first ==="
        configure
        cmake --build "$BUILD_DIR" --target all -- -j"$(sysctl -n hw.ncpu)"
    fi

    echo "=== Running include-what-you-use ==="

    if [[ -n "$ARG" ]]; then
        TARGET_PATH="$ROOT_DIR/$ARG"

        if [[ ! -f "$TARGET_PATH" ]]; then
            echo "Error: File not found: $TARGET_PATH"
            exit 1
        fi

        echo "Checking dependencies for: $ARG"
        iwyu_tool.py -p "$BUILD_DIR" "$TARGET_PATH"
    else
        echo "Checking dependencies for entire project"
        TARGETS=(
                "$ROOT_DIR/src"
                "$ROOT_DIR/include"
                "$ROOT_DIR/tests"
                "$ROOT_DIR/main.cpp"
        )

        iwyu_tool.py -p "$BUILD_DIR" "${TARGETS[@]}"
    fi
}

# dispatch
case "$COMMAND" in
    all)
        build_all
        ;;
    test)
        run_tests
        ;;
    clean)
        clean_build
        ;;
    format)
        format_code
        ;;
    check_deps)
        check_deps
        ;;
    help|"")
        show_help
        ;;
    *)
        echo "Unknown command: $COMMAND"
        show_help
        exit 1
        ;;
esac
