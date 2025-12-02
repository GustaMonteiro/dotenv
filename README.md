<div align="center">
  
# dotenv

</div>


<p align="center">
  <img src="https://github.com/Ayush272002/dotenv/blob/main/.github/banner.svg" width="100%" />
</p>

<p align="center">
  <b>A lightweight, zero-dependency, C++23 header-only library to load and parse .env files.</b><br/>
  Clean API · No exceptions · Typed getters · Tested · Production-ready
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?style=flat-square" />
  <img src="https://img.shields.io/badge/header--only-yes-brightgreen?style=flat-square" />
  <img src="https://img.shields.io/badge/license-MIT-yellow?style=flat-square" />
</p>

# 📦 Features

* **Header-only** — include and use
* **C++23 API**
* **Zero dependencies**
* **No exceptions**, uses `std::expected`
* **Auto-trim, skip comments, strip quotes**
* **Typed getters** (`int`, `double`, `string`)
* **GoogleTest test suite**
* **Works with FetchContent, CMake install**


# 📥 Install

## 🔹 FetchContent (recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
  dotenv
  GIT_REPOSITORY https://github.com/Ayush272002/dotenv.git
  GIT_TAG        v1.0.0
)

FetchContent_MakeAvailable(dotenv)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE dotenv)
```

# 🚀 Usage

## **.env**

```
PORT=8080
HOST="localhost"
DEBUG=true
TIMEOUT=30.5
EMPTY=
SPACED=  some value  
```

## **main.cpp**

```cpp
#include <dotenv/dotenv.hpp>
#include <iostream>
#include <format>

int main() {
    dotenv::load(".env");

    std::cout << "PORT = "   << dotenv::get<int>("PORT").value_or(-1) << "\n";
    std::cout << "HOST = "   << dotenv::get<std::string>("HOST").value_or("undefined") << "\n";
    std::cout << "TIMEOUT = "<< dotenv::get<double>("TIMEOUT").value_or(0.0) << "\n";

    return 0;
}
```

# 📚 API

### Load `.env`

```cpp
dotenv::load(".env");
```

### Get typed values

```cpp
dotenv::get<int>("PORT");
dotenv::get<double>("TIMEOUT");
dotenv::get<std::string>("HOST");
```

### Using `value_or`

```cpp
int port = dotenv::get<int>("PORT").value_or(3000);
```

# 🧪 Tests

Run all tests:

```bash
./build.sh test
```

Or manually:

```bash
cmake -B build
cmake --build build
ctest --test-dir build
```

# 🔧 Build

```bash
./build.sh all
```

# 🚧 Roadmap (v2)

* Variable referencing (`URL=$HOST:$PORT`)
* Strict mode
* Windows `.env` compatibility
* `.env.example` validation

# 🤝 Contributing

1. Fork
2. Create feature branch
3. Run `./build.sh test`
4. Open PR 🎉

---

# 🛡 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
