#include "../include/L3G4200D.h"

/* --------------------------------------------------------------------------*/
/**
 * @brief Default constructor
 */
/* ----------------------------------------------------------------------------*/
L3G4200D::L3G4200D()
{
	L3G4200D::init(2000);
}

/* --------------------------------------------------------------------------*/
/**
 * @brief Constructor with initial scale
 *
 * @param scale
 */
/* ----------------------------------------------------------------------------*/
L3G4200D::L3G4200D(int scale)
{
	L3G4200D::init(scale);
}

void L3G4200D::init(int scale)
{
	fd = wiringPiI2CSetup(L3G4200D_ADDR);

	// Enable x, y, z and turn off power down
	wiringPiI2CWriteReg8(fd, CTRL_REG1, 0b00001111);
	//wiringPiI2CWriteReg8(fd, CTRL_REG1, 0b01001111);

	// Adjust/use the HPF, edit the line below to configure CTRL_REG2
	//wiringPiI2CWriteReg8(fd, CTRL_REG2, 0b00000000);

	// Configure CTRL_REG3 to generate data ready interrupt on INT2
	// No interrupts used on INT1, if you'd like to configure INT1
	// Or INT2 otherwise, consult the datasheet
	//wiringPiI2CWriteReg8(fd, CTRL_REG3, 0b00001000);

	// CTRL_REG4 controls the full-scale range, among other things
	/*
	if(scale == 250){
		wiringPiI2CWriteReg8(fd, CTRL_REG4, 0b00000000);
	}else if(scale == 500){
		wiringPiI2CWriteReg8(fd, CTRL_REG4, 0b00010000);
	}else{
		wiringPiI2CWriteReg8(fd, CTRL_REG4, 0b00110000);
	}
	*/
	//wiringPiI2CWriteReg8(fd, CTRL_REG4, 0x80);

	// CTRL_REG5 controls high-pass filtering of outputs
	//wiringPiI2CWriteReg8(fd, CTRL_REG5, 0b00000000);

}

/* --------------------------------------------------------------------------*/
/**
 * @brief Get new data from L3G4200D
 */
/* ----------------------------------------------------------------------------*/
void L3G4200D::update()
{
	int16_t X = (int16_t)wiringPiI2CReadReg16(fd, L3G4200D_REG_XL);
	gyro_x = (float)X / 32768;
	gyro_x *= 2000.0f;
	int16_t Y = (int16_t)wiringPiI2CReadReg16(fd, L3G4200D_REG_YL);
	gyro_y = (float)Y / 32768;
	gyro_y *= 2000.0f;
	int16_t Z = (int16_t)wiringPiI2CReadReg16(fd, L3G4200D_REG_ZL);
	gyro_z = (float)Z / 32768;
	gyro_z *= 2000.0f;

}
