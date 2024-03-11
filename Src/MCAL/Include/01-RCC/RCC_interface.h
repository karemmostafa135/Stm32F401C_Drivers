#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H
#include "STD_types.h"

/************* options for System Clock ***********/
#define SYSCLK_HSI 		0x0000000C
#define SYSCLK_HSE		0x0000000D
#define SYSCLK_PLL		0x0000000E

/************* options for enable clocks ***********/
#define  HSI				0x00000001
#define  HSE				0x00010000
#define  PLL				0x01000000

/********** options to enable and disable Peripherals on AHB1 bus ******/
#define AHB1ENR_DMA2EN					22
#define AHB1ENR_DMA1EN					21
#define AHB1ENR_CRCEN					12
#define AHB1ENR_GPIOHEN					7
#define AHB1ENR_GPIOEEN					4
#define AHB1ENR_GPIODEN					3
#define AHB1ENR_GPIOCEN					2
#define AHB1ENR_GPIOBEN					1
#define AHB1ENR_GPIOAEN					0

/********** options to enable and disable Peripherals on AHB2 bus ******/
#define AHB2ENR_OTGFSEN				7

/********** options to enable and disable Peripherals on APB1 bus ******/
#define APB1ENR_PWREN					28
#define APB1ENR_I2C3EN					23
#define APB1ENR_I2C2EN					22
#define APB1ENR_I2C1EN					21
#define APB1ENR_USART2EN				17
#define APB1ENR_SPI3EN					15
#define APB1ENR_SPI2EN					14
#define APB1ENR_WWDGEN					11
#define APB1ENR_TIM5EN					3
#define APB1ENR_TIM4EN					2
#define APB1ENR_TIM3EN					1
#define APB1ENR_TIM2EN					0

/********** options to enable and disable Peripherals on APB2 bus ******/
#define APB2ENR_TIM11EN					18
#define APB2ENR_TIM10EN					17
#define APB2ENR_TIM9EN					16
#define APB2ENR_SYSCFGEN				14
#define APB2ENR_SPI4EN					13
#define APB2ENR_SPI1EN					12
#define APB2ENR_SDIOEN					11
#define APB2ENR_ADC1EN					8
#define APB2ENR_USART6EN				5
#define APB2ENR_USART1EN				4
#define APB2ENR_TIM1EN				 	0

/********** options to enable and disable Peripherals on AHB1LPENR bus ******/
#define AHB1LPENR_DMA2LPEN				22
#define AHB1LPENR_DMA1LPEN				21
#define AHB1LPENR_SRAM1LPEN				16
#define AHB1LPENR_FLITFLPEN				15
#define AHB1LPENR_CRCLPEN				12
#define AHB1LPENR_GPIOHLPEN				7
#define AHB1LPENR_GPIOELPEN				4
#define AHB1LPENR_GPIODLPEN				3
#define AHB1LPENR_GPIOCLPEN				2
#define AHB1LPENR_GPIOBLPEN				1
#define AHB1LPENR_GPIOALPEN				0

/********** options to enable and disable Peripherals on AHB2LPENR bus ******/
#define AHB2LPENR_OTGFSLPEN				7

/********** options to enable and disable Peripherals on APB1LPENR bus ******/
#define APB1LPENR_PWRLPEN					28
#define APB1LPENR_I2C3LPEN			        23
#define APB1LPENR_I2C2LPEN			        22
#define APB1LPENR_I2C1LPEN		     	    21
#define APB1LPENR_USART2LPEN			    17
#define APB1LPENR_SPI3LPEN					15
#define APB1LPENR_SPI2LPEN					14
#define APB1LPENR_WWDGLPEN					11
#define APB1LPENR_TIM5LPEN					3
#define APB1LPENR_TIM4LPEN					2
#define APB1LPENR_TIM3LPEN					1
#define APB1LPENR_TIM2LPEN					0

/********** options to enable and disable Peripherals on APB2LPENR bus ******/
#define APB2LPENR_TIM11LPEN					18
#define APB2LPENR_TIM10LPEN					17
#define APB2LPENR_TIM9LPEN					16
#define APB2LPENR_SYSCFGLPEN				14
#define APB2LPENR_SPI4LPEN					13
#define APB2LPENR_SPI1LPEN					12
#define APB2LPENR_SDIOLPEN					11
#define APB2LPENR_ADC1LPEN					8
#define APB2LPENR_USART6LPEN				5
#define APB2LPENR_USART1LPEN				4
#define APB2LPENR_TIM1LPEN				 	0

