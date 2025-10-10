MouseLikeTouchPad_I2C Hid Driver for windows仿鼠标式触摸板驱动

仿鼠标式触摸板是一种模拟鼠标功能的触摸板技术实现，当前的逻辑实现版本是发明人基于仿鼠标触摸板专利技术根据人手指操作时自然状态再次优化改进而成，3指完成鼠标左键/右键/中键和指针的操作，手指与鼠标的各个按键等部件的功能一一对应，其中的中指对应鼠标的指针定位器，食指对应鼠标左键和中键（食指与中指分开时定义为鼠标左键，食指与中指并拢时定义为鼠标中键），无名指对应鼠标右键，中指无名指或者中指与食指2指一起快速触摸后滑动操作时对应鼠标垂直滚轮和水平滚轮，单指重按触控板左下角物理键为鼠标的后退功能键，单指重按触控板右下角物理键为鼠标的前进功能键，单指重按触控板下沿中间物理键为调节鼠标DPI灵敏度（慢/中等/快3段灵敏度），双指重按触控板下沿物理键为滚轮开关（开启鼠标滚轮及触摸板手势功能方便日常应用操作，临时关闭鼠标滚轮及触摸板手势功能以降低游戏误操作率），三指重按触控板下沿物理键为切换滚轮的操作方式（精确式触摸板双指滑动手势/模仿鼠标的滚轮操作，方便少数对触控板双指滑动支持差的应用如PTC Creo可以使用模仿的鼠标滚轮操作），四指重按触控板下沿物理键为切换仿鼠标式触摸板操作方式的开关（关闭时恢复为windows系统原本的触控板方式以方便其他未学习仿鼠标式触摸板操作方法的用户使用，同时也能通过对比体现出仿鼠标式触摸板驱动的强大之处）。

手指与鼠标按键功能定义规则： 
根据鼠标左键/中键普遍用食指操作的习惯、手指自然状态来区分确定鼠标左键/右键/中键操作与滚动操作；中指对应鼠标的指针定位器；中指被定义为指针后食指首次接触到触控板的时间与中指定义为指针的时间差超过阈值常量时食指操作定义 为鼠标左键或中键（食指与中指为分开状态时食指操作定义为鼠标左键，食指与中指并拢状态时食指操作定义为鼠标中键，左键和中键互相切换需要抬起食指后进行改变，即食指与中指在分开、并拢状态之间滑动切换时左键中键定义保持不变）；中指被定义为指针后无名指首次接触到触控板的时间与中指定义为指针的时间差超过阈值常量时无名指操作定义为鼠标右键，指针被定义后无名指/食指首次接触到触控板的时间与中指定义为指针的时间差小于阈值常量时（中指无名指或者中指与食指2指一起快速触摸后滑动操作）定义为鼠标垂直或水平滚轮（正常情况中指无名指或者中指与食指2指一起滑动操作的舒适性最高）。

多点电容式触摸板根据触摸点接触面形状很容易解决手掌的误触（打字时触摸板支撑手掌的椭圆接触面的长宽比特征比正常手指大所以很容易排除过滤掉）。
因为仿鼠标式触摸板驱动的指针移动功能采用鼠标数据报告集合所以windows触摸板设置的更改光标速度选项无效而是通过调节鼠标设置里的光标速度以及单指重按触摸板下沿调节DPI灵敏度方式。

本驱动只适合部分匹配的笔记本电脑型号，硬件兼容标准以安装成功并运行正常来判别。
sys驱动程序文件采用微软认证的数字签名证书安全可靠，如果浏览器下载和windows安全中心弹出警告威胁属于误报，无需担心安全问题。


驱动下载：
复制下面的链接到浏览器地址栏后回车进入下载页面
https://github.com/MouseLikeTouchPad/MouseLikeTouchPad_I2C/releases
选择MouseLikeTouchPad_I2C_Driver_MLTP0001.zip安装包直接下载

