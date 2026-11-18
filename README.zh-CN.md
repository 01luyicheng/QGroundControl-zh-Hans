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

## 构建

本仓库使用上游自带的 composite actions 构建(Qt 6.12)。推 `zh-v*` 格式的 tag 会触发 `release-zh.yml` 自动构建 Linux AppImage 与 Windows 安装包并发布 Release。

```bash
cmake -B build --preset Linux
cmake --build build
cmake --install build --config Release   # 生成 AppImage
```

## 许可

遵循上游双许可(Apache-2.0 / GPL-3.0,见 [LICENSE-APACHE](LICENSE-APACHE)、[LICENSE-GPL](LICENSE-GPL)),版权与功劳属于 QGroundControl 的原作者与贡献者。本仓库的翻译改动同样以相同许可发布。
