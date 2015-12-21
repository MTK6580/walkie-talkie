###################################################################
# Include Project Feautre  (cust_bldr.h)
###################################################################

ifeq ("$(MTK_EMMC_SUPPORT)","yes")
CFG_BOOT_DEV :=BOOTDEV_SDMMC
else
CFG_BOOT_DEV :=BOOTDEV_NAND
endif

CFG_FPGA_PLATFORM :=1
CFG_BATTERY_DETECT :=0
#for NAND project only
#CFG_PMT_SUPPORT :=1


