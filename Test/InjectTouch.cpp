#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <iostream>
#include <vector>
#include <strsafe.h>

#pragma comment(lib, "setupapi.lib")

// Define GUID, IOCTL, struct here to avoid kernel header dependency issues in simple test app
// {2FAC259E-9E9A-4C40-983C-123456789ABC}
DEFINE_GUID(GUID_DEVINTERFACE_MOUSELIKEPTP,
    0x2fac259e, 0x9e9a, 0x4c40, 0x98, 0x3c, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc);

#define FILE_DEVICE_MOUSE 0x0000000f
#define METHOD_BUFFERED 0
#define FILE_ANY_ACCESS 0
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define IOCTL_INJECT_TOUCH_DATA CTL_CODE(FILE_DEVICE_MOUSE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _INJECT_FINGER {
    UCHAR CONFIDENCE : 1;
    UCHAR TIPSWITCH : 1;
    UCHAR CONTACTID : 6;
    USHORT X;
    USHORT Y;
    UCHAR Confidence;
    UCHAR TipSwitch;
    UCHAR ContactID;
} INJECT_FINGER, * PINJECT_FINGER;

typedef struct _INJECT_TOUCH_REPORT {
    INJECT_FINGER Fingers[5];
    USHORT ScanTime;
    UCHAR ContactCount;
    UCHAR IsButtonClicked;
} INJECT_TOUCH_REPORT, * PINJECT_TOUCH_REPORT;

int main()
{
    HDEVINFO hDevInfo;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
    DWORD RequiredSize;
    HANDLE hDevice = INVALID_HANDLE_VALUE;

    std::cout << "Finding MouseLikePTP Device..." << std::endl;

    hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_MOUSELIKEPTP, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "SetupDiGetClassDevs failed: " << GetLastError() << std::endl;
        return 1;
    }

    DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    if (SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_MOUSELIKEPTP, 0, &DeviceInterfaceData)) {
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &DeviceInterfaceData, NULL, 0, &RequiredSize, NULL);
        
        DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
        if (DeviceInterfaceDetailData) {
            DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
            
            if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &DeviceInterfaceData, DeviceInterfaceDetailData, RequiredSize, &RequiredSize, NULL)) {
                std::wcout << L"Device Path: " << DeviceInterfaceDetailData->DevicePath << std::endl;
                
                hDevice = CreateFile(DeviceInterfaceDetailData->DevicePath,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
            }
            free(DeviceInterfaceDetailData);
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open device. Error: " << GetLastError() << std::endl;
        std::cout << "Ensure the driver is installed and the device is active." << std::endl;
        return 1;
    }

    std::cout << "Device opened successfully. Starting injection..." << std::endl;

    // Inject some touch data
    // Simulate a swipe
    for (int i = 0; i < 100; i++) {
        INJECT_TOUCH_REPORT report = { 0 };
        report.ContactCount = 1;
        report.ScanTime = (USHORT)(GetTickCount() & 0xFFFF);
        
        // Finger 0
        report.Fingers[0].CONFIDENCE = 1;
        report.Fingers[0].TIPSWITCH = 1;
        report.Fingers[0].CONTACTID = 0;
        report.Fingers[0].Confidence = 1;
        report.Fingers[0].TipSwitch = 1;
        report.Fingers[0].ContactID = 0;
        report.Fingers[0].X = 1000 + (i * 10);
        report.Fingers[0].Y = 1000 + (i * 10);

        DWORD bytesReturned;
        if (!DeviceIoControl(hDevice, IOCTL_INJECT_TOUCH_DATA, &report, sizeof(report), NULL, 0, &bytesReturned, NULL)) {
            std::cerr << "DeviceIoControl failed: " << GetLastError() << std::endl;
        } else {
            // std::cout << "Injected frame " << i << std::endl;
        }
        Sleep(10);
    }

    // Lift finger
    INJECT_TOUCH_REPORT report = { 0 };
    report.ContactCount = 0;
    DWORD bytesReturned;
    DeviceIoControl(hDevice, IOCTL_INJECT_TOUCH_DATA, &report, sizeof(report), NULL, 0, &bytesReturned, NULL);
    std::cout << "Injection complete." << std::endl;

    CloseHandle(hDevice);
    return 0;
}
