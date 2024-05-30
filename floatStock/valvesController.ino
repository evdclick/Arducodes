/*
 created 29 May 2024
  by William Jimenez
  Maintenance Technician - Cartagena
  modified 29 May 2024
  by William Jimenez
  Maintenance Technician - Cartagena
*/

/*Recursos y códigos de referencia:
https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay


*/
//Borneras de entradas digitales
const int eStop = 26;           //Parada de emergencia
const int valvStrClosed = 27;   //Final de carrera válvula estribor cerrada
const int valvStrOpened = 28;   //Final de carrera válvula estribor abierta
const int valvPrtClosed = 29;   //Final de carrera válvula babor cerrada
const int valvPrtOpened = 30;   //Final de carrera válvula babor abierta
const int pumpReset = 31;       //Pulsador para reset de falla de bomba suministro
const int commandOpenStr = 32;  //Comando de apertura válvula estribor
const int commandOpenPrt = 33;  //Comando de apertura válvula babor

//Borneras de salidas digitales
const int runningSt = 34;         //Indicador parpadeando de sistema operando
const int giro1ValvStr = 35;      //Comando giro apertura válvula estribir
const int giro2ValvStr = 36;      //Comando giro de cierre válvula estribor
const int pwmValvStr = 44;        //Señal PWM motorreductor válvula estribor
const int giro1ValvPrt = 37;      //Comando giro apertura válvula babor
const int giro2ValvPrt = 38;      //Comando giro de cierre válvula babor
const int pwmValvPrt = 46;        //Señal PWM motorreductor válvula babor
const int permPump = 39;          //Permisivo para energizar bomba suministro
const int eStopInd = 40;          //Indicador de parada de emergencia activa
const int valvStrOpenedInd = 41;  //Indicador LED de válvula estribor abierta
const int valvStrClosedInd = 42;  //Indicador LED de válvula estribor cerrada
const int valvPrtOpenedInd = 43;  //Indicador LED de válvula babor abierta
const int valvPrtClosedInd = 45;  //Indicador LED de válvula babor cerrada
const int pumpFaultRst = 47;      //Reset de falla bomba suministro

//Variables para procesamiento intermitencia running
bool ledState = LOW;               // Para variar el estado del LED
unsigned long previousMillis = 0;  // Guardar registro de tiempo de la última vez del LED
const long interval = 500;         // Intervalo para parpadeos (milliseconds)

//Variables para lectura de señales digitales
bool rstRequest = HIGH;
bool eStopRequest = HIGH;
bool readerCmdValvStr = HIGH;
bool readerCmdValvPrt = HIGH;
bool limitSwitchOpened = HIGH;
bool condParaAbrirValvEstribor = LOW;

