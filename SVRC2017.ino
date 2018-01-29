/*
 * Miltiple Ultrasonic Sensors
 * Prints the distance read by many ultrasonic sensors in
 * centimeters and inches. They are supported to four pins
 * ultrasound sensors (liek HC-SC04) and three pins
 * (like PING))) and Seeed Studio sesores).
 *
 * The circuit:
 * * In this circuit there is an ultrasonic module HC-SC04,
 *   PING))) and a Seeed Studio (4 pins, 3 pins, 3 pins,
 *   respectively), attached to digital pins as follows:
 * ---------------------     ---------------------     ---------------------
 * | HC-SC04 | Arduino |     | PING))) | Arduino |     |  Seeed  | Arduino |
 * ---------------------     ---------------------     ---------------------
 * |   Vcc   |   5V    |     |   Vcc   |   5V    |     |   Vcc   |   5V    |
 * |   Trig  |   12    | AND |   SIG   |   10    | AND |   SIG   |    8    |
 * |   Echo  |   13    |     |   Gnd   |   GND   |     |   Gnd   |   GND   |
 * |   Gnd   |   GND   |     ---------------------     ---------------------
 * ---------------------
 * Note: You need not obligatorily use the pins defined above
 *
 * By default, the distance returned by the distanceRead()
 * method is in centimeters, to get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.distanceRead(INC)
 *
 * created 3 Mar 2017
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 *
 * This example code is released into the MIT License.
 */

#include <Ultrasonic.h>
#define led 4
#define TSW 10
#define PSW 8
#define fan 9
#define echo 13
#define trig 12
Ultrasonic ultrasonic1(trig, echo);  // An ultrasonic sensor HC-04

int dist = 0, start_cnt = 0, start_flg = 0, leave_cnt = 0, leave_flg = 0;

void setup() {
  pinMode(TSW,INPUT);//toggle
  pinMode(PSW,INPUT_PULLUP);//psw
  pinMode(led,OUTPUT);//led
  //9,fan analog
  Serial.begin(9600);//for debug
  reset();
}

void smell(){
  Serial.println("1秒後に動作開始");
  digitalWrite(led, HIGH);
  delay(1000);
  analogWrite(fan,255);
  delay(3000);
  analogWrite(fan,0);
  digitalWrite(led, LOW);
}

void reset(){
  Serial.println("リセット");
  leave_flg=0;
  leave_cnt=0;
  start_cnt=0;
  start_flg=0;
  Serial.println("手があるか判定開始");
}

void loop() {
  dist = ultrasonic1.distanceRead();
  Serial.print("Sensor 01: ");
  Serial.print(dist); // Prints the distance on the default unit (centimeters)
  Serial.println("cm");
  if(TSW){
    if( dist < 50 ){
      start_cnt++;
    }
    if( start_cnt > 5 ){
      start_flg = 1;
      Serial.println("手がある");
      Serial.println("手がなくなるか判定開始");
    }
    if( start_flg ){
      if(dist > 100){
        leave_cnt++;
      }
      if(leave_cnt > 10){
        leave_flg = 1;
        Serial.println("なくなった");
      }
      if( leave_flg ){
        smell();
        reset();
      }
    }
  }

  if(!PSW){
    smell();
    reset();
  }

}
