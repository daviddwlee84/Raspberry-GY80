#include <stdlib.h> // for system("CLEAR")
#include <iostream> // for cout
#include <unistd.h> // for usleep()
#include "../include/GY80.h"
//#include "ADXL345.h"
//#include "L3G4200D.h"
//#include "HMC5883L.h"
//#include "BMP085.h"

using namespace std;

int main()
{
	ADXL345 a; //ok
	L3G4200D b;
	HMC5883L c;
	BMP085 d;
	while(1){
		a.update();
		cout << "ADXL345 Acc :";
		cout << a.getAccX() << " " << a.getAccY() << " " <<  a.getAccZ() << endl;

		b.update();
		cout << "L3G4200D Gyro :";
		cout << b.getGyroX() << " " << b.getGyroY() << " " <<  b.getGyroZ() << endl;

		c.update(true);
		cout << "HMC5883L Comp :";
		cout << c.getCompX() << " " << c.getCompY() << " " << c.getCompZ() << endl;

		cout << "BMP085 Pre:";
		cout << d.readTemperature() << "*C " << d.readPressure() << "Pa " << d.readSealevelPressure() << "Pa " << d.readAltitude() << "meters " << endl; 
		usleep(100000);
		//sleep(1);
		system("clear");
	}
	return 0;
}	
