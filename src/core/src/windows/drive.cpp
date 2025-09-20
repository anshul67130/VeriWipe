#include "src\core\include\drive.hpp"

namespace WipeCore {

    DriveInfo DriveAnalyzer::analyze(const std::string& devicePath) {
        // This will call the platform-specific implementation
        #ifdef _WIN32
            // We'll implement this in the Windows-specific file
            // For now, return empty info
            DriveInfo info;
            info.devicePath = devicePath;
            return info;
        #else
            // Linux implementation will go here later
            DriveInfo info;
            info.devicePath = devicePath;
            return info;
        #endif
    }

} // namespace WipeCore