安装和卸载方法：
1.先准备一个外置鼠标避免安装过程中失败无法操作电脑（此驱动不会损坏电脑硬件或数据，只是当驱动不匹配时触摸板失效而已，如果用户精通键盘快捷键操作也可以不需要鼠标）。
2.下载的zip文件驱动程序压缩包解压到任意位置后打开，进入MouseLikeTouchPad_I2C_Driver子文件夹，双击SetSensitivity.reg文件导入注册表设置（该文件用来设置windows资源管理器显示扩展名和触摸板灵敏度）。
3.右键MouseLikeTouchPad_I2C.inf文件弹出的快捷菜单中选择”安装“，在弹出的”用户账户控制“窗口中选择”是“以允许安装硬件设备驱动，若弹出“操作成功完成"提示窗口表示驱动安装成功。
4.右键点击windows开始菜单选择”设备管理器“打开，点击”人体学输入设备“列表展开，如果下方有”MouseLikeTouchPad_I2C“设备出现表示成功安装驱动程序，重新启动计算机后即可使触摸板驱动程序生效，如果”人体学输入设备“列表内没有”MouseLikeTouchPad_I2C“设备出现则需要手动强制更新驱动。
5.手动更新驱动需要确保没有外接的触摸板处于连接状态，然后在“人体学输入设备”子列表里双击打开”符合HID标准的触摸板“，切换到”详细信息“标签页，在”属性“下拉列表里选择”父系“，下方显示的”值“既是触摸板的I2C设备实例路径，也就是MouseLikeTouchPad_I2C驱动程序对应的目标设备，复制粘贴到文本文件记录下该值以便后续查找该设备。
6.在”人体学输入设备“列表里找到I2C HID设备打开其属性并切换到详细信息，属性下拉列表选择“设备实例路径”，查看下方的值是否和刚刚找到的触摸板的I2C设备实例路径相同，如果相同则跳到第7步直接强制更新设备驱动，如果不同则继续查找目标设备，依次在”人体学输入设备“列表里打开每个子设备查看该设备的设备实例路径”直到找到目标，如果都没有则表示该电脑触摸板不是I2C HID设备所以驱动没办法支持。
7.找到的触摸板I2C设备切换到驱动程序标签页点击"更新驱动程序"按钮再选择“浏览我的电脑以查找驱动程序”、选择“让我从计算机的可用驱动程序列表中选取”、去掉勾选的“显示兼容硬件”复选框然后在厂商中选择jxley.HRP再选中右侧型号的“MouseLikeTouchPad_I2C”点下一步、弹出的”更新驱动程序警告“窗口选择是以继续安装驱动即可完成安装，“人体学输入设备”子列表内显示出来"MouseLikeTouchPad_I2C“设备后重启电脑即可使驱动程序生效。
8.驱动的卸载方法为打开设备管理器里“人体学输入设备”子列表内的"MouseLikeTouchPad_I2C“设备属性页面、切换到”驱动程序“标签页点击”卸载设备“按钮、在“卸载设备”对话框中勾选”尝试删除此设备的驱动程序“复选框然后点击”卸载“按钮。



使用操作视频教程网址：
https://space.bilibili.com/3546916633709432
https://www.youtube.com/channel/UC3hQyN-2ZL_q7pCKoASAblQ


MouseLikeTouchPad_I2C Hid Driver for windows仿鼠标式触摸板驱动

