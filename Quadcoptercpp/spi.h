#ifndef __SPI_H__
#define __SPI_H__
class Spi{
private:
	void error_status();

public:
	Spi();
	char transmit(char cData);
};

#endif