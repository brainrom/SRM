cmake_minimum_required(VERSION 3.10)
project(SRM LANGUAGES C)

find_package(PkgConfig REQUIRED)
include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

pkg_check_modules(GL REQUIRED IMPORTED_TARGET gl>=1.2)
if(NOT GL_FOUND)
    message(FATAL_ERROR "OpenGL (GL) version >= 1.2 not found")
endif()

# EGL dependency
pkg_check_modules(EGL REQUIRED IMPORTED_TARGET egl>=1.5)
if(NOT EGL_FOUND)
    message(FATAL_ERROR "EGL version >= 1.5 not found")
endif()

# GLESv2 dependency
pkg_check_modules(GLESV2 REQUIRED IMPORTED_TARGET glesv2>=3.2)
if(NOT GLESV2_FOUND)
    message(FATAL_ERROR "GLESv2 version >= 3.2 not found")
endif()

# libudev dependency
pkg_check_modules(UDEV REQUIRED IMPORTED_TARGET libudev>=249)
if(NOT UDEV_FOUND)
    message(FATAL_ERROR "libudev version >= 249 not found")
endif()

# libdrm dependency
pkg_check_modules(DRM REQUIRED IMPORTED_TARGET libdrm>=2.4.113)
if(NOT DRM_FOUND)
    message(FATAL_ERROR "libdrm version >= 2.4.113 not found")
endif()

# GBM dependency
pkg_check_modules(GBM REQUIRED IMPORTED_TARGET gbm>=23.2.1)
if(NOT GBM_FOUND)
    message(FATAL_ERROR "GBM version >= 23.2.1 not found")
endif()

# libdisplay-info dependency
pkg_check_modules(DISPLAY_INFO REQUIRED IMPORTED_TARGET libdisplay-info)
if(NOT DISPLAY_INFO_FOUND)
    message(FATAL_ERROR "libdisplay-info not found")
endif()


set(SOURCES
    lib/SRMFormat.c
    lib/SRMCrtc.c
    lib/SRMConnectorMode.c
    lib/SRMPlane.c
    lib/SRMEncoder.c
    lib/SRMBuffer.c
    lib/SRMCore.c
    lib/SRMTypes.c
    lib/SRMDevice.c
    lib/SRMList.c
    lib/SRMEGL.c
    lib/private/SRMEncoderPrivate.c
    lib/private/SRMDevicePrivate.c
    lib/private/SRMCorePrivate.c
    lib/private/SRMConnectorPrivate.c
    lib/private/SRMConnectorModePrivate.c
    lib/private/modes/SRMRenderModeItself.c
    lib/private/modes/SRMRenderModeCommon.c
    lib/private/modes/SRMRenderModePrime.c
    lib/private/modes/SRMRenderModeCPU.c
    lib/private/modes/SRMRenderModeDumb.c
    lib/private/SRMListenerPrivate.c
    lib/private/SRMBufferPrivate.c
    lib/private/SRMPlanePrivate.c
    lib/private/SRMListPrivate.c
    lib/private/SRMCrtcPrivate.c
    lib/SRMConnector.c
    lib/SRMLog.c
    lib/SRMListener.c
)


set(SRM_VERSION_MAJOR 0)
set(SRM_VERSION_MINOR 12)
set(SRM_VERSION_PATCH 0)
set(SRM_VERSION_BUILD 1)

# Configure the file
configure_file(
    lib/SRMTypes.hh  # Input template file
    SRMTypes.h       # Output generated file
    @ONLY            # Use @variable@ syntax for substitution
)

# Install the generated header file if needed
install(
    FILES lib/SRMTypes.h
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

function(add_srm_library library_type)

    string(TOLOWER ${library_type} library_type_lower)

    # Create the library
    add_library(srm_${library_type_lower} ${library_type} ${SOURCES})

    # Link libraries
    target_link_libraries(srm_${library_type_lower} PUBLIC
        PkgConfig::GL
        PkgConfig::EGL
        PkgConfig::GLESV2
        PkgConfig::UDEV
        PkgConfig::DRM
        PkgConfig::GBM
        PkgConfig::DISPLAY_INFO
    )
    if (IS_BIG_ENDIAN)
        target_compile_definitions(srm_${library_type_lower} PRIVATE -DSRM_BIG_ENDIAN=1)
    else()
        target_compile_definitions(srm_${library_type_lower} PRIVATE -DSRM_LITTLE_ENDIAN=1)
    endif()

    # Set include directories
    target_include_directories(srm_${library_type_lower} PRIVATE lib/private)
    target_include_directories(srm_${library_type_lower} PUBLIC lib ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

add_srm_library(SHARED)
add_srm_library(STATIC)
