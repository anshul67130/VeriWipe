#include <iostream>
#include <iomanip>
#include "../../../src/core/include/drive.hpp"

int main() {
    std::cout << "Secure Wipe Drive Analyzer Test\n";
    std::cout << "================================\n\n";

    // Test with physical drive 0 (usually main drive)
    std::string devicePath = "\\\\.\\PhysicalDrive0";
    
    std::cout << "Analyzing device: " << devicePath << "\n";
    
    WipeCore::DriveInfo info = WipeCore::DriveAnalyzer::analyze(devicePath);
    
    std::cout << "Model: " << info.model << "\n";
    std::cout << "Serial: " << info.serialNumber << "\n";
    std::cout << "Size: " << (info.size / (1024 * 1024 * 1024)) << " GB\n";
    
    std::cout << "Type: ";
    switch (info.type) {
        case WipeCore::DriveType::ATA_HDD: std::cout << "ATA HDD"; break;
        case WipeCore::DriveType::ATA_SSD: std::cout << "ATA SSD"; break;
        case WipeCore::DriveType::NVMe_SSD: std::cout << "NVMe SSD"; break;
        case WipeCore::DriveType::USB_Removable: std::cout << "USB Removable"; break;
        default: std::cout << "Unknown";
    }
    std::cout << "\n";

    return 0;
}