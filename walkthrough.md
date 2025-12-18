# 虚拟触摸板驱动转换指南

本文档概述了将 `MouseLikeTouchPad_I2C` 驱动程序转换为虚拟的、根枚举（Root-enumerated）的精确式触摸板（PTP）驱动程序所做的更改，并提供了构建和使用说明。

## 修改概述

### 1. 移除硬件依赖
- 从 `MouseLikeTouchPad_I2C.cpp` 和头文件中移除了所有 ACPI 和 SPB (I2C) 代码。
- 移除了 `SpbIoTarget` 的使用以及硬件初始化回调（如 `OnPrepareHardware` 等）。

### 2. 虚拟初始化
- 修改了 `OnDeviceAdd`，将设备初始化为没有任何硬件资源的软件设备。
- 实现了 `HidGetReportDescriptor`，使其返回硬编码的精确式触摸板 (PTP) 报告描述符 (`ParallelMode_PtpReportDescriptor`)。
- 实现了 `HidGetDeviceAttributes`，以返回静态的 VendorID/ProductID。

### 3. 数据注入机制
- 添加了自定义 IOCTL `IOCTL_INJECT_TOUCH_DATA` (0x801)，用于接收来自用户层的触摸数据。
- 暴露了设备接口 `GUID_DEVINTERFACE_MOUSELIKEPTP` (`{2FAC259E-9E9A-4C40-983C-123456789ABC}`) 供应用程序访问。
- 实现了 `OnDeviceControl`，用于解析注入的数据并通过 `SendPtpMultiTouchReport` 转发给操作系统。

### 4. 安装文件更新
- 更新了 `MouseLikeTouchPad_I2C.inf`，使用 `ROOT\MouseLikePTP` 作为硬件 ID。
- 配置 INF 文件以安装 `mshidkmdf` 作为上层过滤器（这是 KMDF HID 迷你驱动程序所必需的）。

---

## 构建和安装说明

### 前置条件
- 安装了带有 WDK (Windows Driver Kit) 的 Visual Studio。建议安装 SDK/WDK 版本 **10.0.22621.0** 以确保最佳兼容性。
- 目标机器已启用“测试签名”模式：
  ```cmd
  bcdedit /set testsigning on
  ```
  (需要重启电脑生效)。
  > **注意**：如果遇到“该值受安全引导策略保护”的错误，您需要在 BIOS/UEFI 设置中**关闭安全启动 (Secure Boot)**。

### 构建驱动程序

#### 选项 A: 云端构建 (GitHub Actions)
如果您本地没有安装 Visual Studio：
1. 将此代码推送到您的 GitHub 仓库。
2. 转到仓库中的 "Actions"（动作）标签页。
3. 选择 "Build Driver"（构建驱动）工作流。
4. 构建完成后，下载 `MouseLikePTP_Driver` 构建产物。
   - 其中包含 `.sys`、`.inf` 驱动文件和 `InjectTouch.exe` 测试工具。

#### 选项 B: 本地构建 (Visual Studio)
1. 在 Visual Studio 中打开解决方案文件。
2. 构建项目（选择 x64/Release 或 Debug）。
3. 输出文件（`MouseLikeTouchPad_I2C.sys`, `.inf`, `.cat`）将位于构建目录中。

### 安装驱动程序
### 安装驱动程序

由于这是一个根枚举设备（虚拟设备），您有两种安装方式。

#### 方法 A：使用设备管理器（推荐，无需额外工具）
1. 打开 **设备管理器** (Win + X -> M)。
2. 点击菜单栏的 **操作 (Action)** -> **添加过时硬件 (Add legacy hardware)**。
   - *（如果没有看到“操作”菜单，请先点击一下右侧列表中的任意设备选中它）*
3. 点击 **下一步**。
4. 选择 **安装我手动从列表选择的硬件(高级)**，点击 **下一步**。
5. 选择 **显示所有设备**，点击 **下一步**。
6. 点击 **从磁盘安装 (Have Disk...)**。
7. 点击 **浏览**，找到并选中您下载/编译的 `MouseLikeTouchPad_I2C.inf` 文件。
8. 此时应显示 "MouseLikePTP Virtual Touchpad"，点击 **下一步** 完成安装。

#### 方法 B：使用 `devcon` 命令（仅限已安装 WDK 的用户）
如果您安装了 WDK，可以在 WDK 安装目录下找到 `devcon.exe` (例如 `C:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe`)。

1. 以管理员身份打开命令提示符 (CMD)。
2. 运行：
   ```cmd
   "C:\path\to\devcon.exe" install MouseLikeTouchPad_I2C.inf ROOT\MouseLikePTP
   ```

### 使用 `InjectTouch.exe` 进行测试
1. 导航到项目中的 `Test` 目录。
2. 编译测试应用程序（使用 VS 或 cl.exe）：
   ```cmd
   cl InjectTouch.cpp user32.lib setupapi.lib
   ```
   *(注：如果是云端构建，直接使用下载的 `InjectTouch.exe` 即可)*
3. 运行 `InjectTouch.exe`。
4. 它应该能成功按打开设备并模拟触摸输入（您应该能看到光标移动或手势效果）。

## 验证
## 验证安装是否成功

1. 打开 **设备管理器**。
2. 展开 **人机接口设备 (Human Interface Devices)** 类别。
3. 查找名为 **"MouseLikeTouchPad PTP Device"** 的设备。
   - **✅ 成功状态**：设备图标上**没有黄色感叹号**，右键属性显示“这个设备运转正常”。
   - **❌ 失败状态 (黄色感叹号)**：
     - **代码 52 (Code 52)**：数字签名验证失败。请检查是否已开启测试签名模式 (`bcdedit /set testsigning on`)，并确保 BIOS 中关闭了 Secure Boot。
     - **代码 10 (Code 10)**：设备无法启动。通常意味着驱动程序内部初始化失败。
