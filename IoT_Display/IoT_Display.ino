/*
   Code for displaying
   Current Date
   Current Time
   Current Weather
   Current Instagram Followers
   &
   Weather forecast for tomorrow

   You can watch out full tutorial video for
   this project on my YouTube Channel

   http://www.youtube.com/techiesms
*/
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
ESP8266WiFiMulti WiFiMulti;

#include <PxMatrix.h>  //https://github.com/2dom/PxMatrix
#include <Ticker.h>  
#include "InstagramStats.h" //https://github.com/witnessmenow/arduino-instagram-stats
#include "JsonStreamingParser.h" //https://github.com/squix78/json-streaming-parser


#define DEBUG 0


WiFiClientSecure secureClient;

InstagramStats instaStats(secureClient);
String userName = "Your_Instagram_Username";

//int previous_count;
int current_count;

String TIME;
String DATE;
String Day;
String Date;
String temperature;
String Status;
String Humidity;
String Forecast_Day;
String MaxMin;
String Forecast_temperature;
String rain;
/********/
#define double_buffer

Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2


// Pins for LED MATRIX

#define matrix_width 32
#define matrix_height 64

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time = 10; //10-50 is usually fine


//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);


// ISR for display refresh
void display_updater()
{
  display.display(70);
}
struct Text {
  const char *text;
  uint16_t width, height;
  int16_t x, y;
  int16_t dx, dy;
} text = {"Hello Techie", 0, 0, 0, 0, 1, 1};

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};
uint16_t textColor = display.color565(0, 255, 255);

/***************************************************************************/

