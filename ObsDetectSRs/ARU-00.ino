//Tabla de equivalencias byte array processing
/*arrayToPcsing[0] = relayStopSignalDcs;
  arrayToPcsing[1] = relayAcusticAlarm
  arrayToPcsing[2] = relayTemporaryBypass
  arrayToPcsing[3] = relayOutput4
  arrayToPcsing[4] = statusRunningLedPin
  arrayToPcsing[5] = boolConversor(ZoneSensor1Disabled)
  arrayToPcsing[6] = boolConversor(ZoneSensor2Disabled)
  arrayToPcsing[7] = boolConversor(ZoneSensor3Disabled)
  arrayToPcsing[8] = boolConversor(ZoneSensor4Disabled)
  arrayToPcsing[9] = boolConversor(ZoneSensor5Disabled)
  arrayToPcsing[10] = boolConversor(statusZoneSensor1);
  arrayToPcsing[11] = boolConversor(statusZoneSensor2);
  arrayToPcsing[12] = boolConversor(statusZoneSensor3);
  arrayToPcsing[13] = boolConversor(statusZoneSensor4);
  arrayToPcsing[14] = boolConversor(statusZoneSensor5);
  arrayToPcsing[15] = boolConversor(statusNorthLimitSignal);
  arrayToPcsing[16] = boolConversor(statusSouthLimitSignal);
  arrayToPcsing[17] = boolConversor(statusTravelingSignal);
  arrayToPcsing[18] = boolConversor(statusBypassKeySignal);
  arrayToPcsing[19] = boolConversor(objectDetected);
  arrayToPcsing[20] = statusNorthLimitSignal && statusSouthLimitSignal;
  arrayToPcsing[21] = boolConversor(operativeCondition1);
  arrayToPcsing[22] = boolConversor(operativeCondition2);
  arrayToPcsing[23] = boolConversor(operativeCondition3);
  arrayToPcsing[24] = preAlarmCounter;
*/

//Declaración de entradas digitales
const int travelingSignal = A0;       //Entrada analoga para habilitar función de interlock y alarma acústica mientras exista traslación ya que las digitales se agotaron
const int BypassKeySignal = A1;       //Entrada analoga para anulación temporal de funcionamiento del dispositivo ya que las digitales se agotaron
const int northLimitSignal = 2;       //Entrada digital de límite de traslación del equipo al norte para ignorar zoneSensor5 en rail clamp
const int southLimitSignal = 3;       //Entrada digital de límite de traslación del equipo al norte para ignorar zoneSensor1, zoneSensor2 y zoneSensor3 en escalera y trip footing
const int zoneSensor1 = 4;            //Entrada digital para sensor de zona 1 instalado en escalera de acceso al stacker
const int zoneSensor2 = 5;            //Entrada digital para sensor de zona 2 instalado en trip footing
const int zoneSensor3 = 6;            //Entrada digital para sensor de zona 3 instalado en trip footing
const int zoneSensor4 = 7;            //Entrada digital para sensor de zona 4 instalado en tripper car
const int zoneSensor5 = 8;            //Entrada digital para sensor de zona 5 instalado en rail clamp

//Declaración de salidas digitales pertenecientes a los módulos de relay de 24VDC
const int relayStopSignalDcs = 9;     //Salida de relay para señal de stop de traslación
const int relayAcusticAlarm = 10;     //Salida de relay para señal de alarma acústica en campo
const int relayTemporaryBypass = 11;  //Reserva para cualquier uso adicional que se requiera
const int relayOutput4 = 12;          //Reserva para cualquier uso adicional que se requiera
byte arrayToPcsing[32];
byte enablePcsing = 200;
byte disablePcsing = 100;

//Salida digital para indicar ejecución del programa
const int runningLedPin = LED_BUILTIN; //Led piloto que trae incorporado Arduino
bool statusRunningLedPin = LOW;        //Variable de apoyo para las intermitencias del led

