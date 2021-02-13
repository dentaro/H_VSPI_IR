//改造グラフィックライブラリhttps://github.com/dentaro/ESP32_LCD_ILI9341_SPI-master
#include "ESP32_LCD_ILI9341_SPI.h"

//赤外線関連ライブラリ
#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

//ピンアサイン
const uint8_t CS_SD = 5;//SD card CS ( Chip Select )

//VSPI接続（基板種類KIIRO/DOMO）
const uint8_t SCLK_OLED =  18; //SCLK
const uint8_t MOSI_OLED =  23; //MOSI (Master Output Slave Input)
const uint8_t MISO_OLED =  19; //これは実際は使っていない。MISO (Master Input Slave Output)
const uint8_t CS_OLED = 22;
const uint8_t DC_OLED =  17; //OLED DC(Data/Command)
const uint8_t RST_OLED =  16; //OLED Reset

//HSPI接続(基板種類BENI)
//const uint8_t SCLK_OLED =  14; //SCLK
//const uint8_t MOSI_OLED =  13; //MOSI (Master Output Slave Input)
//const uint8_t MISO_OLED =  12; //これは実際は使っていない。MISO (Master Input Slave Output)
//const uint8_t CS_OLED = 15;
//const uint8_t DC_OLED =  4; //OLED DC(Data/Command)
//const uint8_t RST_OLED =  2; //OLED Reset

const uint8_t lcd_led_pin = 32;//ディスプレイの明るさ変えたい場合はこのピン

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define PINK 0xF81F
#define ORANGE 0xFBE0
#define PURPLE 0x581F
#define VIOLET 0x917F

//ディスプレイのオブジェクト作る
ESP32_LCD_ILI9341_SPI LCD(SCLK_OLED, MISO_OLED, MOSI_OLED, CS_OLED, DC_OLED, RST_OLED, lcd_led_pin);

//565の色指定
uint8_t red = 31, green = 63, blue = 31;
uint8_t red_max = 31, green_max = 63, blue_max = 31;
//uint8_t V_size = 1, H_size = 1;
//uint8_t num;

//赤外線関連
const uint16_t kRecvPin = 34;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
#if DECODE_AC
const uint8_t kTimeout = 50;
#else   // DECODE_AC
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results
int buttonNo = -1;


void setup() {
  Serial.begin(115200);
  IRsetup();
  //LCD.ILI9341_Init(true, 20000000);//VSPI 10~70MHz
  LCD.ILI9341_Init(true, 100000000);//HSPI ~100MHz
  LCD.Disp_Rotation(253);
  LCD.Display_Clear(0, 0, 239, 319);
  LCD.Brightness(255); //LCD LED Full brightness
}
 
void loop() {
  IRloop();
  if(buttonNo == 0){drawTest(RED);}
  else if(buttonNo == 1){drawTest(BLUE);}
  else if(buttonNo == 2){drawTest(GREEN);}
  else if(buttonNo == 3){drawTest(CYAN);}
  else if(buttonNo == 4){drawTest(MAGENTA);}
  else if(buttonNo == 5){drawTest(YELLOW);}
  else if(buttonNo == 6){drawTest(WHITE);}
  else if(buttonNo == 7){drawTest(PINK);}
  else if(buttonNo == 8){drawTest(ORANGE);}
  else if(buttonNo == 9){drawTest(PURPLE);}
  else if(buttonNo == 10){drawTest(VIOLET);}
  buttonNo = -1;
}

void drawTest(uint16_t _col){
  LCD.Display_Clear(0, 0, 319, 239);
  LCD.Draw_Circle_Line(120, 160, 120, red_max, 0, blue_max);
  LCD.Draw_Circle_Fill(120, 160, 120, _col);
}

void IRsetup() {
#if defined(ESP8266)
  //Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  //Serial.begin(kBaudRate, SERIAL_8N1);
#endif  // ESP8266
  while (!Serial)  // Wait for the serial connection to be establised.
    //delay(50);
  // Perform a low level sanity checks that the compiler performs bit field
  // packing as we expect and Endianness is as we expect.
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();  // Start the receiver
}

void IRloop() {
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    //serialPrintUint64(results.value, HEX);
    if(results.value == 0x1FE48B7)buttonNo = 0;
    else if(results.value == 0x1FE58A7)buttonNo = 1;
    else if(results.value == 0x1FE7887)buttonNo = 2;
    else if(results.value == 0x1FE807F)buttonNo = 3;
    else if(results.value == 0x1FE40BF)buttonNo = 4;
    else if(results.value == 0x1FEC03F)buttonNo = 5;
    else if(results.value == 0x1FE20DF)buttonNo = 6;
    else if(results.value == 0x1FEA05F)buttonNo = 7;
    else if(results.value == 0x1FE609F)buttonNo = 8;
    else if(results.value == 0x1FEE01F)buttonNo = 9;
    else if(results.value == 0x1FE10EF)buttonNo = 10;
    else if(results.value == 0x1FE906F)buttonNo = 11;
    else if(results.value == 0x1FE906F)buttonNo = 12;
    else if(results.value == 0x1FED827)buttonNo = 13;
    else if(results.value == 0x1FEF807)buttonNo = 14;
    else if(results.value == 0x1FE30CF)buttonNo = 15;
    else if(results.value == 0x1FEB04F)buttonNo = 16;
    else if(results.value == 0x1FE708F)buttonNo = 17;
    else if(results.value == 0xFFFFFFFFFFFFFFFF)buttonNo = -1;//repeat信号
    //else buttonNo = -1;
    
    Serial.println("");
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)
  }
}
