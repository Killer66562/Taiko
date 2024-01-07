#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "Wire.h"

//#define TFT_CLK 13
//#define TFT_MISO 12
//#define TFT_MOSI 11
#define TFT_DC 10
#define TFT_CS 8
#define TFT_RST 9

#define COUNTDOWN 500

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

#define CMD_STARTSONG 1
#define PERSIZE 500

#define FPS 60
#define PIXEL_PER_BEAT 80

String songs[] = {"A", "Inochi ni kirawarete iru", "Mayoiuta"};

const byte img[2400] PROGMEM = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XED,0XDF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XCF,0XFE,0X3F,0XFF,0XFF,0XE1,0XCF,0XFF,0XF9,0XFF,0XBB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XC7,0XFC,0X1F,0XFF,0XFF,0XC0,0XC7,0XFF,0XF8,0XFF,0XBA,0XFF,
0XFF,0XFF,0XDF,0XFF,0XBF,0XFF,0XFF,0XFF,0XC7,0XFD,0XFF,0XFF,0XFF,0XC4,0X47,0XFF,
0XF8,0XFF,0XF1,0X7F,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,0XFF,0XC4,0X01,0XE1,0XE0,
0XE1,0XE4,0X47,0XE1,0XC0,0XFF,0XDA,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XC4,0X00,0XC0,0XC0,0X40,0XF8,0X47,0X40,0X80,0XFF,0XED,0XFF,0XFF,0XFF,0XB7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XC4,0X44,0X60,0X44,0X40,0X30,0XC7,0XC6,0X08,0XFF,0X3F,0XFF,
0XFF,0XFF,0XBF,0XF3,0XFF,0XFF,0XFF,0XFF,0XC4,0X44,0X60,0X44,0X40,0X61,0XC7,0XC5,
0X88,0XFE,0X7F,0XFF,0XFF,0XFF,0XFF,0XF0,0X7F,0XFF,0XFF,0XFF,0XC4,0X44,0X40,0X44,
0X46,0X43,0XC7,0XC6,0X88,0XFE,0X7F,0XFF,0XFF,0XFF,0X9F,0XFA,0X07,0XFF,0XFE,0XFF,
0XC4,0X44,0X44,0X44,0X44,0XC7,0XC7,0XC4,0X08,0XFF,0X5D,0XF7,0XFF,0XFF,0XF3,0XFB,
0XE0,0X3F,0XFF,0XDF,0XC4,0X44,0X40,0X40,0X60,0X40,0XC0,0X60,0X40,0XFF,0X9D,0XDC,
0XFF,0X9F,0XFF,0XF9,0XFE,0X3F,0XFF,0XB7,0XE6,0X64,0X60,0X70,0X70,0X80,0X60,0X30,
0X60,0XFF,0XDD,0XFF,0X7F,0X0F,0XFF,0X9D,0XFF,0X9F,0XFE,0XFF,0XFF,0XFC,0XED,0XCC,
0X79,0X0F,0XF2,0XF3,0X7F,0XFF,0XFF,0XEA,0XFE,0X67,0XFF,0X0D,0XFF,0X9F,0XFE,0XFF,
0XFF,0XFC,0XED,0X80,0X78,0X6F,0XF8,0X7A,0XBF,0XFF,0XFF,0XFE,0XFE,0XF7,0XFA,0X65,
0XFF,0X93,0XFE,0XBF,0XFF,0XFE,0XED,0X90,0XF1,0XE7,0XF7,0XB9,0XBF,0XFF,0XFF,0XD5,
0XFC,0XF7,0XF8,0XF1,0XFF,0X91,0XFF,0XFF,0XFF,0XFC,0XED,0XBF,0XF3,0XF7,0XF3,0XD9,
0X7F,0XFF,0XEF,0XFD,0XFD,0XE7,0XF2,0XF8,0XFC,0X90,0X7F,0XFF,0XFF,0XF8,0XEC,0XBF,
0XF7,0XF0,0XFF,0XE7,0XFF,0XFF,0XEF,0XFF,0XF9,0XEF,0XF7,0X3C,0XFB,0X16,0X3F,0XFF,
0XFF,0XE2,0X6E,0X9F,0XEF,0XF2,0XFF,0XFF,0XFF,0XE7,0XFF,0XFF,0XF3,0XCF,0XE7,0XCE,
0XF7,0X17,0X8F,0XFF,0XFF,0XC6,0X6E,0X9F,0XCF,0XFA,0X7F,0XFF,0XFF,0XC3,0XF7,0XBB,
0XF3,0XDF,0XEF,0XE6,0XF7,0X37,0XE1,0XFF,0XFF,0XCF,0X66,0X5F,0XDF,0XFA,0X7F,0XEF,
0XFF,0X99,0XFF,0XFB,0XE7,0X9F,0XCF,0XFB,0X77,0XA7,0XF8,0X3F,0XFF,0XCF,0X77,0X5F,
0XD3,0X81,0X7F,0XFF,0XFF,0XBD,0XFF,0XFF,0XEF,0X3F,0XDF,0XFB,0X7F,0X8F,0XFF,0X3F,
0XFF,0XEF,0X77,0X1E,0X08,0X78,0X3F,0XFF,0XFF,0XBD,0XFF,0XFF,0XCF,0X7F,0X03,0XFD,
0X7B,0X83,0XFF,0X7F,0XFF,0XE7,0X70,0X09,0XE3,0XFF,0X8F,0XFF,0XFF,0XB9,0XFF,0X7F,
0X9E,0X7C,0X3C,0X78,0X73,0XBC,0XFF,0X7F,0XFF,0XF7,0X07,0XF3,0XEF,0XFF,0XC7,0XFF,
0XFF,0X3B,0XFF,0X7F,0XBE,0XF1,0XFF,0X97,0X8B,0XBE,0XFE,0X7F,0XFF,0XF3,0X7B,0XF9,
0XDF,0XFF,0XF3,0XFF,0XFF,0X3B,0XFF,0XFF,0X3C,0XE7,0XFF,0XE7,0X9B,0XBE,0XFE,0XFF,
0XFF,0XFA,0XFD,0X81,0XBF,0XFF,0XF9,0XFF,0XFF,0X73,0XFF,0XFF,0X79,0XCF,0XFF,0XF3,
0X7F,0XDE,0XFC,0XFF,0XFF,0XF9,0XFE,0X7A,0X7E,0XFF,0XFC,0XFF,0XFE,0X73,0XFF,0XF8,
0X7B,0X9F,0XFF,0XF8,0X7D,0XDC,0XFD,0XFF,0XFF,0XFD,0XFE,0XF6,0X7F,0XFF,0XFE,0XFF,
0XFE,0X77,0XFE,0XE0,0XF3,0X3F,0XFF,0XFD,0X7D,0XDD,0XFD,0XFF,0XFF,0XF9,0XFE,0XF8,
0XFF,0XFF,0XFE,0X7F,0XFE,0X67,0XFF,0XCF,0X76,0X7F,0XFF,0XFD,0X3D,0XDD,0XFF,0XFF,
0XFF,0XFB,0XFE,0XFA,0XFF,0XFF,0XFF,0X3F,0XFE,0XE7,0XFF,0X9F,0XA6,0XFF,0XFF,0XFE,
0X9D,0XDF,0XFB,0XFF,0XFF,0XFB,0XFE,0XF1,0XEF,0XFF,0XF7,0XBF,0XFC,0XE7,0XFF,0XBF,
0X8C,0XFF,0XFF,0XFF,0XAD,0XD9,0XE7,0XFF,0XFF,0XFB,0XFE,0XE5,0XFF,0XFF,0XCF,0X9F,
0XFC,0XEF,0XFF,0X3F,0XDD,0XFF,0XFF,0XFF,0X56,0XE8,0XEF,0XFF,0XFF,0XF9,0XFE,0XE7,
0XFF,0XFF,0X87,0XDF,0XFD,0XCF,0XFF,0X7F,0XD9,0XFF,0XFF,0XFF,0X52,0XEA,0XCF,0XFF,
0XFF,0XF9,0XFE,0XD3,0XFF,0XFF,0X03,0XDF,0XFD,0XCF,0XFF,0X7F,0XDB,0XFF,0XFF,0XFF,
0X98,0XE7,0X1F,0XFF,0XF5,0XFD,0XE5,0XBB,0XF1,0XFF,0X03,0XCF,0XF9,0XCF,0XFF,0X7F,
0XD3,0XC3,0XFF,0XFF,0XAA,0XE7,0X9F,0XFF,0XFB,0XFC,0XD9,0X2B,0XE0,0XFF,0X03,0XEF,
0XF9,0XDF,0XFF,0X7F,0XD7,0XC3,0XFF,0XFF,0XA8,0XE7,0X9F,0XFF,0XFF,0XFC,0XBC,0X2B,
0XC0,0XFF,0X03,0XEF,0XFB,0X9F,0XFF,0X0F,0X97,0X83,0XFF,0XFF,0XA8,0XF7,0XBF,0XFF,
0XDF,0XBE,0X3C,0XAB,0XC0,0XFF,0X03,0XE7,0XFB,0X9F,0XFF,0X37,0XA7,0X81,0XFF,0XE7,
0XAD,0X47,0X3F,0XFF,0XFF,0XFF,0X3C,0X6B,0XC0,0XFF,0X87,0XE7,0XF3,0XBF,0XFF,0X77,
0X2F,0X83,0XFF,0X1F,0XEC,0X1B,0X7F,0X7F,0XFF,0XFF,0X3D,0X6B,0XC0,0XFF,0XCF,0XF7,
0XC0,0X3F,0XFE,0X7A,0X0F,0X83,0XFE,0X3F,0XEC,0X7C,0X7E,0XFF,0XFB,0XFF,0XBD,0XAB,
0XE0,0XFF,0XFF,0XF7,0XCF,0X3F,0XFE,0XF9,0XCF,0XC3,0XFE,0X7F,0XEC,0XFE,0XFD,0XBF,
0XFF,0XFF,0X83,0XCB,0XE0,0XFB,0XEF,0XF7,0X9F,0XBF,0XFE,0XF3,0XEF,0XE7,0XFF,0X9F,
0XEC,0XFE,0X7F,0X7F,0XFF,0XFF,0X39,0XEB,0XB1,0XF1,0XCF,0XF7,0X3F,0X9F,0XFE,0X61,
0XEF,0XFF,0XFF,0XFF,0XEC,0XFF,0X3F,0X3F,0XFF,0XE1,0X38,0X0B,0XFD,0XF0,0X0F,0XF7,
0X7F,0XDF,0XFF,0X0C,0X6F,0XEF,0X9F,0XFF,0XAD,0XFF,0X1D,0XFF,0XFF,0XCC,0X78,0X4B,
0XFE,0X00,0X0F,0XF7,0X7F,0XDF,0XFF,0X9E,0X0F,0XE7,0X9F,0X7F,0XAD,0XFF,0X5D,0XFF,
0XFF,0X9E,0XF8,0X7B,0XFE,0X00,0X0F,0XF7,0X7F,0XDF,0XFF,0XFF,0X8F,0XF0,0X07,0X7F,
0XAD,0XFF,0X4D,0XFF,0XFF,0XBF,0X7A,0X75,0XFE,0X00,0X0F,0XF7,0X7F,0XDF,0XFF,0XFF,
0XEF,0XF0,0X00,0XFF,0XAC,0XFF,0X1E,0XFF,0XFF,0X3F,0X34,0X75,0XFF,0X00,0X1F,0XE0,
0X7F,0XDF,0XFF,0XFF,0XEF,0XF0,0X00,0XFF,0XA0,0XE2,0X3F,0X1F,0XFF,0X3F,0X30,0X35,
0XFF,0X00,0X1F,0XE0,0X7F,0XDF,0XFF,0XFF,0XEF,0XF0,0X00,0XFF,0X4E,0XDC,0X7F,0XFF,
0XFF,0X3F,0X10,0X36,0XFF,0X00,0X1F,0XEF,0XC7,0X9F,0XFF,0XFF,0XEF,0XF0,0X01,0XFF,
0X5F,0X5C,0XFF,0XFF,0XFF,0X3F,0X48,0X3A,0XFF,0X80,0X1F,0XFF,0XBB,0XBF,0XFF,0XFF,
0XE7,0XF8,0X01,0XFF,0X47,0X9C,0XC3,0XFF,0XFF,0X3F,0X46,0X3B,0X7F,0X80,0X1F,0XCF,
0XBF,0X3F,0XFF,0XFF,0XE7,0XF8,0X01,0XFE,0XC0,0X1E,0X99,0XFF,0XFF,0XBF,0X0F,0X9D,
0X7F,0XC0,0X3F,0XC1,0XBC,0X7F,0XFF,0XFF,0XF7,0XF8,0X03,0XFE,0X86,0XDE,0X3D,0XFF,
0XFF,0X9F,0X8F,0XDD,0XBF,0XC0,0X3F,0X90,0X79,0XFF,0XFF,0XFF,0XF3,0XF8,0X03,0XFD,
0XA3,0X5C,0X7C,0XFF,0XFF,0XCF,0XCF,0XCE,0XDF,0XE0,0X7F,0X9F,0X3B,0XFF,0XFF,0XFF,
0XFB,0XFC,0X07,0XFD,0X81,0X82,0XFE,0XFF,0XFF,0XE7,0XC7,0XAE,0XDF,0XF8,0XFF,0X3F,
0XBB,0XFF,0XFF,0XFF,0XFB,0XFE,0X07,0XFB,0X46,0XEF,0X7E,0XFF,0XFF,0XF0,0X3E,0X4F,
0X6F,0XFF,0XFE,0X3F,0X83,0XFF,0XFF,0XFF,0XF9,0XFF,0X1F,0XF6,0X17,0X47,0X7E,0XFF,
0XFF,0XFC,0X7A,0X87,0X37,0XFF,0XFD,0X3F,0XEF,0XFF,0XFF,0XFF,0XFC,0XFF,0XFF,0XE6,
0X83,0XA6,0X7E,0XFF,0XFF,0XFF,0X71,0X77,0X99,0XFF,0XFB,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFC,0X7F,0XFF,0XEC,0X0B,0X9A,0X7C,0XFF,0XFF,0XFF,0X22,0XF3,0XCE,0X7F,0XF6,0X7F,
0XFF,0XF0,0X1F,0XFF,0XFE,0X3F,0XFF,0X98,0X03,0X8F,0XBD,0XFF,0XFF,0XFE,0X6C,0XF0,
0X07,0X9F,0X8C,0XFF,0XFF,0X8F,0XF9,0XFF,0XFE,0X5F,0XFF,0X70,0X13,0X87,0X99,0XFF,
0XFF,0XFC,0XF8,0XF0,0X03,0XE0,0X78,0XFF,0XFE,0X7F,0XFE,0X7B,0XFF,0X27,0XFC,0XE0,
0X33,0X07,0XC3,0XFF,0XFF,0XF9,0XF2,0XF2,0X04,0XFF,0XE1,0XFF,0XFC,0XFF,0XFF,0XB5,
0XFF,0XB8,0XC3,0XC8,0X33,0X68,0X1F,0XFF,0XFF,0XFC,0XEA,0X77,0X02,0X1F,0X01,0XFA,
0XF8,0X7F,0XFF,0XDE,0XFF,0X8F,0X1F,0X90,0X72,0X68,0X7F,0XFF,0XFF,0XFE,0X04,0X32,
0X82,0X00,0X13,0XFF,0X77,0XBF,0XFF,0XE7,0X3F,0XC3,0XFE,0X10,0XF2,0X63,0XFF,0XFF,
0XFF,0XFF,0X04,0XB2,0XC1,0X00,0X0B,0XC5,0X8E,0XBF,0XFF,0X0D,0XDF,0XD8,0X00,0X11,
0XF0,0X67,0XFF,0XFF,0XFF,0XFF,0XF6,0X90,0XA1,0X00,0X33,0XBC,0XEA,0X5F,0XFF,0X7A,
0X2F,0XEA,0X00,0X21,0X31,0X67,0XFF,0XFF,0XFF,0XFF,0XF2,0XC0,0XB8,0X80,0X59,0XA8,
0X36,0X7F,0XFF,0XC0,0X3F,0XEC,0X00,0X21,0X31,0X6F,0XFF,0XFF,0XFF,0XFF,0XFA,0XE1,
0X92,0X01,0X49,0XA0,0X3C,0X2F,0XFF,0X50,0X2F,0XF2,0X00,0X00,0X33,0XEF,0XFF,0XFF,
0XC7,0XFF,0XF8,0XF1,0X88,0XF0,0X4C,0XA0,0XE0,0X3F,0XFF,0X29,0XEF,0XD3,0X00,0XE1,
0XE7,0XA7,0XFF,0XFF,0XBF,0XFD,0XFC,0XF9,0XFF,0X80,0X44,0XD4,0XF0,0X6F,0XFF,0X7B,
0X97,0XD1,0X00,0X60,0X07,0X67,0XFF,0XFF,0X3D,0XFF,0XFC,0XFC,0XF4,0X00,0XC7,0X1C,
0XFA,0X4F,0XEC,0XFC,0X17,0XE1,0X00,0X00,0X0C,0XE7,0XFF,0XFF,0X7F,0XFF,0XFE,0XFE,
0X04,0X00,0X87,0X9C,0X1E,0XEF,0XFF,0XC0,0X37,0XA3,0X00,0X00,0X5B,0XCF,0XFF,0X3F,
0X7F,0XFF,0XFE,0XDF,0X82,0X09,0X8C,0XC0,0X1F,0X3F,0XF8,0XC3,0X67,0XE2,0X00,0X00,
0XE7,0X1F,0XFE,0XDF,0X7F,0X7F,0XFE,0XF3,0XA0,0X1F,0X00,0X21,0XFC,0X57,0XEC,0XF7,
0X3F,0XC2,0X80,0X00,0XFE,0X7F,0XF1,0XEF,0XBE,0XFF,0XFE,0X7C,0XE0,0X10,0X00,0X3F,
0XE0,0X14,0X67,0XF6,0X17,0X56,0XE0,0X00,0XF8,0XFF,0XFF,0XE7,0XBC,0X3F,0XFF,0X0F,
0XE0,0X10,0X00,0X38,0X70,0X37,0XAF,0XC0,0X17,0XC4,0X78,0X07,0XF1,0XFF,0XF7,0XFF,
0XC2,0X1E,0XFF,0XC3,0XFF,0XE8,0X03,0XEA,0X5F,0X3C,0X5C,0X01,0XFB,0X8D,0X1E,0X7F,
0X87,0XFF,0XFA,0X39,0X4E,0X07,0XFF,0XF0,0X7F,0XEC,0X03,0XCA,0X4A,0X2B,0XA8,0X29,
0XDC,0X8D,0XC7,0XF8,0X1F,0XFF,0XFA,0X3F,0XFC,0X03,0XEF,0XFC,0X00,0X06,0X22,0X0B,
0X48,0X76,0XEE,0X72,0X3D,0X87,0XE1,0X04,0XFF,0XFF,0XFA,0X1D,0XEC,0XF9,0XDF,0XFF,
0XC3,0XE2,0X33,0X1A,0X48,0X4E,0XF4,0XD0,0X77,0X02,0XF8,0X7D,0XFF,0XFF,0XFA,0X2B,
0XE1,0XE5,0XEF,0XFF,0XF9,0XE6,0X71,0X51,0XC4,0X4A,0XF6,0XDD,0XF7,0X03,0X7E,0X79,
0XFB,0XFF,0XFB,0XFF,0X22,0X65,0XBF,0XFF,0XFC,0X0C,0X55,0X5B,0X6C,0X65,0XA6,0X79,
0X36,0X31,0X8F,0X03,0XFA,0XF8,0XFB,0XFF,0X62,0X7D,0X9F,0XFF,0XFE,0X35,0X7D,0XDF,
0XB1,0X27,0X2D,0X40,0X36,0XB4,0XF7,0XC7,0XF9,0XFE,0XFB,0XF7,0XAB,0X8C,0X5F,0XFF,
0XFF,0XF5,0XBA,0X97,0XA5,0X77,0XF2,0XFF,0XF6,0XFF,0X7F,0XFF,0XF9,0XF7,0X7F,0XF7,
0XB3,0X8D,0XFF,0XFF,0XFF,0XFF,0XFB,0X9F,0X7F,0XF8,0XD7,0XBF,0XEE,0XEB,0X9F,0XFF,
0XF8,0XFF,0X77,0XF7,0X81,0XFC,0XFF,0XFF,0XFF,0XFB,0XFF,0XFA,0X7E,0XB8,0X17,0XFF,
0X3E,0XCF,0XF7,0XFF,0XF0,0X77,0X07,0XE7,0X79,0XF9,0XDF,0XFF,0XFF,0XEB,0XBD,0XCF,
0XFE,0XFB,0X1F,0XFF,0XFE,0XC5,0XF7,0XFF,0XE0,0X38,0X03,0XEF,0XEC,0XD6,0XFE,0X2E,
0XFF,0XFF,0XFF,0XEF,0XFC,0X73,0XC0,0X7F,0XFD,0X97,0XF7,0XFF,0XC7,0X19,0XF3,0XE3,
0XC4,0X0E,0XBF,0XF4,0X7F,0XFF,0X57,0XFF,0X00,0X07,0X20,0X17,0XF3,0XB3,0XF7,0XFF,
0XCF,0XDB,0XF1,0XDD,0X79,0XA6,0XFF,0XFD,0X7F,0XDF,0X73,0XF8,0X77,0X76,0XCE,0XCB,
0XC7,0XBB,0XF7,0XFF,0XEF,0X73,0XF9,0XBE,0X83,0X30,0XFF,0XFE,0XFF,0XEF,0X33,0XE0,
0X77,0X2E,0XEA,0XAF,0X07,0XB9,0XEF,0XFF,0XAF,0X66,0X79,0XFF,0X30,0X34,0XFA,0XF7,
0X7F,0XEF,0X01,0XC0,0X67,0X4E,0XC6,0XBE,0X13,0X3C,0XCF,0XFF,0XAF,0XEC,0X7D,0X5F,
0X44,0X58,0X7E,0X5E,0XFF,0XE6,0X4C,0X0C,0X7F,0X76,0XEC,0XDC,0X30,0X7E,0X07,0XFF,
0XF8,0XED,0XFC,0X6D,0X55,0XD9,0X3B,0XF2,0XF0,0X70,0XFE,0X1E,0X00,0X00,0X00,0X00,
0X78,0X7C,0X1E,0X79,0XFB,0XDF,0X7C,0X67,0X57,0XC1,0XBF,0X2B,0XEF,0XD9,0XFF,0XFE,
0X0F,0XDF,0XDF,0X30,0X7F,0XFD,0X1D,0XF9,0XDF,0XDB,0XFD,0X6F,0X34,0XF0,0X3D,0XE1,
0X5F,0XE1,0XFF,0XFF,0X0F,0XFF,0XFF,0XF0,0XFF,0XFD,0XF1,0X3A,0XEA,0XBB,0XF9,0X76,
0XB0,0X82,0X25,0X3B,0XBF,0XFD,0XFF,0XFF,0X0F,0XFF,0XFF,0XF0,0XFF,0XFC,0XFB,0X1D,
0X75,0X2D,0XF0,0X3E,0XB8,0X99,0X7B,0XBF,0XFF,0XD8,0XFE,0XFF,0X03,0XFF,0XFF,0XC0,
0XFF,0XFD,0XFF,0X4F,0X78,0X50,0XDF,0X3D,0XB9,0X79,0X2D,0XFB,0X63,0XD4,0X7F,0XBF,
0X90,0XDF,0XFB,0X0D,0XFF,0XF9,0XFF,0XEE,0X85,0XEA,0X1A,0X1B,0X99,0X2E,0XAD,0XF5,
0XE3,0XDE,0XFF,0XDF,0X90,0X1F,0X7A,0X09,0XFF,0XF9,0XFF,0XEF,0X6C,0XF0,0XBE,0X57,
0X19,0X04,0X9E,0XE2,0XE3,0XC6,0XFF,0XFF,0XC0,0X0E,0X70,0X03,0XFF,0XF9,0XFF,0XCF,
0XEE,0XF0,0X5E,0XF7,0X5D,0X00,0XAE,0XFE,0XFF,0X81,0XFF,0XDF,0XE0,0X00,0X00,0X03,
0XFF,0XF9,0XFF,0XDF,0XFE,0XF8,0X1F,0XE7,0X9D,0X01,0XA1,0XFC,0XF3,0X80,0XFF,0XEF,
0XE0,0X00,0X10,0X07,0XFF,0XF8,0XFF,0X9F,0X7E,0XFC,0X19,0XE7,0XEC,0X01,0X79,0XF6,
0XF8,0X95,0X7F,0X9F,0XF0,0X30,0X0C,0X0F,0XFF,0XF9,0X5F,0X9F,0XB0,0XFC,0X15,0XE7,
0XEC,0X02,0XAF,0XDE,0X78,0X65,0XFE,0XAF,0XF8,0X20,0X04,0X1F,0XFF,0XF9,0X77,0X9D,
0XD2,0XFA,0X77,0XD7,0XA4,0X00,0XC2,0XDE,0X78,0X7F,0XFF,0X7F,0XFC,0X00,0X00,0X3F,
0XFF,0XF9,0XF9,0X9A,0XB4,0XEF,0XFF,0XF3,0X00,0X00,0X18,0XF4,0X78,0XD9,0X7F,0XEE,
0X03,0X00,0X00,0XC0,0X7F,0XFC,0XF9,0X3E,0XBC,0X5F,0X7D,0XE7,0X00,0X00,0X1E,0XFB,
0X39,0XFF,0XFF,0XD8,0XF8,0XC0,0X01,0X1F,0X1F,0XF8,0X5E,0X3E,0XBC,0XFF,0X7F,0XAF,
0X00,0X70,0X2F,0XFB,0XBE,0XF7,0XFE,0X71,0XFD,0X10,0X00,0XBF,0X8F,0XF8,0X5C,0X3E,
0XD6,0XF0,0X3F,0X1F,0X00,0XF8,0X7B,0XFB,0X1F,0XEF,0XFF,0XF3,0XC0,0X00,0X00,0X03,
0XCF,0XF8,0X7C,0X39,0XC2,0XEF,0XDE,0XCF,0X00,0XF8,0XFF,0XEC,0X47,0XCF,0XFF,0XF7,
0X8F,0XF9,0X9F,0XF1,0XEF,0XF8,0X30,0XBE,0XFE,0XDF,0XF3,0XE9,0X00,0XF9,0XFF,0XB7,
0XA0,0X1F,0XFF,0XF7,0X1F,0XFF,0XFF,0XF8,0XEF,0XFE,0X00,0X3F,0X7E,0XFE,0X81,0XE7,
0X00,0X86,0XFF,0XFC,0XC8,0XFF,0XFF,0XFB,0X3F,0XFF,0XFF,0XFC,0XDF,0XFE,0X00,0XFF,
0X7F,0X7F,0XCD,0XFD,0X00,0X7C,0XFF,0X7F,0XF0,0X6F,0XFF,0XF8,0X3F,0XFF,0XFF,0XFC,
0X1F,0XFE,0X0F,0XFF,0XBD,0XFD,0XED,0XF9,0X17,0XFC,0XF6,0XFF,0XFF,0X6F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XE2,0XFE,0XEE,0XE3,0X0F,0XF9,0XFD,0XFF,
0XFF,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFE,0XFF,0X1E,0XCF,
};

