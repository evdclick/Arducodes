/*
 created 29 May 2024
  by William Jimenez
  Maintenance Technician - Cartagena
  last edit 02 Jun 2024
  by William Jimenez
  Maintenance Technician - Cartagena
*/

/*Recursos y códigos de referencia:
https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay


*/
//Borneras de entradas digitales
const int eStop = 26;           //Parada de emergencia
const int valvStrClosed = 27;   //Final de carrera válvula estribor cerrada
const int valvStrOpen = 28;     //Final de carrera válvula estribor abierta
const int valvPrtClosed = 29;   //Final de carrera válvula babor cerrada
const int valvPrtOpen = 30;     //Final de carrera válvula babor abierta
const int pumpReset = 31;       //Pulsador para reset de falla de bomba suministro
const int commandOpenStr = 32;  //Comando de apertura válvula estribor
const int commandOpenPrt = 33;  //Comando de apertura válvula babor
const int starterFailure = 34;    //Arrancador de la bomba de suministro en falla

//Borneras de salidas digitales
const int runningSt = 35;         //Indicador parpadeando de sistema operando
const int giro1ValvStr = 36;      //Comando giro apertura válvula estribir
const int giro2ValvStr = 37;      //Comando giro de cierre válvula estribor
const int pwmValvStr = 44;        //Señal PWM motorreductor válvula estribor
const int giro1ValvPrt = 38;      //Comando giro apertura válvula babor
const int giro2ValvPrt = 39;      //Comando giro de cierre válvula babor
const int pwmValvPrt = 46;        //Señal PWM motorreductor válvula babor
const int permPump = 40;          //Permisivo para energizar bomba suministro
const int eStopInd = 41;          //Indicador de parada de emergencia activa
const int valvStrOpenInd = 42;    //Indicador LED de válvula estribor abierta
const int valvStrClosedInd = 43;  //Indicador LED de válvula estribor cerrada
const int valvPrtOpenInd = 45;    //Indicador LED de válvula babor abierta
const int valvPrtClosedInd = 47;  //Indicador LED de válvula babor cerrada
const int pumpFaultRst = 48;      //Reset de falla bomba suministro

//Variables para procesamiento intermitencia running
bool ledState = LOW;               // Para variar el estado del LED
unsigned long previousMillis = 0;  // Guardar registro de tiempo de la última vez del LED
const long interval = 500;         // Intervalo para parpadeos (milliseconds)

void setup() {
  //Asignando entradas digitales
  pinMode(eStop, INPUT);           //Parada de emergencia
  pinMode(valvStrClosed, INPUT);   //Final de carrera válvula estribor cerrada
  pinMode(valvStrOpen, INPUT);     //Final de carrera válvula estribor abierta
  pinMode(valvPrtClosed, INPUT);   //Final de carrera válvula babor cerrada
  pinMode(valvPrtOpen, INPUT);     //Final de carrera válvula babor abierta
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
  pinMode(valvStrOpenInd, OUTPUT);       //Indicador LED de válvula estribor abierta
  digitalWrite(valvStrOpenInd, HIGH);    //Forzar indicador de valvula estribor abierta
  pinMode(valvStrClosedInd, OUTPUT);     //Indicador LED de válvula estribor cerrada
  digitalWrite(valvStrClosedInd, HIGH);  //Forzar indicador de valvula estribor cerrada
  pinMode(valvPrtOpenInd, OUTPUT);       //Indicador LED de válvula babor abierta
  digitalWrite(valvPrtOpenInd, HIGH);    //Forzar indicador de valvula babor abierta
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

    //Sección para leer entradas digitales y asignar a variables para preprocesamiento
    bool readerCmdValvStr = digitalRead(commandOpenStr);  //Leer comando de apertura válvula estribor
    bool readerCmdValvPrt = digitalRead(commandOpenPrt);  //Leer comando de apertura válvula babor
    bool limitSwitchStrOpen = digitalRead(valvStrOpen);   //Leer final de carrera válvula estribor abierta
    bool rstRequest = digitalRead(pumpReset);             //Leer final de carrera válvula babor abierta
    bool eStopRequest = digitalRead(eStop);               //Leer estado del botón de parada de emergencia

    //Sección de condicionales establecidas para que los dispositivos instalados operen
    bool condParaAbrirValvEstribor = readerCmdValvStr == LOW && readerCmdValvPrt == HIGH && eStopRequest == LOW && limitSwitchStrOpen == HIGH;
    bool condicionalParaReset = rstRequest == LOW && eStopRequest == HIGH;


    //Sección para identificar solicitud de reset parada de emergencia
    if (condicionalParaReset) {
      digitalWrite(pumpFaultRst, LOW);  //Identificando solicitud de RESET
      delay(5000);
      digitalWrite(pumpFaultRst, HIGH);
      digitalWrite(eStopInd, LOW);  //Normalizar indicador de parada de emergencia
      digitalWrite(permPump, LOW);  //Normalizar permisivo para arranque de bomba suministro
    }

    //Sección para identificar comando de apertura válvula estribor
    if (condParaAbrirValvEstribor) {
      while (limitSwitchStrOpen == HIGH) {
        limitSwitchStrOpen = digitalRead(valvStrOpen);
        digitalWrite(giro1ValvStr, HIGH);
        digitalWrite(giro2ValvStr, LOW);
        analogWrite(pwmValvStr, 180); //Ajustar PWN al 80% aprox para proceder con la apertura de la válvula motorizada
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
