#ifndef __SPI_H__
#define __SPI_H__

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

class Spi{
private:
	void error_status();

public:
	Spi();
	char transmit(char cData);
};

#endif