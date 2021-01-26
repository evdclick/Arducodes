#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;


void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);

  digitalWrite (2, HIGH);
  digitalWrite (3, HIGH);
  digitalWrite (4, HIGH);
  digitalWrite (5, HIGH);
}
void loop() {
  Usb.Task();

  bool ps3StatusOn = PS3.PS3Connected;
  bool ps3NavOn = PS3.PS3NavigationConnected;
  bool joisInMov = PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117;
  bool joiLeftUp = PS3.getAnalogHat(LeftHatY) < 30; //Verificado
  bool joiLeftDown = PS3.getAnalogHat(LeftHatY) > 230;
  bool joiLeftLeft = PS3.getAnalogHat(LeftHatX) < 30;
  bool joiLeftRight = PS3.getAnalogHat(LeftHatX) > 230;
  bool joiRightUp = PS3.getAnalogHat(RightHatY) < 30; //Verificado
  bool joiRightDown = PS3.getAnalogHat(RightHatY) > 230;
  bool joiRightLeft = PS3.getAnalogHat(RightHatX) < 30;
  bool joiRightRight = PS3.getAnalogHat(RightHatX) > 230;
  bool leftButOn = PS3.getAnalogButton(LEFT) && PS3.getAnalogButton(LEFT) > 100;
  bool rigtButOn = PS3.getAnalogButton(RIGHT) && PS3.getAnalogButton(RIGHT) > 100;
  bool upButOn = PS3.getAnalogButton(UP) && PS3.getAnalogButton(UP) > 100;
  bool downButOn = PS3.getAnalogButton(DOWN) && PS3.getAnalogButton(DOWN) > 100;
  bool crosButOn = PS3.getAnalogButton(CROSS) &&  PS3.getAnalogButton(CROSS) > 100;
  bool triaButOn = PS3.getAnalogButton(TRIANGLE) &&  PS3.getAnalogButton(TRIANGLE) > 100;
  bool circButOn = PS3.getAnalogButton(CIRCLE) &&  PS3.getAnalogButton(CIRCLE) > 100;
  bool squareButOn = PS3.getAnalogButton(SQUARE) &&  PS3.getAnalogButton(SQUARE) > 100;
  bool l2MovOn = PS3.getAnalogButton(L2);
  bool r2MovOn = PS3.getAnalogButton(R2);


  if (ps3StatusOn  || ps3NavOn) {
    if (joiLeftUp || joiLeftLeft) {
      digitalWrite (2, LOW);
    }
    else {
      digitalWrite (2, HIGH);
    }
    if (joiRightUp || joiRightLeft) {
      digitalWrite (4, LOW);
    }
    else {
      digitalWrite (4, HIGH);
    }

    if (joiLeftDown || joiLeftRight) {
      digitalWrite (3, LOW);
    }
    else {
      digitalWrite (3, HIGH);
    }
    if (joiRightDown || joiRightRight) {
      digitalWrite (5, LOW);
    }
    else {
      digitalWrite (5, HIGH);
    }
  }
}

