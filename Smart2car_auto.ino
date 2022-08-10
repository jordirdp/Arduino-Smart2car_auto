
/* Project: Smart2car */
/* Author: Jordi Rodriguez */

// Incluye Headers
#include "Pinout.h"
#include "Constants.h"

// Define ultrasonidos HC-SR04
#include <NewPing.h>
NewPing sonar (TRIGGER_PIN, ECHO_PIN);

// Define LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16, 2);


void setup() {

  // Inicia LCD
  lcd.begin();
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("****Smart2car****");
  
  // Inicia Buzzer
  Buzzer_init();
  Buzzer(2,100,100);

  // Inicia Leds
  Led_init(WHITE_R);
  Led_init(WHITE_L);
  Led_init(RED_R);
  Led_init(RED_L);
  for (byte i=0; i<3; i++){
    Led_wheel();
  }

  // Configura sensores de línea
  pinMode(SENSOR_L, INPUT);
  pinMode(SENSOR_R, INPUT);

  // Inicia Motores
  Motores_init();

  lcd.clear();
  lcd.setCursor (0,0);
}


void loop() {
  Lectura_sensores();
  Luces();
  Distancia();
  Linea();
}


void Lectura_sensores() {
  Sensor_left = digitalRead(SENSOR_L);
  Sensor_right = digitalRead(SENSOR_R);

  Sensor_front = sonar.ping_cm();

  Sensor_luz = digitalRead(FOTOSENSOR);
}

void Luces() {
  /* Enciende luces en la oscuridad */
  if (Sensor_luz == HIGH){
    LightOn(WHITE_R);
    LightOn(WHITE_L);
    LightOn(RED_R);
    LightOn(RED_L);
  }
  else {
    LightOff(WHITE_R);
    LightOff(WHITE_L);
    LightOff(RED_R);
    LightOff(RED_L);    
  }
}

void Distancia() {

  while (Sensor_front<=STOP_DISTANCE){
    Para(100);
    Sensor_front = sonar.ping_cm();
    
    /* Retrocede si tiene un obstáculo cerca */
    if (Sensor_front<=ATRAS_DISTANCE) {
      lcd.print("******Atras******");
      Retrocede();
      Buzzer(1,500,500);
    }
  
    /* Se mantiene parado si tiene un obstáculo delante */
    else {
      
      lcd.print("******Stop*******");
      Stop();
      Buzzer(1,100,1000);
    }
  }
}

void Linea() {
  /* Decisiones de movimiento en función de los sensores de suelo */
  if (Sensor_left == 0 && Sensor_right == 0){
    Avanza();
    lcd.print("*****Avanza******");
    
  }
  if (Sensor_left == 0 && Sensor_right == 1){
    Para(100);
    Retrocede();
    Izquierda();
    lcd.print("****Izquierda****");
  }
  if (Sensor_left == 1 && Sensor_right == 0){
    Para(100);
    Retrocede();
    Derecha();
    lcd.print("****Derecha******");
  }

  if (Sensor_left == 1 && Sensor_right == 1){
    Para(100);
    Retrocede();
    Led_wheel();
    Gira_izquierda(); 
    lcd.print("******Atras******");  
  }
}
