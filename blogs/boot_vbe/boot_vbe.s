.section .text  
.global _start  
.code16  
  
 _start:  
        jmp     main

clear_screen:               # 清屏函数  
        movb    $0x06,  %ah     # 功能号0x06  
        movb    $0,     %al     # 上卷全部行，即清屏  
        movb    $0,     %ch     # 左上角行  
        movb    $0,     %ch     # 左上角列    
        movb    $24,    %dh     # 右下角行  
        movb    $79,    %dl     # 右下角列  
        movb    $0x07,  %bh     # 空白区域属性  
        int     $0x10  
        ret  

set_video_mode_0x13:  
        movb    $0,         %ah         # 功能号0x0  
        movb    $MODE_0x13, %al         # 显示模式  
        int     $0x10  

        movw    $0x13,      video_mode  
        movw    $320,       screen_x  
        movw    $200,       screen_y  
        movl    $0xb8000,   video_ram  

        ret  
set_video_mode_vbe_0x103:

        movw    $BUFFER_SEG,        %ax  
        movw    %ax,                %es  
        movw    %ax,                %ds  
        xorw    %di,                %di  

        movb    $0x4f,              %ah         # VBE标准  
        movb    $0x00,              %al         # 功能号  
        int     $0x10  

        cmp     $0x004f,            %ax         # 若有VBE，AX应该为0x004f  
        jne     1f  
        movw    0x04(%di),          %ax  
        cmp     $0x0200,            %ax         # 若VBE版本不是2.0以上，不能使用髙分辨率  
        jb      1f  

        movw    $MODE_VBE_0x103,    %cx  
        movb    $0x4f,              %ah         # 表明VBE标准  
        movb    $0x01,              %al         # 子功能号  
        int     $0x10  

        cmpb    $0x00,              %ah         # 是否调用成功  
        jne     1f  
        cmpb    $0x4f,              %al         # 是否支持该模式  
        jne     1f  
        cmpb    $8,                 0x19(%di)   # 颜色是否占8bit  
        jne     1f  
        cmpb    $4,                 0x1b(%si)   # 颜色的指定方法是否为4，即调色板方式  
        jne     1f  
        movw    (%di),              %ax  
        andw    $0x0080,            %ax  
        jz      1f                              # AX第7个比特是否为1，该位表示线性帧缓存是否有效  

        movw    $MODE_VBE_0x103,    %bx  
        addw    $0x4000,            %bx         # BX第14个比特表示是否使用大的线性缓存区，故置位  
        movb    $0x4f,              %ah         # 表示使用VBE标准  
        movb    $0x02,              %al         # 功能号，表示设置模式  
        int     $0x10  

        movw    $MODE_VBE_0x103,    video_mode  
        movw    0x12(%di),          %ax  
        movw    %ax,                screen_x  
        movw    0x02(%di),          %ax  
        movw    %ax,                screen_y  
        movl    0x28(%di),          %eax  
        movl    %eax,               video_ram  
        movw    $1,                 %ax  
        ret  
        1:  
        movw    $0,                 %ax  
        ret  

        set_screen_bk_color:  
        movw    $VIDEO_PALLETE_PORT,    %dx  
        movb    $PA_INDEX_BACKGROUND,   %al  
        outb    %al,                    %dx  

        movw    $COLOR_SELECTION_PORT,  %dx  
        movb    $10,                     %al     # 红  
        outb    %al,                    %dx  
        movb    $5,                     %al     # 绿  
        outb    %al,                    %dx  
        movb    $1,                    %al     # 蓝（亮度18/63）  
        outb    %al,                    %dx  
        ret  

draw_some_pixels:

        movw    $VIDEO_PALLETE_PORT,    %dx  
        movb    $PA_INDEX_WHITE,        %al  
        outb    %al,                    %dx  
        movw    $COLOR_SELECTION_PORT,  %dx  
        movb    $63,                    %al     # 红  
        outb    %al,                    %dx  
        movb    $63,                    %al     # 绿  
        outb    %al,                    %dx  
        movb    $63,                    %al     # 蓝  
        outb    %al,                    %dx  

        movw    $VIDEO_SEG_GRAPHIC,     %ax  
        movw    %ax,                    %es

        movw    $(800*5),               %di     # 从第五行像素开始开始  
        movb    $PA_INDEX_WHITE,        %al  
        movw    $800,                   %cx     # 画800个连续像素即一条直线

draw_a_pixel:  
        stosb 
        #addl   $799,                   %edi
        loop    draw_a_pixel  
        ret  

main:  
        movw    %cx,    %ax  
        movw    %ax,    %ds  
        movw    %ax,    %es  

        call    clear_screen                    # 清屏  
        call    set_video_mode_vbe_0x103        # 设置显示模式  

        cmpw    $0,     %ax  
        jne     1f  
        call    set_video_mode_0x13  
        1:  
        call    set_screen_bk_color # 设置背景颜色  
        call    draw_some_pixels    # 绘制像素  

        1:  
        jmp     1b  
        VIDEO_SEG_TEXT      = 0x0e00  
        VIDEO_SEG_GRAPHIC   = 0xa000  
        BUFFER_SEG          = 0x800  

        VIDEO_PALLETE_PORT  = 0x3c8  
        COLOR_SELECTION_PORT= 0x3c9  

        MODE_0x13           = 0x13  
        MODE_VBE_0x105      = 0x0105  
        MODE_VBE_0x103      = 0x0103  

        PA_INDEX_BACKGROUND = 0x0  
        PA_INDEX_WHITE      = 0x1  

        video_mode:  
        .short  0  
        screen_x:  
        .short  0  
        screen_y:  
        .short  0  
        video_ram:  

        .long   0  
        .org    0x1fe,  0x90  
        .word   0xaa55  