void setup() {
  //Asignando entradas digitales
  pinMode(eStop, INPUT);           //Parada de emergencia
  pinMode(valvStrClosed, INPUT);   //Final de carrera válvula estribor cerrada
  pinMode(valvStrOpened, INPUT);   //Final de carrera válvula estribor abierta
  pinMode(valvPrtClosed, INPUT);   //Final de carrera válvula babor cerrada
  pinMode(valvPrtOpened, INPUT);   //Final de carrera válvula babor abierta
  pinMode(pumpReset, INPUT);       //Pulsador para reset de falla de bomba suministro
  pinMode(commandOpenStr, INPUT);  //Comando de apertura válvula estribor
  pinMode(commandOpenPrt, INPUT);  //Comando de apertura válvula babor

  // Asignando las salidas digitales
  pinMode(pwmValvPrt, OUTPUT);           //Señal PWM para regular velocidad en motorreductor Prt
  analogWrite(pwmValvPrt, 0);            //Inicializar inmovilizando
  pinMode(pwmValvStr, OUTPUT);           //Señal PWM para regular velocidad en motorreductor Str
  analogWrite(pwmValvStr, 0);            //Inicializar inmovilizando
  pinMode(runningSt, OUTPUT);            //Indicador parpadeando de sistema operando
  pinMode(giro1ValvStr, OUTPUT);         //Comando giro apertura válvula estribir
  digitalWrite(giro1ValvStr, LOW);       //Forzar inmovilización de válvula motorizada
  pinMode(giro2ValvStr, OUTPUT);         //Comando giro de cierre válvula estribor
  digitalWrite(giro2ValvStr, LOW);       //Forzar inmovilización de válvula motorizada
  pinMode(giro1ValvPrt, OUTPUT);         //Comando giro apertura válvula babor
  digitalWrite(giro1ValvPrt, LOW);       //Forzar inmovilización de válvula motorizada
  pinMode(giro2ValvPrt, OUTPUT);         //Comando giro de cierre válvula babor
  digitalWrite(giro2ValvPrt, LOW);       //Forzar inmovilización de válvula motorizada
  pinMode(permPump, OUTPUT);             //Permisivo para energizar bomba suministro
  digitalWrite(permPump, LOW);           //Forzar apagado de permisivo de la bomba de suministro
  pinMode(eStopInd, OUTPUT);             //Indicador de parada de emergencia activa
  digitalWrite(eStopInd, HIGH);          //Forzar indicador de paradad de emergencia
  pinMode(valvStrOpenedInd, OUTPUT);     //Indicador LED de válvula estribor abierta
  digitalWrite(valvStrOpenedInd, HIGH);  //Forzar indicador de valvula estribor abierta
  pinMode(valvStrClosedInd, OUTPUT);     //Indicador LED de válvula estribor cerrada
  digitalWrite(valvStrClosedInd, HIGH);  //Forzar indicador de valvula estribor cerrada
  pinMode(valvPrtOpenedInd, OUTPUT);     //Indicador LED de válvula babor abierta
  digitalWrite(valvPrtOpenedInd, HIGH);  //Forzar indicador de valvula babor abierta
  pinMode(valvPrtClosedInd, OUTPUT);     //Indicador LED de válvula babor cerrada
  digitalWrite(valvPrtClosedInd, HIGH);  //Forzar indicador de valvula babor cerrada
  pinMode(pumpFaultRst, OUTPUT);         //Reset de falla bomba suministro
  digitalWrite(pumpFaultRst, HIGH);      //Inmovilizar relé de reset arrancador bomba suministro
  delay(5000);                           //Aguardar 5 segundos para permitir diagnóstico de componentes en arranque
}


void loop() {
  // put your main code here, to run repeatedly:
  //Sección para crear intermitencia del LED para running
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //Sección para identificar parada de emergencia
    rstRequest = digitalRead(pumpReset);
    eStopRequest = digitalRead(eStop);
    if (rstRequest == LOW && eStopRequest == HIGH) {
      digitalWrite(pumpFaultRst, LOW);  //Identificando solicitud de RESET
      delay(5000);
      digitalWrite(pumpFaultRst, HIGH);
      digitalWrite(eStopInd, LOW);  //Normalizar indicador de parada de emergencia
      digitalWrite(permPump, LOW);  //Normalizar permisivo para arranque de bomba suministro
    }

    //Sección para identificar comando de apertura válvula estribor
    readerCmdValvStr = digitalRead(commandOpenStr);
    readerCmdValvPrt = digitalRead(commandOpenPrt);
    limitSwitchOpened = digitalRead(valvStrOpened);
    condParaAbrirValvEstribor = readerCmdValvStr == LOW && readerCmdValvPrt == HIGH && eStopRequest == LOW && limitSwitchOpened == HIGH;
    if (condParaAbrirValvEstribor) {
      while (limitSwitchOpened == HIGH) {
        limitSwitchOpened = digitalRead(valvStrOpened);
        digitalWrite(giro1ValvStr, HIGH);
        digitalWrite(giro2ValvStr, LOW);
      }
    }


    // Si el LED está apagado enciende y viceversa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // Asignar al LED con el resultado de la condición usada en ledState:
    digitalWrite(runningSt, ledState);
  }
}