The MouseLikeTouchPad is a touchpad technology implementation that simulates the function of the mouse. The current logic implementation version is optimized and improved again by the inventor based on the patented technology of the mouse like touchpad according to the natural state of human fingers during operation. 3 fingers refers to the completion of the operation of the left / right / middle keys of the mouse. The fingers correspond to the functions of various buttons and other parts of the mouse one by one. The middle finger corresponds to the pointer locator of the mouse, The index finger corresponds to the left button and the middle button of the mouse (when the index finger and the middle finger are separated, it is defined as the left button of the mouse, and when the index finger and the middle finger are close together, it is defined as the middle button of the mouse), the ring finger corresponds to the right button of the mouse, and the middle finger ring finger or the middle finger and the index finger 2 together after quick touch correspond to the vertical wheel and the horizontal wheel of the mouse during sliding operation. Pressing the physical key at the lower left corner of the touch panel with a single finger is the backward function key of the mouse, Pressing the physical key at the lower right corner of the touch panel with a single finger is the forward function key of the mouse, pressing the middle physical key at the lower edge of the touch panel with a single finger is to adjust the DPI sensitivity of the mouse (slow / medium / fast 3-stage sensitivity), and pressing the physical key at the lower edge of the touch panel with a double finger is the wheel switch (turning on the mouse wheel and the touch pad gesture function is convenient for daily application and operation, and temporarily turning off the mouse wheel and the touch pad gesture function to reduce the misoperation rate of the game), Pressing the physical key on the lower edge of the touch pad with three fingers is the operation mode to switch the wheel (accurate touch pad two finger sliding gesture / imitating the wheel operation of the mouse, which is convenient for a few applications with poor support for the two finger sliding of the touch pad, such as PTC Creo, which can use the simulated mouse wheel operation), Press the physical key on the lower edge of the touch panel with four fingers to switch the operation mode of the mouse like touch panel (when it is closed, it will revert to the original touch panel mode of the windows system to facilitate the use of other users who have not learned the operation method of the mouse like touch panel. At the same time, it can also reflect the power of the mouse like touch panel drive through comparison).

The mouse like touchpad is a touchpad technology implementation that simulates the function of the mouse. The current logic implementation version is optimized and improved again by the inventor based on the patented technology of the mouse like touchpad according to the natural state of human fingers during operation. 3 refers to the completion of the operation of the left / right / middle keys and fingers of the mouse. The fingers correspond to the functions of various buttons and other parts of the mouse one by one. The middle finger corresponds to the pointer locator of the mouse, The index finger corresponds to the left button and the middle button of the mouse (when the index finger and the middle finger are separated, it is defined as the left button of the mouse, and when the index finger and the middle finger are close together, it is defined as the middle button of the mouse), the ring finger corresponds to the right button of the mouse, and the middle finger ring finger or the middle finger and the index finger 2 together after quick touch correspond to the vertical wheel and the horizontal wheel of the mouse during sliding operation. Pressing the physical key at the lower left corner of the touch panel with a single finger is the backward function key of the mouse, Pressing the physical key at the lower right corner of the touch panel with a single finger is the forward function key of the mouse, pressing the middle physical key at the lower edge of the touch panel with a single finger is to adjust the DPI sensitivity of the mouse (slow / medium / fast 3-stage sensitivity), and pressing the physical key at the lower edge of the touch panel with a double finger is the wheel switch (turning on the mouse wheel and the touch pad gesture function is convenient for daily application and operation, and temporarily turning off the mouse wheel and the touch pad gesture function to reduce the misoperation rate of the game), Pressing the physical key on the lower edge of the touch pad with three fingers is the operation mode to switch the wheel (accurate touch pad two finger sliding gesture / imitating the wheel operation of the mouse, which is convenient for a few applications with poor support for the two finger sliding of the touch pad, such as PTC Creo, which can use the simulated mouse wheel operation), Press the physical key on the lower edge of the touch panel with four fingers to switch the operation mode of the mouse like touch panel (when it is closed, it will revert to the original touch panel mode of the windows system to facilitate the use of other users who have not learned the operation method of the mouse like touch panel. At the same time, it can also reflect the power of the mouse like touch panel drive through comparison).

The multi-point capacitive touch pad can easily solve the wrong touch of the palm according to the shape of the contact surface of the touch point (the aspect ratio of the elliptical contact surface of the palm supported by the touch pad during typing is larger than that of the normal finger, so it is easy to eliminate the filter).