const byte notes[][PERSIZE] PROGMEM = {
  {
    0, 0, 0, 0,
  
    1, 2,
    2, 1, 1, 2, 2, 1, 2, 2,
    1, 1, 2, 2, 2, 2, 2, 2,
    2, 1, 1, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 1,
    1, 1, 2, 2, 1, 2, 2,
    2, 1, 1, 2, 2, 1, 2, 2,
  
    1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 1, 1, 1, 1, 1,
    1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
  
    1, 1, 1, 2, 2, 2, 2,
    2, 1, 1, 2, 2, 1, 1,
    1, 1, 1, 2, 2, 2, 2,
    2, 1, 1, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 1,
    1, 1, 2, 2, 1, 1,
    2, 1, 1, 2, 2, 1, 2, 2,
  
    1, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2,
    2, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2,
    2, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2,
    2, 2, 2, 2, 1, 1,
  
    0, 0, 0, 0, 

    3,
  },
  {
    0, 0, 0, 0,
    1, 1, 
    1, 1, 1, 1, 2, 1, 2, 2, 1, 2,
    1, 1, 2, 2, 2, 2, 1, 1,
    2, 2, 2, 2, 1, 1,
    2, 2, 1, 2, 2, 1, 2,
    1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
    1, 1, 1, 1, 2, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 1, 2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,

    0, 0, 0, 0,

    3,
  },
  {
    0, 0, 0, 0,

    1, 1, 1, 2, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1,
    2, 2, 2,

    2, 2, 2, 2,
    2, 2, 2,
    2,
    2, 2, 2,
    2, 2, 2,
    2, 2, 1,
    2, 2, 1, 2,
    1, 2, 1, 1, 1, 1, 1,
    2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 1, 1,
    2, 1, 2, 1, 1, 2,

    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2,
    1, 1, 1, 1, 2, 1, 1,
    2, 1, 2, 1, 1, 1,
    2, 1, 2, 1, 1, 2,

    1, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 2,
    1, 2, 2, 2, 2,
    1, 1, 2,
    1, 2, 2, 2, 2,
    1, 2, 2, 2, 1,
    2, 2, 2,
    2, 2, 2, 2,
    2, 1, 1, 1, 1, 2,
    2, 2, 2,
    2, 2, 1, 1, 
    2, 2, 1, 1,
    2, 2, 1, 2, 2,
    1, 2, 1, 1, 2,
    2, 2, 2,
    2, 2, 2,
    2, 1, 2, 2, 2,
    1, 2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 2, 1,
    2, 1, 1, 2, 2,
    2, 1, 2, 1, 2,
    2, 2, 2, 2,
    2, 2, 2, 1, 1,
    1, 1, 1, 2, 2, 2,
    1, 2,
    2, 2, 2,
    2, 1, 2, 1, 2,

    1, 2, 1, 2, 1, 2,
    2, 2,
    1, 2, 1, 2, 1, 2,
    //
    1, 2, 1, 2, 1, 2,
    2, 2,
    1, 2, 1, 2, 1, 2,
    //
    2, 1, 2, 1, 1, 1,
    1, 1, 1, 2, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 2,
    1, 2, 2, 1, 1, 1,
    1, 2, 2, 1, 1, 1,
    1, 2, 2, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2,

    2, 2, 1, 1,
    2, 2, 1, 1,
    2, 2, 1, 2, 2, 
    1, 2, 1, 1, 2,
    2, 2, 2,
    2, 2, 2,
    2, 1, 2, 2, 2,
    1, 2, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 2, 1,
    2, 1, 1, 2, 2,
    2, 1, 2, 1, 2,
    2, 2, 2, 2,
    2, 2, 2, 1, 1,
    1, 1, 1, 2, 2, 2,
    1, 2,
    2, 2, 2, 2,
    2, 1, 2, 1, 2,
    2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2,
    2,
    2, 2, 2,
    2, 2, 2,
    2, 2,
    2, 2, 2,
    2,

    0, 0, 0, 0,

    3
  }
};

