DESCRIPCIÓN DEL PROYECTO

Se define bajo la propuesta de centralizar, procesar e indicar las señales necesarias para la operación de las válvulas
motorizadas de cargue/despacho en la barcaza. A continuación la lista de señales y los pines asignados en Arduino y
bornera

SEÑAL                                        PIN ARDUINO    PIN BORNERA        TIPO        CODE NAME
Parada de emergencia                              26            1              ENTRADA      eStop
Final de carrera válvula estribor cerrada         27            2              ENTRADA      valvStrClosed
Final de carrera válvula estribor abierta         28            3              ENTRADA      valvStrOpen
Final de carrera válvula babor cerrada            29            4              ENTRADA      valvPrtClosed
Final de carrera válvula babor abierta            30            5              ENTRADA      valvPrtOpen
Pulsador para reset de falla de bomba suministro  31            6              ENTRADA      pumpReset
Comando de apertura válvula estribor              32            7              ENTRADA      commandOpenStr
Comando de apertura válvula babor                 33            8              ENTRADA      commandOpenPrt
Arrancador de la bomba de suministro en falla     34            9              ENTRADA      starterFailure
Indicador parpadeando de sistema operando         35            10             SALIDA       runningSt
Comando giro apertura válvula estribir            36            11             SALIDA       giro1ValvStr
Comando giro de cierre válvula estribor           37            12             SALIDA       giro2ValvStr
Señal PWM motorreductor válvula estribor          44            13             SALIDA       pwmValvStr
Comando giro apertura válvula babor               38            14             SALIDA       giro1ValvPrt
Comando giro de cierre válvula babor              39            15             SALIDA       giro2ValvPrt
Señal PWM motorreductor válvula babor             46            16             SALIDA       pwmValvPrt
Permisivo para energizar bomba suministro         40            17             SALIDA       permPump
Indicador de parada de emergencia activa          41            18             SALIDA       eStopInd
Indicador LED de válvula estribor abierta         42            19             SALIDA       valvStrOpenInd
Indicador LED de válvula estribor cerrada         43            20             SALIDA       valvStrClosedInd
Indicador LED de válvula babor abierta            45            21             SALIDA       valvPrtOpenInd
Indicador LED de válvula babor cerrada            47            22             SALIDA       valvPrtClosedInd
Reset de falla bomba suministro                   48            23             SALIDA       pumpFaultRst


Las definiciones restantes de señales lógicas y condicionales se encuentran definidas internamente en el código pero serán referidas parcialmente en documentación.
