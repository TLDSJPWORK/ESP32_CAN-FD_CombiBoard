//——————————————————————————————————————————————————————————————————————————————
// CAN FD CombiBoard for ESP32 Example Software
//
// Copyright (c) 2024 TLDSJPWORK
//
// Released under the MIT license.
// see https://opensource.org/licenses/MIT
//——————————————————————————————————————————————————————————————————————————————

//——————————————————————————————————————————————————————————————————————————————
//  ACAN2517FD Demo in loopback mode, for ESP32
//——————————————————————————————————————————————————————————————————————————————

#ifndef ARDUINO_ARCH_ESP32
  #error "Select an ESP32 board"
#endif

//——————————————————————————————————————————————————————————————————————————————

#include <ACAN2517FD.h>
#include <SPI.h>

//——————————————————————————————————————————————————————————————————————————————
//  For using SPI on ESP32, see demo sketch SPI_Multiple_Buses
//  Two SPI busses are available in Arduino, HSPI and VSPI.
//  By default, Arduino SPI use VSPI, leaving HSPI unused.
//  Default VSPI pins are: SCK=18, MISO=19, MOSI=23.
//  You can change the default pin with additional begin arguments
//    SPI.begin (MCP2517_SCK, MCP2517_MISO, MCP2517_MOSI)
//  CS input of MCP2517 should be connected to a digital output port
//  INT output of MCP2517 should be connected to a digital input port, with interrupt capability
//  Notes:
//    - GPIOs 34 to 39 are GPIs – input only pins. These pins don’t have internal pull-ups or
//      pull-down resistors. They can’t be used as outputs.
//    - some pins do not support INPUT_PULLUP (see https://www.esp32.com/viewtopic.php?t=439)
//    - All GPIOs can be configured as interrupts
// See https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
//——————————————————————————————————————————————————————————————————————————————

//static const byte MCP2517_SCK  = 26 ; // SCK input of MCP2517FD
//static const byte MCP2517_MOSI = 19 ; // SDI input of MCP2517FD
//static const byte MCP2517_MISO = 18 ; // SDO output of MCP2517FD
static const byte MCP2517_SCK  = 14 ; // SCK input of MCP2517FD
static const byte MCP2517_MOSI = 13 ; // SDI input of MCP2517FD
static const byte MCP2517_MISO = 12 ; // SDO output of MCP2517FD

//static const byte MCP2517_CS  = 16 ; // CS input of MCP2517FD
//static const byte MCP2517_INT = 32 ; // INT output of MCP2517FD
static const byte MCP2517_CS  = 15 ; // CS input of MCP2517FD
static const byte MCP2517_INT = 27 ; // INT output of MCP2517FD

//——————————————————————————————————————————————————————————————————————————————
//  ACAN2517FD Driver object
//——————————————————————————————————————————————————————————————————————————————

ACAN2517FD can (MCP2517_CS, SPI, MCP2517_INT) ;

#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み

//本体裏側　0x78に接続→0x3C 0x7A→0x3A
SSD1306  display(0x3c, 21, 22); //SSD1306インスタンスの作成（I2Cアドレス,SDA,SCL）

//——————————————————————————————————————————————————————————————————————————————
//   SETUP
//——————————————————————————————————————————————————————————————————————————————

void setup () {
//--- Start serial
  Serial.begin (115200) ;
//----------------------------------- Begin SPI
  SPI.begin (MCP2517_SCK, MCP2517_MISO, MCP2517_MOSI) ;
//--- Configure ACAN2517FD
  Serial.print ("sizeof (ACAN2517FDSettings): ") ;
  Serial.print (sizeof (ACAN2517FDSettings)) ;
  Serial.println (" bytes") ;
  Serial.println ("Configure ACAN2517FD") ;
//--- For version >= 2.1.0
  ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_20MHz, 500 * 1000, DataBitRateFactor::x4,1000) ;
//--- For version < 2.1.0
//  ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_4MHz10xPLL, 125 * 1000, ACAN2517FDSettings::DATA_BITRATE_x1) ;
  settings.mRequestedMode = ACAN2517FDSettings::NormalFD ;
//--- RAM Usage
  Serial.print ("MCP2517FD RAM Usage: ") ;
  Serial.print (settings.ramUsage ()) ;
  Serial.println (" bytes") ;
//--- Begin
  const uint32_t errorCode = can.begin (settings, [] { can.isr () ; }) ;
  if (errorCode == 0) {
    Serial.print ("Bit Rate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("Arbitration Phase segment 1: ") ;
    Serial.println (settings.mArbitrationPhaseSegment1) ;
    Serial.print ("Arbitration Phase segment 2: ") ;
    Serial.println (settings.mArbitrationPhaseSegment2) ;
    Serial.print ("Arbitration SJW:") ;
    Serial.println (settings.mArbitrationSJW) ;
    Serial.print ("Actual Arbitration Bit Rate: ") ;
    Serial.print (settings.actualArbitrationBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("Exact Arbitration Bit Rate ? ") ;
    Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
    Serial.print ("Arbitration Sample point: ") ;
    Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
    Serial.println ("%") ;
  }else{
    Serial.print ("Configuration error 0x") ;
    Serial.println (errorCode, HEX) ;
  }

  display.init();    //ディスプレイを初期化
  display.setFont(ArialMT_Plain_24);    //フォントを設定
  display.clear();
  display.drawString(0, 0, "Disp Start");    //(0,0)の位置に表示
  display.display();   //指定された情報を描画

}

//——————————————————————————————————————————————————————————————————————————————
//   LOOP
//——————————————————————————————————————————————————————————————————————————————

static uint32_t gBlinkLedDate = 0 ;
static uint32_t gReceivedFrameCount = 0 ;
static uint32_t gSentFrameCount = 0 ;

//——————————————————————————————————————————————————————————————————————————————

void loop () {
  CANFDMessage frame ;
  if (gBlinkLedDate < millis ()) {
    gBlinkLedDate += 100 ;
    const bool ok = can.tryToSend (frame) ;
    if (ok) {
      gSentFrameCount += 1 ;
      Serial.print ("Sent: ") ;
      Serial.println (gSentFrameCount) ;
    }else{
      Serial.println ("Send failure") ;
    }
  }
  if (can.available ()) {
    can.receive (frame) ;
    if(frame.id == 0x426) {
      gReceivedFrameCount ++ ;
      char str[100];

      uint8_t count = 0;
      Serial.println(String(frame.data, HEX)) ;
      count  = (frame.data[25] << 6) & 0x40; //bit 207
      count += (frame.data[26] >> 2) & 0x3F; //bit 208-213
      dtostrf((float)(count * 1.0),4,0,str);
      Serial.println("Received: " + String(gReceivedFrameCount) + "\t" + String(frame.id, HEX) + "\tSoc:" + str + "%") ;

      display.clear();
      display.drawString(0, 0, String(frame.id, HEX) + "\nCount:” + str + “”);    //(0,0)の位置にHello Worldを表示
      display.display();   //指定された情報を描画
    }
  }
}

//——————————————————————————————————————————————————————————————————————————————