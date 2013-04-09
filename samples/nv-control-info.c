/*
 * Copyright (c) 2004 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * nv-control-info.c - trivial sample NV-CONTROL client that
 * demonstrates how to determine if the NV-CONTROL extension is
 * present.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <X11/Xlib.h>

#include "NVCtrl.h"
#include "NVCtrlLib.h"

// Used to convert the NV-CONTROL #defines to human readable text.
#define MAKE_ENTRY(ATTRIBUTE) { ATTRIBUTE, #ATTRIBUTE, NULL }

typedef struct {
    int num;
    char *str;
    char *name;
} AttrEntry;

static const char *attr2str(int n, AttrEntry *tbl)
{
    AttrEntry *entry;

    entry = tbl;
    while (entry->str) {
        if (entry->num == n) {
            if (!entry->name) {
                int len;
                entry->name = strdup(entry->str + 8);
                for (len = 0; len < strlen(entry->name); len++) {
                    entry->name[len] = tolower(entry->name[len]);
                }
            }
            return entry->name;
        }
        entry++;
    }

    return NULL;
}

// Attribute -> String table, generated using:
//
// grep 'define.*\/\*' NVCtrl.h | sed 's/.*define \([^ ]*\).*/    MAKE_ENTRY(\1),/' > DATA | head DATA
//
static AttrEntry attr_int_table[] = {
    MAKE_ENTRY(NV_CTRL_FLATPANEL_SCALING),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_DITHERING),
    MAKE_ENTRY(NV_CTRL_DITHERING),
    MAKE_ENTRY(NV_CTRL_DIGITAL_VIBRANCE),
    MAKE_ENTRY(NV_CTRL_BUS_TYPE),
    MAKE_ENTRY(NV_CTRL_VIDEO_RAM),
    MAKE_ENTRY(NV_CTRL_IRQ),
    MAKE_ENTRY(NV_CTRL_OPERATING_SYSTEM),
    MAKE_ENTRY(NV_CTRL_SYNC_TO_VBLANK),
    MAKE_ENTRY(NV_CTRL_LOG_ANISO),
    MAKE_ENTRY(NV_CTRL_FSAA_MODE),
    MAKE_ENTRY(NV_CTRL_TEXTURE_SHARPEN),
    MAKE_ENTRY(NV_CTRL_UBB),
    MAKE_ENTRY(NV_CTRL_OVERLAY),
    MAKE_ENTRY(NV_CTRL_STEREO),
    MAKE_ENTRY(NV_CTRL_EMULATE),
    MAKE_ENTRY(NV_CTRL_TWINVIEW),
    MAKE_ENTRY(NV_CTRL_CONNECTED_DISPLAYS),
    MAKE_ENTRY(NV_CTRL_ENABLED_DISPLAYS),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_MASTER),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_POLARITY),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_DELAY),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_INTERVAL),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_PORT0_STATUS),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_PORT1_STATUS),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_HOUSE_STATUS),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_READY),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_STEREO_SYNC),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_TEST_SIGNAL),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_ETHERNET_DETECTED),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_VIDEO_MODE),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_RATE),
    MAKE_ENTRY(NV_CTRL_FORCE_GENERIC_CPU),
    MAKE_ENTRY(NV_CTRL_OPENGL_AA_LINE_GAMMA),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_TIMING),
    MAKE_ENTRY(NV_CTRL_FLIPPING_ALLOWED),
    MAKE_ENTRY(NV_CTRL_ARCHITECTURE),
    MAKE_ENTRY(NV_CTRL_TEXTURE_CLAMPING),
    MAKE_ENTRY(NV_CTRL_FSAA_APPLICATION_CONTROLLED),
    MAKE_ENTRY(NV_CTRL_LOG_ANISO_APPLICATION_CONTROLLED),
    MAKE_ENTRY(NV_CTRL_IMAGE_SHARPENING),
    MAKE_ENTRY(NV_CTRL_TV_OVERSCAN),
    MAKE_ENTRY(NV_CTRL_TV_FLICKER_FILTER),
    MAKE_ENTRY(NV_CTRL_TV_BRIGHTNESS),
    MAKE_ENTRY(NV_CTRL_TV_HUE),
    MAKE_ENTRY(NV_CTRL_TV_CONTRAST),
    MAKE_ENTRY(NV_CTRL_TV_SATURATION),
    MAKE_ENTRY(NV_CTRL_TV_RESET_SETTINGS),
    MAKE_ENTRY(NV_CTRL_GPU_CORE_TEMPERATURE),
    MAKE_ENTRY(NV_CTRL_GPU_CORE_THRESHOLD),
    MAKE_ENTRY(NV_CTRL_GPU_DEFAULT_CORE_THRESHOLD),
    MAKE_ENTRY(NV_CTRL_GPU_MAX_CORE_THRESHOLD),
    MAKE_ENTRY(NV_CTRL_AMBIENT_TEMPERATURE),
    MAKE_ENTRY(NV_CTRL_PBUFFER_SCANOUT_SUPPORTED),
    MAKE_ENTRY(NV_CTRL_PBUFFER_SCANOUT_XID),
    MAKE_ENTRY(NV_CTRL_GVO_SUPPORTED),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_MODE),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_SOURCE),
    MAKE_ENTRY(NV_CTRL_GVIO_REQUESTED_VIDEO_FORMAT),
    MAKE_ENTRY(NV_CTRL_GVIO_DETECTED_VIDEO_FORMAT),
    MAKE_ENTRY(NV_CTRL_GVO_DATA_FORMAT),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_SYNC_INPUT_DETECTED),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_SYNC_INPUT_DETECT_MODE),
    MAKE_ENTRY(NV_CTRL_GVO_SDI_SYNC_INPUT_DETECTED),
    MAKE_ENTRY(NV_CTRL_GVO_VIDEO_OUTPUTS),
    MAKE_ENTRY(NV_CTRL_GVO_FIRMWARE_VERSION),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_DELAY_PIXELS),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_DELAY_LINES),
    MAKE_ENTRY(NV_CTRL_GVO_INPUT_VIDEO_FORMAT_REACQUIRE),
    MAKE_ENTRY(NV_CTRL_GVO_GLX_LOCKED),
    MAKE_ENTRY(NV_CTRL_GVIO_VIDEO_FORMAT_WIDTH),
    MAKE_ENTRY(NV_CTRL_GVIO_VIDEO_FORMAT_HEIGHT),
    MAKE_ENTRY(NV_CTRL_GVIO_VIDEO_FORMAT_REFRESH_RATE),
    MAKE_ENTRY(NV_CTRL_GPU_OVERCLOCKING_STATE),
    MAKE_ENTRY(NV_CTRL_GPU_2D_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_3D_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_DEFAULT_2D_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_DEFAULT_3D_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_CURRENT_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_OPTIMAL_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GPU_OPTIMAL_CLOCK_FREQS_DETECTION),
    MAKE_ENTRY(NV_CTRL_GPU_OPTIMAL_CLOCK_FREQS_DETECTION_STATE),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_CHIP_LOCATION),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_LINK),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_SIGNAL),
    MAKE_ENTRY(NV_CTRL_USE_HOUSE_SYNC),
    MAKE_ENTRY(NV_CTRL_EDID_AVAILABLE),
    MAKE_ENTRY(NV_CTRL_FORCE_STEREO),
    MAKE_ENTRY(NV_CTRL_IMAGE_SETTINGS),
    MAKE_ENTRY(NV_CTRL_XINERAMA),
    MAKE_ENTRY(NV_CTRL_XINERAMA_STEREO),
    MAKE_ENTRY(NV_CTRL_BUS_RATE),
    MAKE_ENTRY(NV_CTRL_SHOW_SLI_VISUAL_INDICATOR),
    MAKE_ENTRY(NV_CTRL_XV_SYNC_TO_DISPLAY),
    MAKE_ENTRY(NV_CTRL_GVIO_REQUESTED_VIDEO_FORMAT2),
    MAKE_ENTRY(NV_CTRL_GVO_OVERRIDE_HW_CSC),
    MAKE_ENTRY(NV_CTRL_GVO_CAPABILITIES),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_TERMINATION),
    MAKE_ENTRY(NV_CTRL_ASSOCIATED_DISPLAY_DEVICES),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SLAVES),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_MASTERABLE),
    MAKE_ENTRY(NV_CTRL_PROBE_DISPLAYS),
    MAKE_ENTRY(NV_CTRL_REFRESH_RATE),
    MAKE_ENTRY(NV_CTRL_GVO_FLIP_QUEUE_SIZE),
    MAKE_ENTRY(NV_CTRL_CURRENT_SCANLINE),
    MAKE_ENTRY(NV_CTRL_INITIAL_PIXMAP_PLACEMENT),
    MAKE_ENTRY(NV_CTRL_PCI_BUS),
    MAKE_ENTRY(NV_CTRL_PCI_DEVICE),
    MAKE_ENTRY(NV_CTRL_PCI_FUNCTION),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_FPGA_REVISION),
    MAKE_ENTRY(NV_CTRL_MAX_SCREEN_WIDTH),
    MAKE_ENTRY(NV_CTRL_MAX_SCREEN_HEIGHT),
    MAKE_ENTRY(NV_CTRL_MAX_DISPLAYS),
    MAKE_ENTRY(NV_CTRL_DYNAMIC_TWINVIEW),
    MAKE_ENTRY(NV_CTRL_MULTIGPU_DISPLAY_OWNER),
    MAKE_ENTRY(NV_CTRL_GPU_SCALING),
    MAKE_ENTRY(NV_CTRL_FRONTEND_RESOLUTION),
    MAKE_ENTRY(NV_CTRL_BACKEND_RESOLUTION),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_NATIVE_RESOLUTION),
    MAKE_ENTRY(NV_CTRL_FLATPANEL_BEST_FIT_RESOLUTION),
    MAKE_ENTRY(NV_CTRL_GPU_SCALING_ACTIVE),
    MAKE_ENTRY(NV_CTRL_DFP_SCALING_ACTIVE),
    MAKE_ENTRY(NV_CTRL_FSAA_APPLICATION_ENHANCED),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_RATE_4),
    MAKE_ENTRY(NV_CTRL_GVO_LOCK_OWNER),
    MAKE_ENTRY(NV_CTRL_HWOVERLAY),
    MAKE_ENTRY(NV_CTRL_NUM_GPU_ERRORS_RECOVERED),
    MAKE_ENTRY(NV_CTRL_REFRESH_RATE_3),
    MAKE_ENTRY(NV_CTRL_ONDEMAND_VBLANK_INTERRUPTS),
    MAKE_ENTRY(NV_CTRL_GPU_POWER_SOURCE),
    MAKE_ENTRY(NV_CTRL_GPU_CURRENT_PERFORMANCE_MODE),
    MAKE_ENTRY(NV_CTRL_GLYPH_CACHE),
    MAKE_ENTRY(NV_CTRL_GPU_CURRENT_PERFORMANCE_LEVEL),
    MAKE_ENTRY(NV_CTRL_GPU_ADAPTIVE_CLOCK_STATE),
    MAKE_ENTRY(NV_CTRL_GVO_OUTPUT_VIDEO_LOCKED),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_LOCK_STATUS),
    MAKE_ENTRY(NV_CTRL_GVO_ANC_TIME_CODE_GENERATION),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_ALPHA_KEY),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_LUMA_KEY_RANGE),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_CR_KEY_RANGE),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_CB_KEY_RANGE),
    MAKE_ENTRY(NV_CTRL_GVO_COMPOSITE_NUM_KEY_RANGES),
    MAKE_ENTRY(NV_CTRL_SWITCH_TO_DISPLAYS),
    MAKE_ENTRY(NV_CTRL_NOTEBOOK_DISPLAY_CHANGE_LID_EVENT),
    MAKE_ENTRY(NV_CTRL_NOTEBOOK_INTERNAL_LCD),
    MAKE_ENTRY(NV_CTRL_DEPTH_30_ALLOWED),
    MAKE_ENTRY(NV_CTRL_MODE_SET_EVENT),
    MAKE_ENTRY(NV_CTRL_OPENGL_AA_LINE_GAMMA_VALUE),
    MAKE_ENTRY(NV_CTRL_VCSC_HIGH_PERF_MODE),
    MAKE_ENTRY(NV_CTRL_DISPLAYPORT_LINK_RATE),
    MAKE_ENTRY(NV_CTRL_STEREO_EYES_EXCHANGE),
    MAKE_ENTRY(NV_CTRL_NO_SCANOUT),
    MAKE_ENTRY(NV_CTRL_GVO_CSC_CHANGED_EVENT),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SLAVEABLE),
    MAKE_ENTRY(NV_CTRL_GVO_SYNC_TO_DISPLAY),
    MAKE_ENTRY(NV_CTRL_X_SERVER_UNIQUE_ID),
    MAKE_ENTRY(NV_CTRL_PIXMAP_CACHE),
    MAKE_ENTRY(NV_CTRL_PIXMAP_CACHE_ROUNDING_SIZE_KB),
    MAKE_ENTRY(NV_CTRL_IS_GVO_DISPLAY),
    MAKE_ENTRY(NV_CTRL_PCI_ID),
    MAKE_ENTRY(NV_CTRL_GVO_FULL_RANGE_COLOR),
    MAKE_ENTRY(NV_CTRL_SLI_MOSAIC_MODE_AVAILABLE),
    MAKE_ENTRY(NV_CTRL_GVO_ENABLE_RGB_DATA),
    MAKE_ENTRY(NV_CTRL_IMAGE_SHARPENING_DEFAULT),
    MAKE_ENTRY(NV_CTRL_PCI_DOMAIN),
    MAKE_ENTRY(NV_CTRL_GVI_NUM_JACKS),
    MAKE_ENTRY(NV_CTRL_GVI_MAX_LINKS_PER_STREAM),
    MAKE_ENTRY(NV_CTRL_GVI_DETECTED_CHANNEL_BITS_PER_COMPONENT),
    MAKE_ENTRY(NV_CTRL_GVI_REQUESTED_STREAM_BITS_PER_COMPONENT),
    MAKE_ENTRY(NV_CTRL_GVI_DETECTED_CHANNEL_COMPONENT_SAMPLING),
    MAKE_ENTRY(NV_CTRL_GVI_REQUESTED_STREAM_COMPONENT_SAMPLING),
    MAKE_ENTRY(NV_CTRL_GVI_REQUESTED_STREAM_CHROMA_EXPAND),
    MAKE_ENTRY(NV_CTRL_GVI_DETECTED_CHANNEL_COLOR_SPACE),
    MAKE_ENTRY(NV_CTRL_GVI_DETECTED_CHANNEL_LINK_ID),
    MAKE_ENTRY(NV_CTRL_GVI_DETECTED_CHANNEL_SMPTE352_IDENTIFIER),
    MAKE_ENTRY(NV_CTRL_GVI_GLOBAL_IDENTIFIER),
    MAKE_ENTRY(NV_CTRL_FRAMELOCK_SYNC_DELAY_RESOLUTION),
    MAKE_ENTRY(NV_CTRL_GPU_COOLER_MANUAL_CONTROL),
    MAKE_ENTRY(NV_CTRL_THERMAL_COOLER_LEVEL),
    MAKE_ENTRY(NV_CTRL_THERMAL_COOLER_LEVEL_SET_DEFAULT),
    MAKE_ENTRY(NV_CTRL_THERMAL_COOLER_CONTROL_TYPE),
    MAKE_ENTRY(NV_CTRL_THERMAL_COOLER_TARGET),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_SUPPORTED),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_STATUS),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_CONFIGURATION_SUPPORTED),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_CONFIGURATION),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_DEFAULT_CONFIGURATION),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_SINGLE_BIT_ERRORS),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_DOUBLE_BIT_ERRORS),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_AGGREGATE_SINGLE_BIT_ERRORS),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_AGGREGATE_DOUBLE_BIT_ERRORS),
    MAKE_ENTRY(NV_CTRL_GPU_ECC_RESET_ERROR_STATUS),
    MAKE_ENTRY(NV_CTRL_GPU_POWER_MIZER_MODE),
    MAKE_ENTRY(NV_CTRL_GVI_SYNC_OUTPUT_FORMAT),
    MAKE_ENTRY(NV_CTRL_GVI_MAX_CHANNELS_PER_JACK),
    MAKE_ENTRY(NV_CTRL_GVI_MAX_STREAMS),
    MAKE_ENTRY(NV_CTRL_GVI_NUM_CAPTURE_SURFACES),
    MAKE_ENTRY(NV_CTRL_OVERSCAN_COMPENSATION),
    MAKE_ENTRY(NV_CTRL_GPU_PCIE_GENERATION),
    MAKE_ENTRY(NV_CTRL_GVI_BOUND_GPU),
    MAKE_ENTRY(NV_CTRL_GVIO_REQUESTED_VIDEO_FORMAT3),
    MAKE_ENTRY(NV_CTRL_ACCELERATE_TRAPEZOIDS),
    MAKE_ENTRY(NV_CTRL_GPU_CORES),
    MAKE_ENTRY(NV_CTRL_GPU_MEMORY_BUS_WIDTH),
    MAKE_ENTRY(NV_CTRL_GVI_TEST_MODE),
    MAKE_ENTRY(NV_CTRL_COLOR_SPACE),
    MAKE_ENTRY(NV_CTRL_COLOR_RANGE),
    MAKE_ENTRY(NV_CTRL_GPU_SCALING_DEFAULT_TARGET),
    MAKE_ENTRY(NV_CTRL_GPU_SCALING_DEFAULT_METHOD),
    MAKE_ENTRY(NV_CTRL_DITHERING_MODE),
    MAKE_ENTRY(NV_CTRL_CURRENT_DITHERING),
    MAKE_ENTRY(NV_CTRL_CURRENT_DITHERING_MODE),
    MAKE_ENTRY(NV_CTRL_THERMAL_SENSOR_READING),
    MAKE_ENTRY(NV_CTRL_THERMAL_SENSOR_PROVIDER),
    MAKE_ENTRY(NV_CTRL_THERMAL_SENSOR_TARGET),
    MAKE_ENTRY(NV_CTRL_SHOW_MULTIGPU_VISUAL_INDICATOR),
    MAKE_ENTRY(NV_CTRL_GPU_CURRENT_PROCESSOR_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_GVIO_VIDEO_FORMAT_FLAGS),
    MAKE_ENTRY(NV_CTRL_GPU_PCIE_MAX_LINK_SPEED),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_RESET_TRANSCEIVER_TO_FACTORY_SETTINGS),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_TRANSCEIVER_CHANNEL),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_TRANSCEIVER_MODE),
    MAKE_ENTRY(NV_CTRL_SYNCHRONOUS_PALETTE_UPDATES),
    MAKE_ENTRY(NV_CTRL_DITHERING_DEPTH),
    MAKE_ENTRY(NV_CTRL_CURRENT_DITHERING_DEPTH),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_TRANSCEIVER_CHANNEL_FREQUENCY),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_TRANSCEIVER_CHANNEL_QUALITY),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_TRANSCEIVER_CHANNEL_COUNT),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_PAIR_GLASSES),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_UNPAIR_GLASSES),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_DISCOVER_GLASSES),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_IDENTIFY_GLASSES),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_SYNC_CYCLE),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_MISSED_SYNC_CYCLES),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_BATTERY_LEVEL),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_BATTERY_LEVEL),
    MAKE_ENTRY(NV_CTRL_GVO_ANC_PARITY_COMPUTATION),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_PAIR_EVENT),
    MAKE_ENTRY(NV_CTRL_3D_VISION_PRO_GLASSES_UNPAIR_EVENT),
    { -1, NULL, NULL }
};

