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
- 安装了带有 WDK (Windows Driver Kit) 的 Visual Studio。
- 目标机器已启用“测试签名”模式：
  ```cmd
  bcdedit /set testsigning on
  ```
  (需要重启电脑生效)。

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
由于这是一个根枚举设备（虚拟设备），请使用 `devcon` 工具（包含在 WDK 中）进行安装：

1. 以管理员身份打开命令提示符 (CMD)。
2. 导航到包含已构建驱动文件的文件夹。
3. 运行：
   ```cmd
   devcon install MouseLikeTouchPad_I2C.inf ROOT\MouseLikePTP
   ```
4. 在弹出的安装提示中确认安装。

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
- 检查设备管理器：在“人机接口设备”或“鼠标和其他指针设备”下应该会出现一个新的 HID 设备。
- 使用 `Device Manager` 或 `moumon` 等工具验证操作系统是否正在接收输入报告。
