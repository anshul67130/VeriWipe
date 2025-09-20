#ifndef DRIVE_HPP
#define DRIVE_HPP

#include <string>
#include <cstdint> // used for uint64_t

namespace WipeCore {
    enum class DriveType{
        Unknown,
        ATA_HDD,
        ATA_SSD,
        NVMe_SSD,
        USB_Removable
    };

    struct DriveInfo {
        std::string devicePath;
        std::string model;
        std::string serialNumber;
        std::string firmwareVersion;
        uint64_t size;
        DriveType type;
        //can add more fields later
    };

    class DriveAnalyzer {
        public: 
            static DriveInfo analyze(const std::string& devicePath);

    };

}

#endif 