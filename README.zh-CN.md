# QGroundControl 简体中文汉化版(zh-CN Localization)

本仓库是 [mavlink/qgroundcontrol](https://github.com/mavlink/qgroundcontrol) 的简体中文汉化版,**非官方**构建。在上游 master 基础上完成了一次系统性翻译审核与修复,构建产物由 GitHub Actions 自动发布到 [Releases](../../releases)。

## 相比上游的改动

### 1. 翻译修正(准确性)
- 修正 138 处错译(`qgc_source_zh_CN.ts` 130 条 + `qgc_json_zh_CN.ts` 8 条),典型问题包括:
  - 术语错误:「点式车辆」→「载具指向」、UTM「时区」→「投影带」、摇杆「水平」→「偏航」、直升机「集体稳定曲线」→「总距稳定曲线」
  - 机翻破碎:「内部错误:呼叫车辆_requestNextMission…」等
  - 占位符被篡改、「值: %1%」凭空多百分号、译文混入 `&apos;s`、零宽字符、繁体混入等
- 批量补翻 755 条高可见度条目(两轮:450 条工具栏/链路/电池/规划/校准/RTK/日志,305 条蓝牙/机架固件插件/云台/电调/安全组件等),未翻译条数从约 2386 条降至约 1644 条。

### 2. i18n 改造
- 为 25 处未包裹翻译函数的用户可见字符串补上 `qsTr()`(QML)/ `tr()` / `QCoreApplication::translate()`(C++),涉及 VehicleConfigView、BluetoothSettings、ParameterEditorController、MAVLinkProtocol、QGCMapEngineManager、FirmwareImage 等。
- 顺带修复「半句在 qsTr 外拼接」导致的语序硬伤(如 "… tiles)"、"… [Connected]" 片段),改为完整的 `%1` 模板。

## 使用方法

| 平台 | 产物 | 说明 |
|---|---|---|
| Windows x64 | `QGroundControl-installer-AMD64.exe` | 直接安装(未签名,SmartScreen 提示时选择「仍要运行」) |
| Linux x64 | `QGroundControl-x86_64.AppImage` | `chmod +x` 后直接运行(glibc 2.38+) |

首次启动后在 应用设置 → General → Language 中选择简体中文。

### 3. 中国大陆改版默认值（开箱即用，无需申请API）

为适配国内使用，本版默认值与上游不同，默认即可用，无需申请任何 API Key：

| 项目 | 上游默认 | 本版默认 | 说明 |
|---|---|---|---|
| 地图提供商 | Bing（国内不可访问，会白屏） | **OpenStreetMap（免费，无需Token）** | 国际开源地图，国内可访问，无需 Token |
| 地图类型 | Hybrid | **Map（道路图）** | OpenStreetMap 道路图 |
| 界面语言 | 跟随系统 | **简体中文** | 首次启动即中文 |

#### 地图选择

- **OpenStreetMap（默认）**：免费、无需 Token，国内可访问，开箱即用。
- **天地图 (TianDiTu)**：中国官方地图，国内访问质量更好，但需要免费 API Token。如需使用，去 [天地图开放平台](https://lbs.tianditu.gov.cn) 申请免费 Key，填入 **General → Maps → TianDiTu Token**，然后将 Map Provider 切换为 TianDiTu。

如需使用其他地图（Bing、Google、Mapbox、Esri、VWorld 等），在 **General → Maps → Map Provider** 下拉框切换即可。这些海外地图在国内可能访问受限。

## 与上游同步

本 fork 以某一上游 master 快照为基线,且直接修改了上游文件,同步时以下位置最易冲突:

- `src/Settings/App.SettingsGroup.json`、`src/Settings/AppSettings.cc`(语言默认值等 fork 改动)
- `translations/qgc_source_zh_CN.ts`、`qgc_json_zh_CN.ts`(上游 Crowdin 会定期重写)
- 做过 `qsTr()`/`tr()` 包裹改造的 `src` 源文件

发布前执行 `git fetch origin && git rebase origin/master` 同步上游。ts 文件冲突时保留本地译文、再合并上游新增条目(可借助 `lupdate`)。注意:界面语言默认值改动依赖 Qt6 的 `QLocale::Chinese` 枚举数值(= 58,JSON 里硬编码),升级 Qt 大版本时需复核该数值是否变化。

## 构建

本仓库使用上游自带的 composite actions 构建(Qt 6.12)。推 `zh-v*` 格式的 tag 会触发 `release-zh.yml` 自动构建 Linux AppImage 与 Windows 安装包并发布 Release。

```bash
cmake -B build --preset Linux
cmake --build build
cmake --install build --config Release   # 生成 AppImage
```

## 许可

遵循上游双许可(Apache-2.0 / GPL-3.0,见 [LICENSE-APACHE](LICENSE-APACHE)、[LICENSE-GPL](LICENSE-GPL)),版权与功劳属于 QGroundControl 的原作者与贡献者。本仓库的翻译改动同样以相同许可发布。
