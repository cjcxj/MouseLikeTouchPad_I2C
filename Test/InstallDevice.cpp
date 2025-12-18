#include <windows.h>
#include <setupapi.h>
#include <newdev.h>
#include <iostream>
#include <string>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "newdev.lib")

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: InstallDevice.exe <path_to_inf_file>" << std::endl;
        std::cout << "Example: InstallDevice.exe MouseLikeTouchPad_I2C.inf" << std::endl;
        return 1;
    }

    std::string infPath = argv[1];
    const char* hwid = "ROOT\\MouseLikePTP";

    std::cout << "Installing driver from: " << infPath << std::endl;
    std::cout << "Hardware ID: " << hwid << std::endl;

    // Convert to wide string
    std::wstring wInfPath(infPath.begin(), infPath.end());
    std::wstring wHwid(hwid, hwid + strlen(hwid));

    BOOL reboot = FALSE;
    
    // Install the driver
    if (!UpdateDriverForPlugAndPlayDevicesW(
        NULL,
        wHwid.c_str(),
        wInfPath.c_str(),
        INSTALLFLAG_FORCE,
        &reboot))
    {
        DWORD error = GetLastError();
        std::cerr << "Failed to install driver. Error code: 0x" << std::hex << error << std::endl;
        
        if (error == ERROR_NO_SUCH_DEVINST) {
            std::cout << "\nDevice instance does not exist. Creating new device..." << std::endl;
            
            // Create a new device node
            HDEVINFO DeviceInfoSet = SetupDiCreateDeviceInfoList(NULL, NULL);
            if (DeviceInfoSet == INVALID_HANDLE_VALUE) {
                std::cerr << "Failed to create device info list" << std::endl;
                return 1;
            }

            SP_DEVINFO_DATA DeviceInfoData;
            DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

            if (!SetupDiCreateDeviceInfoW(
                DeviceInfoSet,
                L"ROOT\\MouseLikePTP",
                &GUID_DEVCLASS_HIDCLASS,
                NULL,
                NULL,
                DICD_GENERATE_ID,
                &DeviceInfoData))
            {
                std::cerr << "Failed to create device info" << std::endl;
                SetupDiDestroyDeviceInfoList(DeviceInfoSet);
                return 1;
            }

            // Set hardware ID
            if (!SetupDiSetDeviceRegistryPropertyW(
                DeviceInfoSet,
                &DeviceInfoData,
                SPDRP_HARDWAREID,
                (PBYTE)wHwid.c_str(),
                (DWORD)((wHwid.length() + 2) * sizeof(wchar_t))))
            {
                std::cerr << "Failed to set hardware ID" << std::endl;
                SetupDiDestroyDeviceInfoList(DeviceInfoSet);
                return 1;
            }

            // Register the device
            if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE, DeviceInfoSet, &DeviceInfoData))
            {
                std::cerr << "Failed to register device" << std::endl;
                SetupDiDestroyDeviceInfoList(DeviceInfoSet);
                return 1;
            }

            std::cout << "Device created successfully!" << std::endl;
            SetupDiDestroyDeviceInfoList(DeviceInfoSet);

            // Try installing again
            if (!UpdateDriverForPlugAndPlayDevicesW(
                NULL,
                wHwid.c_str(),
                wInfPath.c_str(),
                INSTALLFLAG_FORCE,
                &reboot))
            {
                std::cerr << "Failed to install driver after creating device. Error: 0x" << std::hex << GetLastError() << std::endl;
                return 1;
            }
        }
        else {
            return 1;
        }
    }

    std::cout << "\nDriver installed successfully!" << std::endl;
    if (reboot) {
        std::cout << "A system reboot is required." << std::endl;
    }

    return 0;
}
