
#include "../../proj/tl_common.h"


#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"

extern void user_init();
extern  void main_loop();

_attribute_ram_code_ void irq_handler(void)
{
#if 0
    u32 src = reg_irq_src;
    if(src & FLD_IRQ_TMR1_EN){
        irq_host_timer1();
        reg_tmr_sta = FLD_TMR_STA_TMR1;
    }
#endif

}


typedef void (*gp_intt_callback_func)();
static gp_intt_callback_func gp4_irq_callback = NULL;

void gp4_irq()
{
	if(gp4_irq_callback && gpio_read(GPIO_GP4))
	{
		u32 test_pins = {GPIO_GP4,GPIO_GP17,GPIO_GP22,GPIO_SWS};
		u32 i=0;
		u32 value = 1;
		
		for(i=0;i<4;i++){
			gpio_set_func(test_pins[i], AS_GPIO);
			gpio_set_output_en(test_pins[i],1);
			gpio_set_input_en(test_pins[i],0);
			gpio_write((test_pins[i],value);
		}
		value = (value+1)%2;
		
		sleep_us(1000);
	}
}

void gpio_test()
{
	irq_enable();
	gpio_set_func(GPIO_GP4, AS_GPIO);
	gpio_set_input_en(GPIO_GP4, 1);
	gpio_set_interrupt(GPIO_GP4, 0);	// rising edge
}

int main (void) {
    cpu_wakeup_init();

    //clock_init
    write_reg8(0x66, 0x26); // 32M pll

    gpio_init();

    gpio_test();
   // reg_irq_mask = FLD_IRQ_ZB_RT_EN;
//    rf_drv_init(CRYSTAL_TYPE);
#if(DEBUG_FROM_FLASH)
	rf_drv_1M_init_flash();
#else
	rf_drv_1M_init_OTP();
#endif
    

#if(MODULE_ADC_ENABLE)
    	adc_init();
#endif
	user_init ();

    while (1) {
        main_loop ();
    }
}



