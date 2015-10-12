/**
 * Example for sending Audio over USB Virtual COM Port
 */
/* Include core modules */
#include "stm32fxxx_hal.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_usb_device.h"
#include "tm_stm32_usb_device_cdc.h"
#include "Timer.h"
#include "SampleAudio.h"
#include "diag/Trace.h"

/* USB CDC settings */
TM_USBD_CDC_Settings_t USB_FS_Settings;

/* Character value */
char ch;
char string_array[30];
	
int main(void) {
	/* Init system */
	TM_RCC_InitSystem();
	
	/* Init HAL layer */
	HAL_Init();
	
	/* Timer Init for SysTick */
	timer_start();

	/* Init leds */
	TM_DISCO_LedInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Init USB peripheral */
	TM_USB_Init();
	
	/* Init VCP on FS port.. */
	TM_USBD_CDC_Init(TM_USB_FS);
	
	/* Start USB device mode on FS port.. */
	TM_USBD_Start(TM_USB_FS);
	
	while (1) {
		/* Process USB CDC device, send remaining data if needed */
		/* It is better if you call this in periodic timer, like each ms in SYSTICK handler */
		TM_USBD_CDC_Process(TM_USB_FS);
		
		/* Check if device is ready, if drivers are installed if needed on FS port */
		if (TM_USBD_IsDeviceReady(TM_USB_FS) == TM_USBD_Result_Ok) {
			TM_DISCO_LedOn(LED_GREEN);
		} else {
			TM_DISCO_LedOff(LED_GREEN);
		}
		
		/* Check if user has changed parameter for COM port */
		TM_USBD_CDC_GetSettings(TM_USB_FS, &USB_FS_Settings);
		
		/* Check if updated */
		if (USB_FS_Settings.Updated) {
			/* Update settings for UART here if needed */
			TM_USBD_CDC_Puts(TM_USB_FS, "USB FS settings changed!\n");
		}
		
		/* Check if anything received on FS port */
		if (TM_USBD_CDC_Getc(TM_USB_FS, &ch)) {
			/* One character received */
			
			/* Send it back */
			/* TM_USBD_CDC_Putc(TM_USB_FS, ch); */

			/* Control LEDs based on input (debug) */
			switch(ch){
			case '1':
				TM_DISCO_LedToggle(LED_BLUE);
				break;
			case '2':
				TM_DISCO_LedToggle(LED_ORANGE);
				TM_USBD_CDC_Puts(TM_USB_FS, "Toggling Orange LED\n");
				break;
			case '3':
				TM_DISCO_LedToggle(LED_RED);
				TM_USBD_CDC_Puts(TM_USB_FS, "Toggling Red LED\n");
				break;
			case '4':
				TM_DISCO_LedToggle(LED_GREEN);
				break;
			case '5':
				/* Transfer Sample Audio */
				  trace_puts("Sending Audio Sample");

				/* For now, send entire sample audio data */
				/* TODO get size of sample to be transfered from serial port */

				/* at 115k BAUD 800 bytes is sub 10 msec total transfer time */
				for(int i = 0; i < SAMPLEAUDIO_SIZE;i++)
				{
					/* Need to split 16 bit samples into upper and lower bytes */
					/* Note that these will need to be reconstructed in the
					 * processing script. Updated Buffer size to 1024 from 256 in
					 * USBD_CDC library file for this operation.
					 */
					/* Send MSB first */
					TM_USBD_CDC_Putc(TM_USB_FS, Hi(SampleAudio[i]));
					trace_putchar((int)Hi(SampleAudio[i]));
					/* Then Send LSB */
					TM_USBD_CDC_Putc(TM_USB_FS, Lo(SampleAudio[i]));
					trace_putchar((int)Lo(SampleAudio[i]));
				}
				break;
			default:
				break;
			}
		}
		//Wait for next system tick
		while (g_SysTick_Flag == 0){};
		g_SysTick_Flag = 0;
		TM_DISCO_LedToggle(LED_BLUE);
	}
}

