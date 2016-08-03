/*
 * controlDeivce.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */


#ifndef DETECTOR_DEVICE_CONTROLDEVICE_CPP_
#define DETECTOR_DEVICE_CONTROLDEVICE_CPP_

#include "../Main.h"
#include <wiringPi.h>

//red
#define LED1 1 // BCM_GPIO 18
#define LED2 0 // BCM_GPIO 17
//yellow
#define LED3 2 // BCM_GPIO 27
#define LED4 4 // BCM_GPIO 23
//green
#define LED5 3 // BCM_GPIO 22
#define LED6 5 // BCM_GPIO 24




	static bool *DoDevice(){
		if (wiringPiSetup () == -1)
			printf("GPIO설치가 안되어 있습니다.")

		pinMode (LED1, OUTPUT) ;
		pinMode (LED2, OUTPUT) ;
		pinMode (LED3, OUTPUT) ;
		pinMode (LED4, OUTPUT) ;
		pinMode (LED5, OUTPUT) ;
		pinMode (LED6, OUTPUT) ;


		digitalWrite (LED1, 1) ; // On
		digitalWrite (LED2, 1) ; // On
		digitalWrite (LED3, 1) ; // On
		digitalWrite (LED4, 1) ; // On
		digitalWrite (LED5, 1) ; // On
		digitalWrite (LED6, 1) ; // On

		system("omxplayer -o both /home/pi/do.wav");

		delay (1000) ; // ms

		digitalWrite (LED1, 0) ; // Off
		digitalWrite (LED2, 0) ; // Off
		digitalWrite (LED3, 0) ; // Off
		digitalWrite (LED4, 0) ; // Off
		digitalWrite (LED5, 0) ; // Off
		digitalWrite (LED6, 0) ; // Off
		return 1 ;
	}



#endif /* DETECTOR_DEVICE_CONTROLDEVICE_CPP_ */
