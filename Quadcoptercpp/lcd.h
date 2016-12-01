#include "spi.h"

#include <stdint.h>
#include <string.h>


/**************************Modes*****************************/
/********************Power***********************/
#define SLPIN	0x10 //Enter low power mode
#define SLPOUT	0x11 //Exit low power mode
#define SWRESET 0x01 //Software reset
/************************************************/

/********************Display***********************/
#define	PTLON	0x12 //Partial mode on
#define	PTLAR	0x30 //Partial area (partial mode)

#define NORON	0x13 //Normal mode on
#define INVOFF	0x20 //Display inversion off
#define	INVON	0x21 //Display inversion on
#define	IDMOFF	0x38 //Idle mode off
#define	IDMON	0x39 //Idle mode on
/************************************************/
/************************************************************/
#define	GAMSET	0x26 //Set gamma curve

#define	DISPOFF	0x28 //Display off
#define	DISPON	0x29 //Display on

#define	CASET	0x2A //Column address set
#define	RASET	0x2B //Row address set

#define	RAMWR	0x2C //RAM write
#define	RGBSET	0x2D //Change LookUpTable?
#define	RAMRD	0x2E //RAM read
#define	MADCTL	0x36 //Memory data access control

#define	SCRLAR	0x33 //Scroll area set
#define	VSCSAD	0x37 //Vertical scroll start address of RAM (used with SCRLAR, NORON to exit)

#define	TEOFF	0x34 //Tearing effect line off (active low)
#define	TEON	0x35 //Tearing effect line on

#define	COLMOD	0x3A //Interface pixel format

/********************Adjust frequency***********************/
#define	FRMCTR1	0xB1 //In normal mode (full colors)
#define	FRMCTR2	0xB2 //In idle mode (8-colors)
#define	FRMCTR3	0xB3 //In partial mode + full colors
/***********************************************************/

#define	INVCTR	0xB4 //Display inversion control
#define	GCV	0xFC //Gate pump clock frequency variable (save power)

/********************Useless shite**************************/
#define	PWCTR1	0xC0 //Power control setting (GVDD)
#define	PWCTR2	0xC1 //Power control setting (VGH/VGL)
#define	PWCTR3	0xC2 //Power control setting (normal:opamp/booster)
#define	PWCTR4	0xC3 //Power control setting (idle:opamp/booster)
#define	PWCTR5	0xC4 //Power control setting (partial:opamp/booster)
#define	VMCTR1	0xC5 //VCOM control 1
#define	VMOFCTR	0xC7 //Set VCOM offset control
#define	WRID2	0xD1 //SET LCM version code
#define	WRID3	0xD2 //Customer project code
#define	NVCTR1	0xD9 //NVM control status
#define	NVCTR2	0xDE //NVM read command
#define	NVCTR3	0xDF //NVM write command
#define	GAMCTRP1	0xE0 //Gamma adjustment +
#define	GAMCTRN1	0x15 //Gamma adjustment -
/************************************************************/

#define SS 2
#define COMMAND_PIN 0
#define RST 1

//Screen specs: 159x127
#define SCRN_W 127
#define SCRN_H 169


class Lcd{
private:
	Spi spi;
	uint16_t currentColor;
	void set_address_pointer(int xStart, int xEnd, int yStart, int yEnd);

public:
	Lcd();
	void writeString(int posX, int posY, char const * str);
	void writeChar(int posX, int posY, char c);
	void transmitCommand(char cData);
	void transmitData(char cData);
	void clearScreen();
};
