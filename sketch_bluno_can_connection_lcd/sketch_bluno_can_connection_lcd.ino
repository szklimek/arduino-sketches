#include <LiquidCrystal.h>
#include <SPI.h>
#include "mcp_can.h"

const int spiCSPin = 10;
const int ledPin = 2;
boolean ledON = 1;

MCP_CAN CAN(spiCSPin);

LiquidCrystal lcd(
  7 /* RS pin */,
  8 /* E (enable) */,
  5 /* D4 */,
  4 /* D5 */,
  3/* D6 */,
  2/* D7 */);

const int contrastPin = 6;

void setup() {
  lcd.begin(16 /* Columns */, 2 /* Rows */);
  pinMode(contrastPin, OUTPUT);

  // Initialize proper lcd contrast
  analogWrite(contrastPin, 0);

  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
    lcd.print("CAN BUS Init Failed. Retrying");
    delay(100);
  }
  lcd.print("CAN BUS  Init OK!");
}

void loop() {
//  if (Serial.available())  {
//    String receivedString = Serial.readString();
//
//    // Serial.write expects C-type string
//    char* convertedString = (char*) malloc(sizeof(char)*(receivedString.length() + 1));
//    receivedString.toCharArray(convertedString, receivedString.length() + 1);
//
//    Serial.write(convertedString);
//    Serial.println();
//    lcd.clear();
//    lcd.print(receivedString);
//  }
//  delay(1000);

  unsigned char len = 0;
  unsigned char buf[8];

  if(CAN_MSGAVAIL == CAN.checkReceive())
  {
      CAN.readMsgBuf(&len, buf);
      unsigned long canId = CAN.getCanId();

      Serial.println("-----------------------------");
      Serial.print("Data from ID: 0x");
      Serial.println(canId, HEX);

      for(int i = 0; i<len; i++) {
          Serial.print(buf[i]);
          Serial.print("\t");
      }
      Serial.println();
    }
}
