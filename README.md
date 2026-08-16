<p align="center">
<img width="210" height="210" align="left" style="float: left; margin: 0 10px 0 0;" src="https://raw.githubusercontent.com/ruizhi-lab/Plumbum/dev/assets/icons/plumbum.png" alt="Plumbum"/>
</br>
<h1>Plumbum - Unleash Your Xray & V2Ray</h1>
基于 <b>Qt6</b> 的跨平台 <b>Xray / V2Ray</b> 客户端，支持现代暗色 QML 界面。
</br>
内核支持 <b>Xray-core</b> 与 <b>V2Ray-core</b>（v5 配置格式）
</p>

## 特性

- 🖥️ **现代 Qt6 QML 界面** — 暗色主题、侧边栏导航、卡片式连接列表
- 🚀 **双内核支持** — Xray-core / V2Ray-core 自动适配（含 Xray 26.x）
- 🔌 **多协议** — VMess、VLESS、Shadowsocks、Trojan、HTTP、SOCKS 等
- 📡 **订阅管理** — 订阅分组、一键更新
- 📊 **实时流量统计** — gRPC API 实时速度与总流量显示
- ⚡ **延迟测试** — 连接级延迟检测

## 构建

要求：Qt 6.5+、CMake 3.20+、gRPC、protobuf

```bash
# 克隆并初始化子模块
git clone --recurse-submodules https://github.com/ruizhi-lab/Plumbum.git
cd Plumbum

# QML 界面构建
cmake -B build -DPLUMBUM_QT6=ON -DPLUMBUM_UI_TYPE=QML -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# QWidget 界面构建（原有界面）
cmake -B build-widget -DPLUMBUM_QT6=ON -DPLUMBUM_UI_TYPE=QWidget -DCMAKE_BUILD_TYPE=Release
cmake --build build-widget -j$(nproc)
```

## 使用

1. 在「设置」页配置内核路径（如 `/usr/bin/xray` 或 `/usr/bin/v2ray`）与 geo 数据目录
2. 在「连接」页导入分享链接（`vmess://`、`vless://`、`ss://`、`trojan://`）
3. 点击「Connect」即可连接，默认 SOCKS 代理端口 1089、HTTP 端口 8889

## 项目结构

```
src/
├── base/            # 基础配置与数据模型
├── core/            # 连接管理、内核交互、配置生成
├── components/      # 插件、订阅、延迟测试等
├── plugins/         # 内置协议插件
└── ui/
    ├── qml/         # 现代 Qt6 QML 界面
    ├── widgets/     # 原 QWidget 界面
    └── cli/         # 命令行界面
```

## Star History

![stars](https://starchart.cc/Plumbum/Plumbum.svg)