const int beats[][PERSIZE] PROGMEM = {
    {
    1000, 1000, 1000, 1000,
  
    750, 250, 500, 500, 1000, 250, 250, 500,
    500, 250, 250, 500, 500, 1000, 500, 500,
    750, 250, 500, 500, 500, 500, 500, 500,
    500, 250, 250, 500, 500, 1000, 1000,
    1000, 1000, 500, 500, 500, 500,
    500, 500, 500, 500, 2000,
    750, 250, 500, 500, 1000, 500, 500,
    500, 250, 250, 500, 500, 1000, 500, 500,
  
    250, 250, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 250, 250, 250, 250, 1000,
    250, 250, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 250, 250, 250, 250, 250, 250, 250, 250,
  
    1000, 750, 250, 500, 500, 500, 500,
    500, 250, 250, 500, 500, 1000, 1000,
    1000, 750, 250, 500, 500, 500, 500,
    500, 250, 250, 500, 500, 1000, 1000,
    1000, 1000, 500, 500, 500, 500,
    500, 500, 500, 500, 2000,
    750, 250, 500, 500, 1000, 1000,
    500, 250, 250, 500, 500, 1000, 500, 500,
  
    1000, 500, 500, 1000, 500, 500,
    1000, 500, 500, 1000, 500, 500,
    1000, 500, 500, 1000, 500, 500,
    500, 500, 500, 500, 1000, 500, 500,
    1000, 500, 500, 1000, 500, 500,
    500, 500, 500, 500, 1000, 500, 500,
    1000, 500, 500, 1000, 500, 500,
    500, 500, 500, 500, 1000, 1000,
  
    1000, 1000, 1000, 1000, 

    -1,
  },
  {
    1000, 1000, 1000, 1000,

    7000, 7000,
    1000, 1000, 1000, 1000, 3000, 1000, 1500, 1500, 1000, 6000,
    1000, 1000, 1500, 1500, 1500, 1500, 1000, 1000,
    1500, 1500, 1500, 1500, 1000, 1000,
    1500, 1500, 1000, 1500, 1500, 1000, 4000,
    1000, 1000, 1000, 1000,
    500, 500, 500, 500, 1000, 500, 500, 500, 500, 3000,
    500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000, 1000,
    1000, 500, 500, 500, 500, 1000, 500, 500, 2000, 500, 500,
    1000, 500, 1000, 500, 3000, 1000, 1000,
    500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000, 500, 500,
    500, 500, 1000, 500, 500, 500, 500, 500, 500, 2000, 500, 500,
    1000, 500, 500, 500, 500, 1000, 500, 500, 2000, 1000,
    1000, 500, 1000, 500, 5000,
    500, 1000, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 1000,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 1000, 1000,
    500, 500, 500, 500, 1000, 500, 500, 500, 500, 1000, 1000, 1000,
    1500, 500, 1500, 500, 1500, 500, 2000,
    500, 1000, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 1000,
    500, 500, 500, 500, 500, 500, 500, 500, 1000, 1000, 1000, 1000,
    1000, 500, 500, 1000, 500, 500, 1000, 1000, 500, 500, 500, 500,
  
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 2000, 500, 500, 
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 
    500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000, 1000,
    1000, 500, 500, 500, 500, 500, 500, 1000, 1000, 500, 500, 500, 500, 
    
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 2000, 500, 500, 
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 2000, 500, 500, 
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 1000, 1000,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 3000,

    1000, 1000, 1000, 1000,

    -1
  },
  {
    1000, 1000, 1000, 1000,

    500, 500, 500, 1000, 500, 500, 500,
    1000, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 1000, 500, 500, 500,
    1000, 500, 500, 500, 500, 500, 500,
    1500, 1500, 1000,

    1000, 1000, 1000, 1000,
    1500, 1500, 1000,
    4000,
    1500, 1500, 1000,
    1500, 1500, 1000,
    1500, 2000, 500,
    1000, 1000, 500, 1500,
    500, 1000, 500, 500, 500, 500, 500,
    1000, 500, 2500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 2500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 1500, 500, 500,
    1000, 500, 1000, 500, 500, 4500,

    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 2500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 2500,
    500, 500, 500, 500, 1000, 500, 500,
    1000, 500, 1000, 500, 500, 500,
    1000, 500, 1000, 500, 500, 4500,

    500, 1000, 1000, 1000, 1000,
    500, 500, 500, 500, 500, 1000,
    500, 1000, 1000, 1000, 1000,
    500, 500, 2500,
    500, 1000, 1000, 1000, 1000,
    500, 1000, 750, 750, 500,
    2000, 1000, 1000,
    1000, 1000, 1000, 1000,
    1500, 500, 500, 500, 500, 5500,
    1000, 1000, 1000,
    1000, 2000, 500, 500, 
    1000, 2000, 500, 500,
    1000, 1000, 500, 1000, 1000,
    500, 1000, 500, 500, 1000,
    1000, 2000, 1000,
    1000, 2000, 1000,
    1000, 500, 1000, 1000, 1000,
    500, 1000, 500, 1500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 1000, 1000, 500,
    1000, 500, 500, 1000, 1000,
    1000, 500, 1000, 500, 1000,
    1000, 1000, 1000, 1000,
    1000, 1000, 1000, 500, 500,
    500, 500, 500, 1000, 1000, 1000,
    500, 4000,
    1000, 1000, 1000,
    1000, 500, 1000, 500, 1000,

    500, 1000, 500, 1000, 500, 2500,
    1000, 1000,
    500, 1000, 500, 1000, 500, 4500,
    //
    500, 1000, 500, 1000, 500, 2500,
    1000, 1000,
    500, 1000, 500, 1000, 500, 4500,
    //
    1000, 500, 1000, 500, 500, 500,
    500, 500, 500, 1000, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1500, 500, 500, 500, 1000,
    500, 1000, 1000, 500, 500, 500,
    500, 1000, 1000, 500, 500, 500,
    500, 1000, 1000, 500, 500, 500,
    1000, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 1000, 1000, 1000,

    1000, 2000, 500, 500,
    1000, 2000, 500, 500,
    1000, 1000, 500, 1000, 1000, 
    500, 1000, 500, 500, 1000,
    1000, 2000, 1000,
    1000, 2000, 1000,
    1000, 500, 1000, 1000, 1000,
    500, 1000, 500, 1500,
    500, 500, 500, 500, 500, 500, 500, 500,
    1000, 500, 1000, 1000, 500,
    1000, 500, 500, 1000, 1000,
    1000, 500, 1000, 500, 1000,
    1000, 1000, 1000, 1000,
    1000, 1000, 1000, 500, 500,
    500, 500, 500, 1000, 1000, 1000,
    500, 3000,
    1000, 1000, 1000, 1000,
    1000, 500, 1000, 500, 1000,
    1500, 1500, 1000,
    1000, 1000, 1000, 1000,
    1500, 1500, 1000,
    4000,
    1500, 1500, 1000,
    1500, 1500, 1000,
    1500, 2500,
    1500, 1000, 1500,
    4000,

    1000, 1000, 1000, 1000,

    -1
  }
  /*,
  {
    1, 1, 1, 1,
  
    0.75, 0.25, 0.5, 0.5, 1, 0.25, 0.25, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
    0.75, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 1, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 2,
    0.75, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
  
    0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 1,
    0.25, 0.25, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25,
  
    1, 0.75, 0.25, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 0.75, 0.25, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 1,
    1, 1, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 2,
    0.75, 0.25, 0.5, 0.5, 1, 1,
    0.5, 0.25, 0.25, 0.5, 0.5, 1, 0.5, 0.5,
  
    1, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 0.5, 1, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 1, 1,
  
    1, 1, 1, 1, 

    -1,
  }
  */
  /*,
  {
    1, 1, 1, 1,

    7, 7,
    1, 1, 1, 1, 3, 1, 1.5, 1.5, 1, 6,
    1, 1, 1.5, 1.5, 1.5, 1.5, 1, 1,
    1.5, 1.5, 1.5, 1.5, 1, 1,
    1.5, 1.5, 1, 1.5, 1.5, 1, 4,
    1, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 3,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 1,
    1, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 2, 0.5, 0.5,
    1, 0.5, 1, 0.5, 3, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5,
    0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 2, 1,
    1, 0.5, 1, 0.5, 5,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    1.5, 0.5, 1.5, 0.5, 1.5, 0.5, 2,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1,
    1, 0.5, 0.5, 1, 0.5, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5,
  
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 2, 1,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5, 
    
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 3,

    1, 1, 1, 1,

    -1
  },
  {
    1, 1, 1, 1,

    0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1.5, 1.5, 1,

    1, 1, 1, 1,
    1.5, 1.5, 1,
    4,
    1.5, 1.5, 1,
    1.5, 1.5, 1,
    1.5, 2, 0.5,
    1, 1, 0.5, 1.5,
    0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 1.5, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 4.5,

    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    1, 0.5, 2.5,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 0.5,
    1, 0.5, 1, 0.5, 0.5, 4.5,

    0.5, 1, 1, 1, 1,
    0.5, 0.5, 0.5, 0.5, 0.5, 1,
    0.5, 1, 1, 1, 1,
    0.5, 0.5, 2.5,
    0.5, 1, 1, 1, 1,
    0.5, 1, 0.75, 0.75, 0.5,
    2, 1, 1,
    1, 1, 1, 1,
    1.5, 0.5, 0.5, 0.5, 0.5, 5.5,
    1, 1, 1,

    1, 1, 1, 1,

    -1
  }
  */
};

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
const int songsCount = sizeof(songs) / sizeof(String);
volatile int currentIdx = 0;
volatile int currentNoteCount = 0;
volatile long countDown = 0;
volatile bool isPlaying = false;
volatile bool reDrawMenu = false;
volatile bool reDrawBeat = false;
volatile bool btn1C = false;
volatile bool btn2C = false;
volatile bool p2Btn1C = false;
volatile bool p2Btn2C = false;
volatile bool prepare = false;
volatile bool judge = false;
volatile bool p1PointChanged = false;
volatile bool p2PointChanged = false;
volatile bool endGame = false;

