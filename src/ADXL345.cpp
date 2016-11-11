#include "../include/ADXL345.h"

ADXL345::ADXL345()
{
	ADXL345::init();
	gforce = false;
}
/* --------------------------------------------------------------------------*/
/**
 * @brief constructor 2
 *
 * @param enable_gforce
 * 				False (default): result is returned in m/s^2
 * 				True		   : result is returned in gs
 */
/* ----------------------------------------------------------------------------*/
ADXL345::ADXL345(bool enable_gforce)
{
	gforce = enable_gforce;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief ADXL345 Initialize
 */
/* ----------------------------------------------------------------------------*/
void ADXL345::init(void)
{
	fd = wiringPiI2CSetup(ADXL345_Addr);
	// Set Bandwidth Rate
	wiringPiI2CWriteReg8(fd, BW_RATE, BW_RATE_100HZ);
	// Set Range
	int dataformat = wiringPiI2CReadReg8(fd, DATA_FORMAT);
	dataformat &= ~0x0F;
	dataformat |= RANGE_2G;
	dataformat |= 0x08;
	wiringPiI2CWriteReg8(fd, DATA_FORMAT, dataformat);
	// Enable measurement
	wiringPiI2CWriteReg8(fd, POWER_CTL, MEASURE);
}

/* --------------------------------------------------------------------------*/
/**
 * @brief Get new data from ADXL345
 */
/* ----------------------------------------------------------------------------*/
void ADXL345::update()
{
	//int X0 = wiringPiI2CReadReg8(fd, Reg_X0);
	//int X1 = wiringPiI2CReadReg8(fd, Reg_X1);
	//int X = X0 | X1 << 8;
	int X = wiringPiI2CReadReg16(fd, Reg_X0);
	if(X & (1 << 16 -1))
		X = X - (1<<16);

	int Y = wiringPiI2CReadReg16(fd, Reg_Y0);
	if(Y & (1 << 16 -1))
		Y = Y - (1<<16);

	int Z = wiringPiI2CReadReg16(fd, Reg_Z0);
	if(Z & (1 << 16 -1))
		Z = Z - (1<<16);

	acc_x = X * SCALE_MULTIPLIER;
	acc_y = Y * SCALE_MULTIPLIER;
	acc_z = Z * SCALE_MULTIPLIER;

	if(!gforce){
		acc_x *= EARTH_GRAVITY_MS2;
		acc_y *= EARTH_GRAVITY_MS2;
		acc_z *= EARTH_GRAVITY_MS2;
	}
	
}
