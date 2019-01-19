################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Sources_Drive/F2837xD_Adc.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Adc.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_CodeStartBranch.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_CpuTimers.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_CpuTimers.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_DBGIER.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_DBGIER.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_DefaultISR.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_DefaultISR.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Dma.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Dma.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_ECap.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_ECap.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_EPwm.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_EPwm.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_EQep.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_EQep.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Emif.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Emif.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_GlobalVariableDefs.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/source/F2837xD_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Gpio.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Gpio.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_I2C.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_I2C.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Ipc.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Ipc.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Ipc_Driver.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Ipc_Driver.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Ipc_Driver_Lite.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Ipc_Driver_Lite.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Ipc_Driver_Util.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Ipc_Driver_Util.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Mcbsp.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Mcbsp.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_PieCtrl.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_PieCtrl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_PieVect.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_PieVect.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Sci.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Sci.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Spi.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Spi.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_SysCtrl.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_SysCtrl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_TempSensorConv.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_TempSensorConv.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_Upp.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_Upp.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_can.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_can.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_sci_io.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_sci_io.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_sdfm_drivers.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_sdfm_drivers.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_struct.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_struct.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Sources_Drive/F2837xD_usDelay.obj: D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/source/F2837xD_usDelay.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --opt_for_speed=2 --fp_mode=relaxed --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Project" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_headers/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/include" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/F2837xD_common/" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Bsp" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_Mod" --include_path="D:/Wangjiangbo/Documents/GitHub/__CCS/Gas_LaserBoard/Sources_App" --advice:performance=all --define=CPU1 --define=ccs_c2k -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources_Drive/$(basename $(<F)).d_raw" --obj_directory="Sources_Drive" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