volatile float pixelPerFrame = 4; //畫面更新橫移量
volatile float pixelPerBeat = 80; //拍子間隔
volatile float lastStartPixel; //上一個起始像素
volatile float currentStartPixel; //當前起始像素
volatile int lastStartIdx; //上一個起始位置
volatile int currentStartIdx; //當前起始位置
volatile int checkIdx;
volatile int checkPixel;
volatile int bpm;
volatile float minRange;
volatile float maxRange;

volatile int p1Point = 0;
volatile int p2Point = 0;
const float judgePixel = 40;
const float centerPixel = 40;

//畫面更新計時器
ISR(TIMER1_COMPA_vect) {
  TCNT1 = 0;
  if(isPlaying) {
    lastStartPixel = currentStartPixel;
    lastStartIdx = currentStartIdx;
    currentStartPixel -= pixelPerFrame;
    checkPixel -= pixelPerFrame;
    if(currentStartPixel < 0)
      currentStartPixel += (float)(pixelPerBeat * pgm_read_word(beats[currentIdx] + (currentStartIdx++))) / 1000;
    if((int) pgm_read_word(beats[currentIdx] + currentStartIdx) < 0 || currentStartIdx >= PERSIZE) {
      isPlaying = false;
      endGame = true;
      tft.fillScreen(ILI9341_BLACK);
      return;
    }
    reDrawBeat = true;
  }
}

