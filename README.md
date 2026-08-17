# CoinFlip

CoinFlip 是一个使用 Qt/C++ 编写的翻金币益智小游戏。玩家选择关卡后进入 4x4 棋盘，点击任意金币会翻转当前金币以及上下左右相邻金币，当所有金币都翻到正面时通关。

## 功能特性

- 主菜单、关卡选择、游戏场景三段式界面
- 内置 20 个关卡数据
- 4x4 翻金币玩法
- 金币逐帧翻转动画
- 按钮弹跳动画与按下状态切换
- 点击、返回、翻金币、胜利音效
- 通关后显示胜利动画

## 技术栈

- C++17
- Qt Widgets
- Qt Multimedia
- qmake 项目结构
- Qt Resource System 资源管理

## 项目结构

```text
CoinFlip/
├── main.cpp                 # 程序入口
├── CoinFlip.pro             # qmake 项目配置
├── mainscene.*              # 主菜单场景
├── chooselevelscene.*       # 关卡选择场景
├── playscene.*              # 游戏场景与胜利逻辑
├── mypushbutton.*           # 自定义图片按钮
├── mycoin.*                 # 金币按钮与翻转动画
├── dataconfig.*             # 20 个关卡的初始化数据
├── mainscene.ui             # 主界面 UI 文件
├── res.qrc                  # Qt 资源文件
└── res/                     # 图片与音效资源
