# mt6582 platform boardconfig

# Use the non-open-source part, if present
-include vendor/mediatek/mt6580/BoardConfigVendor.mk

# Use the common part
include device/mediatek/common/BoardConfig.mk

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_VARIANT := cortex-a7
TARGET_CPU_SMP := true
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true
TARGET_BOARD_PLATFORM := mt6580
MTK_PLATFORM := MT6580
TARGET_NO_FACTORYIMAGE:=true
KERNELRELEASE := 3.4

ifeq ($(MTK_EMMC_SUPPORT), yes)
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_FLASH_BLOCK_SIZE := 4096
else
TARGET_USERIMAGES_USE_EXT4 := false
TARGET_USERIMAGES_USE_UBIFS := true

PRODUCT_PACKAGES += mkfs_ubifs
PRODUCT_PACKAGES += ubinize

MKUBIFS = $(HOST_OUT_EXECUTABLES)/mkfs_ubifs$(HOST_EXECUTABLE_SUFFIX)
UBINIZE = $(HOST_OUT_EXECUTABLES)/ubinize$(HOST_EXECUTABLE_SUFFIX)
endif

# display related
TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true
ifeq ($(FPGA_EARLY_PORTING), no)
MTK_HWC_SUPPORT := yes
else
MTK_HWC_SUPPORT := No
endif

MTK_HWC_VERSION := 1.4.1

# For MM thread priority
ifneq ($(MTK_BASIC_PACKAGE), yes)
MTK_USE_RT_PRIORITY := yes
endif

BOARD_CONNECTIVITY_VENDOR := MediaTek
BOARD_USES_MTK_AUDIO := true

ifeq ($(MTK_AGPS_APP), yes)
   BOARD_AGPS_SUPL_LIBRARIES := true
else
   BOARD_AGPS_SUPL_LIBRARIES := false
endif

ifeq ($(MTK_GPS_SUPPORT), yes)
  BOARD_GPS_LIBRARIES := true
else
  BOARD_GPS_LIBRARIES := false
endif

ifeq ($(strip $(BOARD_CONNECTIVITY_VENDOR)), MediaTek)

BOARD_CONNECTIVITY_MODULE := conn_soc 
BOARD_MEDIATEK_USES_GPS := true
# bobule KK aosp workaround to fix build error
#ifeq ($(strip $(BOARD_CONNECTIVITY_MODULE)), soc_conn)
#combo_config := hardware/mediatek/config/$(strip $(BOARD_CONNECTIVITY_MODULE))/board_config.mk
#include $(combo_config)
#endif

endif

ifeq ($(MTK_WLAN_SUPPORT), yes)
BOARD_WLAN_DEVICE := MediaTek
WPA_SUPPLICANT_VERSION := VER_0_8_X_MTK
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_mt66xx
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_mt66xx
WIFI_DRIVER_FW_PATH_PARAM:="/dev/wmtWifi"
WIFI_DRIVER_FW_PATH_STA:=STA
WIFI_DRIVER_FW_PATH_AP:=AP
WIFI_DRIVER_FW_PATH_P2P:=P2P
ifneq ($(strip $(MTK_BSP_PACKAGE)),yes)
MTK_WIFI_CHINESE_SSID := yes
ifeq ($(MTK_EAP_SIM_AKA), yes)
CONFIG_RILD_MULTI_SIM := yes
endif
endif
ifeq ($(strip $(MTK_BSP_PACKAGE)),yes)
MTK_WIFI_GET_IMSI_FROM_PROPERTY := yes
endif
endif

# mkbootimg header, which is used in LK
BOARD_KERNEL_BASE = 0x80000000
BOARD_KERNEL_OFFSET = 0x00008000
BOARD_RAMDISK_OFFSET = 0x04000000
BOARD_TAGS_OFFSET = 0xE000000
ifneq ($(MTK_K64_SUPPORT), yes)
		BOARD_KERNEL_CMDLINE = bootopt=64S3,32S1,32S1
else
		TARGET_USES_64_BIT_BINDER := true
TARGET_IS_64_BIT := true
BOARD_KERNEL_CMDLINE = bootopt=64S3,32N2,64N2
endif
BOARD_MKBOOTIMG_ARGS := --kernel_offset $(BOARD_KERNEL_OFFSET) --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_TAGS_OFFSET)


# Add MTK compile options to wrap MTK's modifications on AOSP.
COMMON_GLOBAL_CFLAGS += -DMTK_AOSP_ENHANCEMENT
COMMON_GLOBAL_CPPFLAGS += -DMTK_AOSP_ENHANCEMENT

# ptgen
MTK_PTGEN_CHIP := $(shell echo $(TARGET_BOARD_PLATFORM) | tr '[a-z]' '[A-Z]')
include device/mediatek/build/build/tools/ptgen/$(MTK_PTGEN_CHIP)/ptgen.mk

