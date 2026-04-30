# AXCL 新增工具链接入说明

本文档说明如何在 AXCL 现有 build 系统中接入新的 host 工具链。示例场景为 OpenWrt arm64 gcc。

> **Tip**
> 新增工具链前，建议先完整执行一次现有 `host=x86` 编译，保留一份可工作的基线结果，后续对比会更直接。

## 现有编译系统与 x86_64 示例

### 参考编译选项

| host 参数 | 说明 | 输出目录 |
| --- | --- | --- |
| `x86` | x86_64 | `out/axcl_linux_x86` |
| `arm64` | aarch64 | `out/axcl_linux_arm64` |

### 现有编译命令

| host | 编译命令 | 用途 |
| --- | --- | --- |
| `x86` | `cd build && make host=x86 clean all install -j128` | x86_64 基线编译 |
| `arm64` | `cd build && make host=arm64 clean all install -j128` | 现有 arm64 编译 |

### x86_64 编译命令

```bash
cd build && make host=x86 clean all install -j128
```

这一步的作用很直接：先确认当前 build 链路是通的，再开始新增工具链。

### x86_64 输出目录

```text
out/axcl_linux_x86/
├── bin/      # 可执行文件
├── lib/      # 库文件
├── include/  # 头文件
├── ko/       # 内核模块
└── json/     # 配置文件
```

补充说明：
- `out/axcl_linux_x86` 是 x86_64 主输出目录。
- `out/python` 是当前 x86_64 构建附带生成的 Python wheel 输出目录。

## 编译系统主流程

| 阶段 | 文件 | 作用 |
| --- | --- | --- |
| 顶层入口 | `build/Makefile` | 接收 `host` 参数并执行 `clean`、`all`、`install` |
| host 分派 | `build/config.mak` | 解析 `host`，设置 `HOST`，并加载对应 `*_config.mak` |
| 用户态规则 | `build/rules.mak` | 按 `HOST` 转发到对应 `*_rules.mak` |
| 内核态规则 | `build/krules.mak` | 按 `HOST` 转发到对应 `*_krules.mak` |
| host 配置目录 | `build/projects/` | 存放各 host 的 `config`、`rules`、`krules` 文件 |
| 3rdparty 路径选择 | `logger/Makefile`、`protocol/proto/static.mak`、`protocol/package/Makefile`、`test/*/Makefile` 等 | 这些文件里有不少路径直接按 `$(ARCH)` 选择 `3rdparty` 目录 |

这里有两个变量需要分开看：
- `HOST` 用来区分完整构建目标，也决定输出目录名称。
- `ARCH` 用来描述底层架构，很多 3rdparty 头文件和库路径选择仍然依赖它。

对 OpenWrt arm64 场景，建议初版采用下面这组配置：

| 变量 | 建议值 | 说明 |
| --- | --- | --- |
| `HOST` | `openwrt_arm64` | 用于区分新工具链入口和输出目录 |
| `ARCH` | `arm64` |  |

## 3rdparty 处理方式

第三方组件先独立编译，再把安装结果按架构放进 `3rdparty/` 目录，AXCL 主构建阶段主要消费这些现成产物。

只有 `ffmpeg` 比较特殊：仓库里已经带了源码目录，实际构建由 `3rdparty/ffmpeg/build.sh` 单独完成。

### 需要关注的 3rdparty 组件

| 组件 | 当前方式 | 当前版本 | 下载地址 | 接入说明 |
| --- | --- | --- | --- | --- |
| `ffmpeg` | 仓内源码 + 独立构建脚本 | `n7.1` | `3rdparty/ffmpeg/FFmpeg-n7.1/` | OpenWrt 接入时需要重点检查 `3rdparty/ffmpeg/build.sh` 的 configure 参数 |
| `googletest` | 预编译安装结果 | `1.15.0` | `https://github.com/google/googletest/releases/tag/v1.15.0` | 如果现有 `arm64` 产物不能复用，需要重新预编译并补充新的目录选择逻辑 |
| `protobuf` | 预编译安装结果 | `3.20.3` | `https://github.com/protocolbuffers/protobuf/releases/tag/v3.20.3` | 如果现有 `arm64` 产物不能复用，需要重新预编译并补充新的目录选择逻辑 |
| `spdlog` | 预编译安装结果 | `1.14.1` | `https://github.com/gabime/spdlog/releases/tag/v1.14.1` | 如果现有 `arm64` 产物不能复用，需要重新预编译并补充新的目录选择逻辑 |

