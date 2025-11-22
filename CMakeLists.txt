cmake_minimum_required(VERSION 3.19)

project(com.mirabox.msfsDock VERSION 0.1)

# set the C++ standard to C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)
if (MSVC)
  add_definitions("/Zc:__cplusplus" -DUNICODE=1)
endif()
include_directories("${CMAKE_SOURCE_DIR}")

# StreamDock CPP SDK
add_subdirectory(StreamDockCPPSDK/Vendor)
add_subdirectory(StreamDockCPPSDK/StreamDockSDK)

set(
  SOURCES
  BaseAction.cpp
  GDIFonts.cpp
  GDIPlusManager.cpp
  Logger.cpp
  SimManager.cpp
  main.cpp
  Plugin.cpp
  DialAction.cpp
  ToggleAction.cpp
)
add_executable(
  msfsDock
  ${SOURCES}
)

target_include_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)
target_link_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)

# Link StreamDock CPP SDK
target_link_libraries(msfsDock PUBLIC StreamDockSDK)
target_link_libraries(msfsDock PRIVATE SimConnect)
# If you need to link dll dynamic libraries at runtime, you need to copy these dll dynamic libraries to the same directory as exe
# You need to copy the lib file to the directory where the .sln file is located (that is, the build directory of cmake)

# set_target_properties(msfsDock PROPERTIES RUNTIME_OUTPUT_DIRECTORY "com.rvoronov.msfsDockPlugin")
