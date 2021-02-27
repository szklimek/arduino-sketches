#include <LiquidCrystal.h>

// ----- Optional segment LED display
# include "DFRobot_LedDisplayModule.h"
/*DFRobot_LedDisplayModule Constructor
 *Parameter &wire  Wire
 *Parameter: The IIC address in default is 0x48
*/
DFRobot_LedDisplayModule LED(Wire, 0x48);
// ----- end Optional segment LED display

LiquidCrystal lcd(
  12 /* RS pin*/, 
  11 /* E (enable) */, 
  5 /* D4 */, 
  4 /* D5 */, 
  3/* D6 */, 
  2/* D7 */);

const int contrastPin = 6;

void setup() {
  Serial.begin(115200);  //initial the Serial for BLE connection
  
  lcd.begin(16 /* Columns */, 2 /* Rows */);
  pinMode(contrastPin, OUTPUT);

  // ----- Optional segment LED display
  // Initialize segment LED display module
  while(LED.begin4() != 0)
  {
    Serial.println("Failed to initialize the chip , please confirm the chip connection!");
    delay(1000);
  }
  LED.setDisplayArea4(1,2,3,4);
  // ----- end Optional segment LED display
}

void loop() {
  if (Serial.available())  {
    String receivedString = Serial.readString();
    char* convertedString = (char*) malloc(sizeof(char)*(receivedString.length() + 1));
    receivedString.toCharArray(convertedString, receivedString.length() + 1);
    
    Serial.write(convertedString);
    Serial.println();
    lcd.clear();
    lcd.print(receivedString);

    // ----- Optional segment LED display
    int maxNumberLength = 3;
    boolean isNumberValid= isValidNumber(receivedString, maxNumberLength);
    Serial.print("Valid number received: ");
    Serial.println(isNumberValid);
    if (isNumberValid) {
        LED.print4("1","0","0","");
    } else {
        LED.print4("0","0","0","0");
    }
    // ----- end Optional segment LED display
    
    free(convertedString);
  }
  analogWrite(contrastPin, 0);
  delay(50);
}

boolean isValidNumber(String input, int maxLength) {
    int length = input.length();
    if (length > maxLength) {
        return false;
    }
    for (int index = 0; index < length - 1; index++) {
        if (!isDigit(input[index])) {
            return false;
        }
    }
    return true;
}
