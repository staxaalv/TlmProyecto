// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;
SoftwareSerial mySerial(8, 9);
// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int gx, gy, gz;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y;
float girosc_ang_x_prev, girosc_ang_y_prev;

/*struct gyroGrade {//12Bytes
  float yGradeT1;
  float yGradeT2;
  float yGradeT3;// 4 bytes
};*/

struct gyroGrade {//12Bytes
  float yGradeT1;
};
 
 int tiempo=0;
 gyroGrade angulo;
 bool answer;
 

void setup() {
  Serial.begin(9600);    //Iniciando puerto serial
  mySerial.begin(9600);
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  /*if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  tiempo_prev=millis();*/
}



void loop() {
  // Leer las velocidades angulares 
  sensor.getRotation(&gx, &gy, &gz);
  
  //Calcular los angulos rotacion:
  
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  
  girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;

  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;

//para enviar los tres datos
  tiempo++;
  /*
  if(tiempo==7){
    angulo.yGradeT1=girosc_ang_y;
    }
  if(tiempo==14){
    angulo.yGradeT2=girosc_ang_y;
    }
  if(tiempo==21){
    angulo.yGradeT3=girosc_ang_y;
    answer = sigfoxSend(&angulo, sizeof(gyroGrade));//// enviamos por sigfox
    tiempo=0;
    }*/
    
  Serial.println(girosc_ang_y);
  angulo={girosc_ang_y};         
  answer = sigfoxSend(&angulo, sizeof(gyroGrade));//// enviamos por sigfox
 
  //Mostrar los angulos separadas por un [tab]
  //Serial.print("Rotacion en X:  ");
  //Serial.print(girosc_ang_x); 
  //Serial.print("tRotacion en Y: ");
  //Serial.print(angulo.yGradeT1);
  //Serial.print(" ");
  //Serial.print(angulo.yGradeT2);
  //Serial.print(" ");
  //Serial.print(angulo.yGradeT3);
  //Serial.println();
  
  
  delay(950);
}
  bool sigfoxSend(const void* data, uint8_t len) {
  uint8_t* bytes = (uint8_t*)data;
  mySerial.print("AT$P=0\r");
  delay(100);
  mySerial.print("AT$RC\r");
  mySerial.print("AT$SF=");
  for(uint8_t i = len-1; i < len; --i) {
    if (bytes[i] < 16) {mySerial.print("0");}
    mySerial.print(bytes[i], HEX);
  }
  mySerial.print('\r');
  delay(5000);
  }
