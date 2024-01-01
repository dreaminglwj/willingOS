#include "sysTick.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数

void setSysTickClkSource(uint32_t source) {
    if ( IsSysTickClkSource( source ) ) {
        if (source == SysTickClkSourceHCLK) {
            SysTick->CTRL |= SysTickClkSourceHCLK;
        } else {
            SysTick->CTRL |= SysTickClkSourceHCLKDiv8;
        }
    }
}

/* 初始化系统节拍 */
void initSysTick( uint8_t sysClk ) {
    uint32_t reloadValue = 0;
    setSysTickClkSource( SysTickClkSourceHCLK );
    reloadValue = SysClockFrequence / SYS_TICK_RATE; /* 根据configTICK_RATE_HZ设定溢出时间
		//										     reload为24位寄存器,最大值:16777216,在72M下,约0.233s */

	fac_us=sysClk;				
	reloadValue = sysClk * (1000000/SYS_TICK_RATE);
//  SystemCoreClock/1000000;				
	//reloadValue*=1000000/SYS_TICK_RATE;			
												
	fac_ms=fac_us * fac_us;			   

    /* 配置硬件 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* 开启systick 中断 */
    SysTick->LOAD = reloadValue; /* 每1/SYS_TICK_RATE秒中断一次 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* 开启SysTick */
}


void delay_ms(uint32_t n){
	u32 temp;		   
	SysTick->LOAD=(u32)n*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	


}


void delay_us(uint32_t n){
	u32 temp;	    	 
	SysTick->LOAD=n*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

void delay_xms(uint32_t n){

}