/*

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
  if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
    Serial.print(F("\r\nLeftHatX: "));
    Serial.print(PS3.getAnalogHat(LeftHatX));

    if (PS3.getAnalogHat(LeftHatX) < 30) {
      digitalWrite (4, LOW);
    }
    else {
      digitalWrite (4, HIGH);
    }
    if (PS3.getAnalogHat(LeftHatX) > 254) {
      digitalWrite (5, LOW);
    }
    else {
      digitalWrite (5, HIGH);
    }
    Serial.print(F("\tLeftHatY: "));
    Serial.print(PS3.getAnalogHat(LeftHatY));

    if (PS3.getAnalogHat(LeftHatY) < 30) {
      digitalWrite (2, LOW);
    }
    else {
      digitalWrite (2, HIGH);
    }

    if (PS3.getAnalogHat(LeftHatY) > 215) {
      digitalWrite (3, LOW);
    }
    else {
      digitalWrite (3, HIGH);
    }
    if (PS3.PS3Connected) { // The Navigation controller only have one joystick
      Serial.print(F("\tRightHatX: "));
      Serial.print(PS3.getAnalogHat(RightHatX));



      Serial.print(F("\tRightHatY: "));
      Serial.print(PS3.getAnalogHat(RightHatY));
    }
  }

  // Analog button values can be read from almost all buttons
  if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
    Serial.print(F("\r\nL2: "));
    Serial.print(PS3.getAnalogButton(L2));
    if (PS3.PS3Connected) {
      Serial.print(F("\tR2: "));
      Serial.print(PS3.getAnalogButton(R2));
    }
  }





  ///////////////////////////////////////
  // LEFT OR SQUARE
  if ((PS3.getAnalogButton(LEFT) && PS3.getAnalogButton(LEFT) > 100) ||  (PS3.getAnalogButton(SQUARE) &&  PS3.getAnalogButton(SQUARE) > 100)) {
    Serial.print(F("\r\nLEFT OR SQUARE "));
    digitalWrite (2, LOW);
  }  else {
    digitalWrite (2, HIGH);
  }
  ///////////////////////////////////////
  // RIGHT OR CIRCLE
  if ((PS3.getAnalogButton(RIGHT) && PS3.getAnalogButton(RIGHT) > 100) ||  (PS3.getAnalogButton(CIRCLE) &&  PS3.getAnalogButton(CIRCLE) > 100)) {
    Serial.print(F("\r\nRIGHT OR CIRCLE "));
    digitalWrite (3, LOW);
  }  else {
    digitalWrite (3, HIGH);
  }
  ///////////////////////////////////////
  ///////////////////////////////////////
  // UP OR TRIANGLE
  if ((PS3.getAnalogButton(UP) && PS3.getAnalogButton(UP) > 100) ||  (PS3.getAnalogButton(TRIANGLE) &&  PS3.getAnalogButton(TRIANGLE) > 100)) {
    Serial.print(F("\r\nUP OR TRIANGLE "));
    digitalWrite (4, LOW);
  } else {
    digitalWrite (4, HIGH);
  }
  ///////////////////////////////////////
  ///////////////////////////////////////
  // DOWN OR CROSS
  if ((PS3.getAnalogButton(DOWN) && PS3.getAnalogButton(DOWN) > 100) ||  (PS3.getAnalogButton(CROSS) &&  PS3.getAnalogButton(CROSS) > 100)) {
    Serial.print(F("\r\nDOWN OR CROSS "));
    digitalWrite (5, LOW);
  } else {
    digitalWrite (5, HIGH);
  }
  ///////////////////////////////////////





  if (PS3.getButtonClick(PS)) {
    Serial.print(F("\r\nPS"));
    PS3.disconnect();
  }
  else {
    if (PS3.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTraingle"));
      PS3.setRumbleOn(RumbleLow);
    }
    if (PS3.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle"));
      PS3.setRumbleOn(RumbleHigh);
    }

    if (PS3.getButtonClick(SQUARE)) {
      Serial.print(F("\r\nSquare"));
      digitalWrite (4, HIGH);
    }



    if (PS3.getButtonClick(L1))
      Serial.print(F("\r\nL1"));
    if (PS3.getButtonClick(L3))
      Serial.print(F("\r\nL3"));
    if (PS3.getButtonClick(R1))
      Serial.print(F("\r\nR1"));
    if (PS3.getButtonClick(R3))
      Serial.print(F("\r\nR3"));

    if (PS3.getButtonClick(SELECT)) {
      Serial.print(F("\r\nSelect - "));
      PS3.printStatusString();
    }
    if (PS3.getButtonClick(START)) {
      Serial.print(F("\r\nStart"));
      printAngle = !printAngle;
    }
  }


  #if 0 // Set this to 1 in order to see the angle of the controller
  if (printAngle) {
    Serial.print(F("\r\nPitch: "));
    Serial.print(PS3.getAngle(Pitch));
    Serial.print(F("\tRoll: "));
    Serial.print(PS3.getAngle(Roll));
  }
  #endif
  }

  #if 0 // Set this to 1 in order to enable support for the Playstation Move controller
  else if (PS3.PS3MoveConnected) {
  if (PS3.getAnalogButton(T)) {
    Serial.print(F("\r\nT: "));
    Serial.print(PS3.getAnalogButton(T));
  }
  if (PS3.getButtonClick(PS)) {
    Serial.print(F("\r\nPS"));
    PS3.disconnect();
  }
  else {
    if (PS3.getButtonClick(SELECT)) {
      Serial.print(F("\r\nSelect"));
      printTemperature = !printTemperature;
    }
    if (PS3.getButtonClick(START)) {
      Serial.print(F("\r\nStart"));
      printAngle = !printAngle;
    }
    if (PS3.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTriangle"));
      PS3.moveSetBulb(Red);
    }
    if (PS3.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle"));
      PS3.moveSetBulb(Green);
    }
    if (PS3.getButtonClick(SQUARE)) {
      Serial.print(F("\r\nSquare"));
      PS3.moveSetBulb(Blue);
    }
    if (PS3.getButtonClick(CROSS)) {
      Serial.print(F("\r\nCross"));
      PS3.moveSetBulb(Yellow);
    }
    if (PS3.getButtonClick(MOVE)) {
      PS3.moveSetBulb(Off);
      Serial.print(F("\r\nMove"));
      Serial.print(F(" - "));
      PS3.printStatusString();
    }
  }
  if (printAngle) {
    Serial.print(F("\r\nPitch: "));
    Serial.print(PS3.getAngle(Pitch));
    Serial.print(F("\tRoll: "));
    Serial.print(PS3.getAngle(Roll));
  }
  else if (printTemperature) {
    Serial.print(F("\r\nTemperature: "));
    Serial.print(PS3.getTemperature());
  }
  }
  #endif
  }
*/
