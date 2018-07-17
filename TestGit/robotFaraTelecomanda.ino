#include <Servo.h>
#include <stdlib.h>

#include <IRremote.h>
/*
PWR       FF629D     
CH          FFE21D        
|<<          FF22DD  
>|            FFC23D              
>>|          FF02FD               
-              FFE01F 
Plus        FFA857         
EQ          FF906F          
0             FF6897            
100         FF9867           
200         FFB04F           
1             FF30CF                 
2             FF18E7                 
3             FF7A85                  
4             FF10EF                   
5             FF38C7                    
6             FF5AA5                  
7             FF42BD                  
8             FF4AB5             
9             FF52AD 
dublura      FFFFFFFF

*/

const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
bool autonomous=false;

Servo srv;



#define TRIG_PIN 12
#define ECHO_PIN 10
#define PWM1 3   // for motor1
#define PWM2 11  // for motor2
#define M1 7  // for motor1
#define M2 8  // for motor1
#define M3 13  // for motor2
#define M4 4  // for motor2

#define DEBUG 0
#define DEBUG_SERVO 0
#define DEBUG_SONAR 0
#define SERVO_PIN 6

// pentru sonar getDistance()
long duration;
int distance;

// pentru servo searchLeftandRight(const int pin)
unsigned int  dreapta , stanga;

//motor speeds

byte motor1_speed = 128;
byte motor2_speed = 128;



void go(int spd1, int spd2)
{
  if(spd1>=0)
  {
    digitalWrite(13,LOW);
    digitalWrite(4,HIGH);
    if(spd2>=0)
    {
     digitalWrite(7,LOW);
     digitalWrite(8,HIGH);
     analogWrite(PWM1 , spd1);
     analogWrite(PWM2 , spd2);
    }
    else
    {
      digitalWrite(7,HIGH);
      digitalWrite(8,LOW);
      analogWrite(PWM1 , spd1);
      analogWrite(PWM2 , -spd2);
    }
  }
  else
  {
    digitalWrite(13,HIGH);
    digitalWrite(4,LOW);
    if(spd2>=0)
    {
     digitalWrite(7,LOW);
     digitalWrite(8,HIGH);
     analogWrite(PWM1 , -spd1);
     analogWrite(PWM2 , spd2);
    }
     else
     {
      digitalWrite(7,HIGH);
       digitalWrite(8,LOW);
       analogWrite(PWM1 , -spd1);
       analogWrite(PWM2 , -spd2);
     }
    }
  }

void setup() {
  pinMode(TRIG_PIN , OUTPUT);
  pinMode(ECHO_PIN , INPUT);
  pinMode(3,OUTPUT);      //pwm motor1
  pinMode(11,OUTPUT);      //pwm motor2
  pinMode(13,OUTPUT);            //for motor1
  pinMode(4,OUTPUT);      //for motor1
  pinMode(7,OUTPUT);      //for motor2
  pinMode(8,OUTPUT);      //for motor2
 
 // Serial.begin(9600);
}

int getDistance(){
  digitalWrite(TRIG_PIN , LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN , HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN , LOW);
  duration = pulseIn(ECHO_PIN , HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

bool searchLeftandRight(int pin){
    srv.attach(pin);
    srv.write(0);
    delay(1000);
    dreapta = getDistance();

    srv.write(180);
    delay(1000);  

    stanga = getDistance();
            
  // revine la pozitia initiala
    srv.write(85);
    delay(1000);
    
    srv.detach();

   if(stanga > dreapta)return true; //Trebuie mers in stanga
   return false;   //Trebuie mers in dreapta
}

void loop() {
 if(getDistance() <= 25){ //25 cm
        go(0,0);
        delay(500);
        if(searchLeftandRight(6)){
          //Trebuie sa se intoarca spre stanga
           // Serial.println("==== STANGA =====");
            go(130,-130);
             delay(500);
            go(0,0);
            delay(100);
           
        } else {
          //Trebuie sa se intoarca spre dreapta
          // Serial.println("==== DREAPTA =====");
          go(-130,130);
         
            delay(600);
             go(0,0);
             delay(100);
        }
         
   } else {
        //Merge in fata pana intalneste obstacol
         go(130,130);
        
   }
 /* go(130,130);

  
  delay(3000);

    irrecv.enableIRIn();
 if (irrecv.decode(&results)){
 // Serial.println(results.value, HEX);
        if(autonomous){
          
            if(results.value == 0xFF30CF){
                go(0,0);
                //delay(2000);
              //  Serial.println(autonomous);
                autonomous = false;
             //    Serial.println(autonomous);
            } 
       } else {
             if(results.value == 0xFF7A85){
             // Serial.println(autonomous);
              autonomous = true;
              // Serial.println(autonomous);
              
              
            }
              
       }
     irrecv.resume();
 
//detachInterrupt(irparams.irrecv);
/*
 if(autonomous){
        if(getDistance() <= 25){ //25 cm
            
           go(0,0);
            if(searchLeftandRight(SERVO_PIN)){
              //Trebuie sa se intoarca spre stanga
               // Serial.println("==== STANGA =====");
                go(-motor1_speed,motor2_speed);
                 delay(500);
                go(0,0);
                delay(500);
            } else {
              //Trebuie sa se intoarca spre dreapta
              // Serial.println("==== DREAPTA =====");
               go(motor1_speed,-motor2_speed);
             
                delay(500);
                 go(0,0);
                 delay(500);
            }
       } else {
            //Merge in fata pana intalneste obstacol
             go(motor1_speed , motor2_speed);
            
       } 
       
   }

  */
  //irrecv.resume();

}



