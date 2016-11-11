#ifndef __ADXL345_H
#define __ADXL345_H

#include <wiringPiI2C.h>

// ADXL345
#define DATA_FORMAT 0x31
#define BW_RATE 0x2C
#define POWER_CTL 0x2D

#define EARTH_GRAVITY_MS2 9.80665
#define SCALE_MULTIPLIER 0.004

#define ATE_1600HZ 0x0F
#define BW_RATE_800HZ 0x0E
#define BW_RATE_400HZ 0x0D
#define BW_RATE_200HZ 0x0C
#define BW_RATE_100HZ 0x0B
#define BW_RATE_50HZ  0x0A
#define BW_RATE_25HZ  0x09

#define RANGE_2G  0x00
#define RANGE_4G  0x01
#define RANGE_8G  0x02
#define RANGE_16G 0x03

#define MEASURE   0x08
#define AXES_DATA 0x32

#define Reg_X0 0x32
#define Reg_X1 0x33
#define Reg_Y0 0x34
#define Reg_Y1 0x35
#define Reg_Z0 0x36
#define Reg_Z1 0x37

#define ADXL345_Addr 0x53 // the default 7-bit slave address

class ADXL345
{
	private:
		int fd;
		float acc_x;
		float acc_y;
		float acc_z;
		bool gforce;

	public:
		ADXL345();
		ADXL345(bool);

		void init();
		void update();
		float getAccX(){ return acc_x; }
		float getAccY(){ return acc_y; }
		float getAccZ(){ return acc_z; }
		
};
#endif