//Variables para decidir si quedan deshabilitados los sensores de detección de obstáculo
//Funciona con lógica inversa HABILITADO = false; DESHABILITADO = true ....¿Sensor Deshabilitado?
bool ZoneSensor1Disabled = false;     //¿Deshabilitado sensor zona 1 instalado en escalero de acceso al stacker?
bool ZoneSensor2Disabled = true;      //¿Deshabilitado sensor zona 2 instalado en trip footing?
bool ZoneSensor3Disabled = true;      //¿Deshabilitado sensor zona 3 instalado en trip footing?
bool ZoneSensor4Disabled = false;      //¿Deshabilitado sensor zona 4 instalado en tripper car?
bool ZoneSensor5Disabled = false;     //¿Deshabilitado sensor zona 5 instalado en rail clamp?

//Variables de cómputo para toma de decisiones
bool statusZoneSensor1 = true;        //Variable asignada para leer estado lógico del sensor de escalera
bool statusZoneSensor2 = true;        //Variable asignada para leer estado lógico de sensor instalado en trip footing
bool statusZoneSensor3 = true;        //Variable asignada para leer estado lógico de sensor instalado en trip footing
bool statusZoneSensor4 = true;        //Variable asignada para leer estado lógico del sensor tripper car
bool statusZoneSensor5 = true;        //Variable asignada para leer estado lógico del sensor rail clamp
bool operativeCondition1 = false;     //Variable que agrupa los estados lógicos de todos los sensores
bool operativeCondition2 = false;     //variable que agrupa los estados lógicos que deben ser reconocidos cuando el equipo está en extremo sur
bool operativeCondition3 = false;     //variable que agrupa los estados lógicos que deben ser reconocidos cuando el equipo está en extremo norte
bool statusNorthLimitSignal = true;   //Variable asignada para leer el estado del equipo en el extremo límite sur y poder ignorar zoneSensor1, zoneSensor2 y zoneSensor3
bool statusSouthLimitSignal = true;   //Variable asignada para leer el estado del equipo en el extremo límite sur y poder ignorar zoneSensor5
bool statusTravelingSignal = false;   //Variable asignada para leer el estado de la señal de bypass temporar que anula la ejecución del programa durante un tiempo establecido para mantenimiento o ignorar señales falsas
bool statusBypassKeySignal = false;
bool objectDetected = false;          //Variable asignada para leer el estado de la señal en la que se advierte que uno o más sensores ultrasónicos tienen de forma permanente un obstáculo cercano
bool statusFlagged = false;           //Para vigilancia de señal de stop

//Variables de cómputo para activación temporizada de salidas de relay
unsigned long previousMillis = 0;       //Almacenará el último valor de conteo de tiempo para ir comparando
unsigned long currentMillis = 0;        //Variable para contener el valor actual del conteo de ejecución del procesador y así poder temporizar en simultánea con la ejecucución
unsigned long previousMillisRunning = 0;//Base de conteo para el led interno de ejecución de programa
unsigned long previousMillisToStopInDcs = 0; //Almacenará el último valor de conteo de tiempo para ir comparando y sostener la señal de interlock en DCS
const long intervalStatusRunning = 200;   //Intervalo al cual se dará la indicación de ejecución
int deadBypassTimer = 0;                  //Parámetro de temporización para correr ciclo de programa sin tomar decisiones
int maxBypassTimer = 900;                 // Tiempo  máximo en segundos en que se activa la señal de bypass temporal
int intervalAcusticAlarm = 4000;          //Número mínimo de milisegundos que la alarma acústica se activará al detectar un obstáculo
int intervalToStopInDcs = 6000;           //Número mínimo de milisegundos que el interlock a la traslación se activará al detectar un obstáculo
unsigned int preAlarmCounter = 0;      //Acumulador de potenciales detecciones de obstáculos por cualquiera de los sensores ultrasónicos instalados
int analogValueTravelingSignal = 900; //Preconcepción de valor análogo de señal de traslación con el fin de convertirlo a digital a nivel de programa
int analogValueBypassKeySignal = 900; //Preconcepción de valor análogo con el fin de convertirlo a digital a nivel de programa