uint16_t static Insta_logo[]  = {
  0x0000, 0x0000, 0x6BFB, 0x531A, 0x52F9, 0x5AB9, 0x5A99, 0x6278, 0x6A78, 0x7258, 0x7A17, 0x81F8, 0x8A18, 0x598E, 0x0000, 0x0000,   // 0x0010 (16) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0020 (32) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0030 (48) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0040 (64) pixels
  0x0000, 0x49B5, 0x51D7, 0x5197, 0x61D7, 0x7237, 0x8257, 0x8A57, 0x9217, 0x9A16, 0x99D6, 0x9935, 0x90D4, 0x9136, 0x9176, 0x0000,   // 0x0050 (80) pixels
  0x0000, 0x59D6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1021, 0x0000, 0x0000, 0x0000, 0x0000, 0x914A,   // 0x0060 (96) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0800, 0x0000, 0x0000, 0x0000,   // 0x0070 (112) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0000, 0x0820, 0x0020, 0x0000, 0x0000, 0x0000,   // 0x0080 (128) pixels
  0x8279, 0x6956, 0x6935, 0xCD7C, 0xFFDF, 0xF7BF, 0xFFDF, 0xFFBF, 0xFFBF, 0xFF7E, 0xFFDF, 0xFFBE, 0xCC19, 0xA0F4, 0xA115, 0xAA36,   // 0x0090 (144) pixels
  0x0000, 0x61D6, 0x0000, 0x0822, 0x71B4, 0x8193, 0x8972, 0x9172, 0x0000, 0x0000, 0xA96F, 0xA96E, 0xB18F, 0xB18E, 0x0800, 0xB9AD,   // 0x00A0 (160) pixels
  0x8129, 0x0000, 0x9969, 0xC1CB, 0xB9AA, 0xC9EA, 0xC1EA, 0x0000, 0x0000, 0xDA29, 0xDA48, 0xDA68, 0xDA88, 0x0000, 0xD268, 0xE2A7,   // 0x00B0 (176) pixels
  0xBA46, 0x0000, 0xBA86, 0xEB68, 0xDB47, 0xDB68, 0xDBA8, 0x0000, 0xF46A, 0xF4AB, 0xF4EB, 0xF52C, 0xD4AA, 0xED4C, 0xED6C, 0x0840,   // 0x00C0 (192) pixels
  0x79D6, 0x70F4, 0xC4BA, 0xFFDF, 0xE65C, 0xA9D3, 0xB1B2, 0xA971, 0xB191, 0xB9D3, 0xC295, 0xCB96, 0xFFDF, 0xDCBA, 0xB0B3, 0xA974,   // 0x00D0 (208) pixels
  0x0000, 0x61D6, 0x0000, 0x0821, 0x7995, 0x0000, 0x0000, 0x9171, 0x9970, 0x0000, 0xA96F, 0x0000, 0x0000, 0x0000, 0x0000, 0xA98C,   // 0x00E0 (224) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC9EA, 0x0000, 0xD229, 0x0000, 0x0000, 0x0820, 0xDA68, 0x0000, 0xD287, 0x0000,   // 0x00F0 (240) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE3E9, 0x0000, 0xF48A, 0x0000, 0x0000, 0xF50B, 0xDCCB, 0x0000, 0x0000, 0xDD0B,   // 0x0100 (256) pixels
  0x81D4, 0x8973, 0xFFFF, 0xCC98, 0x98EF, 0xA8CF, 0xB990, 0xC293, 0xC293, 0xC170, 0xD16F, 0xFFDF, 0xD16F, 0xFFBF, 0xC153, 0xB993,   // 0x0110 (272) pixels
  0x0020, 0x61B6, 0x0000, 0x0801, 0x79B5, 0x0000, 0x0000, 0x0000, 0x9971, 0x0000, 0xA14E, 0x914C, 0x992C, 0xB16E, 0x0800, 0xA16B,   // 0x0120 (288) pixels
  0x0000, 0x0000, 0x7927, 0xB9AA, 0xB1A9, 0xB9CA, 0xC9EA, 0x0000, 0xC208, 0x0000, 0x0000, 0x0000, 0xDA88, 0x0000, 0xCA46, 0x0000,   // 0x0130 (304) pixels
  0x0000, 0x0000, 0x9A25, 0xDB07, 0xF388, 0xF3E9, 0xE3C8, 0x0000, 0xF48A, 0x0000, 0x0000, 0xE4EB, 0xED2B, 0x0000, 0x0820, 0xDD0B,   // 0x0140 (320) pixels
  0x91B3, 0xA192, 0xFF9F, 0xC273, 0xB0AD, 0xCA51, 0xFFFF, 0xFFDF, 0xFFFF, 0xFF9E, 0xDB74, 0xD14E, 0xD12E, 0xFFDF, 0xCA13, 0xC193,   // 0x0150 (336) pixels
  0x0001, 0x59B5, 0x0000, 0x0001, 0x7194, 0x0000, 0x0000, 0x0000, 0x9970, 0x0020, 0x0000, 0x0000, 0x0001, 0xB18E, 0x0801, 0xA16B,   // 0x0160 (352) pixels
  0x0000, 0x0000, 0xA98A, 0x0000, 0x0000, 0x0000, 0xC9EA, 0x0000, 0xA9A8, 0x0000, 0x0000, 0x0800, 0xDA68, 0x0000, 0xD287, 0x0000,   // 0x0170 (368) pixels
  0x0000, 0x0000, 0xE307, 0x0000, 0x0000, 0x0820, 0xEBE9, 0x0000, 0xEC8A, 0x0000, 0x0000, 0xECCB, 0xDCCB, 0x0000, 0x0020, 0xDD0C,   // 0x0180 (384) pixels
  0xA1B2, 0xA96F, 0xF7BF, 0xCA90, 0xD1EF, 0xFF7F, 0xE475, 0xC88C, 0xC88C, 0xEC55, 0xFFBF, 0xC8CD, 0xDA11, 0xFFDF, 0xC991, 0xC972,   // 0x0190 (400) pixels
  0x0001, 0x59D5, 0x0001, 0x0000, 0x7193, 0x0000, 0x0000, 0x0000, 0xA171, 0x0000, 0xA96F, 0xA96F, 0xA16D, 0xA98D, 0x0820, 0xA16B,   // 0x01A0 (416) pixels
  0xB9AC, 0x0000, 0xA98A, 0xB9AB, 0xB1AA, 0xB9AA, 0xC9EA, 0x0000, 0x1020, 0xDA29, 0xD228, 0xD248, 0xDA68, 0x0000, 0xB226, 0x0000,   // 0x01B0 (432) pixels
  0x0000, 0x0000, 0xDB07, 0xEB68, 0xCB07, 0xD368, 0xDBC8, 0x0000, 0xCBC8, 0x0000, 0x0000, 0xE4CB, 0xE4CB, 0x0000, 0x0000, 0xDD0B,   // 0x01C0 (448) pixels
  0xB9CF, 0xC1CD, 0xFF9F, 0xE2EE, 0xDA4B, 0xFFFF, 0xE1EA, 0xD92A, 0xD90B, 0xD9CE, 0xFFBE, 0xD9AF, 0xDA10, 0xFFBE, 0xD170, 0xC971,   // 0x01D0 (464) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1021,   // 0x01E0 (480) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDA68, 0x0000, 0x0000, 0x0000,   // 0x01F0 (496) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0200 (512) pixels
  0xCA6C, 0xD269, 0xF7BF, 0xEBAC, 0xE246, 0xFFDF, 0xF4B2, 0xE986, 0xE147, 0xF494, 0xFFDF, 0xD8EB, 0xE230, 0xFFFF, 0xD16F, 0xD191,   // 0x0210 (528) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0220 (544) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x9186, 0xC208, 0xC207, 0xDA48, 0x0820, 0x0000, 0x0000, 0x0000,   // 0x0230 (560) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0240 (576) pixels
  0xDAEA, 0xE2C7, 0xFF9F, 0xF40B, 0xEAA3, 0xF46C, 0xFFFF, 0xFFDE, 0xFFDF, 0xFFDE, 0xEB0E, 0xE0CA, 0xE22F, 0xFFFF, 0xD94E, 0xD990,   // 0x0250 (592) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0260 (608) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1821, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0270 (624) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0280 (640) pixels
  0xE367, 0xEB24, 0xFFBF, 0xFCCB, 0xF321, 0xFB42, 0xFC07, 0xFD2E, 0xFD0F, 0xF2E8, 0xF1C6, 0xE108, 0xE311, 0xFFFF, 0xD90D, 0xD990,   // 0x0290 (656) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02A0 (672) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02B0 (688) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02C0 (704) pixels
  0xF3E7, 0xF3A2, 0xFDD0, 0xF7BF, 0xFD2B, 0xFC65, 0xFC24, 0xFB83, 0xFB23, 0xF2E5, 0xF2A7, 0xF2CB, 0xFFDE, 0xEC96, 0xD8CC, 0xD990,   // 0x02D0 (720) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02E0 (736) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x02F0 (752) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0300 (768) pixels
  0xBBC7, 0xFCA5, 0xFCC4, 0xFEF6, 0xFFFF, 0xFFFE, 0xFFBD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFBD, 0xFF7D, 0xF515, 0xE12B, 0xD90D, 0xA9CD,   // 0x0310 (784) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0320 (800) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0330 (816) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0340 (832) pixels
  0x0000, 0xED07, 0xFDC9, 0xFDE8, 0xFE6D, 0xFE8E, 0xFE6E, 0xFE2E, 0xFDED, 0xFD6D, 0xFC8B, 0xF328, 0xE9C7, 0xE18A, 0xB0CB, 0x0000,   // 0x0350 (848) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0360 (864) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0370 (880) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0380 (896) pixels
  0x0000, 0x0000, 0xFED0, 0xFECF, 0xFECE, 0xFEAD, 0xFE8E, 0xFE4C, 0xFDEA, 0xED09, 0xD3E8, 0xC2E7, 0xBA47, 0xBB0F, 0x0000, 0x0000,   // 0x0390 (912) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03A0 (928) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03B0 (944) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03C0 (960) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03D0 (976) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03E0 (992) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x03F0 (1008) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0400 (1024) pixels
};



