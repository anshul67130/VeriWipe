# Create the test executable
add_executable(wiper_test
    src/main.cpp
)

# Link with our core library
target_link_libraries(wiper_test PRIVATE wiper_core)

# Install the executable
install(TARGETS wiper_test RUNTIME DESTINATION bin)