void setup() {
  for (byte i = 0; i < 32; i++) {
    arrayToPcsing[i] = 100;
  }
  Serial.begin(115200);                 //Inicializar la comunicación serial a 9600bps

  //Inicializar todos los puntos del microcontrolador que serán entradas digitales
  pinMode (northLimitSignal, INPUT);  //Configurando como entrada digital la señal de límite de recorrido del equipo en el norte
  pinMode (southLimitSignal, INPUT);  //Configurando como entrada digital la señal de límite de recorrido del equipo en el sur
  pinMode (zoneSensor5, INPUT);       //Configurando como entrada digital la señal del sensor instalado en la estación de rail clamps
  pinMode (zoneSensor4, INPUT);       //Configurando como entrada digital la señal del sensor instalado en la zona de tripper car
  pinMode (zoneSensor3, INPUT);       //Configurando como entrada digital la señal de sensor instalado en trip footing
  pinMode (zoneSensor2, INPUT);       //Configurando como entrada digital la señal de sensor instalado en trip footing
  pinMode (zoneSensor1, INPUT);       //Configurando como entrada digital la señal del sensor instalado en la escalera de acceso

  //Inicializar todos los puntos del microcontrolador que serán salidas digitales
  pinMode (relayStopSignalDcs, OUTPUT);   //Configurando salida de relay asignada para la señal de detención de la traslación en DCS
  pinMode (relayAcusticAlarm, OUTPUT);    //Configurando salida de relay asignada para la señal de alarma acústica en campo
  pinMode (relayTemporaryBypass, OUTPUT); //Configurando salida de relay asignada para la señal piloto de dispositivo en bypass temporal
  pinMode (relayOutput4, OUTPUT);         //Configurando salida de relay de reserva
  pinMode (runningLedPin, OUTPUT);        //COnfigurando led interno como salida digital

  digitalWrite (relayStopSignalDcs, HIGH); //Activando salida de relay para señal de detención de traslación en DCS como activa en BAJO con el objetivo de identificar algún problema en fluido eléctrico del dispositivo
  digitalWrite (relayAcusticAlarm, LOW); //Desactivando salida de relay para señal de alarma acústica como normalmente apagada
  digitalWrite (relayTemporaryBypass, LOW); //Desactivando salida de relay para señal piloto de dispositivo en bypass temporal
  digitalWrite (relayOutput4, LOW); //Desactivando salida de relay de reserva
  arrayToPcsing[5] = boolConversorNot(ZoneSensor1Disabled);
  arrayToPcsing[6] = boolConversorNot(ZoneSensor2Disabled);
  arrayToPcsing[7] = boolConversorNot(ZoneSensor3Disabled);
  arrayToPcsing[8] = boolConversorNot(ZoneSensor4Disabled);
  arrayToPcsing[9] = boolConversorNot(ZoneSensor5Disabled);
}