void drawInsta(int x, int y)
{
  int counter = 0;
  for (int yy = 0; yy < 16; yy++)
  {
    for (int xx = 0; xx < 64; xx++)
    {
      display.drawPixel(xx + x , yy + y, Insta_logo[counter]);
      counter++;
    }
  }
}



void getInstagramStatsForUser() {
  if (DEBUG)Serial.println("Getting instagram user stats for " + userName );
  InstagramUserStats response = instaStats.getUserStats(userName);
  if (DEBUG)Serial.println("Response:");
  if (DEBUG)Serial.print("Number of followers: ");
  if (DEBUG)Serial.println(response.followedByCount);
  current_count = response.followedByCount;
}

/***************************************************************************/
void setup(void) {
  if (DEBUG)Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID1", "PASS1");
  WiFiMulti.addAP("SSID2", "PASS2");
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  /***************************************************************************/
  display.begin(16);
  display.flushDisplay();
  display.setTextWrap(false);
  uint16_t textColor = display.color565(255, 255, 255);
  display.setTextColor(textColor);
  delay(1000);




  display_ticker.attach(0.002, display_updater);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print("Connecting ");

  display.setCursor(20, 13);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print(" to ");

  display.setCursor(0, 25);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print(" Internet ");

  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  delay(50);

  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  delay(50);

  delay(1000);
  while ((WiFiMulti.run() != WL_CONNECTED))
  {
    if (DEBUG)if (DEBUG)Serial.println("...");
    delay(1);
  }
  /***************************************************************************/
  // wait for WiFi connection

  display.clearDisplay();
  display.setCursor(0, 13);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print(" Connected ");

  digitalWrite(10, HIGH);
  delay(2000);
  digitalWrite(10, LOW);

}


