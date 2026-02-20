cmake_minimum_required(VERSION 3.19)

project(msfsDock VERSION 0.9.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# MSVC runtime
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "$<$<CONFIG:Debug>:MultiThreadedDebugDLL>$<$<CONFIG:Release>:MultiThreaded>"
    )
endif()

include_directories("${CMAKE_SOURCE_DIR}")

# StreamDock CPP SDK
add_subdirectory(StreamDockCPPSDK/Vendor)
add_subdirectory(StreamDockCPPSDK/StreamDockSDK)

set(SOURCES
    ui/GDIFonts.cpp
    ui/GDIPlusManager.cpp
    ui/UIManager.cpp
    plugin/Logger.cpp
    plugin/Plugin.cpp
    main.cpp
    core/BaseAction.cpp
    core/ButtonAction.cpp
    core/DialAction.cpp
    core/GaugeAction.cpp
    core/SwitchAction.cpp
    SimManager/SimManager.cpp
    SimData/SimData.cpp
)

add_executable(msfsDock ${SOURCES})

# ------------------------------------------------
# VERSION define for C++
# ------------------------------------------------
target_compile_definitions(msfsDock PRIVATE
    MSFSDOCK_VERSION="${PROJECT_VERSION}"
    UNICODE=1
)

# ------------------------------------------------
# MSVC specific compiler flags (ONLY for C++)
# ------------------------------------------------
if (MSVC)
    target_compile_options(msfsDock PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
    )
endif()

# ------------------------------------------------
# Windows version info resource
# ------------------------------------------------
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/version.rc.in
    ${CMAKE_BINARY_DIR}/resources/version.rc
    @ONLY
)

target_sources(msfsDock PRIVATE ${CMAKE_BINARY_DIR}/resources/version.rc)

# ------------------------------------------------
# Hardware platform
# ------------------------------------------------
option(HW_PLATFORM "Target hardware platform" "ELGATO")

if (HW_PLATFORM STREQUAL "ELGATO")
    target_compile_definitions(msfsDock PRIVATE HW_PLATFORM_ELGATO)
elseif (HW_PLATFORM STREQUAL "MIRABOX")
    target_compile_definitions(msfsDock PRIVATE HW_PLATFORM_MIRABOX)
else()
    message(FATAL_ERROR "Unknown HW_PLATFORM: ${HW_PLATFORM}")
endif()

# ------------------------------------------------
# SimConnect
# ------------------------------------------------
target_include_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)
target_link_directories(msfsDock PRIVATE ${CMAKE_SOURCE_DIR}/SimConnect)

# ------------------------------------------------
# Link libraries
# ------------------------------------------------
target_link_libraries(msfsDock PUBLIC StreamDockSDK)
target_link_libraries(msfsDock PRIVATE SimConnect)
