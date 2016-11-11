#ifndef __BMP085_H
#define __BMP085_H

#include <wiringPiI2C.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#define BMP085_DEBUG 0

#define BMP085_ADDR 0x77

//oversampling setting parameters
#define ULTRALOWPOWER 0
#define STANDARD      1
#define HIGHRES       2
#define ULTRAHIGHRES  3

//Calibration coefficients (16bits define MSB)
#define CAL_AC1           0xAA
#define CAL_AC2           0xAC
#define CAL_AC3           0xAE
#define CAL_AC4           0xB0
#define CAL_AC5           0xB2
#define CAL_AC6           0xB4
#define CAL_B1            0xB6
#define CAL_B2            0xB8
#define CAL_MB            0xBA
#define CAL_MC            0xBC
#define CAL_MD            0xBE

#define CONTROL           0xF4
#define TEMPDATA          0xF6 //MSB 16bits
#define PRESSUREDATA      0xF8 //MSB 16bits
#define READTEMPCMD       0x2E
#define READPRESSURECMD   0x34


class BMP085 {
	private:
		uint8_t oversampling;
		
		int fd; //device number
		int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
		uint16_t ac4, ac5, ac6;
		
		int32_t computeB5(int32_t UT);
		uint8_t readReg8(int reg);
		uint16_t readReg16(int reg);
		void writeReg8(int reg, int val);

	public:

		BMP085();

		bool init(uint8_t mode = ULTRAHIGHRES);  // by default go highres
		float readTemperature(void);
		int32_t readPressure(void);
		int32_t readSealevelPressure(float altitude_meters = 0);
		float readAltitude(float sealevelPressure = 101325); // std atmosphere
		uint16_t readRawTemperature(void);
		uint32_t readRawPressure(void);
};

#endif
