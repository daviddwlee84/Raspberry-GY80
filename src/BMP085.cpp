#include "../include/BMP085.h"
#include <stdio.h>

BMP085::BMP085(void)
{
	if(!init(ULTRAHIGHRES)) printf("BMP085 initialize failed");
}

bool BMP085::init(uint8_t mode)
{
	fd = wiringPiI2CSetup(BMP085_ADDR);

	if(mode > ULTRAHIGHRES)
		mode = ULTRAHIGHRES;

	oversampling = mode;

	if(readReg8(0xD0) != 0x55) return false;

	 /* read calibration data */
	ac1 = readReg16(CAL_AC1);
	ac2 = readReg16(CAL_AC2);
	ac3 = readReg16(CAL_AC3);
	ac4 = readReg16(CAL_AC4);
	ac5 = readReg16(CAL_AC5);
	ac6 = readReg16(CAL_AC6);
	
	b1 = readReg16(CAL_B1);
	b2 = readReg16(CAL_B2);
	
	mb = readReg16(CAL_MB);
	mc = readReg16(CAL_MC);
	md = readReg16(CAL_MD);

	return true;
}

int32_t BMP085::computeB5(int32_t UT)
{
	int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
	int32_t X2 = ((int32_t)mc << 11) / (X1 + (int32_t)md);
	return X1 + X2;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief Read uncompensated temperature value
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
uint16_t BMP085::readRawTemperature(void)
{
	writeReg8(CONTROL, READTEMPCMD);
	usleep(4500);
	return readReg16(TEMPDATA);
}


/* --------------------------------------------------------------------------*/
/**
 * @brief Read uncompensated pressure value
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
uint32_t BMP085::readRawPressure(void)
{
	uint32_t raw;
	writeReg8(CONTROL, READPRESSURECMD + (oversampling << 6));

	if(oversampling == ULTRALOWPOWER)
		usleep(5000);
	else if(oversampling == STANDARD)
		usleep(8000);
	else if(oversampling == HIGHRES)
		usleep(14000);
	else
		usleep(26000);

	raw = readReg8(PRESSUREDATA) << 16;
	raw += readReg8(PRESSUREDATA-1) << 8;
	raw += readReg8(PRESSUREDATA-2);
	raw >>= (8 - oversampling);

	return raw;
}

int32_t BMP085::readPressure(void)
{
	int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
	uint32_t B4, B7;

	UT = readRawTemperature();
	UP = readRawPressure();

//Datasheet numbers
#if BMP085_DEBUG == 1
	UT = 27898;
	UP = 23843;
	ac6 = 23153;
	ac5 = 32757;
	mc = -8711;
	md = 2868;
	b1 = 6190;
	b2 = 4;
	ac3 = -14383;
	ac2 = -72;
	ac1 = 408;
	ac4 = 32741;
	oversampling = 0;
#endif

	B5 = computeB5(UT);

	//Calculate Pressure
	B6 = B5 - 4000;
	X1 = ((int32_t)b2 * ((B6 * B6) >> 12)) >> 11;
	X2 = ((int32_t)ac2 * B6) >> 11;
	X3 = X1 + X2;
	B3 = ((((int32_t)ac1 * 4 + X3) << oversampling) + 2) / 4;

	X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

	if(B7 < 0x80000000) {
		p = (B7 * 2) / B4;
	}else{
		p = (B7 / B4) * 2;
	}
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;

	p = p + ((X1 + X2 + (int32_t)3791) >> 4);

	return p;
}

int32_t BMP085::readSealevelPressure(float altitude_meters)
{
	float pressure = readPressure();
	return (int32_t)(pressure / pow(1.0 - altitude_meters / 44330, 5.255));
}

/* --------------------------------------------------------------------------*/
/**
 * @brief Calculate true temperature
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
float BMP085::readTemperature(void)
{
	int32_t UT, B5;
	float temperature;

	UT = readRawTemperature();

//Datasheet Numbers
#if BMP085_DEBUG == 1
	UT = 27898;
	ac6 = 23153;
	ac5 = 32757;
	mc = -8711;
	md = 2868;
#endif

	B5 = computeB5(UT);
	temperature = (B5+8) >> 4;
	temperature /= 10;
		    
	return temperature;
}

float BMP085::readAltitude(float sealevelPressure)
{
	float altitude;
	float pressure = readPressure();

	altitude = 44330 * (1.0 - pow(pressure / sealevelPressure, 0.1903));

	return altitude;
}

uint8_t BMP085::readReg8(int reg)
{
	return wiringPiI2CReadReg8(fd, reg);
}
uint16_t BMP085::readReg16(int reg)
{
	return wiringPiI2CReadReg16(fd, reg);
}
void BMP085::writeReg8(int reg, int val)
{
	wiringPiI2CWriteReg8(fd, reg, val);
}
