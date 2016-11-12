#include "../include/GY80.h"
#include "uart/uart.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	char Msg[20];
	float acc_x, acc_y, gyro_z;

	ADXL345 adxl345;
	L3G4200D l3g4200d;

	uart_initialization();

	while(1)
	{
		adxl345.update();
		acc_x = adxl345.getAccX();
		acc_y = adxl345.getAccY();

		l3g4200d.update();
		gyro_z = l3g4200d.getGyroZ();

		sprintf(Msg, "%f,%f,%f\n", acc_x, acc_y, gyro_z);
		printf("%s", Msg);
		uart_send(Msg);
		usleep(50000);
	}
	uart_close();
	return 0;
}