static AttrEntry attr_str_table[] = {
    MAKE_ENTRY(NV_CTRL_STRING_PRODUCT_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_VBIOS_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_NVIDIA_DRIVER_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_DEVICE_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_TV_ENCODER_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_GVIO_FIRMWARE_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_GVO_FIRMWARE_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_CURRENT_MODELINE),
    MAKE_ENTRY(NV_CTRL_STRING_ADD_MODELINE),
    MAKE_ENTRY(NV_CTRL_STRING_DELETE_MODELINE),
    MAKE_ENTRY(NV_CTRL_STRING_CURRENT_METAMODE),
    MAKE_ENTRY(NV_CTRL_STRING_ADD_METAMODE),
    MAKE_ENTRY(NV_CTRL_STRING_DELETE_METAMODE),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_PRODUCT_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_PRODUCT_ID),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_SERIAL_NUMBER),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_BUILD_DATE),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_FIRMWARE_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_FIRMWARE_REVISION),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_HARDWARE_VERSION),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_HARDWARE_REVISION),
    MAKE_ENTRY(NV_CTRL_STRING_MOVE_METAMODE),
    MAKE_ENTRY(NV_CTRL_STRING_VALID_HORIZ_SYNC_RANGES),
    MAKE_ENTRY(NV_CTRL_STRING_VALID_VERT_REFRESH_RANGES),
    MAKE_ENTRY(NV_CTRL_STRING_XINERAMA_SCREEN_INFO),
    MAKE_ENTRY(NV_CTRL_STRING_NVIDIA_XINERAMA_INFO_ORDER),
    MAKE_ENTRY(NV_CTRL_STRING_SLI_MODE),
    MAKE_ENTRY(NV_CTRL_STRING_PERFORMANCE_MODES),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_FAN_STATUS),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_TEMPERATURES),
    MAKE_ENTRY(NV_CTRL_STRING_VCSC_PSU_INFO),
    MAKE_ENTRY(NV_CTRL_STRING_GVIO_VIDEO_FORMAT_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_GVO_VIDEO_FORMAT_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_GPU_CURRENT_CLOCK_FREQS),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_HARDWARE_REVISION),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_FIRMWARE_VERSION_A),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_FIRMWARE_DATE_A),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_FIRMWARE_VERSION_B),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_FIRMWARE_DATE_B),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_TRANSCEIVER_ADDRESS),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_GLASSES_FIRMWARE_VERSION_A),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_GLASSES_FIRMWARE_DATE_A),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_GLASSES_ADDRESS),
    MAKE_ENTRY(NV_CTRL_STRING_3D_VISION_PRO_GLASSES_NAME),
    MAKE_ENTRY(NV_CTRL_STRING_CURRENT_METAMODE_VERSION_2),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_TYPE_BASENAME),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_TYPE_ID),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_DP_GUID),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_EDID_HASH),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_TARGET_INDEX),
    MAKE_ENTRY(NV_CTRL_STRING_DISPLAY_NAME_RANDR),
    { -1, NULL, NULL }
};