## 以 OpenWrt arm64 工具链为例新增工具链

### 需要修改和新增的文件

| 类型 | 文件或目录 | 是否必需 | 说明 |
| --- | --- | --- | --- |
| 修改 | `build/config.mak` | 是 | 增加 `openwrt_arm64` 分支，让顶层 make 能识别新的 host |
| 新增 | `build/projects/axcl_linux_openwrt_arm64_config.mak` | 是 | 定义 OpenWrt 工具链变量 |
| 新增 | `build/projects/axcl_linux_openwrt_arm64_rules.mak` | 是 | 用户态规则文件，初版可直接复用 arm64 模板 |
| 新增 | `build/projects/axcl_linux_openwrt_arm64_krules.mak` | 是 | 内核态规则文件，初版可直接复用 arm64 模板 |
| 修改 | `3rdparty` | 是 | 预编译和修改路径 |

### 模板来源

| 新文件 | 推荐模板 |
| --- | --- |
| `axcl_linux_openwrt_arm64_config.mak` | `build/projects/axcl_linux_arm64_config.mak` |
| `axcl_linux_openwrt_arm64_rules.mak` | `build/projects/axcl_linux_arm64_rules.mak` |
| `axcl_linux_openwrt_arm64_krules.mak` | `build/projects/axcl_linux_arm64_krules.mak` |

### 接入步骤

| 步骤 | 操作 | 说明 |
| --- | --- | --- |
| 1 | 执行 `cd build && make host=x86 clean all install -j128` | 先拿到一份可工作的 x86_64 基线结果 |
| 2 | 在 `build/config.mak` 中增加 `openwrt_arm64` 分支 | 让 `make host=openwrt_arm64` 能识别新的 host |
| 3 | 复制 arm64 模板，新增 `axcl_linux_openwrt_arm64_config.mak`、`rules.mak`、`krules.mak` | 为新 host 建立完整入口 |
| 4 | 在新的 `*_config.mak` 中设置 OpenWrt 工具链前缀，并保持 `ARCH=arm64` | 先把工具链差异收敛在配置文件里 |
| 5 | 3rdparty | 预编译和路径修改 |
| 9 | 执行 `cd build && make host=openwrt_arm64 clean all install -j128` | 验证新 host 已完成接入 |
| 10 | 检查 `out/axcl_linux_openwrt_arm64` 下的 `bin`、`lib`、`include`、`ko`、`json` | 确认编译结果符合预期 |

### OpenWrt config 文件关键变量

| 变量 | 作用 | OpenWrt arm64 示例中的处理方式 |
| --- | --- | --- |
| `CROSS` | 工具链前缀 | 修改为 OpenWrt arm64 musl gcc 12.3.0 对应前缀 |
| `CC` | C 编译器 | 通常由 `$(CROSS)gcc` 派生 |
| `CPP` | C++ 编译器 | 通常由 `$(CROSS)g++` 派生 |
| `LD` | 链接器 | 通常由 `$(CROSS)ld` 派生 |
| `AR` | 归档工具 | 通常由 `$(CROSS)ar` 派生 |
| `STRIP` | strip 工具 | 通常由 `$(CROSS)strip` 派生 |
| `OBJCOPY` | objcopy 工具 | 通常由 `$(CROSS)objcopy` 派生 |
| `ARCH` | 架构标识 | 保持为 `arm64` |

### 编译验证

```bash
cd build && make host=openwrt_arm64 clean all install -j128
```

### 结果检查

| 检查项 | 预期结果 |
| --- | --- |
| 输出根目录 | 生成 `out/axcl_linux_openwrt_arm64` |
| `bin` 目录 | 存在可执行文件输出 |
| `lib` 目录 | 存在库文件输出 |
| `include` 目录 | 存在头文件输出 |
| `ko` 目录 | 如果当前流程包含驱动构建，则应存在模块输出 |
| `json` 目录 | 如果当前流程包含配置安装，则应存在 JSON 配置输出 |