void btn1Clicked() {
  btn1C = true;
  if (countDown)
    return;
  if(!isPlaying) {
    currentIdx = currentIdx - 1 < 0 ? songsCount - 1 : currentIdx - 1;
    reDrawMenu = true;
  }
  countDown = COUNTDOWN;
}

void btn2Clicked() {
  btn2C = true;
  if (countDown)
    return;
  if(!isPlaying) {
    currentIdx = currentIdx + 1 >= songsCount ? 0 : currentIdx + 1;
    reDrawMenu = true;
  }
  countDown = COUNTDOWN;
}

void stopPlaying () {
  Wire.beginTransmission(1);
  Wire.write(6);
  Wire.endTransmission();
}

void resetArgs() {
  //重要公式
  //pixelPerFrame = pixelPerBeat / ((60 / (float)bpm * 1000) / (1000 / (float)FPS)); //畫面更新橫移量
  pixelPerBeat = pixelPerFrame * 60 * FPS / bpm;
  minRange = judgePixel - pixelPerFrame;
  maxRange = judgePixel + pixelPerFrame * 2;
  lastStartPixel = -1; //上一個起始像素
  currentStartPixel = judgePixel - pixelPerBeat; //當前起始像素
  lastStartIdx = -1; //上一個起始位置
  currentStartIdx = 0; //當前起始位置
  checkPixel = currentStartPixel;
  checkIdx = currentStartIdx;
  p1Point = 0;
  p2Point = 0;
  cli();
  TCNT1 = 0;
  sei();
}

