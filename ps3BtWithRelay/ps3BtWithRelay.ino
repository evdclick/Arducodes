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

}
void loop() {
  Usb.Task();
  bool ps3StatusOn = PS3.PS3Connected;
  bool ps3NavOn = PS3.PS3NavigationConnected;
  bool joisInMov = PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117;
  bool joiLeftUp = PS3.getAnalogHat(LeftHatY) < 30;
  bool joiLeftDown = PS3.getAnalogHat(LeftHatY) > 230;
  bool joiLeftLeft = PS3.getAnalogHat(LeftHatX) < 30;
  bool joiLeftRight = PS3.getAnalogHat(LeftHatX) > 230;
  bool joiRightUp = PS3.getAnalogHat(RightHatY) < 30;
  bool joiRightDown = PS3.getAnalogHat(RightHatY) > 230;
  bool joiRightLeft = PS3.getAnalogHat(RightHatX) < 30;
  bool joiRightRight = PS3.getAnalogHat(RightHatX) > 230;
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
  bool l1ToConfirm = (joiLeftUp || joiLeftLeft || upButOn || leftButOn) || (joiRightUp || joiRightLeft || circButOn || triaButOn );
  bool l2ToConfirm = (joiLeftDown || joiLeftRight || downButOn || rightButOn) || (joiRightDown || joiRightRight || squareButOn || crosButOn);

  if (ps3StatusOn  || ps3NavOn) {
    if (joiLeftUp || joiLeftLeft || upButOn || leftButOn) {
      digitalWrite (2, LOW);
    }
    else {
      if (!!l1MovOn) {
        digitalWrite (2, HIGH);
      }
    }
    if (joiRightUp || joiRightLeft || circButOn || triaButOn ) {
      digitalWrite (4, LOW);
    }
    else {
      if (!!l1MovOn) {
        digitalWrite (4, HIGH);
      }
    }
    if (joiLeftDown || joiLeftRight || downButOn || rightButOn) {
      digitalWrite (3, LOW);
    }
    else {
      if (!!l2MovOn) {
        digitalWrite (3, HIGH);
      }
    }
    if (joiRightDown || joiRightRight || squareButOn || crosButOn) {
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



    if (PS3.PS3Connected || PS3.PS3NavigationConnected)
    {
      if (PS3.getButtonPress(PS)) {
        bool set1 = true;
        while (set1) {
          Serial.println("Control desconectado");
          delay(100);
          PS3.disconnect();
        }
      }


    }

  }
}

