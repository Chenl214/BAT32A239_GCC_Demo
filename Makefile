# BAT32A239微控制器的GCC编译工具链的Makefile
TARGET = GCC_Demo
MCU = cortex-m0plus   	# 使用的ARM Cortex-M0+微控制器架构


# 工具链定义
CC = arm-none-eabi-gcc.exe      # ARM嵌入式C编译器
AS = arm-none-eabi-gcc.exe -x assembler-with-cpp  # ARM汇编器(支持预处理)
CP = arm-none-eabi-objcopy.exe  # 目标文件转换工具
SZ = arm-none-eabi-size.exe     # 查看目标文件大小的工具


# 优化选项和编译标志
OPT = -Os  										# 优化级别为-Os(优化代码大小)，在M0+上比-O2更合适
DEBUG = -g  									# 生成调试信息
MCUFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian # 指定MCU架构和指令集
DEFS = -DBAT32A239  							# 定义宏，标识目标芯片


# 头文件包含路径
INCLUDES = -IDriver/inc 	\
           -IDevice/Source 	\
		   -IDevice/Include \
		   -ICMSIS/Include


# 链接脚本定义
LDSCRIPT = Device/Source/GCC/gcc_arm.ld


# 源文件列表，包含所有需要编译的C和汇编文件
C_SOURCES = \
  main.c \
  Device/Source/system_BAT32A239.c \
  Device/Source/isr_BAT32A239.c

# 启动汇编文件
ASM_SOURCES = \
  Device/Source/GCC/startup_BAT32A239.s


# 目标文件生成规则
OBJECTS = $(addprefix build/,$(notdir $(C_SOURCES:.c=.o)))  	# C源文件生成的目标文件
vpath %.c $(sort $(dir $(C_SOURCES)))  							# 设置C源文件搜索路径
OBJECTS += $(addprefix build/,$(notdir $(ASM_SOURCES:.s=.o)))  	# 汇编源文件生成的目标文件
vpath %.s $(sort $(dir $(ASM_SOURCES)))  						# 设置汇编源文件搜索路径


# 编译器选项
CFLAGS = $(MCUFLAGS) $(OPT) $(DEBUG) $(DEFS) $(INCLUDES) \
         -Wall -fdata-sections -ffunction-sections \
         -std=gnu99 --specs=nano.specs -u _printf_float

ASFLAGS = $(MCUFLAGS) $(DEBUG)  # 汇编器选项


# 链接器选项
LDFLAGS = $(MCUFLAGS) -specs=nano.specs -T "$(CURDIR)/$(LDSCRIPT)" $(DEBUG) \
          -Wl,-Map=build/$(TARGET).map,--cref -Wl,--gc-sections \
          -lc -lm -lnosys


# 默认构建目标
all: build/$(TARGET).elf build/$(TARGET).hex build/$(TARGET).bin


# 构建规则
build/%.o: %.c Makefile | build  # C源文件编译规则
	$(CC) -c $(CFLAGS) $< -o $@

build/%.o: %.s Makefile | build  # 汇编源文件编译规则
	$(AS) -c $(ASFLAGS) $< -o $@

build/$(TARGET).elf: $(OBJECTS) $(LDSCRIPT)  # 生成ELF文件
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

build/$(TARGET).hex: build/$(TARGET).elf  # 生成HEX文件
	$(CP) -O ihex $< $@

build/$(TARGET).bin: build/$(TARGET).elf  # 生成BIN文件
	$(CP) -O binary -S $< $@

build:  # 创建构建目录
	mkdir build


# 如果链接脚本不存在则生成
$(LDSCRIPT):
	@echo "Generating linker script..."
	@echo "MEMORY {" > $(LDSCRIPT)
	@echo "  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K" >> $(LDSCRIPT)
	@echo "  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 32K" >> $(LDSCRIPT)
	@echo "}" >> $(LDSCRIPT)
	@echo "INCLUDE cortex-m0plus.ld" >> $(LDSCRIPT)

clean:  # 清理构建目录
	rd /s /q build

flash: build/$(TARGET).bin  # 烧录目标(需要实现具体烧录命令)
	@echo "Flashing not configured - please implement flash tool command"

.PHONY: all clean flash  # 声明伪目标