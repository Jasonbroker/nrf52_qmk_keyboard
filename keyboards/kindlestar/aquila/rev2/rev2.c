#include "hal.h"


void check_standby(void)
{
    // if (PWR->CSR & PWR_CSR_SBF) {
    //     PWR->CR |= PWR_CR_CWUF;
    //     PWR->CR |= PWR_CR_CSBF;
    //     NVIC_SystemReset();
    //     // HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1); // for hal
    // }
}



void POWER_EnterSleep_v1(void)
{

#if(DEBUG_ENABLE)
// DBGMCU->CR |= DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;
#endif
    // https://www.youtube.com/watch?v=O82rj9qxkgs
    // enable pwr control clock
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

    // #if (!defined(DEBUG) || !defined(USE_DBG_STANDBY))
    // /* Disable DBG_STANDBY. Prevent DBG_STANDBY from being enabled by debugger when
    //  * downloading programs, causing standby mode power consumption to be too high */
    // SET_BIT(RCC->APB2ENR, RCC_APB2ENR_DBGMCUEN);
    // CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
    // #endif


    // // set sleepdeep mask
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // // power down deep sleep,  1 is standby mode
    PWR->CR |= PWR_CR_PDDS;

    PWR->CSR |= PWR_CSR_EWUP1;
    PWR->CR |= PWR_CR_CWUF;


    __WFI();
}