void judgeClick() {
  bool pressed = false;
  if (checkPixel <= minRange)
    checkPixel += pixelPerBeat * ((float) pgm_read_word(beats[currentIdx] + (checkIdx++)) / 1000);
  if (checkPixel <= maxRange) {
    requestForBtnStates();
    byte note = pgm_read_byte(notes[currentIdx] + checkIdx);
    if (note == 1) {
      if (!digitalRead(BUTTON1_PIN)) {
        pressed = true;
        p1PointChanged = true;
        ++ p1Point;
      }
      if (p2Btn1C) {
        pressed = true;
        p2PointChanged = true;
        ++ p2Point;
      }
    }
    else if (note == 2) {
      if (!digitalRead(BUTTON2_PIN)) {
        pressed = true;
        p1PointChanged = true;
        ++ p1Point;
      }
      if (p2Btn2C) {
        pressed = true;
        p2PointChanged = true;
        ++ p2Point;
      }
    }
    if(pressed)
      checkPixel += pixelPerBeat * ((float) pgm_read_word(beats[currentIdx] + (checkIdx++)) / 1000);
  }
}

void sendByte() {
  Wire.beginTransmission(1);
  Wire.write(5);
  Wire.write(currentIdx);
  Wire.endTransmission();
}

void requestForBPM() {
  Wire.beginTransmission(1);
  Wire.write(1);
  Wire.endTransmission();
  if (prepare) {
    Wire.requestFrom(1, 1);
    bpm = Wire.read();
  }
}

