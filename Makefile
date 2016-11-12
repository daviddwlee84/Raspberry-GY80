
all:
#	g++ -o test.out example/simple_test.cpp src/ADXL345.cpp src/L3G4200D.cpp src/BMP085.cpp src/HMC5883L.cpp -lwiringPi
	g++ -o uart.out example/uart_transmit.cpp example/uart/uart.c src/ADXL345.cpp src/L3G4200D.cpp src/BMP085.cpp src/HMC5883L.cpp -lwiringPi


clean:
	rm *.out
