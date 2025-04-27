# 构建说明

本文档提供 TinyCompiler 项目的详细构建指南，适用于不同操作系统和编译环境。

## 环境要求

- **C++编译器**：支持 C++11 标准
  - Windows: MinGW-GCC、MSVC
  - Linux: GCC/G++
  - macOS: Clang
- **CMake**: 3.10 或更高版本
- **操作系统**: Windows、Linux 或 macOS

## Windows 环境构建

### 使用 MinGW-GCC

1. **安装必要工具**

   - [MinGW-W64](https://sourceforge.net/projects/mingw-w64/): 提供 GCC 编译器
   - [CMake](https://cmake.org/download/): 构建系统

2. **命令行构建**

   ```bash
   # 创建并进入构建目录
   mkdir build
   cd build

   # 配置项目
   cmake -G "MinGW Makefiles" ..

   # 编译项目
   mingw32-make

   # 运行编译器
   ../bin/tiny-compiler.exe [源文件路径] [语法文件路径]
   ```

### 使用 Visual Studio

1. **安装必要工具**

   - [Visual Studio](https://visualstudio.microsoft.com/)：包含 C++开发组件
   - [CMake](https://cmake.org/download/)：可以使用 Visual Studio 内置的 CMake

2. **使用命令行构建**

   ```bash
   # 创建并进入构建目录
   mkdir build
   cd build

   # 配置项目 (根据您的VS版本调整生成器)
   cmake -G "Visual Studio 17 2022" -A x64 ..

   # 编译项目
   cmake --build . --config Release

   # 运行编译器
   ../bin/tiny-compiler.exe [源文件路径] [语法文件路径]
   ```

3. **使用 Visual Studio IDE**
   - 打开 Visual Studio
   - 选择"打开本地文件夹"并选择 TinyCompiler 根目录
   - Visual Studio 会自动检测 CMake 配置
   - 使用 IDE 顶部的下拉菜单选择构建配置和目标
   - 点击"构建"按钮编译项目

## Linux 环境构建

1. **安装必要工具**

   ```bash
   # Ubuntu/Debian
   sudo apt update
   sudo apt install build-essential cmake

   # CentOS/RHEL/Fedora
   sudo dnf install gcc gcc-c++ cmake
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

   # 运行编译器
   ../bin/tiny-compiler.exe [源文件路径] [语法文件路径]
   ```

## macOS 环境构建

1. **安装必要工具**

   ```bash
   # 安装Homebrew (如果尚未安装)
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

   # 安装构建工具
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

   # 运行编译器
   ../bin/tiny-compiler.exe [源文件路径] [语法文件路径]
   ```

## 常见问题及解决方案

### 找不到 C++编译器

- **症状**：配置项目时出现"找不到 C++编译器"的错误
- **解决方案**：
  - 确保已正确安装编译器
  - 将编译器所在目录添加到系统 PATH 环境变量中
  - 使用`g++ --version`或`cl`命令检查编译器是否可用

### CMake 版本过低

- **症状**：运行 cmake 命令时出现版本错误
- **解决方案**：
  - 检查当前 CMake 版本：`cmake --version`
  - 从[CMake 官网](https://cmake.org/download/)下载更新版本
  - 安装后确保新版本在 PATH 中

### 构建失败

- **症状**：编译时出现错误
- **解决方案**：
  - 清理构建目录后重新构建：删除 build 目录并重新创建
  - 检查编译器错误信息，修复相关代码问题
  - 确保所有依赖库都已正确安装

### 运行时找不到输入文件

- **症状**：运行编译器时找不到默认源文件或语法文件
- **解决方案**：
  - 检查 tiny-compiler 和 data 相对路径是否正确
  - 确保 data 目录结构完整，包含所需的测试文件和语法规则
  - 使用绝对路径或正确的相对路径指定输入文件
  - 手动创建缺失的文件目录

注意：在 Linux/macOS 上可能需要 sudo 权限。
