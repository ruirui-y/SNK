# SNK - 综合性游戏自动化与硬件控制系统

## 项目简介

本项目是基于 **C++** 和 **Qt 框架**开发的综合性游戏自动化与硬件控制系统。它主要面向特定游戏（如KOF/街机类）提供完整的生命周期管理能力，覆盖了从硬件串口通信（投币/按键）、屏幕状态监控（截图与文字识别）、媒体播放（待机动画）到软件加密保护的全套解决方案。

*“这是我入行的第一份Qt项目。”*

## 技术栈与核心组件

- **开发语言**：C++
- **GUI 框架**：Qt (包含大量的自定义 Widget 与 QSS 样式表)
- **并发控制**：自定义线程池 (`ThreadPool`, `Worker`)
- **硬件通信**：串口通信 (`SerialPort`, `SerialThread`)
- **图像与视觉**：屏幕截取 (`GlobaScreenShot`)、OCR 文字识别 (`TextRecognition`)
- **安全保护**：ViKey 软加密狗集成 (`ViKey.dll`)

------

## 核心功能模块

### 1. 业务与界面模块

系统包含丰富的交互界面，用于管理游戏的不同状态和营收数据：

- **`mainWindow`**：主控窗口，统筹各个子模块的生命周期。
- **营业与计费**：`toubiWidget` (投币管理)、`zhangdanWidget` (账单/营收统计)。
- **调试与控制**：`tiaoshiWidget` (后台调试界面)。
- **多媒体展示**：`showmovie` / `VideoWidget` 负责播放游戏待机动画 (`StandbyAnimation`)。
- **出招表/游戏辅助**：`chuzhaobiao.qrc` 配合图片资源，提供游戏内的动态辅助显示。

### 2. 自动化与游戏注入模块

负责与目标游戏进程进行深度交互：

- **按键与信号模拟**：`KeyPress` (按键映射与发送)、`SignalSend` / `SignalRelay` (底层信号中继与下发)。
- **状态监测**：通过 `GlobaScreenShot` 捕获游戏画面，结合 `TextRecognition` (OCR) 识别当前游戏状态（如选人界面、对战结算）。
- **遮罩与美化**：`MaskWidget`、`ShowMinScreen` 和各种 `mask*.png` 用于在特定场景下遮挡游戏原始UI或叠加自定义元素。
- **游戏挂载支持**：通过外部动态链接库（如 `DllKof.dll`）实现特定游戏（拳皇）的定制化逻辑注入与切换 (`SwitchGame`)。

### 3. 底层基础设施

- **串口硬件交互**：`SerialPort` 类配合 `SerialThread` 在独立线程中高效处理投币器、实体按键等外设的 I/O 通信。
- **软件加密**：`Vikey` 目录封装了商用级加密狗接入逻辑 (`ViKey.lib`, `Encryption.h`)，保护核心商业逻辑不被轻易破解。
- **日志与工具**：基于 `LogRecord` 维护本地运行日志 (`log.txt`)，并利用 `Singletion` 模式统筹全局单例对象。

------

## 目录结构概览

```plaintext
├── CaptureStatic/          # 独立的静态截图组件库工程
├── SNK/                    # 主体 Qt 工程目录
│   ├── SNK.vcxproj         # Visual Studio 工程文件
│   ├── mainWindow.cpp/h    # 主界面控制
│   ├── SerialPort.cpp/h    # 串口通信底层驱动
│   ├── TextRecognition.*   # OCR 文字识别模块
│   ├── GlobaScreenShot.*   # 屏幕捕获模块
│   ├── SwitchGame.cpp/h    # 游戏进程切换与挂载逻辑
│   ├── *Widget.cpp         # 各种UI弹窗与悬浮层组件
│   └── *.ui / *.qrc        # Qt 界面布局文件与资源打包配置
└── Vikey/                  # 软件加密与授权模块
    ├── include/            # 加密接口头文件
    ├── lib/                # 静态链接库
    └── dll/                # 运行时动态库 (ViKey.dll)
```



------

## 构建与运行说明

本项目基于 **Qt5** 框架并使用 **Visual Studio** 进行构建。

### 准备工作

1. 确保已安装 **Qt5** 以及对应的 **Visual Studio** 工具集。
2. 项目主要依赖 Qt 标准库与串口模块，请确保 Qt 安装时勾选了相关组件。

### 构建步骤

1. 使用 Visual Studio 打开 `SNK/SNK.sln` 或 `SNK/SNK.vcxproj` 解决方案文件。
2. 在 Visual Studio 中选择合适的配置（Debug/Release）和平台（x86/x64）。
3. 点击“生成” -> “生成解决方案”。
4. 构建完成后，可执行文件将生成在输出目录中。
5. 运行前，需将 `Vikey/dll/` 目录下的 `ViKey.dll` 及项目所需的资源文件放置于可执行文件同级目录。

### 环境要求

- **操作系统**：Windows
- **硬件依赖**：项目需要连接串口硬件（如投币器、按键板）才能完整体验核心功能。
- **加密狗**：部分商业功能依赖 ViKey 加密狗，测试或学习用途可忽略。

------

## 贡献与授权

本项目为一个功能完整、模块划分清晰的桌面应用项目，可作为 **Qt 开发学习、游戏自动化研究** 或 **串口通信实践** 的参考。