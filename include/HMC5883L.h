#ifndef __HMC5883L_H
#define __HMC5883L_H

#include <wiringPiI2C.h>

// Data Output (R)
#define REG_XM 0x03
#define REG_XL 0x04
#define REG_YM 0x07
#define REG_YL 0x08
#define REG_ZM 0x05
#define REG_ZL 0x06

// Status Register (R)
#define REG_STATUS 0x09
// Identification Register (R)
#define REG_IDENT_A 0x0A
#define REG_IDENT_B 0x0B
#define REG_IDENT_C 0x0C

// Configuration Register (R/W)
#define REG_CONFIG_A 0x00
#define REG_CONFIG_B 0x01
// Mode Register (R/W)
#define REG_MODE 0x02

#define HMC5883L_Addr 0x1E

/***** Configuration Register A *****/
// Select number of samples averaged (1 to 8) per measurement output.
// CRA6 to CRA5 (MA1 to MA0)
typedef enum{
	SAMPLES_8 = 0b11, // Default
	SAMPLES_4 = 0b10,
	SAMPLES_2 = 0b01,
	SAMPLES_1 = 0b00
} samples_t;

// Data Output Rate Bits. These bits set the rate at which data is
// written to all three data output registers.
// CRA4 to CRA2 (DO2 to DO0)
typedef enum
{// Data Output Rates
	// 0b111 Not used
	DATARATE_75HZ       = 0b110,
	DATARATE_30HZ       = 0b101,
	DATARATE_15HZ       = 0b100,  // Default
	DATARATE_7_5HZ      = 0b011,
	DATARATE_3HZ        = 0b010,
	DATARATE_1_5HZ      = 0b001,
	DATARATE_0_75_HZ    = 0b000
} dataRate_t;

// Measurement Configuration Bits. These bits define the measurement
// flow of the device, specifically whether or not to incorporate an
// applied bias into the measurement.
// CRA1 to CRA0 (MS1 to MS0)
typedef enum
{// Measurement Mode
	// 0b11 is reserved.
	NORMAL   = 0b00, // Default
	POSITIVE = 0b01,
	NEGATIVE = 0b10
} measurement_t; // unused yet

/***** Configuration Register B *****/
// Gain Configuration Bits. These bits configure the gain for the
// device. The gain configuration is common for all channels.
// CRB7 to CRB5 (GN2 to GN0)
// (CRB4 to CRB0 These bits must be cleared for correct operation.)
// Output Range 0xF800~0X07FF (-2048~2047)
typedef enum
{// Recommended Sensor Field Range
	RANGE_8_1GA     = 0b111, // Gain (LSB/Gauss) 230
	RANGE_5_6GA     = 0b110, // Gain (LSB/Gauss) 330
	RANGE_4_7GA     = 0b101, // Gain (LSB/Gauss) 390
	RANGE_4GA       = 0b100, // Gain (LSB/Gauss) 440
	RANGE_2_5GA     = 0b011, // Gain (LSB/Gauss) 660
	RANGE_1_9GA     = 0b010, // Gain (LSB/Gauss) 820
	RANGE_1_3GA     = 0b001, // Gain (LSB/Gauss) 1090 (Default)
	RANGE_0_88GA    = 0b000  // Gain (LSB/Gauss) 1370
} range_t;

/***** Mode Register *****/
// Mode Select Bits. These bits select the operation mode of this device.
// MR1 to MR0 (MD1 to MD0)
// (MR7 to MR2 These bits must be cleared for correct operation.
// Bit MR7 bit is set internally after each single-measurement operation.)
typedef enum
{// Operating Mode
	// 0b11 IDLE mode too.
	IDLE          = 0b10,
	SINGLE        = 0b01, // Default
	CONTINOUS     = 0b00
} opt_mode_t;

// 3 axis digital compass
class HMC5883L
{
	private:
		int fd;
		float comp_x;
		float comp_y;
		float comp_z;
		float mgPerDigit;
		int xOffset, yOffset;

		int readReg8(int);
		int readReg16(int);
		void writeReg8(int, int);

	public:
		HMC5883L();

		bool init(void);
		void update(bool func = false);
		
		void setOffset(int, int);

		void setRange(range_t);
		range_t getRange(void);

		void setMeasurementMode(opt_mode_t);
		opt_mode_t getMeasurementMode(void);

		void setDataRate(dataRate_t);
		dataRate_t getDataRate(void);

		void setSamples(samples_t);
		samples_t getSamples(void);

		float getCompX(){ return comp_x; }
		float getCompY(){ return comp_y; }
		float getCompZ(){ return comp_z; }

};
#endif