void requestForBtnStates() {
  Wire.beginTransmission(1);
  Wire.write(2);
  Wire.endTransmission();
  Wire.requestFrom(1, 2);
  p2Btn1C = Wire.read();
  p2Btn2C = Wire.read();
}

void drawMenu() {
  tft.fillScreen(ILI9341_BLACK);
  for(int i=currentIdx-2, j=1;i<=currentIdx+2;++i, ++j) {
    tft.setCursor(20, 40*j);
    if(j == 3) {
      tft.fillCircle(0, 125, 5, ILI9341_RED);
      tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    }
    else
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.println(songs[(i + songsCount) % songsCount]);
  }
}

void drawBeat() {
  tft.setCursor(0, 0);
  if (p1PointChanged) {
    p1PointChanged = false;
    tft.setCursor(120, 180);
    tft.print(p1Point);
  }
  if (p2PointChanged) {
    p2PointChanged = false;
    tft.setCursor(280, 180);
    tft.print(p2Point);
  }
  if (lastStartIdx >= 0) {
    for(int i=lastStartIdx, p=lastStartPixel;2+p < 320;++i) {
      float beat = (float) pgm_read_word(beats[currentIdx] + i) / 1000;
      if (beat < 0)
        break;
      if (pgm_read_byte(notes[currentIdx] + i) != 0)
        tft.fillCircle(2+p, 120, 12, ILI9341_BLACK);
      p += pixelPerBeat * beat;
    }
  }
  tft.drawCircle(centerPixel, 120, 12, ILI9341_WHITE);
  for(int i=currentStartIdx, p=currentStartPixel;p < 320;++i) {
    float beat = (float) pgm_read_word(beats[currentIdx] + i) / 1000;
    if(beat < 0)
      break;
    byte note = pgm_read_byte(notes[currentIdx] + i);
    if (note == 1)
      tft.fillCircle(0+p, 120, 8, ILI9341_RED);
    else if(note == 2)
      tft.fillCircle(0+p, 120, 8, ILI9341_CYAN);
    p += pixelPerBeat * beat;
  }
}

