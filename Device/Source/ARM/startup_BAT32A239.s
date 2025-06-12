/**************************************************************************//**
 * @file     startup_BAT32A239.s
 * @brief    CMSIS Cortex-M ARMv6-M based Core Device Startup File for BAT32A239
 * @version  V1.00
 * @date     2019/04/24
 ******************************************************************************/
.syntax unified
.cpu cortex-m0
.thumb

/* 栈配置 */
.equ Stack_Size, 0x00000600

.section .stack
.align 3
.global __initial_sp
Stack_Mem:
    .space Stack_Size
__initial_sp = Stack_Mem + Stack_Size

/* 堆配置 */
.equ Heap_Size, 0x00000100

.section .heap
.align 3
.global __heap_base
.global __heap_limit
Heap_Mem:
    .space Heap_Size
__heap_base = Heap_Mem
__heap_limit = Heap_Mem + Heap_Size

/* 向量表 */
.section .vectors, "a"
.global __Vectors
.global __Vectors_End
.global __Vectors_Size

__Vectors:
    .word __initial_sp          /* 栈顶指针 */
    .word Reset_Handler         /* 复位中断 */
    .word NMI_Handler           /* NMI */
    .word HardFault_Handler     /* 硬错误 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word SVC_Handler           /* SVC */
    .word 0                     /* 保留 */
    .word 0                     /* 保留 */
    .word PendSV_Handler        /* PendSV */
    .word SysTick_Handler       /* SysTick */

    /* 外设中断 */
    .word IRQ00_Handler         /* LVI */
    .word IRQ01_Handler         /* INTP0/INTP6 */
    .word IRQ02_Handler         /* INTP1/INTP7 */
    .word IRQ03_Handler         /* INTP2/INTP8 */
    .word IRQ04_Handler         /* INTP3/INTP9 */
    .word IRQ05_Handler         /* INTP4/INTP10 */
    .word IRQ06_Handler         /* INTP5/INTP11 */
    .word IRQ07_Handler         /* ST2/SPI20/IIC20 */
    .word IRQ08_Handler         /* SR2/SPI21/IIC21 */
    .word IRQ09_Handler         /* SRE2 */
    .word IRQ10_Handler         /* ST0/SPI00/IIC00 */
    .word IRQ11_Handler         /* SR0/SPI01/IIC01 */
    .word IRQ12_Handler         /* SRE0/TM01H */
    .word IRQ13_Handler         /* ST1/SPI10/IIC10 */
    .word IRQ14_Handler         /* SR1/SPI11/IIC11 */
    .word IRQ15_Handler         /* SRE1/TM03H */
    .word IRQ16_Handler         /* IICA/DIV */
    .word IRQ17_Handler         /* TM00 */
    .word IRQ18_Handler         /* TM01 */
    .word IRQ19_Handler         /* TM02 */
    .word IRQ20_Handler         /* TM03 */
    .word IRQ21_Handler         /* ADC */
    .word IRQ22_Handler         /* RTC/IT */
    .word IRQ23_Handler         /* KEY */
    .word IRQ24_Handler         /* CMP0 */
    .word IRQ25_Handler         /* CMP1 */
    .word IRQ26_Handler         /* TMA */
    .word IRQ27_Handler         /* TMM0 */
    .word IRQ28_Handler         /* TMM1 */
    .word IRQ29_Handler         /* TMB */
    .word IRQ30_Handler         /* TMC */
    .word IRQ31_Handler         /* FMC */
__Vectors_End:

.equ __Vectors_Size, __Vectors_End - __Vectors

/* 复位处理 */
.section .text.Reset_Handler
.thumb_func
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    .extern SystemInit
    .extern main
    
    /* 初始化.data段（从FLASH到RAM） */
    ldr r1, =__etext          /* .data段在FLASH中的起始地址 */
    ldr r2, =__data_start__   /* .data段在RAM中的起始地址 */
    ldr r3, =__data_end__     /* .data段在RAM中的结束地址 */
    
    /* 检查.data段大小是否为零 */
    cmp r2, r3
    beq .L_data_copy_done
    
