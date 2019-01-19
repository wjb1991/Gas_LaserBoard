################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Sources_DriveLib/can.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/can.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/interrupt.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/interrupt.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/sysctl.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/sysctl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/systick.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/systick.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/uart.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/uart.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/uartstdio.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/utils/uartstdio.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/usb.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/usb.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_DriveLib/usb_hal.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/driverlib/usb_hal.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=_FLASH --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_DriveLib/$(basename $(<F)).d_raw" --obj_directory="Sources_DriveLib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


