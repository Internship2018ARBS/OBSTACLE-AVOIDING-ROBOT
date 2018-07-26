#include <Servo.h>
#include <stdlib.h>
#include <IRremote.h>                 // Library used together with the remote and the ky-022 infrared receiver module

/* 
 *  CODES RECEIVED WHEN PRESSING CERTAIN KEYS ON THE REMOTE
key           code

PWR           FF629D     
CH            FFE21D        
|<<           FF22DD  
>|            FFC23D              
>>|           FF02FD               
-             FFE01F 
Plus          FFA857         
EQ            FF906F          
0             FF6897            
100           FF9867           
200           FFB04F           
1             FF30CF                 
2             FF18E7                 
3             FF7A85                  
4             FF10EF                   
5             FF38C7                    
6             FF5AA5                  
7             FF42BD                  
8             FF4AB5             
9             FF52AD 
doublePress       FFFFFFFF
*/

#define RECV_PIN 2                     // Receiver pin for remote
IRrecv irrecv(RECV_PIN);               // Initialising the IRrecv variable with the receive pin
decode_results results;                // Initialise a variable for decoding the codes received from the remote
unsigned long key_value = 0;           // The pressed key
volatile bool autonomous=true;         // Robot state

Servo srv;

#define TRIG_PIN 12
#define ECHO_PIN 10
#define PWM1 5                         // for motor1
#define PWM2 6                         // for motor2
#define M1 7                           // for motor1
#define M2 8                           // for motor1
#define M3 13                          // for motor2
#define M4 4                           // for motor2
#define IR_SENSOR_1 A0
#define IR_SENSOR_2 A5
#define IR_SENSOR_3 9

#define BUZZER_PIN A3

#define GREEN_LED A2
#define GREEN_LED_DELAY 250
bool GREEN_LED_STATE = true;
bool BUZZER_STATE = false;
unsigned long GREEN_LED_TIMER = 0;




unsigned long BUZZER_TIMER = 0;
unsigned long BUZZER_DELAY = 250;
unsigned long currentMillis;



int clock= A1;                  //IC pin 11 (yellow)
int latch=11;                   //IC pin 12 (green) 
int data=A4;                    //IC pin 14 (blue)

byte pattern[]=
{  
B00000010,   
B00000110,  
B00001110,
B00011110, 
B00111110,
B01111110,  
}; 

void lights(int i)
{
  digitalWrite(latch,LOW);   
  shiftOut(data,clock,MSBFIRST,pattern[i]);
  digitalWrite(latch,HIGH);   
}


/*
 *   The buzzer buzzes 250 miliseconds after that it does not buzz for another 250 miliseconds
 */
void buzzer_buzz()
{
   // -99999 value for BUZZER_DELAY to not buzz
  if(BUZZER_DELAY != -99999)
  {
    currentMillis = millis();
	// checking if an amount of time has passed : BUZZER_DELAY which is 250 miliseconds
    //BUZZER_TIMER is initialy 0
    if(currentMillis - BUZZER_TIMER >= BUZZER_DELAY)
    {
            if(BUZZER_STATE)
            {
                 digitalWrite(BUZZER_PIN , LOW);     // not BUZZ
                 BUZZER_STATE = false;
            }  
            else 
            {
              digitalWrite(BUZZER_PIN , HIGH);      // BUZZ
                BUZZER_STATE = true;
            }
            BUZZER_TIMER = currentMillis;
      }
    } 
    else 
    {
      digitalWrite(BUZZER_PIN , LOW);
    }
}


/*
 * The led is on for 250 miliseconds after that is off for 250 miliseconds
 */
void blink_green_led()
{
  currentMillis = millis();
   //  GREEN_LED_DELAY is 250 miliseconds
  if(currentMillis - GREEN_LED_TIMER >= GREEN_LED_DELAY)
  {
          if(GREEN_LED_STATE){
            analogWrite(GREEN_LED , 255);
            GREEN_LED_STATE = false;
          } else {
            GREEN_LED_STATE = true;
            analogWrite(GREEN_LED , 0);
          }
          GREEN_LED_TIMER = currentMillis;
    }
}

#define SERVO_PIN 3

// pentru sonar getDistance()

long duration;
int distance;



// for servo searchLeftandRight(const int pin)
unsigned int  dreapta , stanga , stanga_135 , dreapta_45;


//motor speeds
const short  motor1_speed = 120;            //90
const short motor2_speed = 100;             //70

const short  motor1_speed_turn = 120;       //90
const short motor2_speed_turn = 100;        //70