static AttrEntry attr_bin_table[] = {
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_EDID),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_MODELINES),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_METAMODES_VERSION_1),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_XSCREENS_USING_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_GPUS_USED_BY_XSCREEN),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_GPUS_USING_FRAMELOCK),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_DISPLAY_VIEWPORT),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_FRAMELOCKS_USED_BY_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_GPUS_USING_VCSC),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_VCSCS_USED_BY_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_COOLERS_USED_BY_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_GPUS_USED_BY_LOGICAL_XSCREEN),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_THERMAL_SENSORS_USED_BY_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_GLASSES_PAIRED_TO_3D_VISION_PRO_TRANSCEIVER),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_DISPLAY_TARGETS),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_DISPLAYS_CONNECTED_TO_GPU),
    MAKE_ENTRY(NV_CTRL_BINARY_DATA_METAMODES_VERSION_2),
    { -1, NULL, NULL }
};


static AttrEntry attr_strop_table[] = {
    MAKE_ENTRY(NV_CTRL_STRING_OPERATION_ADD_METAMODE),
    MAKE_ENTRY(NV_CTRL_STRING_OPERATION_GTF_MODELINE),
    MAKE_ENTRY(NV_CTRL_STRING_OPERATION_CVT_MODELINE),
    MAKE_ENTRY(NV_CTRL_STRING_OPERATION_BUILD_MODEPOOL),
    MAKE_ENTRY(NV_CTRL_STRING_OPERATION_GVI_CONFIGURE_STREAMS),
    { -1, NULL, NULL }
};


