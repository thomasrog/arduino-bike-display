
#include "SerialCommand.h"
#include "U8glib.h"

#define arduinoLED 13   // Arduino LED on board

SerialCommand sCmd;     // The demo SerialCommand object

String toBeDisplayedString = "hallo";

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);    // I2C / TWI 

void setup() {
  pinMode(arduinoLED, OUTPUT);      // Configure the onboard LED for output
  digitalWrite(arduinoLED, LOW);    // default to LED off
 
  Serial.begin(115200);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",    LED_on);          // Turns LED on
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("print", printCommand); // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized); // Handler for command that isn't matched  (says "What?")
  Serial.println("Ready");
  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
 
}
void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, toBeDisplayedString.c_str());
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
   u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
 
}



void LED_on() {
  Serial.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}

void LED_off() {
  Serial.println("LED off");
  digitalWrite(arduinoLED, LOW);
}
void printCommand(){

  char *arg;

  Serial.println("We're in processServoCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    toBeDisplayedString = String(arg);
    Serial.print("First argument was: ");
    Serial.println(arg);
  }
  else {
    Serial.println("No arguments");
  }

}
void sayHello() {
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    Serial.print("Hello ");
    Serial.println(arg);
  }
  else {
    Serial.println("Hello, whoever you are");
  }
}

void unrecognized(const char *command) {
  Serial.println("What?");
}
