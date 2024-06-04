import processing.serial.*;

Serial myPort;
PrintWriter output;

int rquad=40;
int xquad=200;
int yquad=200;
boolean overRect = false;

//Colores del boton
int red=100;
int green=100;
int blue=100;

boolean status=false;
String texto="LED OFF";

int xlogo=400;
int ylogo=400;

int valor;

float rojo;
float verde;
float azul;
byte[] clip = new byte[32];

color redColor = color(250, 0, 0);
color greenColor = color(0, 75, 00);


long last_time=0;
int duration=1000;
byte blinker=100;

PImage img;

void setup() {
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 115200);
  //output = createWriter("temp.txt");



  size(800, 600);
  background(134, 92, 181); //Azul puerto
  PFont f = loadFont("Arial-BoldMT-48.vlw"); //Tipo de fuente. Si no existe, se crea desde TOOLS 
  textFont(f, 38);
  textAlign(CENTER);
  fill (237, 240, 252);
  text("Impala Terminals S.A.S.", 480, 35);
  textFont(f, 20);
  text("Monitoreo y control válvulas motorizadas y cargue/suministro", 480, 65);
  fill (0);
  textFont(f, 18);
  textAlign(LEFT);
  text("Valvula motorizada estribor", 30, 120);
  text("Estados de alarma y operación", 500, 120);
  text("Ejecutando apertura", 95, 155);
  text("Pulso operativo", 585, 160);
  text("Ejecutando cierre", 95, 195);
  text("Parada Emergencia", 585, 200);
  text("Válvula totalmente cerrada", 95, 235);
  text("Falla bomba suministro", 585, 240);
  text("Válvula totalmente abierta", 95, 275);
  text("TAG leído babor", 585, 280);
  text("TAG leído estribor", 585, 320);
  text("Perm. bomba suministro", 585, 360);

  text("Válvula motorizada babor", 30, 370);
  text("Ejecutando apertura", 95, 405);
  text("Ejecutando cierre", 95, 445);
  text("Válvula totalmente cerrada", 95, 485);
  text("Válvula totalmente abierta", 95, 525);

  //Firs check if image logo file exists
  File fImg=dataFile("logpnsacolor.png");
  boolean exist=fImg.isFile();
  //In case of that put it on the left top corner
  if (exist) {
    img = loadImage("logpnsacolor.png");
    image(img, 5, 5, width/5, height/7);
  }
  
}


void draw() {

  if (myPort.available()>31) {  
     clip=myPort.readBytes();
  }
  long current_time=millis();


  if ((current_time - last_time)> duration) {
    println(current_time);
    last_time=millis();
    if (blinker==byte(200)) {
      blinker=byte(100);
    } else {
      blinker=byte(200);
    }
  }

  for (byte tex=0; tex<=31; tex++) {
    clip[tex]=byte(200);
  }

  clip[8]=blinker;

  float[] xpos = {560, 560, 560, 560, 560,  70,  70,  70,  70,  70,  70,  70, 560, 850,  70}; 
  float[] ypos = {270, 230, 190, 310, 150, 150, 190, 230, 270, 480, 520, 400, 350, 650, 440};
  float d1p=28;
  for (byte i=0; i<15; i++) {
    statusIndicator(clip[i], xpos[i], ypos[i], d1p, d1p);
  }
  println(int(clip));
}

//To fill color acording to byte value
void stateType(byte value) {
  if (value==byte(200)) {
    fill(greenColor); //Colores para verde
  } else {
    fill(redColor); //Colores para rojo
  }
} 

//To scan status byte array and indicate each one of them according to table order from arduino
void statusIndicator(byte stFlag, float xcord, float ycord, float d1, float d2) {
  stateType(stFlag);
  ellipseMode(CENTER);
  ellipse(xcord, ycord, d1, d2);
}



/*
void draw() {
 
 if (mouseX > xquad-rquad && mouseY < xquad+rquad &&
 mouseY > yquad-rquad && mouseY < yquad+rquad)
 {
 stroke(205, 100, 100);
 overRect=true; //Contorno del rectangulo en un color especifico
 } else
 {
 stroke(0, 0, 0); //Contorno del rectangulo en un color especifico
 overRect=false;
 }
 
 //Dibujar rectangulo
 //  fill(red, green, blue);
 //  rectMode(RADIUS);
 //  rect(xquad, yquad, rquad, rquad);
 
 //Crear texto de color negro con la palabra led
 fill (0, 0, 0);
 //PFont f;
 PFont f = loadFont("Aharoni-Bold-50.vlw"); //Tipo de fuente. Si no existe, se crea desde TOOLS 
 textFont(f, 20);
 text(texto, 170, 270);
 
 if (myPort.available()>31) {  
 clip=myPort.readBytes();
 }
 
 valor=int(clip[4]);
 text("Temperatura = ", 390, 200);
 rectMode(RADIUS);
 fill(19, 149, 240); //Azul puerto
 stroke(19, 149, 240);
 rect(530, 200, 20, 20);
 fill(0, 0, 0);
 text(valor, 530, 200);
 
 // output.print(hour() + ":");
 // output.print(minute() + ":");
 // output.print(second());
 // output.print("   "+ valor + "°C ");
 //  output.println("");
 
 float temp = map (valor, 10, 42, 0, 255);//Escalamos la temperatura donde maximo sea 32ºC y mínimo 15ºC
 rojo=temp;
 verde=temp*-1 + 255;
 azul=temp*-1 + 255;
 //Dibujamos una esfera para colorear la temperatura
 noStroke();
 //valor
 if (valor==200) {
 fill(0, 75, 00); //Colores para verde
 } else {
 fill(250, 0, 0); //Colores para rojo
 }
 //fill(rojo, verde, azul);
 ellipseMode(CENTER);
 ellipse(590, 193, 40, 40);
 }
 
 void mousePressed()  //Cuando el mouse está apretado
 {
 if (overRect==true) //Si el mouse está dentro de rect
 {
 status=!status; //El estado del color cambia
 //    myPort.write("A"); //Envia una "A" para que el Arduino encienda el led
 if (status==true)
 {
 //Color del botón rojo
 red=255;
 green=0;
 blue=0;
 texto="LED ON";
 }
 if (status==false)
 {
 //Color del botón negro
 red=100;
 green=100;
 blue=100;
 texto="LED OFF";
 }
 }
 }
 */