void drawPrepare() {
  tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
  tft.setCursor(60, 100);
  tft.println(songs[currentIdx]);
  delay(1000);
  tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
  tft.setTextSize(2);
  for (int i=0;i<PERSIZE;++i) {
    if (pgm_read_word(notes[currentIdx] + i) == 3) {
        currentNoteCount = i;
        break;
    }
  }
  sendByte();
  requestForBPM();
  resetArgs();
  tft.setCursor(0, 0);
  tft.println(songs[currentIdx]);
  tft.setCursor(0, 180);
  tft.println("P1 Point: ");
  tft.setCursor(160, 180);
  tft.println("P2 Point: ");
  //tft.fillRect((unsigned int)minRange, 0, (unsigned int)maxRange, 240, ILI9341_YELLOW);
  prepare = false;
  isPlaying = true;
}

void end(){
    tft.fillScreen(ILI9341_BLACK);
    tft.fillRect(60, 50, 200, 70, 0x8888);
    tft.fillRect(60, 120, 200, 70, 0x8888);
    //score
    tft.setCursor(65, 55);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Player1:");
    tft.print(p1Point);
    tft.setCursor(170, 100);
    tft.setTextSize(2);
    if(p1Point < currentNoteCount / 3){ //Bad
      tft.setTextColor(ILI9341_BLACK);
      tft.print("Bad");
    }
    else if(p1Point < (currentNoteCount / 3) * 2){
      tft.setTextColor(ILI9341_BLUE);
      tft.print("Good");
    }
    else{
      tft.setTextColor(ILI9341_RED);
      tft.print("Perfect");
    }


    tft.setCursor(65, 125);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Player2:");
    tft.print(p2Point);

    tft.setCursor(170, 170);
    tft.setTextSize(2);
    if(p2Point < currentNoteCount / 3){ //Bad
      tft.setTextColor(ILI9341_BLACK);
      tft.print("Bad");
    }
    else if(p2Point < (currentNoteCount / 3) * 2){
      tft.setTextColor(ILI9341_BLUE);
      tft.print("Good");
    }
    else{
      tft.setTextColor(ILI9341_RED);
      tft.print("Perfect");
    }
    delay(5000);
    reDrawMenu = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), btn1Clicked, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), btn2Clicked, FALLING);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setRotation(3);
  tft.drawBitmap(80, 60, img, 160, 120, ILI9341_WHITE);
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000101;
  TIMSK1 |= B00000010;
  OCR1A = 15625 / FPS;

  /*
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= B00000111;
  TIMSK2 |= B00000100;
  OCR2B = 127;
  */

  sei();
  Wire.begin(0);
  Wire.setClock(34000);

  stopPlaying ();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(countDown > 0)
    --countDown;
  if (prepare) {
    drawPrepare();
  }
  else if(!isPlaying) {
    if(btn1C && btn2C)
      prepare = true;
  }
  if (endGame) {
    endGame = false;
    end();
  }
  if(reDrawBeat) {
    reDrawBeat = false;
    drawBeat();
  }
  if(isPlaying) {
    judgeClick();
  }
  btn1C = false;
  btn2C = false;
  p2Btn1C = false;
  p2Btn2C = false;
  if(reDrawMenu) {
    reDrawMenu = false;
    drawMenu();
  }
}
