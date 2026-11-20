# QGroundControl 简体中文汉化版(zh-CN Localization)

本仓库是 [mavlink/qgroundcontrol](https://github.com/mavlink/qgroundcontrol) 的简体中文汉化版,**非官方**构建。在上游 master 基础上完成了一次系统性翻译审核与修复,构建产物由 GitHub Actions 自动发布到 [Releases](../../releases)。

## 相比上游的改动

### 1. 翻译修正(准确性)
- 修正 138 处错译(`qgc_source_zh_CN.ts` 130 条 + `qgc_json_zh_CN.ts` 8 条),典型问题包括:
  - 术语错误:「点式车辆」→「载具指向」、UTM「时区」→「投影带」、摇杆「水平」→「偏航」、直升机「集体稳定曲线」→「总距稳定曲线」
  - 机翻破碎:「内部错误:呼叫车辆_requestNextMission…」等
  - 占位符被篡改、「值: %1%」凭空多百分号、译文混入 `&apos;s`、零宽字符、繁体混入等
- 批量补翻 450 条高可见度条目(工具栏状态词、链路/电池告警、规划视图、遥控器校准、RTK 诊断、日志查看器等),未翻译比例从 58% 降至约 48%。

### 2. i18n 改造
- 为 25 处未包裹翻译函数的用户可见字符串补上 `qsTr()`(QML)/ `tr()` / `QCoreApplication::translate()`(C++),涉及 VehicleConfigView、BluetoothSettings、ParameterEditorController、MAVLinkProtocol、QGCMapEngineManager、FirmwareImage 等。
- 顺带修复「半句在 qsTr 外拼接」导致的语序硬伤(如 "… tiles)"、"… [Connected]" 片段),改为完整的 `%1` 模板。

## 使用方法

| 平台 | 产物 | 说明 |
|---|---|---|
| Windows x64 | `QGroundControl-installer-AMD64.exe` | 直接安装(未签名,SmartScreen 提示时选择「仍要运行」) |
| Linux x64 | `QGroundControl-x86_64.AppImage` | `chmod +x` 后直接运行(glibc 2.38+) |

首次启动后在 应用设置 → General → Language 中选择简体中文。

### 3. 中国大陆改版默认值

为适配国内使用，本版默认值与上游不同：

| 项目 | 上游默认 | 本版默认 | 说明 |
|---|---|---|---|
| 地图提供商 | Bing（国内不可访问，会白屏） | **天地图 (TianDiTu)** | 中国官方地图服务，国内可正常访问 |
| 地图类型 | Hybrid | **Road（道路图）** | 天地图只有 Road/Satellite，无 Hybrid |
| 界面语言 | 跟随系统 | **简体中文** | 首次启动即中文 |

#### 天地图 Token 申请

天地图需要 API Token 才能显示地图。**首次启动后请务必按以下步骤设置：**

1. 打开 QGC → 右上角齿轮 → **General → Maps**
2. 在 **Tokens** 区块找到 **TianDiTu Token** 输入框
3. 去 [天地图开放平台](https://lbs.tianditu.gov.cn) 注册账号，在「开发资源」→「Key 管理」中申请一个免费 Key（类型选「地图服务」，配额默认 30 万/天，足够日常使用）
4. 把 Key 粘贴到 TianDiTu Token 输入框，重启 QGC 即可正常显示地图

> **重要**：Token 缺失时天地图瓦片为空，地图区域会显示白屏（与 Bing 国内白屏现象相同）。填入 Token 前请勿误以为软件有 bug。

#### 地图切换

如需使用其他地图（Bing、Google、Mapbox、Esri、VWorld 等），在 **General → Maps → Map Provider** 下拉框切换即可。这些海外地图在国内可能访问受限。

## 构建

本仓库使用上游自带的 composite actions 构建(Qt 6.12)。推 `zh-v*` 格式的 tag 会触发 `release-zh.yml` 自动构建 Linux AppImage 与 Windows 安装包并发布 Release。

```bash
cmake -B build --preset Linux
cmake --build build
cmake --install build --config Release   # 生成 AppImage
```

## 许可

遵循上游双许可(Apache-2.0 / GPL-3.0,见 [LICENSE-APACHE](LICENSE-APACHE)、[LICENSE-GPL](LICENSE-GPL)),版权与功劳属于 QGroundControl 的原作者与贡献者。本仓库的翻译改动同样以相同许可发布。
