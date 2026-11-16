# AXCL Android arm64 适配补丁

本目录提供 Android arm64 适配补丁及接入步骤。仓库基线源码保持不变，Android 构建功能在应用补丁后启用。
本文介绍 AXCL Runtime 在 Android arm64 环境的适配流程、驱动交付方式和 BSP 系统集成要求，并列出 Orange Pi 5 / Android 12 / Linux 5.10.160 的已实测参考环境。推荐由方案提供方预先准备目标主控匹配的 PCIe 驱动，将 Runtime、工具、五个驱动和同版本 PAC 打进 APK；主控系统集成 AXCL 包管理服务后，用户安装 APK 即可自动部署运行环境。

## Android 适配流程

适配工作分为 BSP/方案提供方、主控系统集成方和最终用户三部分：

| 角色 | 工作内容 | 交付或结果 |
| --- | --- | --- |
| AXCL/BSP 提供方 | 用 Android NDK 为 `arm64-v8a` 编译 AXCL Runtime 和工具；为目标主控内核准备五个 PCIe 驱动；选择同版本 PAC | 包含 Runtime、工具、驱动和 PAC 的 APK，以及配套 OEM 系统集成文件 |
| 主控系统集成方 | 将 AXCL 包管理服务、签名验证器、受信签名摘要及启动配置集成进 Android BSP；配置服务权限、SELinux 和设备访问策略 | 已集成 AXCL 安装服务的主控系统镜像 |
| 最终用户 | 在已集成的主控系统上执行 `adb install -r AXCL-Runtime-*.apk` | 系统服务自动校验 APK、部署 Runtime/PAC、加载驱动；用户无需启动应用或执行安装脚本 |

### 驱动准备方式

适配方按目标 BSP 可选择以下一种方式：

1. **编译机预编译（推荐）**：使用目标内核的 Kbuild 源码/构建输出、最终 `.config` 和配套工具链编译五个 `.ko`，再通过 `--modules-dir` 将其打入 APK。客户主控不需要内核源码或编译器。这是本仓库当前实测采用的交付方式。
2. **目标机现场编译（可选）**：将驱动源码放入 APK，并在目标主控预先部署 make、编译器及与正在运行内核匹配的 Kbuild 资料，再配置 `build.conf`。此方式要求 OEM 提供 Android 本机可运行的构建工具，不能假定普通 Android 镜像已经具备这些条件。

无论采用哪种方式，驱动都必须匹配目标主控的内核 ABI。相同的 `uname -r` 字符串不能单独证明模块兼容；若启用 `CONFIG_MODVERSIONS`，还需对应构建生成的 `Module.symvers`。PAC 必须与打包的 AXCL SDK 版本匹配。

### 安装前提和适用范围

- 目标系统已完成 AXCL OEM 系统服务集成，并信任待安装 APK 的签名。普通 Android 的 `adb install` 不会自行安装 `.ko`、写入 `/vendor` 或赋予应用 root 权限。
- PCIe 硬件、供电、设备树和主控 PCIe 控制器已正确配置；驱动和 PAC 已按目标主控准备。
- APK 可复用于驱动 ABI、Android ABI/API 及 PAC 均匹配的设备组合。更换主控内核、内核配置或 Android BSP 后，应重新核对模块兼容性和 OEM 权限策略。
- 本仓库支持预编译驱动和现场编译两条路径，但现场编译以及其他厂商主控仍须按具体 BSP 验证。

## 适用版本

