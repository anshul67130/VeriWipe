#include "src\core\include\drive.hpp"
#include <windows.h>
#include <fileapi.h>
#include <winioctl.h>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>

// Helper function to convert wide char to std::string
std::string WideToMultiByte(const wchar_t* wideStr) {
    if (wideStr == nullptr) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    if (size == 0) return "";
    std::string multiByteStr(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &multiByteStr[0], size, nullptr, nullptr);
    return multiByteStr;
}

// Helper function to trim whitespace from string
std::string TrimString(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

namespace WipeCore {

    DriveInfo DriveAnalyzer::analyze(const std::string& devicePath) {
        DriveInfo info;
        info.devicePath = devicePath;
        
        HANDLE hDevice = CreateFileA(
            devicePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hDevice == INVALID_HANDLE_VALUE) {
            DWORD error = GetLastError();
            info.model = "Error: Cannot open device (Error code: " + std::to_string(error) + ")";
            return info;
        }

        // Get drive size
        GET_LENGTH_INFORMATION lengthInfo;
        DWORD bytesReturned = 0;
        if (DeviceIoControl(hDevice, IOCTL_DISK_GET_LENGTH_INFO, 
                           nullptr, 0, &lengthInfo, sizeof(lengthInfo), 
                           &bytesReturned, nullptr)) {
            info.size = lengthInfo.Length.QuadPart;
        }

        // Get storage property
        STORAGE_PROPERTY_QUERY query{};
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;

        STORAGE_DESCRIPTOR_HEADER descHeader{};
        if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                           &query, sizeof(query),
                           &descHeader, sizeof(descHeader),
                           &bytesReturned, nullptr)) {
            CloseHandle(hDevice);
            info.model = "Error: Failed to get storage descriptor header";
            return info;
        }

        if (descHeader.Size > 0) {
            // Use uint8_t for portability
            std::vector<uint8_t> buffer(descHeader.Size);
            STORAGE_DEVICE_DESCRIPTOR* deviceDesc = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buffer.data());

            if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                               &query, sizeof(query),
                               deviceDesc, descHeader.Size,
                               &bytesReturned, nullptr)) {
                
                // Get model name
                if (deviceDesc->ProductIdOffset != 0) {
                    const char* productId = reinterpret_cast<const char*>(deviceDesc) + deviceDesc->ProductIdOffset;
                    info.model = TrimString(productId);
                }

                // Get serial number
                if (deviceDesc->SerialNumberOffset != 0) {
                    const char* serialNumber = reinterpret_cast<const char*>(deviceDesc) + deviceDesc->SerialNumberOffset;
                    info.serialNumber = TrimString(serialNumber);
                }

                // Get firmware version
                if (deviceDesc->ProductRevisionOffset != 0) {
                    const char* firmware = reinterpret_cast<const char*>(deviceDesc) + deviceDesc->ProductRevisionOffset;
                    info.firmwareVersion = TrimString(firmware);
                }

                // Determine drive type based on bus type
                switch (deviceDesc->BusType) {
                    case BusTypeAta:
                    case BusTypeSata:
                        info.type = (info.model.find("SSD") != std::string::npos) 
                                  ? DriveType::ATA_SSD : DriveType::ATA_HDD;
                        break;
                    case BusTypeNvme:
                        info.type = DriveType::NVMe_SSD;
                        break;
                    case BusTypeUsb:
                        info.type = DriveType::USB_Removable;
                        break;
                    default:
                        info.type = DriveType::Unknown;
                }
            }
        }

        CloseHandle(hDevice);
        return info;
    }

} 