Because the pointer movement function driven by the MouseLikeTouchPad adopts the mouse data report set, the option to change the cursor speed in the windows touch pad setting is invalid. Instead, adjust the DPI sensitivity by adjusting the cursor speed in the mouse setting and pressing the lower edge of the touch pad with a single finger.

This driver is only suitable for partially matched laptop models, and hardware compatibility standards are determined by successful installation and normal operation.

The sys driver file uses a Microsoft Certified Professional certified digital signature certificate that is secure and reliable. If the browser downloads and the Windows Security Center pops up a warning threat that is a false alarm, there is no need to worry about security issues.

Description of project folder classification:


Driver download：
Copy the following link to the browser address bar and press enter to enter the download page
https://github.com/MouseLikeTouchPad/MouseLikeTouchPad_I2C/releases
Select "MouseLikeTouchPad_I2C_Driver_MLTP0001.zip" installation package to download directly


Installation and uninstallation methods:
1. Prepare an external mouse first to avoid installation failures and inability to operate the computer (this driver will not damage the computer hardware or data, but the touchpad will only fail when the driver does not match. If the user is proficient in keyboard shortcut operations, they may not need a mouse).
2. Extract the downloaded zip file driver package to any location and open it. Go to the MouseLikeTouchPad-I2C-Driver sub folder, double-click the SetSensitivity.Reg file to import the registry settings (which is used to set the Windows Explorer display extension and touchpad sensitivity).
3. Right click on the MouseLikeTouchPad_I2C. inf file and select "Install" from the shortcut menu. In the "User Account Control" window that pops up, select "Yes" to allow the installation of hardware device drivers. If a "The operation completed successfully" prompt window pops up, it indicates that the driver installation is successful.
4. Right click on the Windows Start menu and select "Device Manager" to open it. Click on the "Human Input Devices" list to expand it. If there is a "MouseLikeTouchPade_I2C" device below, it indicates that the driver has been successfully installed. After restarting the computer, the touchpad driver can take effect. If there is no "MouseLikeTouchPad_I2C" device in the "Human Input Devices" list, you need to manually update the driver.
5. To manually update the driver, it is necessary to ensure that no external touchpad is connected. Then, double-click to open the "HID compliant touchpad" in the "Human input device" sublist, switch to the "Details" tab, select "Parent" in the "Property" drop-down list, and the "value" displayed below is both the I2C device instance path of the touchpad, which corresponds to the target device of the MouseLikeTouchPad_I2C driver program. Copy and paste the value into a text file to record it for subsequent device search.
6. Find the I2C HID device in the "Human Input Devices" list, open its properties, and switch to "Details". Select "Device instance path" from the drop-down list of "Property", and check if the value below is the same as the I2C device instance path of the just found touchpad. If it is the same, skip to step 7 and directly update the device driver. If it is different, continue to search for the target device. Open each sub device in the "Human Input Devices" list to view the device instance path of the device until the target is found. If it is not, it means that the computer touchpad is not an I2C HID device, so the driver cannot be supported.
7. Switch to the driver tab of the found touchpad I2C device, click on the "Update Driver" button, then select "Browse my computer for driver software", select "Let me pick from a list of device drivers on my computer", uncheck the "Show compatible hardware" checkbox,  then select jxley.HRP from the Manufacturer  and selects the "MouseLikeTouchPad_I2C" Model on the right and clicks "Next". The "Update Driver Warning" window pops up, and selecting "Yes" to continue installing the driver will complete the installation. The "Human Input Devices" sublist displays "MouseLikeTouchPad_I2C" device. After restarting the computer, the driver will take effect.
8. The uninstallation method for drivers is to open the "MouseLikeTouchPad_I2C" device properties page in the "Human Input Devices" sublist in the Device Manager, switch to the "Drivers" tab, click the "Uninstall Device" button, select the "Delete the driver software for this device" checkbox in the "Uninstall Device" dialog box, and then click the "Uninstall" button.

the operation video tutorial website:
https://space.bilibili.com/3546916633709432
https://www.youtube.com/channel/UC3hQyN-2ZL_q7pCKoASAblQ