void go(const short spd1, const short spd2)
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


void setup() 
{
  pinMode(TRIG_PIN , OUTPUT);
  pinMode(ECHO_PIN , INPUT);
  pinMode(PWM1,OUTPUT);                 //pwm motor1
  pinMode(PWM2,OUTPUT);                 //pwm motor2
  pinMode(M1,OUTPUT);                   //for motor1
  pinMode(M2,OUTPUT);                   //for motor1
  pinMode(M3,OUTPUT);                   //for motor2
  pinMode(M4,OUTPUT);                   //for motor2
  pinMode(IR_SENSOR_1,INPUT_PULLUP);
  pinMode(IR_SENSOR_2,INPUT_PULLUP);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(clock,OUTPUT);   
  pinMode(latch,OUTPUT);   
  pinMode(data,OUTPUT); 
  pinMode(IR_SENSOR_3 , INPUT_PULLUP);
  
  irrecv.enableIRIn();                  // REMOTE initializing pin

  srv.attach(SERVO_PIN);
  srv.write(85);
  srv.detach();
  delay(500);

  //Serial.begin(9600);

}



int getDistance()
{
  digitalWrite(TRIG_PIN , LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN , HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN , LOW);
  duration = pulseIn(ECHO_PIN , HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}



byte  searchLeftandRight(int pin)
{
    digitalWrite(BUZZER_PIN , LOW);
    srv.attach(pin);
    
    srv.write(0);
    delay(350);
    dreapta = getDistance();
   

    
    srv.write(45);
    delay(350);
    dreapta_45 = getDistance();
    
    
    srv.write(135);
    delay(350);
    stanga_135 = getDistance();
    
    
    srv.write(180);
    delay(350);  
    stanga = getDistance();
     

    srv.write(85);
    delay(350);
    
    srv.detach();

     if(stanga_135 >= dreapta_45 && stanga_135 >= stanga && stanga_135 >= dreapta)
     {
       // left 135
          return 2;
      }

       if(dreapta_45 >= stanga_135 && dreapta_45 >= stanga && dreapta_45 >= dreapta)
       {
       // right 45
          return 3;
      }

      if(dreapta >= dreapta_45 && dreapta >= stanga_135 && dreapta >= stanga)
      {
      // right
          return 0;
      }

      if(stanga >= dreapta_45 && stanga >= stanga_135 && stanga >= dreapta)
      {
      // left
          return 1;
      }
}

unsigned long distance_from_obstacle;
byte rotation_degrees;
short switch_direction_sensor_3 = -1;

void autonomousRoutine()
{
      distance_from_obstacle = getDistance();
    
     if(distance_from_obstacle > 100)
              lights(0);
      else 
      if(distance_from_obstacle > 80 && distance_from_obstacle <= 100)
              lights(1);
      else 
      if(distance_from_obstacle > 60 && distance_from_obstacle <= 80)
              lights(2);
      else 
      if(distance_from_obstacle > 45 && distance_from_obstacle <= 60)
              lights(3);
      else 
      if(distance_from_obstacle > 37 && distance_from_obstacle <= 45)
              lights(4);
      else 
      if(distance_from_obstacle <= 37)
              lights(5);

      if(distance_from_obstacle > 10 &&  digitalRead(IR_SENSOR_1) == LOW && digitalRead(IR_SENSOR_2) == HIGH ) 
      {
			// if left sensor detects an obstacle the robot stops for 500 miliseconds after that it turns to right for 400 miliseconds
			// after that it stops for 100 miliseconds
            go(0,0);
            delay(500);
            go(motor1_speed_turn,-motor2_speed_turn );
            delay(400);
            go(0,0);
            delay(100);
      } 
      else 
      if(distance_from_obstacle > 10 &&  digitalRead(IR_SENSOR_1) == HIGH && digitalRead(IR_SENSOR_2) == LOW )
      {
		   // if right sensor detects an obstacle the robot stops for 500 miliseconds after that it turns to left for 400 miliseconds
		   // after that it stops for 100 miliseconds
           go(0,0);
           delay(500);
           go(-motor1_speed_turn ,motor2_speed_turn );
           delay(400);
           go(0,0);
           delay(100);
      }
      else 
      if(distance_from_obstacle > 10 && digitalRead(IR_SENSOR_1) == HIGH && digitalRead(IR_SENSOR_2) == HIGH && digitalRead(IR_SENSOR_3) == LOW)
      {
			  // if middle sensor detects an obstacle the robot stops for 500 miliseconds 
             go(0,0);
             delay(500);
             go(-motor1_speed_turn * switch_direction_sensor_3 ,motor2_speed_turn * switch_direction_sensor_3);
             /* 1)Daca senzorul din mojloc vede un obstacol se intoarce spre stanga
              * 2)Daca senzorul din stanga vede un obstacol se intoarce sprea dreapta
              * 3)Daca zenzorul din stanga iara vede un obstacol se va intoarce spre dreapta
              * Adica alterneaza stanga si dreapta
              */
             switch_direction_sensor_3 *= -1;
                delay(400);
                go(0,0);
               delay(100);
      }
      else  
      if(distance_from_obstacle <= 10 || digitalRead(IR_SENSOR_2) == LOW || digitalRead(IR_SENSOR_1) == LOW )
      { //25 cm
      
           digitalWrite(GREEN_LED , LOW);
            go(0,0);
            delay(500);
            // 0 dreapta 1 stanga 2 stanga_135 3 dreapta_45
          rotation_degrees =  searchLeftandRight(SERVO_PIN);
            if(rotation_degrees == 0)
            {
              //Trebuie sa se intoarca spre dreapta
              // Serial.println("==== DREAPTA =====");
              go(motor1_speed_turn,-motor2_speed_turn );
              delay(500);
              go(0,0);
              delay(500);
              
            }
            else 
            if(rotation_degrees == 1)
            {
             //TREBUIE SA MEARGA IN STANGA, dar merge?!
              go(-motor1_speed_turn ,motor2_speed_turn );
              delay(500);
              go(0,0);
              delay(500);
              
                 
            }
            else 
            if (rotation_degrees == 2)
            {
             //STANGA 135
              go(-motor1_speed_turn ,motor2_speed_turn);
              delay(400);
              go(0,0);
              delay(400);
            }
            else 
            {
               //DREAPTA 45
                go(motor1_speed_turn ,-motor2_speed_turn );
                delay(400);
                go(0,0);
                delay(400);
            }
           
       } 
      
       else 
       {
            //Merge in fata pana intalneste obstacol, marvelous
           
             go(motor1_speed,motor2_speed);
            
       }
     
}

void loop() 
{


 if (irrecv.decode(&results))                   // If there are results to be decoded, decode them and do the task according to the received code.
 {

  
 // Serial.println(results.value, HEX);
        if(autonomous)                          // If the robot is autonomous, if the pressed key is 1, the robot becomes non-autonomous, else(if it is non-aut), if key = 3, the robot becomes autonomous.
        {
            if(results.value == 0xFF30CF)
            {
                autonomous = false;
                GREEN_LED_STATE = false;
                analogWrite(GREEN_LED , 0);
            } 
       } 
       else 
       {
            if(results.value == 0xFF7A85)
            {
              autonomous = true;
            } 
            else 
            {
              switch(results.value)             // if the robot is non-autonomous and if:
                         {
                                                        case 0xFF18E7:                  //key = 2, go forward
                                                                        {
                                                                          go(motor1_speed,motor2_speed);
                                                                          break ;
                                                                        }
                                                        case 0xFF10EF:                  //key = 4, turn left  
                                                                      {
                                                                        go(-motor1_speed, motor2_speed);
                                                                        delay(500);
                                                                        go(0,0);
                                                                        break ;
                                                                      }
                                                        case 0xFF38C7:                  //key = 5, stop
                                                                        {
                                                                          go(0,0);
                                                                          break ;
                                                                        }
                                                          case 0xFF5AA5:                 //key = 6, turn right
                                                                        {
                                                                          go(motor1_speed, -motor2_speed);
                                                                          delay(500);
                                                                          go(0,0);
                                                                          break ;
                                                                        }
                                                          case 0xFF4AB5:                  //key = 8, go backwards
                                                                        {
                                                                          go(-motor1_speed, -motor2_speed);
                                                                          break ;
                                                                        }
                          }
                          key_value = results.value;                                       // Save the value of the key pressed.
              }
       }
     irrecv.resume();     //  After receiving, this must be called to reset the receiver and prepare it to receive another code.                 
  } 
  
  if(autonomous)
  {
    autonomousRoutine();
    blink_green_led();
   
  }
  else 
  {
    distance_from_obstacle = getDistance();
        if(distance_from_obstacle < 80)
        {
               BUZZER_DELAY = 250;
               buzzer_buzz();
               if(distance_from_obstacle < 40)
               {
                go(0,0);
               }
        } 
        else 
        {
          BUZZER_DELAY = -99999;
           buzzer_buzz();
        }
  }
 

}
