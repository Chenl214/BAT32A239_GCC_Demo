# BAT32A239 GCC Example Project

## 项目描述
这是一个基于BAT32A239微控制器的GCC示例项目，展示了如何使用GCC工具链进行嵌入式开发。

## 工具安装指南
1. 安装GCC ARM嵌入式工具链：
   - 链接：https://developer.arm.com/downloads/-/gnu-rm
   - windows系统选择gcc-arm-none-eabi-10.3-2021.10-win3.zip下载即可
   - 解压缩后将bin路径添加至系统变量PATH中，例如添加：D:\Development_Toolchains\gcc-arm-none-eabi-10.3-2021.10\bin

2. 安装cmake：
   - 链接：https://cmake.org/download/
   - windows系统选择Windows x64 ZIP版本下载即可
   - 解压缩后将bin路径添加至系统变量PATH中，例如添加：D:\Development_Toolchains\cmake-4.0.2-windows-x86_64\bin

3. 安装MinGW-w64工具：
   - 链接：tps://github.com/niXman/mingw-builds-binaries/releases
   - windows系统选择x86_64-15.1.0-release-win32-seh-ucrt-rt_v12-rev0.7z下载即可
   - 解压缩后将bin路径添加至系统变量PATH中，例如添加：D:\Development_Toolchains\mingw64\bin
   - 将bin文件夹下面的mingw32-make.exe复制并重命名为make.exe

## Makefile说明
本项目使用Makefile管理构建过程，主要包含以下目标：
- `make` 或 `make all`: 编译整个项目
- `make clean`: 清理构建生成的文件
- `make flash`: 烧录程序到开发板(需配置OpenOCD)
- `make debug`: 启动GDB调试会话

关键变量说明：
- `CC`: 指定交叉编译器路径
- `CFLAGS`: 编译选项
- `LDFLAGS`: 链接选项
- `TARGET`: 生成的目标文件名


## 使用说明
### 编译与构建
1. 编译项目
   ```bash
   make
   ```

2. 清理构建文件
   ```bash
   make clean
   ```

3. 重新编译项目
   ```bash
   make clean && make
   ```

### 烧录程序
1. 连接开发板：
   - 使用USB转TTL调试器连接开发板的SWD接口
   - 确保供电正常(3.3V)

2. 烧录程序：
   ```bash
   make flash
   ```
   需要预先安装并配置OpenOCD

### 调试程序
1. 启动GDB调试会话：
   ```bash
   make debug
   ```
   需要预先安装GDB ARM工具链

## 硬件连接
- 开发板: BAT32A239评估板
- 调试接口: SWD (SWCLK+SWDIO)
- 供电: 3.3~5V DC


## 常见问题
Q: 编译时报错"arm-none-eabi-gcc not found"
A: 请检查GCC工具链是否安装正确，PATH环境变量是否包含工具链bin目录

Q: 烧录失败
A: 请检查：
   - 开发板连接是否正确
   - OpenOCD配置是否正确
   - 开发板供电是否正常