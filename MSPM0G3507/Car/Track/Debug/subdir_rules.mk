################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/27ti/ccs2011/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/src" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/inc" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Board" -I"D:/1 END/3 BW/only_line3/1/lcd_m0" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Debug" -I"C:/TI/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_05_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1831424673: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/27ti/ccs2011/ccs/utils/sysconfig_1.24.0/sysconfig_cli.bat" --script "D:/1 END/3 BW/only_line3/1/lcd_m0/empty.syscfg" -o "." -s "C:/TI/mspm0_sdk_2_05_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1831424673 ../empty.syscfg
device.opt: build-1831424673
device.cmd.genlibs: build-1831424673
ti_msp_dl_config.c: build-1831424673
ti_msp_dl_config.h: build-1831424673
Event.dot: build-1831424673

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/27ti/ccs2011/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/src" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/inc" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Board" -I"D:/1 END/3 BW/only_line3/1/lcd_m0" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Debug" -I"C:/TI/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_05_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/TI/mspm0_sdk_2_05_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/27ti/ccs2011/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/src" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/BSP/inc" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Board" -I"D:/1 END/3 BW/only_line3/1/lcd_m0" -I"D:/1 END/3 BW/only_line3/1/lcd_m0/Debug" -I"C:/TI/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_05_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