/*********** to enable and disable peripherals *********/
#define ENABLE_PERI			    1
#define DISABLE_PERI			0
/******************** typedef sector ***********/







typedef enum{
	RCC_Nok,
	RCC_Ok
}RCC_Error_status_t;

/*******
 * @brief function to enable a clock on the system
 * @options
 * 1/HSI
 * 2/HSE
 * 3/PLL
 * @return if return equal 0 this means its not ok and if its 1 it means it is okay
 */

RCC_Error_status_t RCC_EnableClock(uint32_t Copy_Clk);

 /*******
  * @brief function to disable a clock on the system
  * @options
  * 1/HSI
  * 2/HSE
  * 3/PLL
  * @return if return equal 0 this means its not ok and if its 1 it means it is okay
  * @constrains before disable any clock make sure it is not the system clock
  */

 RCC_Error_status_t RCC_DisableClock(uint32_t Copy_Clk);

 /****
  * @brief function to set the clock source for pll
  * @options
  * 1/HSI
  * 2/HSE
  * @constrains before choosing any source to PLL make sure it is on and ready

  */
/**
 *@brief to get if clock is ready or not
 *@options
 *1/HSI
 *1/HSE
 *1/PLL
 *@return 1 if read 0 if not and 2 if wrong paramter
 */
 uint32_t RCC_Get_Ready_Status(uint32_t Get_read );


 /****
   * @brief function to set the clock source for pll
   * @options
   * 1/HSI
   * 2/HSE
   * @constrains before choosing any source to PLL make sure it is on and ready

   */

RCC_Error_status_t RCC_Set_PLLSrc(uint32_t Copy_PllSrc);

/*
 * @brief functions to set the PLL configurations factors
 * @options
 * 1/ Copy_N_factor from 2 to 432
 * 2/ Copy_M_factor from 2 to 63
 * 3/Copy_P_factor 2,4,6,8
 */

RCC_Error_status_t RCC_PllConfig(uint32_t Copy_N_factor,uint32_t Copy_M_factor,uint32_t Copy_P_factor );

