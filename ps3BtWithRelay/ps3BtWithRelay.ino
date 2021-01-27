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

  //Init digital outputs
  pinMode (2, OUTPUT);
  digitalWrite (2, HIGH);
  pinMode (3, OUTPUT);
  digitalWrite (3, HIGH);
  pinMode (4, OUTPUT);
  digitalWrite (4, HIGH);
  pinMode (5, OUTPUT);
  digitalWrite (5, HIGH);
  Usb.Task();
}
void loop() {
  bool ps3StatusOn = PS3.PS3Connected;
  bool ps3NavOn = PS3.PS3NavigationConnected;
  bool joysInMov = PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117;
  bool joyLeftUp = PS3.getAnalogHat(LeftHatY) < 30;
  bool joyLeftDown = PS3.getAnalogHat(LeftHatY) > 230;
  bool joyLeftLeft = PS3.getAnalogHat(LeftHatX) < 30;
  bool joyLeftRight = PS3.getAnalogHat(LeftHatX) > 230;
  bool joyRightUp = PS3.getAnalogHat(RightHatY) < 30;
  bool joyRightDown = PS3.getAnalogHat(RightHatY) > 230;
  bool joyRightLeft = PS3.getAnalogHat(RightHatX) < 30;
  bool joyRightRight = PS3.getAnalogHat(RightHatX) > 230;
  bool leftButOn = PS3.getAnalogButton(LEFT) > 100;
  bool rightButOn = PS3.getAnalogButton(RIGHT) > 100;
  bool upButOn = PS3.getAnalogButton(UP) > 100;
  bool downButOn = PS3.getAnalogButton(DOWN) > 100;
  bool crosButOn = PS3.getAnalogButton(CROSS) > 100;
  bool triaButOn = PS3.getAnalogButton(TRIANGLE) > 100;
  bool circButOn = PS3.getAnalogButton(CIRCLE) > 100;
  bool squareButOn = PS3.getAnalogButton(SQUARE) > 100;
  bool l2MovOn = PS3.getAnalogButton(L2) > 100;
  bool r2MovOn = PS3.getAnalogButton(R2) > 100;
  bool l1MovOn = PS3.getAnalogButton(L1) > 100;
  bool r1MovOn = PS3.getAnalogButton(R1) > 100;
  bool l3MovOn = PS3.getAnalogButton(L3) > 100;
  bool r3MovOn = PS3.getAnalogButton(R3) > 100;
  bool l1ToConfirm = (joyLeftUp || joyLeftLeft || upButOn || leftButOn) || (joyRightUp || joyRightLeft || circButOn || triaButOn );
  bool l2ToConfirm = (joyLeftDown || joyLeftRight || downButOn || rightButOn) || (joyRightDown || joyRightRight || squareButOn || crosButOn);


  if (ps3StatusOn  || ps3NavOn) {
    if (joyLeftUp || joyLeftLeft || upButOn || leftButOn) {
      digitalWrite (2, LOW);
    }
    else {
      if (!!l1MovOn) {
        digitalWrite (2, HIGH);
      }
    }
    if (joyRightUp || joyRightLeft || circButOn || triaButOn ) {
      digitalWrite (4, LOW);
    }
    else {
      if (!!l1MovOn) {
        digitalWrite (4, HIGH);
      }
    }
    if (joyLeftDown || joyLeftRight || downButOn || rightButOn) {
      digitalWrite (3, LOW);
    }
    else {
      if (!!l2MovOn) {
        digitalWrite (3, HIGH);
      }
    }
    if (joyRightDown || joyRightRight || squareButOn || crosButOn) {
      digitalWrite (5, LOW);
    }
    else {
      if (!!l2MovOn) {
        digitalWrite (5, HIGH);
      }
    }
    if ((l1MovOn || r1MovOn) && !l1ToConfirm) {
      digitalWrite (2, LOW);
      digitalWrite (4, LOW);
      if (l1MovOn) {
        delay(5000);
      }
    } else {
      if ((!l1MovOn || !r1MovOn) && !l1ToConfirm) {
        digitalWrite (2, HIGH);
        digitalWrite (4, HIGH);
      }
    }
    if ((l2MovOn || r2MovOn) && !l2ToConfirm) {
      digitalWrite (3, LOW);
      digitalWrite (5, LOW);
      if (l2MovOn) {
        delay(5000);
      }
    } else {
      if ((!l2MovOn || !r2MovOn) && !l2ToConfirm) {
        digitalWrite (3, HIGH);
        digitalWrite (5, HIGH);
      }
    }

    if (l2MovOn && r2MovOn) {
      Serial.println("PS Equivalent detected");
      l1ToConfirm = false;
      l2ToConfirm = false;
      PS3.disconnect();
    }

  }
}