void loop() {
  //Inicialmente evaluar si se debe ejecutar un bucle temporal del programa para deshabilitar

  //funcionamiento del dispositivo mediante switch de bypass
  analogValueBypassKeySignal = analogRead(BypassKeySignal);
  if (analogValueBypassKeySignal  < 400) {
    statusBypassKeySignal = true;
    arrayToPcsing[18] = boolConversor(statusBypassKeySignal); //Prepare status color report for processing
  }
  else {
    statusBypassKeySignal = false;
    arrayToPcsing[18] = boolConversor(statusBypassKeySignal);
  }

  statusBypassKeySignal = false;
  arrayToPcsing[18] = boolConversor(statusBypassKeySignal);

  if (statusBypassKeySignal == true) {
    //Serial.println("Bypass Activo");
    digitalWrite (relayStopSignalDcs, HIGH);    //Activando salida de relay para señal de detención de traslación en DCS como activa en BAJO
    arrayToPcsing[0] = enablePcsing;
    digitalWrite (relayAcusticAlarm, LOW);      //Desactivando salida de relay para señal de alarma acústica como normalmente apagada
    arrayToPcsing[1] = disablePcsing;
    digitalWrite (relayTemporaryBypass, HIGH);  //Activando salida de relay para señal piloto de dispositivo en bypass temporal
    arrayToPcsing[2] = enablePcsing;
    delay(5000);
    for (deadBypassTimer = 0; deadBypassTimer <= maxBypassTimer; deadBypassTimer++) {   //Se generan conteos manuales para temporizar el modo bypass
      unsigned long currentMillisRunning = millis();                                    //Mientras está en bypass mantiene la comparación para indicar con intermitencias de 1segundo la ejecución
      if (currentMillisRunning - previousMillisRunning >= intervalStatusRunning) {      //Ciclo que define las intermitencias
        previousMillisRunning = currentMillisRunning;
        if (statusRunningLedPin == LOW) {                                               //Bloque flip-flop o basculante
          statusRunningLedPin = HIGH;
          arrayToPcsing[4] = enablePcsing;
        } else {
          statusRunningLedPin = LOW;
          arrayToPcsing[4] = disablePcsing;
        }
        digitalWrite (runningLedPin, statusRunningLedPin);                               //Ejecuta intermitencia tanto el led piloto
        digitalWrite (relayOutput4, statusRunningLedPin);                                //Como en led exterior
        arrayToPcsing[3] = arrayToPcsing[4];
      }
      delay(1000);

      analogValueBypassKeySignal = analogRead(BypassKeySignal); //En esta sección de código se implementa usar la misma llave de bypass para rearmar
      if (analogValueBypassKeySignal  < 400) {                  //el dispositivo si se terminó de ejecutar la tarea mucho antes de que finalizara
        statusBypassKeySignal = true;                           //el ciclo de temporización
        arrayToPcsing[18] = boolConversor(statusBypassKeySignal);
      }
      else {
        statusBypassKeySignal = false;
        arrayToPcsing[18] = boolConversor(statusBypassKeySignal);
      }
      if (statusBypassKeySignal == true) {                       //Si se detecta que el switch de bypass volvió a ser activado, finaliza la temporización
        deadBypassTimer = maxBypassTimer - 5;                    //Antes de rearmarse por segundo pulso de bypass durante dicho ciclo, mantiene ejecucición por cinco segundos
      }
    }
    digitalWrite (relayTemporaryBypass, LOW);  //DEsactivando salida de relay para señal piloto de dispositivo en bypass temporal
    arrayToPcsing[2] = disablePcsing;
    delay(5000);                                                //Al salirse del modo bypass hace una espera de cinco segundos
  }

  unsigned long currentMillisRunning = millis();                //Variable para ciclo de intermitencias en ejecución normal
  if (currentMillisRunning - previousMillisRunning >= intervalStatusRunning) {
    previousMillisRunning = currentMillisRunning;
    if (statusRunningLedPin == LOW) {
      statusRunningLedPin = HIGH;
      arrayToPcsing[4] = enablePcsing;
    } else {
      statusRunningLedPin = LOW;
      arrayToPcsing[4] = disablePcsing;
    }
    digitalWrite (runningLedPin, statusRunningLedPin);                               //Ejecuta intermitencia tanto el led piloto
    digitalWrite (relayOutput4, statusRunningLedPin);                                //Como en led exterior
    arrayToPcsing[3] = arrayToPcsing[4];
  }

  analogValueTravelingSignal = analogRead(travelingSignal);         //Toma lectura del valor análogo del switch de indicación de traslación conectado en 43/44 del M1 en RD-570
  if (analogValueTravelingSignal < 400) {                           //Compara si el valor es suficientemente bajo o cercano a 0VDC para constatar que la traslación realmente está ocurriendo
    statusTravelingSignal = true;                                   //En caso tal la bandera de traslación es true
    arrayToPcsing[17] = boolConversor(statusTravelingSignal);
  }
  else {
    statusTravelingSignal = false;                                  //En caso contrario la bandera de traslación es false
    arrayToPcsing[17] = boolConversor(statusTravelingSignal);
  }

  //Referencia para temporizar alarmas y otras señales de indicación en dispositivo
  currentMillis = millis();
  //Leer las entradas digitales relativas a los distintos sensores y señales
  statusZoneSensor1 = digitalRead(zoneSensor1);             //Leer estado del sensor de escalera
  arrayToPcsing[10] = boolConversor(statusZoneSensor1);
  statusZoneSensor2 = digitalRead(zoneSensor2);             //Leer estado de los sensores instalados en el trip footing
  arrayToPcsing[11] = boolConversor(statusZoneSensor2);
  statusZoneSensor3 = digitalRead(zoneSensor3);             //Leer estado de los sensores instalados en el trip footing
  arrayToPcsing[12] = boolConversor(statusZoneSensor3);
  statusZoneSensor4 = digitalRead(zoneSensor4);             //Leer estado del sensor instalado en tripper car
  arrayToPcsing[13] = boolConversor(statusZoneSensor4);
  statusZoneSensor5 = digitalRead(zoneSensor5);             //Leer estado del sensor instalado en rail clamp
  arrayToPcsing[14] = boolConversor(statusZoneSensor5);
  statusNorthLimitSignal = digitalRead(northLimitSignal);   //Cerciorarse de indicación límite norte basada en el encoder traslación desde el DCS
  arrayToPcsing[15] = boolConversor(statusNorthLimitSignal);
  statusSouthLimitSignal = digitalRead(southLimitSignal);   //Cerciorarse de indicación límite sur basada en el encoder traslación desde el DCS
  arrayToPcsing[16] = boolConversor(statusSouthLimitSignal);
  arrayToPcsing[20] = statusNorthLimitSignal && statusSouthLimitSignal;
  arrayToPcsing[20] = boolConversorNot(arrayToPcsing[20]); //Check location between
  //Simplificando las posibles condiciones de detección según la ubicación/trasación del stacker
  operativeCondition1 = ((statusZoneSensor1 || ZoneSensor1Disabled) && (statusZoneSensor2 || ZoneSensor2Disabled) && (statusZoneSensor3 || ZoneSensor3Disabled)  && (statusZoneSensor4 || ZoneSensor4Disabled)  && (statusZoneSensor5 || ZoneSensor5Disabled)); //Analizando si cualquier sensor se activa
  arrayToPcsing[21] = boolConversor(operativeCondition1);
  operativeCondition2 = (statusZoneSensor4 || ZoneSensor4Disabled) && (statusZoneSensor5 || ZoneSensor5Disabled) ;                                             //Analizando si se activan los sensores tripper car y en rail clamps
  arrayToPcsing[22] = boolConversor(operativeCondition2);
  operativeCondition3 = (statusZoneSensor1 || ZoneSensor1Disabled) && (statusZoneSensor2 || ZoneSensor2Disabled) && (statusZoneSensor3 || ZoneSensor3Disabled)  && (statusZoneSensor4 || ZoneSensor4Disabled) ;                //Analizando si se activan los sensores escalera, trip footing y tripper car
  arrayToPcsing[23] = boolConversor(operativeCondition3);

  //Analizar si el equipo se está trasladando de acuerdo a las lecturas normales o se está trasladando en los límites norte y sur
  //y con ello decidir si se debe ignorar alguna zona de detección específica
  //======================================================
  if (statusSouthLimitSignal == true && statusNorthLimitSignal == true) {
    if (operativeCondition1 == false) {    //Si no se encuentra en límites norte o sur de traslación vigilar si algún sensor se activó
      objectDetected = true;                                                                                    //En caso de que la condición se cumpla, leantar bandera de objeto detectado
      arrayToPcsing[19] = boolConversor(objectDetected);
    }
    else {
      objectDetected = false;
      arrayToPcsing[19] = boolConversor(objectDetected);
    }
  }
  else if (statusSouthLimitSignal == false && statusNorthLimitSignal == true) {
    if (operativeCondition2 == false) {//Si se encuentra en límite sur de traslación vigilar si se activó Zona4 o o Zona5
      objectDetected = true;                                                                                   //En caso de que la condición se cumpla, leantar bandera de objeto detectado
      arrayToPcsing[19] = boolConversor(objectDetected);
    } else {
      objectDetected = false;
      arrayToPcsing[19] = boolConversor(objectDetected);
    }
  }
  else if (statusSouthLimitSignal == true && statusNorthLimitSignal == false) {
    if (operativeCondition3 == false)//Si se encuentra en límite sur de traslación vigilar si se activó Zona1, Zona2And3 o Zona4
    {
      objectDetected = true;                                                                                  //En caso de que la condición se cumpla, leantar bandera de objeto detectado
      arrayToPcsing[19] = boolConversor(objectDetected);
      //Serial.println(arrayToPcsing[22]);
    }
    else {
      objectDetected = false;                                                                                //Si todo está normal y no hay objeto detectado, limpiar la respectiva bandera
      arrayToPcsing[19] = boolConversor(objectDetected);
      //Serial.println(arrayToPcsing[22]);
    }
  }

  //======================================================
  //Parte del programa que permite hacer seguimiento de las señales mediante monitor de comunicación serial
  //Serial.print("TD-530 = ");
  //Serial.print(statusTravelingSignal);                          //Imprimir en consola el estado de detección del sensor ubicado en zona escalera
  //Serial.print(" ---Z1 = ");
  //Serial.print(statusZoneSensor1);                          //Imprimir en consola el estado de detección del sensor ubicado en zona escalera
  //Serial.print(" |Z2 = ");
  //Serial.print(statusZoneSensor2);                      //Imprimir en consola el estado de detección del sensor ubicado en zona trip footing
  //Serial.print(" |Z3 = ");
  //Serial.print(statusZoneSensor3);                      //Imprimir en consola el estado de detección del sensor ubicado en zona trip footing
  //Serial.print(" |Z4 = ");
  //Serial.print(statusZoneSensor4);                          //Imprimir en consola el estado de detección del sensor ubicado en zona tripper car
  //Serial.print(" | Z5 = ");
  //Serial.print(statusZoneSensor5);                          //Imprimir en consola el estado de detección del sensor ubicado en zona rail clamp
  //Serial.print ("| North = ");
  //Serial.print(statusNorthLimitSignal);                     //Imprimir en consola el estado de encoder traslación en límite norte
  //Serial.print("| South = ");
  //Serial.print(statusSouthLimitSignal);                   //Imprimir en consola el estado de encoder traslación en límite sur
  //Serial.print(" |Object: ");
  //Serial.print(objectDetected);
  //Serial.print(" Bypass = ");
  //Serial.println(statusBypassKeySignal);

  //Detectar si se requiere acumular los conteos para activar alarma acústica y alarma de detención de traslación
  if (objectDetected == true)                         //Si el análisis de estado arroja que hay un obstáculo en traslación
  {
    preAlarmCounter++;                                //Se da inicio a los conteos de preaviso
    arrayToPcsing[24] = preAlarmCounter;
  }
  else {
    preAlarmCounter = 0;                              //Se procede a reiniciar conteos de preaviso
    arrayToPcsing[24] = preAlarmCounter;
  }

  if ((preAlarmCounter > 10) && (statusTravelingSignal == true)) //Si los conteos de preaviso son cercanos a 1.5 segundos
  {
    digitalWrite(relayAcusticAlarm, HIGH);                 //Se activa entonces la alarma acústica
    arrayToPcsing[1] = enablePcsing;
    previousMillis = currentMillis;
  }

  if ((preAlarmCounter > 30) && (statusTravelingSignal == true)) //Si los conteos de preaviso son superiores a 4.5 segundos y se está trasladando
  {
    digitalWrite(relayStopSignalDcs, LOW);                 //Se activa entonces la señal de relay stop (activo BAJO)
    arrayToPcsing[0] = enablePcsing;
    previousMillisToStopInDcs = currentMillis;
  }

  if (currentMillis - previousMillis >= intervalAcusticAlarm) {
    digitalWrite(relayAcusticAlarm, LOW);                 //Y a normalizar salida de relay que confirma señal de obstáculo al DCS
    arrayToPcsing[1] = disablePcsing;
  }

  if (currentMillis - previousMillisToStopInDcs >= intervalToStopInDcs) {
    digitalWrite(relayStopSignalDcs, HIGH);                 //Se desactiva el interlock por detección de obstáculo en DCS
    arrayToPcsing[0] = disablePcsing;
  }
  delay(100);             //Esperar 100 milisegundos mientras los sensores toman nueva lectura y generan alarma por detección de obstáculo

  //This block will be for transit code to processing
  arrayToPcsing[28] = 80;
  arrayToPcsing[29] = 80;
  arrayToPcsing[30] = 80;
  arrayToPcsing[31] = 80;
  Serial.write((byte*)&arrayToPcsing, sizeof(arrayToPcsing));

}

byte boolConversor(bool statusVal) {
  if (statusVal == true) {
    return enablePcsing;//enablePcsing;
  }
  else {
    return disablePcsing;//disablePcsing;
  }
}

byte boolConversorNot(bool statusVal) {
  if (statusVal == false) {
    return enablePcsing;//enablePcsing;
  }
  else {
    return disablePcsing;//disablePcsing;
  }
}
