#ifndef __L3G4200D_H
#define __L3G4200D_H

#include <stdint.h>
#include <wiringPiI2C.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

#define L3G4200D_REG_XL 0x28
#define L3G4200D_REG_XM 0x29
#define L3G4200D_REG_YL 0x2A
#define L3G4200D_REG_YM 0x2B
#define L3G4200D_REG_ZL 0x2C
#define L3G4200D_REG_ZM 0x2D

#define L3G4200D_ADDR 0x69 // I2C address L3G4200D

class L3G4200D
{
	private:
		int fd;
		float gyro_x;
		float gyro_y;
		float gyro_z;
		bool gforce;

	public:
		L3G4200D();
		L3G4200D(int);

		void init(int);
		void update();
		float getGyroX(){ return gyro_x; }
		float getGyroY(){ return gyro_y; }
		float getGyroZ(){ return gyro_z; }
		
};
#endif
