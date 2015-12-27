========================================================================
    WIN32 应用程序：S-GUI 项目概述
========================================================================

This is a Small Graphical User Interface.

说明：
  这是一款面向嵌入式系统的GUI库在PC上的模拟工程。工程目录下的GUI文件夹就是这个GUI库的源码。
  本工程之前在STM32F429Discovery开发板上运行。

本项目在Visual Studio 2015上开发。

项目的头文件包含相对路径为：
 GUI\Config
 GUI\Core\inc
 GUI\Font
 GUI\Widget

工程配置:
 1.在 项目 -> S-GUI属性 -> C/C++ -> 常规 -> 附加包含目录 输入 "GUI\Config;GUI\Core\inc;GUI\Font;GUI\Widget;%(AdditionalIncludeDirectories)"。
 2.在 项目 -> S-GUI属性 -> C/C++ -> 预编译头 -> 预编译头 输入 "不使用预编译头"。

建议使用空格代替Tab。