static void print_perms(NVCTRLAttributePermissionsRec *perms)
{
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_READ)                      ? 'R' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_WRITE)                     ? 'W' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_DISPLAY)                   ? 'D' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_GPU)                       ? 'G' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_FRAMELOCK)                 ? 'F' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_X_SCREEN)                  ? 'X' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_XINERAMA)                  ? 'I' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_VCSC)                      ? 'V' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_GVI)                       ? 'S' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_COOLER)                    ? 'C' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_THERMAL_SENSOR)            ? 'T' : '_');
    printf("%c", (perms->permissions & ATTRIBUTE_TYPE_3D_VISION_PRO_TRANSCEIVER) ? '3' : '_');
}

/* Used to stringify NV_CTRL_XXX #defines */

#define ADD_NVCTRL_CASE(FMT) \
case (FMT):                  \
    return #FMT;

static const char *GetAttrTypeName(int value)
{
    switch (value) {
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_UNKNOWN);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_INTEGER);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_BITMASK);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_BOOL);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_RANGE);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_INT_BITS);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_64BIT_INTEGER);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_STRING);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_BINARY_DATA);
        ADD_NVCTRL_CASE(ATTRIBUTE_TYPE_STRING_OPERATION);
    default:
        return "Invalid Value";
    }
}



int main(void)
{
    Display *dpy;
    Bool ret;
    int event_base, error_base, major, minor, screens, i;
    char *str;
        
    /*
     * open a connection to the X server indicated by the DISPLAY
     * environment variable
     */
    
    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open display '%s'.\n", XDisplayName(NULL));
        return 1;
    }
    
    /*
     * check if the NV-CONTROL X extension is present on this X server
     */

    ret = XNVCTRLQueryExtension(dpy, &event_base, &error_base);
    if (ret != True) {
        fprintf(stderr, "The NV-CONTROL X extension does not exist on '%s'.\n",
                XDisplayName(NULL));
        return 1;
    }

    /*
     * query the major and minor extension version
     */

    ret = XNVCTRLQueryVersion(dpy, &major, &minor);
    if (ret != True) {
        fprintf(stderr, "The NV-CONTROL X extension does not exist on '%s'.\n",
                XDisplayName(NULL));
        return 1;
    }

    /*
     * print statistics thus far
     */

    printf("NV-CONTROL X extension present\n");
    printf("  version        : %d.%d\n", major, minor);
    printf("  event base     : %d\n", event_base);
    printf("  error base     : %d\n", error_base);
    
    /*
     * loop over each screen, and determine if each screen is
     * controlled by the NVIDIA X driver (and thus supports the
     * NV-CONTROL X extension); then, query the string attributes on
     * the screen.
     */

    screens = ScreenCount(dpy);
    for (i = 0; i < screens; i++) {
        if (XNVCTRLIsNvScreen(dpy, i)) {
            printf("Screen %d supports the NV-CONTROL X extension\n", i);

            ret = XNVCTRLQueryStringAttribute(dpy, i,
                                              0, /* XXX not curently used */
                                              NV_CTRL_STRING_PRODUCT_NAME,
                                              &str);
            if (ret) {
                printf("  GPU            : %s\n", str);
                XFree(str);
            }
            
            ret = XNVCTRLQueryStringAttribute(dpy, i,
                                              0, /* XXX not curently used */
                                              NV_CTRL_STRING_VBIOS_VERSION,
                                              &str);
            
            if (ret) {
                printf("  VideoBIOS      : %s\n", str);
                XFree(str);
            }

            ret = XNVCTRLQueryStringAttribute(dpy, i,
                                              0, /* XXX not curently used */
                                              NV_CTRL_STRING_NVIDIA_DRIVER_VERSION,
                                              &str);

            if (ret) {
                printf("  Driver version : %s\n", str);
                XFree(str);
            }
        }
    }

    /*
     * print attribute permission and type information.
     */

    printf("Attributes (Integers):\n");
    for (i = 0; i < NV_CTRL_LAST_ATTRIBUTE; i++) {
        const char *name = attr2str(i, attr_int_table);
        if (name) {
            NVCTRLAttributePermissionsRec perms;

            printf("  (%3d) [Perms: ", i);

            memset(&perms, 0, sizeof(NVCTRLAttributePermissionsRec));

            XNVCTRLQueryAttributePermissions(dpy, i, &perms);
            print_perms(&perms);
            printf("] [ ");
            printf("%-32s", GetAttrTypeName(perms.type));
            printf("] - %s\n", name);
        }
    }

    printf("Attributes (Strings):\n");
    for (i = 0; i < NV_CTRL_STRING_LAST_ATTRIBUTE; i++) {
        const char *name = attr2str(i, attr_str_table);
        if (name) {
            NVCTRLAttributePermissionsRec perms;

            printf("  (%3d) [Perms: ", i);

            memset(&perms, 0, sizeof(NVCTRLAttributePermissionsRec));

            XNVCTRLQueryStringAttributePermissions(dpy, i, &perms);
            print_perms(&perms);
            printf("] [ ");
            printf("%-32s", GetAttrTypeName(perms.type));
            printf("] - %s\n", name);
        }
    }

    printf("Attributes (Binary Data):\n");
    for (i = 0; i < NV_CTRL_BINARY_DATA_LAST_ATTRIBUTE; i++) {
        const char *name = attr2str(i, attr_bin_table);
        if (name) {
            NVCTRLAttributePermissionsRec perms;

            printf("  (%3d) [Perms: ", i);

            memset(&perms, 0, sizeof(NVCTRLAttributePermissionsRec));

            XNVCTRLQueryBinaryDataAttributePermissions(dpy, i, &perms);
            print_perms(&perms);
            printf("] [ ");
            printf("%-32s", GetAttrTypeName(perms.type));
            printf("] - %s\n", name);
        }
    }

    printf("Attributes (String Operations):\n");
    for (i = 0; i < NV_CTRL_STRING_OPERATION_LAST_ATTRIBUTE; i++) {
        const char *name = attr2str(i, attr_strop_table);
        if (name) {
            NVCTRLAttributePermissionsRec perms;

            printf("  (%3d) [Perms: ", i);

            memset(&perms, 0, sizeof(NVCTRLAttributePermissionsRec));

            XNVCTRLQueryStringOperationAttributePermissions(dpy, i, &perms);
            print_perms(&perms);
            printf("] [ ");
            printf("%-32s", GetAttrTypeName(perms.type));
            printf("] - %s\n", name);
        }
    }

    /*
     * close the display connection
     */

    XCloseDisplay(dpy);

    return 0;
}
