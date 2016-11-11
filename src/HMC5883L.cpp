#include "../include/HMC5883L.h"


HMC5883L::HMC5883L()
{
	HMC5883L::init();
}


bool HMC5883L::init()
{
	fd = wiringPiI2CSetup(HMC5883L_Addr);

	if((readReg8(REG_IDENT_A) != 0x48)
	|| (readReg8(REG_IDENT_B) != 0x34)
	|| (readReg8(REG_IDENT_C) != 0x33))
		return false;

	setRange(RANGE_1_3GA);
	setMeasurementMode(CONTINOUS);
	setDataRate(DATARATE_15HZ);
	setSamples(SAMPLES_1);

	mgPerDigit = 0.92f;

	return true;
}

void HMC5883L::update(bool func)
{
	if(func){ //Raw
		comp_x = readReg16(REG_XM) - xOffset;
		comp_y = readReg16(REG_YM) - yOffset;
		comp_z = readReg16(REG_ZM);
	}
	else { //Normalize
		comp_x = ((float)readReg16(REG_XM) - xOffset) * mgPerDigit;
		comp_y = ((float)readReg16(REG_YM) - yOffset) * mgPerDigit;
		comp_z = (float)readReg16(REG_ZM) * mgPerDigit;
	}
}

int HMC5883L::readReg8(int reg)
{
	return wiringPiI2CReadReg8(fd, reg);
}
int HMC5883L::readReg16(int reg)
{
	return wiringPiI2CReadReg16(fd, reg);
}
void HMC5883L::writeReg8(int reg, int val)
{
	wiringPiI2CWriteReg8(fd, reg, val);
}
			


void HMC5883L::setOffset(int xo, int yo)
{
	xOffset = xo;
	xOffset = yo;
}

void HMC5883L::setRange(range_t range)
{
	switch(range)
	{
		case RANGE_0_88GA:
			mgPerDigit = 0.073f;
			break;
		case RANGE_1_3GA:
			mgPerDigit = 0.92f;
			break;
		case RANGE_1_9GA:
			mgPerDigit = 1.22f;
			break;
		case RANGE_2_5GA:
			mgPerDigit = 1.52f;
			break;
		case RANGE_4GA:
			mgPerDigit = 2.27f;
			break;
		case RANGE_4_7GA:
			mgPerDigit = 2.56f;
			break;
		case RANGE_5_6GA:
			mgPerDigit = 3.03f;
			break;
		case RANGE_8_1GA:
			mgPerDigit = 4.35f;
			break;

		default:
			break;
	}
	writeReg8(REG_CONFIG_B, range << 5);
}

//not define yet
range_t HMC5883L::getRange(void)
{
	return (range_t)((readReg8(REG_CONFIG_B) >> 5));
}

void HMC5883L::setMeasurementMode(opt_mode_t mode)
{
	int value;

	value = readReg8(REG_MODE);
	value &= 0b11111100;
	value |= mode;

	writeReg8(REG_MODE, value);
}

opt_mode_t HMC5883L::getMeasurementMode(void)
{
	int value;

	value = readReg8(REG_MODE);
	value &= 0b00000011;

	return (opt_mode_t)value;
}

void HMC5883L::setDataRate(dataRate_t dataRate)
{
	int value;
	
	value = readReg8(REG_CONFIG_A);
	value &= 0b11100011;
	value |= (dataRate << 2);

	writeReg8(REG_CONFIG_A, value);
}

dataRate_t HMC5883L::getDataRate(void)
{
	int value;

	value = readReg8(REG_CONFIG_A);
	value &= 0b00011100;
	value >>= 2;

	return (dataRate_t)value;
}

void HMC5883L::setSamples(samples_t samples)
{
	int value;

	value = readReg8(REG_CONFIG_A);
	value &= 0b10011111;
	value |= (samples << 5);

	writeReg8(REG_CONFIG_A, value);
}

samples_t HMC5883L::getSamples(void)
{
	int value;

	value = readReg8(REG_CONFIG_A);
	value &= 0b01100000;
	value >>= 5;

	return (samples_t)value;
}