/***************************************************************************/
void loop() {

  //drawLoGo(0, 0);
  display.clearDisplay();
  display.setCursor(3, 10);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print(" Fetching ");
  display.setCursor(15, 20);
  display.setTextColor(myRED);
  display.setTextSize(1);
  display.print(" Data ");

  delay(1000);
  display.clearDisplay();
  delay(1000);
  display.setCursor(0 , 12);
  display.setTextSize(1);
  display.setTextColor(myRED);
  display.print(" *");
  display.setTextColor(myMAGENTA);
  display.print(" *");
  display.setTextColor(myBLUE);
  display.print(" *");
  display.setTextColor(myYELLOW);
  display.print(" *");
  display.setTextColor(myGREEN);
  display.print(" *");


  /***************************************************************************
           FETCHING INSTAGRAM FOLLOWERS
  ***************************************************************************/
  getInstagramStatsForUser();

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=GZ9FLG4G62ACY3LR"); //HTTP

    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      if (DEBUG)Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        if (DEBUG)Serial.println(payload);
        /***************************************************************************
            FOR TIME
        ***************************************************************************/
        char* temp = &payload[0];
        int i = 0;
        while (temp[i] != 'h' || temp[i + 1] != '1' )
          i++;
        TIME = payload.substring(i + 4);
        int j = 0;
        while (TIME.charAt(j) != '<')
          j++;
        TIME = TIME.substring(0, j);
        if (DEBUG)Serial.println(TIME);
        int k = 0;
        while (TIME.charAt(k) != ':')
          k++;
        k++;
        while (TIME.charAt(k) != ':')
          k++;
        TIME = TIME.substring(0, k);
        if (TIME.charAt(k) != '0')
          TIME = "0" + TIME;
        if (DEBUG)Serial.println(TIME);

        /***************************************************************************
          FOR DATE
        ***************************************************************************/
        int subject = i + j;
        payload = payload.substring(subject);
        temp = &payload[0];
        i = 0;
        while (temp[i] != 'c' || temp[i + 1] != 't' || temp[i + 2] != 'd' || temp[i + 3] != 'a' || temp[i + 4] != 't' )
          i++;
        DATE = payload.substring(i + 7);
        j = 0;
        while (DATE.charAt(j) != '<')
          j++;
        DATE = DATE.substring(0, j);
        k = 0;
        while (DATE.charAt(k) != ',')
          k++;
        Day = DATE.substring(0, k);
        int l = k + 1;
        while (DATE.charAt(l) != ',')
          l++;
        Date = DATE.substring(k + 2, l);
        if (DEBUG)Serial.println(DATE);
        /***************************************************************************/
      }
    } else {
      if (DEBUG)Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      delay(1000);
      while (1);
    }
    http.end();




    display.clearDisplay();
    display.setCursor(1, 0);
    display.setTextColor(myMAGENTA);
    display.setTextSize(2);
    display.print(TIME);

    display.setCursor(0, 15);
    display.setTextColor(myYELLOW);
    display.setTextSize(1);
    display.print(Day);

    display.setCursor(0, 23);
    display.setTextColor(myYELLOW);
    display.setTextSize(1);
    display.print(Date);

    delay(10000);

    display.clearDisplay();

    display.setCursor(1, 0);
    display.setTextColor(myMAGENTA);
    display.setTextSize(2);
    display.print(TIME);

    display.setTextSize(1);
    drawInsta(0, 16);

    display.setTextColor(myCYAN);
    display.setCursor(32, 25);
    display.print(current_count);

    delay(5000);

    /***************************************************************************
         FOR CURRENT WEATHER
       ***************************************************************************/

    if ((WiFiMulti.run() == WL_CONNECTED)) {
      HTTPClient http;

      http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=W6PLXP5564JOY72X"); //HTTP
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {

        // HTTP header has been send and Server response header has been handled
        if (DEBUG)Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {

          String payload = http.getString();
          //if(DEBUG)Serial.println(payload);


          char* temp = &payload[0];
          int i = 0;
          while (temp[i] != 'c' || temp[i + 1] != 'a' || temp[i + 2] != 'r' || temp[i + 3] != 'd' || temp[i + 4] != '-' || temp[i + 5] != 't' || temp[i + 6] != 'e' || temp[i + 7] != 'm' || temp[i + 8] != 'p')// || temp[i + 9] != 'm' || temp[i + 10] != 'e')
            i++;
          temperature = payload.substring(i + 26);
          int j = 0;
          while (temperature.charAt(j) != '<')
            j++;
          temperature = temperature.substring(0, j);
          if (DEBUG)Serial.print("temperature - "); if (DEBUG)Serial.println(temperature);

          /***************************************************************************
            FOR DATE
          ***************************************************************************/
          int subject = i + j;
          payload = payload.substring(subject);
          temp = &payload[0];
          i = 0;
          while (temp[i] != 'c' || temp[i + 1] != 'a' || temp[i + 2] != 'r' || temp[i + 3] != 'd' || temp[i + 4] != '-' || temp[i + 5] != 'p' || temp[i + 6] != 'h' || temp[i + 7] != 'r' || temp[i + 8] != 'a' || temp[i + 9] != 's' || temp[i + 10] != 'e')
            i++;
          Status = payload.substring(i + 13);
          j = 0;
          while (Status.charAt(j) != '<')
            j++;
          Status = Status.substring(0, j);
          if (DEBUG)Serial.print("Forecast - "); if (DEBUG)Serial.println(Status);

          subject = i + j;
          payload = payload.substring(subject);
          temp = &payload[0];
          i = 0;
          while (temp[i] != 'H' || temp[i + 1] != 'u' || temp[i + 2] != 'm' || temp[i + 3] != 'i' || temp[i + 4] != 'd' || temp[i + 5] != 'i' || temp[i + 6] != 't' || temp[i + 7] != 'y' )//|| temp[i + 8] != 'a' || temp[i + 9] != 'n' || temp[i + 10] != '>')// || temp[i + 11] != '>') || temp[i + 12] != '<')// || temp[i + 13] != '"')
            i++;
          Humidity = payload.substring(i + 39);
          j = 0;
          while (Humidity.charAt(j) != '<')
            j++;
          Humidity = Humidity.substring(0, j);
          if (DEBUG)Serial.print("Humidity - "); if (DEBUG)Serial.println(Humidity);
        }
      } else {
        if (DEBUG)Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }

    display.clearDisplay();

    display.setCursor(1, 0);
    display.setTextColor(myMAGENTA);
    display.setTextSize(2);
    display.print(TIME);

    display.setTextColor(myCYAN);
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print("T ");
    display.setCursor(9, 16);
    display.print(temperature);
    display.print("C ");

    display.setTextColor(myCYAN);
    display.setCursor(33, 16);
    display.setTextSize(1);
    display.print("H ");
    display.setCursor(41, 16);
    display.print(Humidity);
    display.print("%");

    display.setTextColor(myCYAN);
    display.setCursor(0, 24);
    display.setTextSize(1);
    display.print(Status);
    delay(5000);
    /***************************************************************************
             FOR WEATHER FORECAST
    ***************************************************************************/

    if ((WiFiMulti.run() == WL_CONNECTED)) {

      HTTPClient http;
      if (DEBUG)Serial.print("[HTTP] begin...\n");
      // configure traged server and url

      http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=42BJTZ02HMS9H8XY"); //HTTP
      if (DEBUG)Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if (DEBUG)Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          //if(DEBUG)Serial.println(payload);


          char* temp = &payload[0];
          int i = 0;
          while (temp[i] != 'd' || temp[i + 1] != 'a' || temp[i + 2] != 'y' || temp[i + 3] != 'p' || temp[i + 4] != 'a' || temp[i + 5] != 'r' || temp[i + 6] != 't' || temp[i + 7] != 'N' || temp[i + 8] != 'a' || temp[i + 9] != 'm' || temp[i + 10] != 'e')
            i++;
          Forecast_Day = payload.substring(i + 13);
          int j = 0;
          while (Forecast_Day.charAt(j) != '<')
            j++;
          Forecast_Day = Forecast_Day.substring(0, j);
          if (Forecast_Day == "Mon" || Forecast_Day == "MON")
            Forecast_Day = "Monday";
          else if (Forecast_Day == "Tue" || Forecast_Day == "TUE")
            Forecast_Day = "Tuesday";
          else if (Forecast_Day == "Wed" || Forecast_Day == "WED")
            Forecast_Day = "Wednesday";
          else if (Forecast_Day == "Thu" || Forecast_Day == "THU")
            Forecast_Day = "Thursday";
          else if (Forecast_Day == "Fri" || Forecast_Day == "FRI")
            Forecast_Day = "Friday";
          else if (Forecast_Day == "Sat" || Forecast_Day == "Sat")
            Forecast_Day = "Saturday";
          else if (Forecast_Day == "Sun" || Forecast_Day == "Sun")
            Forecast_Day = "Sunday";
          else

            if (DEBUG)Serial.print("Day - "); if (DEBUG)Serial.println(Forecast_Day);

          int subject = i + j;
          payload = payload.substring(subject);
          temp = &payload[0];
          i = 0;
          while (temp[i] != 'd' || temp[i + 1] != 'p' || temp[i + 2] != '1' || temp[i + 3] != '-' || temp[i + 4] != 'h' || temp[i + 5] != 'i' || temp[i + 6] != 'g' || temp[i + 7] != 'h' || temp[i + 8] != 'L' || temp[i + 9] != 'o' || temp[i + 10] != 'w')
            i++;
          MaxMin = payload.substring(i + 13);
          j = 0;
          while (MaxMin.charAt(j) != '<')
            j++;
          MaxMin = MaxMin.substring(0, j);

          if (MaxMin == "Max.")
          {
            MaxMin = "Mx";
            if (DEBUG)Serial.print("Max Temperature - ");
          }
          else
          {
            MaxMin = "Mi";
            if (DEBUG)Serial.print("Min Temperature - ");
          }
          //if(DEBUG)Serial.println(MaxMin);

          subject = i + j;
          payload = payload.substring(subject);
          temp = &payload[0];
          i = 0;
          while (temp[i] != 'p' || temp[i + 1] != 'a' || temp[i + 2] != 'r' || temp[i + 3] != 't' || temp[i + 4] != '-' || temp[i + 5] != 't' || temp[i + 6] != 'e' || temp[i + 7] != 'm' || temp[i + 8] != 'p' )//|| temp[i + 9] != 'o' || temp[i + 10] != 'w')
            i++;
          Forecast_temperature = payload.substring(i + 26);
          j = 0;
          while (Forecast_temperature.charAt(j) != '<')
            j++;
          Forecast_temperature = Forecast_temperature.substring(0, j);
          if (DEBUG)Serial.print(Forecast_temperature); if (DEBUG)Serial.println(" C");

          subject = i + j;
          payload = payload.substring(subject);
          temp = &payload[0];
          i = 0;
          while (temp[i] != 'v' || temp[i + 1] != 'a' || temp[i + 2] != 'l' || temp[i + 3] != '"' || temp[i + 4] != '>' || temp[i + 5] != '<' || temp[i + 6] != 's' || temp[i + 7] != 'p' || temp[i + 8] != 'a' || temp[i + 9] != 'n' || temp[i + 10] != '>')// || temp[i + 11] != '>') || temp[i + 12] != '<')// || temp[i + 13] != '"')
            i++;
          rain = payload.substring(i + 11);
          j = 0;
          while (rain.charAt(j) != '<')
            j++;
          rain = rain.substring(0, j);
          if (DEBUG)Serial.print("Chances of Rain - "); if (DEBUG)Serial.print(rain); if (DEBUG)Serial.println("%");

        }
      } else {
        if (DEBUG)Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
      display.clearDisplay();

      display.setCursor(1, 0);
      display.setTextColor(myMAGENTA);
      display.setTextSize(2);
      display.print(TIME);

      display.setTextColor(myCYAN);
      display.setCursor(8, 16);
      display.setTextSize(1);
      display.print(Forecast_Day);

      display.setTextColor(myCYAN);
      display.setCursor(0, 25);
      display.setTextSize(1);
      display.print(MaxMin);
      display.setCursor(13, 25);
      display.print(Forecast_temperature);

      display.setTextColor(myCYAN);
      display.setCursor(33, 25);
      display.setTextSize(1);
      display.print("R");
      display.setCursor(42, 25);
      display.print( rain);
      display.print("%");
      delay(10000);

    }
  }
}
