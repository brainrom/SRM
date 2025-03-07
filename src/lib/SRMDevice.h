#ifndef SRMDEVICE_H
#define SRMDEVICE_H

#include <SRMTypes.h>
#include <SRMEGL.h>
#include <EGL/egl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup SRMDevice SRMDevice
 *
 * @brief Representation of an open DRM device, typically a GPU.
 *
 * An @ref SRMDevice represents a DRM device, typically a GPU, which can contain multiple connectors
 * and a specific rendering mode (**ITSELF**, **PRIME**, **DUMB** or **CPU**).
 *
 * @{
 */

/**
 * @brief Sets the device user data.
 */
void srmDeviceSetUserData(SRMDevice *device, void *userData);

/**
 * @brief Retrieves the device user data.
 */
void *srmDeviceGetUserData(SRMDevice *device);

/**
 * @brief Get the @ref SRMCore instance to which this device belongs.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the @ref SRMCore instance that this device belongs to.
 */
SRMCore *srmDeviceGetCore(SRMDevice *device);

/**
 * @brief Get the DRM device name (e.g., `/dev/dri/card0`) associated with this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return The name of the DRM device associated with this @ref SRMDevice.
 */
const char *srmDeviceGetName(SRMDevice *device);

/**
 * @brief Get the file descriptor of the DRM device associated with this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return The file descriptor of the DRM device.
 */
Int32 srmDeviceGetFD(SRMDevice *device);

/**
 * @brief Get the client DRM driver's support for Stereo 3D capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the client DRM driver supports Stereo 3D capability, 0 otherwise.
 */
UInt8 srmDeviceGetClientCapStereo3D(SRMDevice *device);

/**
 * @brief Get the client DRM driver's support for Universal Planes capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the client DRM driver supports Universal Planes capability, 0 otherwise.
 */
UInt8 srmDeviceGetClientCapUniversalPlanes(SRMDevice *device);

/**
 * @brief Get the client DRM driver's support for Atomic API capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the client DRM driver supports Atomic API capability, 0 otherwise.
 *
 * @note Setting the **SRM_FORCE_LEGACY_API** environment variable to 1, disables the Atomic API.
 */
UInt8 srmDeviceGetClientCapAtomic(SRMDevice *device);

/**
 * @brief Get the client DRM driver's support for Aspect Ratio capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the client DRM driver supports Aspect Ratio capability, 0 otherwise.
 */
UInt8 srmDeviceGetClientCapAspectRatio(SRMDevice *device);

/**
 * @brief Get the client DRM driver's support for Writeback Connectors capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the client DRM driver supports Writeback Connectors capability, 0 otherwise.
 */
UInt8 srmDeviceGetClientCapWritebackConnectors(SRMDevice *device);

/**
 * @brief Get the driver's support for Dumb Buffer capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Dumb Buffer capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapDumbBuffer(SRMDevice *device);

/**
 * @brief Get the driver's support for Prime Import capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Prime Import capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapPrimeImport(SRMDevice *device);

/**
 * @brief Get the driver's support for Prime Export capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Prime Export capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapPrimeExport(SRMDevice *device);

/**
 * @brief Get the driver's support for Add FB2 Modifiers capability.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Add FB2 Modifiers capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapAddFb2Modifiers(SRMDevice *device);

/**
 * @brief Get the driver's support for Async Page Flip capability.
 *
 * @note This specifically indicates support for the legacy API, no the atomic API @see srmDeviceGetCapAtomicAsyncPageFlip().
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Async Page Flip capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapAsyncPageFlip(SRMDevice *device);

/**
 * @brief Get the driver's support for Atomic Async Page Flip capability.
 *
 * @note Atomic async page flips are supported since Linux 6.8.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device driver supports Atomic Async Page Flip capability, 0 otherwise.
 */
UInt8 srmDeviceGetCapAtomicAsyncPageFlip(SRMDevice *device);

/**
 * @brief Retrieve the driver's support for monotonic timestamps.
 *
 * @see srmConnectorGetPresentationClock()
 * @see srmConnectorGetPresentationTime()
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the DRM device reports vblank timestamps with CLOCK_MONOTONIC, 0 if it uses CLOCK_REALTIME.
 */
UInt8 srmDeviceGetCapTimestampMonotonic(SRMDevice *device);

/**
 * @brief Check if the device can perform rendering.
 *
 * This function returns true when the device can import buffers from the allocator device,
 * which also means it can perform rendering for other non-rendering devices.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return 1 if the device is capable of rendering, 0 otherwise.
 */
UInt8 srmDeviceIsRenderer(SRMDevice *device);

/**
 * @brief Get the device that performs rendering for this device.
 *
 * The rendering device is equal to this device only when the **ITSELF** rendering mode is used.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the @ref SRMDevice that performs rendering for this device.
 */
SRMDevice *srmDeviceGetRendererDevice(SRMDevice *device);

/**
 * @brief Get the rendering mode of the device.
 *
 * If the rendering device is equal to this device, then the rendering mode is **ITSELF**.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return The rendering mode of the device (@ref SRM_RENDER_MODE).
 */
SRM_RENDER_MODE srmDeviceGetRenderMode(SRMDevice *device);

/**
 * @brief Get a list of CRTCs (Cathode Ray Tube Controllers) of this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of pointers to the CRTCs (@ref SRMCrtc) associated with this device.
 */
SRMList *srmDeviceGetCrtcs(SRMDevice *device);

/**
 * @brief Get a list of encoders of this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of pointers to the encoders (@ref SRMEncoder) associated with this device.
 */
SRMList *srmDeviceGetEncoders(SRMDevice *device);

/**
 * @brief Get a list of planes of this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of pointers to the planes (@ref SRMPlane) associated with this device.
 */
SRMList *srmDeviceGetPlanes(SRMDevice *device);

/**
 * @brief Get a list of connectors of this device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of pointers to the connectors (@ref SRMConnector) associated with this device.
 */
SRMList *srmDeviceGetConnectors(SRMDevice *device);

/**
 * @brief Get a list of all DMA (Direct Memory Access) texture formats supported by the device.
 *
 * @note This list includes both render and external formats.
 * 
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of DMA texture formats supported by the device.
 */
SRMList *srmDeviceGetDMATextureFormats(SRMDevice *device);

/**
 * @brief Get a list of DMA (Direct Memory Access) render formats supported by the device.
 *
 * @note Render formats have the `GL_TEXTURE_2D` target.
 * 
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of DMA render formats supported by the device.
 */
SRMList *srmDeviceGetDMARenderFormats(SRMDevice *device);

/**
 * @brief Get a list of DMA (Direct Memory Access) external formats supported by the device.
 *
 * @note External formats have the `GL_TEXTURE_EXTERNAL_OES` target.
 * 
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A list of DMA external formats supported by the device.
 */
SRMList *srmDeviceGetDMAExternalFormats(SRMDevice *device);

/**
 * @brief Get the `EGLDisplay` associated with the device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the `EGLDisplay` associated with the device.
 */
EGLDisplay *srmDeviceGetEGLDisplay(SRMDevice *device);

/**
 * @brief Get the `EGLContext` associated with the device.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the `EGLContext` associated with the device.
 */
EGLContext *srmDeviceGetEGLContext(SRMDevice *device);

/**
 * @brief Returns a structure with boolean variables indicating which EGL extensions the device supports.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the @ref SRMEGLDeviceExtensions structure indicating the supported EGL extensions.
 */
const SRMEGLDeviceExtensions *srmDeviceGetEGLExtensions(SRMDevice *device);

/**
 * @brief Returns a structure with pointers to many available EGL functions.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the @ref SRMEGLDeviceFunctions structure containing pointers to avaliable EGL functions.
 */
const SRMEGLDeviceFunctions *srmDeviceGetEGLFunctions(SRMDevice *device);

/**
 * @brief Returns a structure with boolean variables indicating which OpenGL extensions the device supports.
 *
 * @param device A pointer to the @ref SRMDevice instance.
 *
 * @return A pointer to the @ref SRMGLDeviceExtensions structure indicating the supported OpenGL extensions.
 */
const SRMGLDeviceExtensions *srmDeviceGetGLExtensions(SRMDevice *device);

/**
 * @brief Sets the EGL display and context for the specified device as current for the calling thread.
 *
 * If no existing EGL context is associated with the calling thread, a new one
 * is created and made current.
 *
 * @param device Pointer to the @ref SRMDevice whose EGL context should be made current.
 */
void srmDeviceMakeCurrent(SRMDevice *device);

/**
 * @brief Ensures that all pending rendering commands are completed before continuing.
 */
void srmDeviceSyncWait(SRMDevice *device);

/// @cond OMIT

// TODO: Enabling /disabling GPUs on the fly is still unsupported

UInt8 srmDeviceSetEnabled(SRMDevice *device, UInt8 enabled);
UInt8 srmDeviceIsEnabled(SRMDevice *device);
/// @endcond OMIT

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // SRMDEVICE_H
