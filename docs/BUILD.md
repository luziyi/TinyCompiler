# 编译构建流程

本项目使用 CMake 构建系统，支持多种编译器和操作系统。以下是在不同环境下的详细构建步骤。

## Windows 环境

### 使用 MinGW-GCC

1. **安装必要工具**

   - 安装 [MinGW](https://sourceforge.net/projects/mingw-w64/files/) (推荐 MinGW-W64)
   - 安装 [CMake](https://cmake.org/download/) (3.10 或更高版本)
   - 确保将 MinGW 的 bin 目录添加到系统 PATH 环境变量中

2. **构建步骤**

   ```bash
   # 创建并进入构建目录
   mkdir build
   cd build

   # 配置项目 (使用MinGW Makefiles生成器)
   cmake -G "MinGW Makefiles" ..

   # 编译项目
   mingw32-make
   ```

3. **运行编译器**

   ```bash
   # 返回到项目根目录
   cd ..

   # 运行编译器
   ./compiler.exe [ps源文件路径]
   ```

### 使用 Microsoft Visual C++ (MSVC)

1. **安装必要工具**

   - 安装 [Visual Studio](https://visualstudio.microsoft.com/) (包含 C++开发工具)
   - 安装 [CMake](https://cmake.org/download/) (3.10 或更高版本)

2. **使用命令行构建**

   ```bash
   # 创建并进入构建目录
   mkdir build
   cd build

   # 配置项目 (使用VS生成器，-A指定平台)
   cmake -G "Visual Studio 16 2019" -A x64 ..

   # 编译项目
   cmake --build . --config Release
   ```

3. **使用 Visual Studio IDE**

   ```bash
   # 创建并进入构建目录
   mkdir build
   cd build

   # 生成Visual Studio解决方案
   cmake -G "Visual Studio 16 2019" -A x64 ..

   # 打开解决方案文件
   start TinyCompiler.sln
   ```

   - 在 Visual Studio 中，右键点击`compiler`项目，选择"设为启动项目"
   - 点击"本地 Windows 调试器"按钮运行编译器

4. **运行编译器**

   ```bash
   # 返回到项目根目录
   cd ..

   # 运行编译器
   .\Release\compiler.exe [源文件路径]
   ```

## Linux 环境

### 使用 GCC/G++

1. **安装必要工具**

   ```bash
   # Debian/Ubuntu系统
   sudo apt-get update
   sudo apt-get install build-essential cmake

   # CentOS/RHEL/Fedora系统
   sudo yum install gcc gcc-c++ cmake
   ```

2. **构建步骤**

   ```bash
   # 创建并进入构建目录
   mkdir -p build
   cd build

   # 配置项目
   cmake ..

   # 编译项目
   make
   ```

3. **运行编译器**

   ```bash
   # 返回到项目根目录
   cd ..

   # 运行编译器
   ./compiler [源文件路径]
   ```

## macOS 环境

1. **安装必要工具**

   ```bash
   # 安装Homebrew (如果尚未安装)
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

   # 安装CMake和编译器
   brew install cmake
   ```

2. **构建步骤**

   ```bash
   # 创建并进入构建目录
   mkdir -p build
   cd build

   # 配置项目
   cmake ..

   # 编译项目
   make
   ```

3. **运行编译器**

   ```bash
   # 返回到项目根目录
   cd ..

   # 运行编译器
   ./compiler [源文件路径]
   ```

## 常见问题及解决方案

1. **找不到编译器**

   - 确保编译器已正确安装并添加到系统 PATH 中
   - 使用`gcc --version`或`g++ --version`(Linux/MinGW)或`cl`(MSVC)验证编译器是否可用

2. **CMake 版本过低**

   - 本项目要求 CMake 3.10 或更高版本
   - 使用`cmake --version`检查当前版本
   - 需要时从官方网站下载更新版本

3. **找不到目标文件**

   - Windows 下可能需要指定构建类型路径，如`.\Debug\compiler.exe`或`.\Release\compiler.exe`
   - 确保在正确的目录中运行命令

4. **库链接错误**

   - 确保所有依赖库都已正确构建
   - 检查 CMakeLists.txt 中的库路径是否正确

5. **无法创建 result 目录**
   - 程序会尝试自动创建 result 目录
   - 如果遇到权限问题，请手动创建该目录
