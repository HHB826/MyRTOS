#define NVIC_INT_CTRL       0xE000ED04      // 中断控制及状态寄存器
#define NVIC_PENDSVSET      0x10000000      // 触发软件中断的值
#define NVIC_SYSPRI2        0xE000ED22      // 系统优先级寄存器
#define NVIC_PENDSV_PRI     0x000000FF      // 配置优先级

#define MEM32(addr)         *(volatile unsigned long *)(addr)
#define MEM8(addr)          *(volatile unsigned char *)(addr)

void triggerPendSVC (void) 
{
    MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;   // 向NVIC_SYSPRI2写NVIC_PENDSV_PRI，设置其为最低优先级
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // 向NVIC_INT_CTRL写NVIC_PENDSVSET，用于PendSV
}

/**********************************************************************************************************
** Function name        :   PendSV_Handler
** Descriptions         :   PendSV异常处理函数。很有些会奇怪，看不到这个函数有在哪里调用。实际上，只要保持函数头不变
**                          void PendSV_Handler (), 在PendSV发生时，该函数会被自动调用
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
typedef struct _BlockType_t 
{
    unsigned long * stackPtr;
}BlockType_t;

BlockType_t * blockPtr;

void delay (int count) 
{
    while (--count > 0);
}

int flag;

unsigned long stackBuffer[1024];
BlockType_t block;

int main () 
{
    blockPtr = &block;
    for (;;) {
        flag = 0;
        delay(100);
        flag = 1;
        delay(100);
        

        block.stackPtr = &stackBuffer[1024];		
        triggerPendSVC();
    }
    
    return 0;
}