/****
  * @brief function to set the clock source for system
  * @options
  * 1/SYSCLK_HSI
  * 2/SYSCLK_HSE
  * 3/SYSCLK_PLL
  * @constrains before choosing any source  make sure it is on and ready

  */

  RCC_Error_status_t RCC_Set_SysClk(uint32_t Copy_ClkSrc);

  /****
    * @brief function to get the clock source for pll
    * @options
    * 1/HSI represented by 0
    * 2/HSE represented by 1
    * 3/PLL represented by 2
    */

  void RCC_Get_SysClk(uint32_t* Copy_ClkSrc);

  /*
   * @brief to enable and disable an peripheral connected on AHB1 bus
   * @options for peripherals
    1/AHB1ENR_DMA2EN
    2/AHB1ENR_DMA1EN
	3/AHB1ENR_CRCEN
	4/AHB1ENR_GPIOHEN
	5/AHB1ENR_GPIOEEN
	6/AHB1ENR_GPIODEN
	7/AHB1ENR_GPIOCEN
	8/AHB1ENR_GPIOBEN
    9/AHB1ENR_GPIOAEN
   * @options for mode
   * 1/ENABLE_PERI
   * 2/DISABLE_PERI
   */

  RCC_Error_status_t RCC_AHB1ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);

  /*
   * @brief to enable and disable an peripheral connected on AHB2 bus
   * @options for peripherals
   * AHB2ENR_OTGFSEN
   * @options for mode
   * 1/ENABLE_PERI
   * 2/DISABLE_PERI
   */

  RCC_Error_status_t RCC_AHB2ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);

  /*
     * @brief to enable and disable an peripheral connected on APB1 bus
     * @options for peripherals
     * 1/APB1ENR_PWREN
     * 2/APB1ENR_I2C3EN
     * 3/APB1ENR_I2C2EN
     * 4/APB1ENR_I2C1EN
     * 5/APB1ENR_USART2EN
     * 6/APB1ENR_SPI3EN
     * 7/APB1ENR_SPI2EN
     * 8/APB1ENR_WWDGEN
     * 9/APB1ENR_TIM5EN
     * 10/APB1ENR_TIM4EN
     * 11/APB1ENR_TIM3EN
     * 12/APB1ENR_TIM2EN
     * @options for mode
     * 1/ENABLE_PERI
     * 2/DISABLE_PERI
     */

  RCC_Error_status_t RCC_APB1ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);


      /*
       * @brief to enable and disable an peripheral connected on APB2 bus
       * @options for peripherals
        1/APB2ENR_TIM11EN
		2/APB2ENR_TIM10EN
		3/APB2ENR_TIM9EN
		4/APB2ENR_SYSCFGEN
		5/APB2ENR_SPI4EN
		6/APB2ENR_SPI1EN
		7/APB2ENR_SDIOEN
		8/APB2ENR_ADC1EN
		9/APB2ENR_USART6EN
		10/APB2ENR_USART1EN
		11/APB2ENR_TIM1EN
       * @options for mode
       * 1/ENABLE_PERI
       * 2/DISABLE_PERI
       */

  RCC_Error_status_t RCC_APB2ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);

    /*
           * @brief to enable and disable an peripheral connected on AHB1L bus
           * @options for peripherals
            1/AHB1LPENR_DMA2LPEN
    		2/AHB1LPENR_DMA1LPEN
    		3/AHB1LPENR_SRAM1LPEN
    		4/AHB1LPENR_FLITFLPEN
    		5/AHB1LPENR_CRCLPEN
    		6/AHB1LPENR_GPIOHLPEN
    		7/AHB1LPENR_GPIOELPEN
    		8/AHB1LPENR_GPIODLPEN
    		9/AHB1LPENR_GPIOCLPEN
    		10/AHB1LPENR_GPIOBLPEN
    		11/AHB1LPENR_GPIOALPEN
           * @options for mode
           * 1/ENABLE_PERI
           * 2/DISABLE_PERI
           */

  RCC_Error_status_t RCC_AHB1LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);

        /*
          * @brief to enable and disable an peripheral connected on AHB2LPENR bus
          * @options for peripherals
            AHB2LPENR_OTGFSEN
          * @options for mode
          * 1/ENABLE_PERI
          * 2/DISABLE_PERI
          */

  RCC_Error_status_t RCC_AHB2LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);

        /*
            * @brief to enable and disable an peripheral connected on APB1LP bus
            * @options for peripherals
            * 1/APB1LPENR_PWRLPEN
            * 2/APB1LPENR_I2C3LPEN
            * 3/APB1LPENR_I2C2LPEN
            * 4/APB1LPENR_I2C1LPEN
            * 5/APB1LPENR_USART2LPEN
            * 6/APB1LPENR_SPI3LPEN
            * 7/APB1LPENR_SPI2LPEN
            * 8/APB1LPENR_WWDGLPEN
            * 9/APB1LPENR_TIM5LPEN
            * 10/APB1LPENR_TIM4LPEN
            * 11/APB1LPENR_TIM3LPEN
            * 12/APB1LPENR_TIM2LPEN
            * @options for mode
            * 1/ENABLE_PERI
            * 2/DISABLE_PERI
            */

  RCC_Error_status_t RCC_APB1LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);
/*
			* @brief to enable and disable an peripheral connected on APB2LP bus
			* @options for peripherals
			* 1/APB2LPENR_TIM11LPEN
			* 2/APB2LPENR_TIM10LPEN
			* 3/APB2LPENR_TIM9LPEN
			* 4/APB2LPENR_SYSCFGLPEN
			* 5/APB2LPENR_SPI4LPEN
			* 6/APB2LPENR_SPI1LPEN
			* 7/APB2LPENR_SDIOLPEN
			* 8/APB2LPENR_ADC1LPEN
			* 9/APB2LPENR_USART6LPEN
			* 10/APB2LPENR_USART1LPEN
			* 11/APB2LPENR_TIM1LPEN
			* @options for mode
			* 1/ENABLE_PERI
			* 2/DISABLE_PERI
    */

  RCC_Error_status_t RCC_APB2LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode);


#endif
