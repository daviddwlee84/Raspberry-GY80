
all:
	g++ -o test.out src/main.cpp src/ADXL345.cpp src/L3G4200D.cpp src/BMP085.cpp src/HMC5883L.cpp -lwiringPi

clean:
	rm test.out