.L_data_copy_loop:
    ldr r0, [r1]           /* 从FLASH加载 */
    adds r1, #4            /* 递增源指针 */
    str r0, [r2]           /* 存储到RAM */
    adds r2, #4            /* 递增目标指针 */
    cmp r2, r3             /* 检查是否结束 */
    blo .L_data_copy_loop  /* 如果未结束则继续 */
    
.L_data_copy_done:
    
    /* 清零.bss段 */
    ldr r1, =__bss_start__   /* .bss段起始地址 */
    ldr r2, =__bss_end__     /* .bss段结束地址 */
    movs r0, #0             /* 清零寄存器 */
    
    /* 检查.bss段大小是否为零 */
    cmp r1, r2
    beq .L_bss_clear_done
    
.L_bss_clear_loop:
    str r0, [r1]           /* 存储0 */
    adds r1, #4            /* 递增指针 */
    cmp r1, r2             /* 检查是否结束 */
    blo .L_bss_clear_loop  /* 如果未结束则继续 */
    
.L_bss_clear_done:
    
    /* 调用系统初始化和main函数 */
    ldr r0, =SystemInit
    blx r0
    ldr r0, =main
    bx r0
.size Reset_Handler, . - Reset_Handler

/* 默认中断处理 */
.macro DEFAULT_HANDLER handler_name
.section .text.\handler_name
.thumb_func
.weak \handler_name
.type \handler_name, %function
\handler_name:
    b .
.size \handler_name, . - \handler_name
.endm

DEFAULT_HANDLER NMI_Handler
DEFAULT_HANDLER HardFault_Handler
DEFAULT_HANDLER MemManage_Handler
DEFAULT_HANDLER BusFault_Handler
DEFAULT_HANDLER UsageFault_Handler
DEFAULT_HANDLER SVC_Handler
DEFAULT_HANDLER DebugMon_Handler
DEFAULT_HANDLER PendSV_Handler
DEFAULT_HANDLER SysTick_Handler
DEFAULT_HANDLER IRQ00_Handler
DEFAULT_HANDLER IRQ01_Handler
DEFAULT_HANDLER IRQ02_Handler
DEFAULT_HANDLER IRQ03_Handler
DEFAULT_HANDLER IRQ04_Handler
DEFAULT_HANDLER IRQ05_Handler
DEFAULT_HANDLER IRQ06_Handler
DEFAULT_HANDLER IRQ07_Handler
DEFAULT_HANDLER IRQ08_Handler
DEFAULT_HANDLER IRQ09_Handler
DEFAULT_HANDLER IRQ10_Handler
DEFAULT_HANDLER IRQ11_Handler
DEFAULT_HANDLER IRQ12_Handler
DEFAULT_HANDLER IRQ13_Handler
DEFAULT_HANDLER IRQ14_Handler
DEFAULT_HANDLER IRQ15_Handler
DEFAULT_HANDLER IRQ16_Handler
DEFAULT_HANDLER IRQ17_Handler
DEFAULT_HANDLER IRQ18_Handler
DEFAULT_HANDLER IRQ19_Handler
DEFAULT_HANDLER IRQ20_Handler
DEFAULT_HANDLER IRQ21_Handler
DEFAULT_HANDLER IRQ22_Handler
DEFAULT_HANDLER IRQ23_Handler
DEFAULT_HANDLER IRQ24_Handler
DEFAULT_HANDLER IRQ25_Handler
DEFAULT_HANDLER IRQ26_Handler
DEFAULT_HANDLER IRQ27_Handler
DEFAULT_HANDLER IRQ28_Handler
DEFAULT_HANDLER IRQ29_Handler
DEFAULT_HANDLER IRQ30_Handler
DEFAULT_HANDLER IRQ31_Handler

/* 堆栈初始化 */
.ifdef __MICROLIB
    .global __initial_sp
    .global __heap_base
    .global __heap_limit
.else
    .extern __use_two_region_memory
    .global __user_initial_stackheap

.section .text.__user_initial_stackheap
.thumb_func
__user_initial_stackheap:
    ldr r0, = Heap_Mem
    ldr r1, = (Stack_Mem + Stack_Size)
    ldr r2, = (Heap_Mem + Heap_Size)
    ldr r3, = Stack_Mem
    bx lr
.size __user_initial_stackheap, . - __user_initial_stackheap
.endif

.end