| 项目 | 版本或要求 |
| --- | --- |
| 源码仓库 | [AXERA-TECH/axcl-runtime](https://github.com/AXERA-TECH/axcl-runtime) |
| SDK 版本 | V3.10.2 |
| 源码基线 commit | `c0a009bb4d5b643db5974b52ce0882404b0bb64c` |
| 编译机 | Linux x86_64 |
| Android 用户态 | arm64-v8a，API 24 及以上 |
| 系统安装服务 | Android 9 / API 28 及以上 |

PAC 文件头中的固件版本可带构建时间后缀，例如 `V3.10.2_20251111020143`；打包器按 SDK 基线版本 `V3.10.2` 校验。

补丁针对上述独立仓库基线制作。完整 SDK 或其他版本的源码应先核对目录、接口和构建规则。

本目录包含：

```text
android-porting/
├── .gitattributes
├── README.md
├── axcl-android.patch
└── SHA256SUMS
```

`axcl-android.patch` 为一个完整补丁，包含 7 个已有文件的修改和 42 个新增文件，包括工具链配置、构建脚本、安装服务、测试及说明文档。
构建产物、预编译依赖、内核模块、PAC 和签名文件不包含在补丁中。

## 1. 准备目标主控及编译材料

确认主控完成 PCIe 供电、复位、时钟和设备树配置，并能在 `/sys/bus/pci/devices` 枚举到 AX650 子卡。

编译前准备以下材料：

| 材料 | 来源 | 用途 |
| --- | --- | --- |
| 与主控运行内核匹配的源码、最终 `.config`、构建输出及工具链 | 目标主控 BSP | 编译五个 PCIe 驱动；启用 `CONFIG_MODVERSIONS` 时需对应的 `Module.symvers` |
| V3.10.2 的 `AX650_card` PAC | 配套 AXCL SDK/固件包 | 启动 AX650 子卡 |
| NDK r29、Python 3.10+、CMake 3.22+、Ninja、GNU Make | 编译机工具环境 | 编译 Android 依赖及 Runtime |
| JDK 17、Android SDK Platform 35、Build Tools 35.0.0 | 编译机工具环境 | 编译和签名 APK |

主控内核构建资料需与实际运行的系统镜像对应。内核版本字符串一致不足以保证模块 ABI 一致。

## 2. 获取源码并应用补丁

获取包含本目录的源码版本：

```bash
git clone --branch main --single-branch --no-tags https://github.com/AXERA-TECH/axcl-runtime.git
cd axcl-runtime
```

以下命令均在仓库根目录执行，适用于与上述基线匹配且尚未应用 Android 补丁的源码。
补丁独立分发时，将完整的 `android-porting/` 目录放到源码根目录。

```bash
# 校验补丁文件
(cd android-porting && sha256sum -c SHA256SUMS)

# 检查补丁是否适用，再应用
git apply --check android-porting/axcl-android.patch
git apply android-porting/axcl-android.patch
```

如果检查报告冲突，先确认源码基线及本地修改，不使用强制覆盖方式应用。
补丁检查通过仅表示文件上下文匹配，其他源码版本仍需独立验证。

应用后将新增 `android/` 和 Android 工具链配置，并更新相应构建入口及兼容处理。
原有 Linux 构建入口保留；Android 构建使用 `OS=android host=arm64`。

## 3. 编译主控 PCIe 驱动

在编译机上使用目标 BSP 的内核工具链。以下示例适用于采用 LLVM 的 BSP，实际工具链及参数以目标 BSP 为准：

```bash
export PATH=/path/to/bsp/clang/bin:$PATH
LLVM=1 LLVM_IAS=1 \
python3 android/build_modules.py \
  --kernel-build /path/to/bsp/kernel-output \
  --output android/.build/board-modules
```

将 `/path/to/...` 替换为实际路径；`--output` 指定一个尚未创建的目录。
输出包括：

```text
ax_pcie_host_dev.ko
ax_pcie_msg.ko
ax_pcie_mmb.ko
axcl_host.ko
ax_pcie_p2p_rc.ko
kernel-release.txt
driver-build-info.json
```

启用模块签名校验的平台，在打包前按照 BSP 的流程对 `.ko` 签名。
驱动针对目标主控内核构建，内核更新后应重新确认兼容性并按需重新构建。

## 4. 编译 Runtime 并生成 APK

设置编译机工具路径，编译依赖和用户态程序：

```bash
export ANDROID_NDK_HOME=/opt/android-ndk-r29
export ANDROID_HOME=/opt/android-sdk
export JAVA_HOME=/opt/jdk-17
export PATH=$JAVA_HOME/bin:$PATH

make -C build OS=android host=arm64 deps
make -C build OS=android host=arm64 all install
```

首次执行 `deps` 会下载并校验指定版本的第三方源码，然后进行 NDK 编译。
用户态结果位于 `out/axcl_android_arm64/`。

将前一步生成的驱动和同版本 PAC 打包：

```bash
python3 android/build_apk.py --skip-native \
  --pac /path/to/ax650_card.pac \
  --modules-dir android/.build/board-modules
```

`android/dist/` 将生成 APK、安装 ZIP、构建记录及 `AXCL-OEM-Integration-*` 系统集成目录。
APK 包含 Runtime、工具、五个驱动及完整 PAC。

构建脚本使用本地生成的评估签名。产品发布时按产品签名流程处理 APK，并同步系统安装服务使用的签名摘要。

## 5. 集成主控系统服务

在目标 BSP 中集成生成的 `AXCL-OEM-Integration-*` 文件：

| 文件 | 系统路径 |
| --- | --- |
| `axcl-package-manager` | `/vendor/bin/axcl-package-manager` |
| `axcl-runtime.rc` | `/vendor/etc/init/axcl-runtime.rc` |
| `axcl-package-verifier.jar` | `/vendor/framework/axcl-package-verifier.jar` |
| `axcl-signing.sha256` | `/vendor/etc/axcl-signing.sha256` |

配置服务的运行身份、模块加载权限、设备访问权限及 SELinux 策略，并部署更新后的系统镜像。
rc 模板中的 SELinux 域用于 userdebug/eng 调试环境，产品系统应配置专用服务域。

此步骤为平台级集成，同一主控系统完成后可复用。详细配置见应用补丁后生成的 `android/platform/README.md`。

## 6. 安装及运行验证

在已完成系统集成的主控上安装对应 APK，文件名以实际产物为准：

```bash
adb install -r AXCL-Runtime-1.6.3-prebuilt-arm64-v8a.apk
```

系统安装服务异步部署 Runtime、PAC 和驱动。使用具有对应访问权限的 shell 检查：

```bash
adb shell cat /data/vendor/axcl-manager/status
adb shell cat /data/vendor/axcl-manager/last-operation.log
adb shell /data/vendor/axcl/current/bin/axcl-smi
```

`status` 为 `installed` 表示 Runtime 部署及驱动加载完成，`axcl-smi` 用于检查子卡运行状态。
平台发布前验证内存/DMA、实际模型推理，以及重启、更新和卸载流程。

## 补丁撤销

需要撤销时，先检查反向应用，再执行：

```bash
git apply --reverse --check android-porting/axcl-android.patch
git apply --reverse android-porting/axcl-android.patch
```

上述命令撤销补丁所涉及的源码改动；构建缓存及已部署到主控的内容另行管理。
补丁应用后产生的其他源码修改应先保存，反向检查失败时先处理相关差异。

## 已实测参考环境：Orange Pi 5

| 项目 | 实测配置或结果 |
| --- | --- |
| 主控系统 | Orange Pi 5，Android 12（API 32），arm64 |
| 运行内核 | Linux `5.10.160` |
| 用户态及固件 | AXCL SDK / Runtime / Driver / PAC `V3.10.2` |
| 驱动准备 | APK 内预置针对该内核构建的五个 PCIe `.ko`；未在 Android 主控上现场编译 |
| OEM 集成 | 使用主控已有的 AXCL 包管理服务；测试时将系统验证器和签名摘要同步为本次评估签名 APK 对应版本 |
| 安装结果 | 卸载旧 APK 后安装本次 APK；系统服务自动部署新 payload、PAC 并加载五个驱动，无需打开应用 |
| 设备检查 | 状态为 `installed`；`axcl-smi` 显示 AXCL/Driver/Firmware `V3.10.2`，识别到 PCIe `0004:41:00.0` 上的子卡 |
| 模型验证 | 板上留存的 AX650 512×288 HVCP 示例模型及参考输出，在新 Runtime 上推理和校验通过，平均耗时约 `0.964 ms` |

本次实测覆盖 Orange Pi 5、Android 12、内核 `5.10.160` 的预编译驱动交付流程，包括 APK 构建、安装、系统服务部署、驱动/PAC 启动和模型推理。测试板原有 OEM 服务；测试时同步更新了其签名摘要和验证器以接受评估签名 APK。其他主控和内核组合、目标机现场编译及产品签名发布需按目标 BSP 验证。未集成 AXCL 系统服务的 Android 镜像不能只通过安装 APK 完成驱动部署。

补丁检查、Android 用户态构建、自动化检查和 APK 签名流程见应用补丁后的 `android/VALIDATION.md`。其他厂商/内核组合、目标机现场编译、冷启动恢复及产品签名发布仍需按目标 BSP 验证。

本配置构建 Runtime、Native 库及不依赖 FFmpeg 的示例，暂不包含 FFmpeg/PPL、Python wheel、SKEL 及完整测试程序集。
