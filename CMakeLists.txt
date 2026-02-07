cmake_minimum_required(VERSION 3.19)

project(com.mirabox.msfsDock VERSION 0.7.3)

# set the C++ standard to C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED true)

if (MSVC)
    add_definitions("/Zc:__cplusplus" -DUNICODE=1)

    # Debug → MDd, Release → MT
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "$<$<CONFIG:Debug>:MultiThreadedDebugDLL>$<$<CONFIG:Release>:MultiThreaded>"
    )
endif()

include_directories("${CMAKE_SOURCE_DIR}")

# StreamDock CPP SDK
add_subdirectory(StreamDockCPPSDK/Vendor)
add_subdirectory(StreamDockCPPSDK/StreamDockSDK)

set(
  SOURCES
  ui/GDIFonts.cpp
  ui/GDIPlusManager.cpp
  ui/UIManager.cpp
  plugin/Logger.cpp
  plugin/Plugin.cpp
  main.cpp
  core/ButtonAction.cpp
  core/DialAction.cpp
  core/GaugeAction.cpp
  core/SwitchAction.cpp
  SimManager/SimManager.cpp
  SimData/SimData.cpp
)

add_executable(
  msfsDock
  ${SOURCES}
)

option(HW_PLATFORM "Target hardware platform" "ELGATO")

if (HW_PLATFORM STREQUAL "ELGATO")
    target_compile_definitions(msfsDock PRIVATE HW_PLATFORM_ELGATO)
elseif (HW_PLATFORM STREQUAL "MIRABOX")
    target_compile_definitions(msfsDock PRIVATE HW_PLATFORM_MIRABOX)
else()
    message(FATAL_ERROR "Unknown HW_PLATFORM: ${HW_PLATFORM}")
endif()

target_include_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)
target_link_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)

# Link StreamDock CPP SDK
target_link_libraries(msfsDock PUBLIC StreamDockSDK)
target_link_libraries(msfsDock PRIVATE SimConnect)
# If you need to link dll dynamic libraries at runtime, you need to copy these dll dynamic libraries to the same directory as exe
# You need to copy the lib file to the directory where the .sln file is located (that is, the build directory of cmake)

# set_target_properties(msfsDock PROPERTIES RUNTIME_OUTPUT_DIRECTORY "com.rvoronov.msfsDockPlugin")
