# Create a library for our core functionality
add_library(wiper_core STATIC
    src/drive.cpp
    src/wiper.cpp
    src/windows/drive_win.cpp
)

# Include directories
target_include_directories(wiper_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Windows specific libraries
if(WIN32)
    target_link_libraries(wiper_core PUBLIC Setupapi)
endif()