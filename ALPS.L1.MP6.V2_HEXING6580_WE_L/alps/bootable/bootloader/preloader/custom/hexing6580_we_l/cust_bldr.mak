###################################################################
# Include Project Feautre  (cust_bldr.h)
###################################################################

ifeq ("$(MTK_EMMC_SUPPORT)","yes")
CFG_BOOT_DEV :=BOOTDEV_SDMMC
else
CFG_BOOT_DEV :=BOOTDEV_NAND
endif

#for NAND project only
#CFG_PMT_SUPPORT :=1


