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


#include <IRremote.h>

const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
bool autonomous=true;


void go(int n, int m){}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();

}

void loop() {
  // put your main code here, to run repeatedly:
/*  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
         {// results.value = key_value, keep doing the same thing
          Serial.println("keep doing the same");
          }
        switch(results.value){
          case 0xFF30CF:
          {
          Serial.println("1");
          autonomous = true;
           Serial.println("autonomous = true");
          break ;
          }
          case 0xFF7A85:
          {
          Serial.println("3");
          autonomous = false;
           Serial.println("autonomous = false");
          break ;
          }
        }

        if(!autonomous){
        switch(results.value){
         
          case 0xFF18E7:
          {
          Serial.println("2");
          go(130,130);
           Serial.println("forward");
          break ;
          }
          case 0xFF10EF:
          {
          Serial.println("4");
          Serial.println("left");
          go(-130, 130);
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF38C7:
          {
          Serial.println("5");
             Serial.println("stop");
          go(0,0);
          delay(500);
          break ;
          }
          case 0xFF5AA5:
          {
          Serial.println("6");
             Serial.println("right");
          go(130, -130);
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          {
          Serial.println("8");
          go(-130, -130);
          Serial.println("backwards");
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
       }
      else
      {// there is signal from the remote, but the robot is autonomous, so the other piece of code is executed;
           Serial.println("there is signal, autonomous, the other piece of code is exec");
        }
       
        key_value = results.value;
        irrecv.resume(); 
  }

  else{
       Serial.println("the other piece of code is exec");
    //no signal received from the remote, so the robot stops if autonomous == false. Also, if autonomous == true, the other piece of code will be executed
    if(!autonomous){   Serial.println("stop"); go(0,0); delay(1000);     Serial.println("if aut = false, robot stops");}
    }
*/

if (irrecv.decode(&results)){

  if(autonomous)
  {
    if(results.value == 0xFF7A85)
       {
        
       Serial.println("3");
        Serial.println("stop");
        go(0,0);
        delay(500);
       autonomous = false;
       Serial.println("autonomous = false");}
  }
  else
  if (results.value == 0XFFFFFFFF){
    Serial.println("do the same");
  
  }

  else
  {  Serial.println("stop");
    go(0,0);
    delay(500);
     switch(results.value){
         case 0xFF30CF:
          {
          Serial.println("1");
          autonomous = true;
           Serial.println("autonomous = true");
          break ;
          }
           case 0xFF18E7:
          {
          Serial.println("2");
          go(130,130);
           Serial.println("forward");
          break ;
          }
          case 0xFF10EF:
          {
          Serial.println("4");
          Serial.println("left");
          go(-130, 130);
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF38C7:
          {
          Serial.println("5");
             Serial.println("stop");
          go(0,0);
          delay(500);
          break ;
          }
          case 0xFF5AA5:
          {
          Serial.println("6");
             Serial.println("right");
          go(130, -130);
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          {
          Serial.println("8");
          go(-130, -130);
          Serial.println("backwards");
          delay(800);
          go(0,0);
          delay(800);
          go(130,130);
          break ;
          }
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
    }
   key_value = results.value;
        irrecv.resume();}
delay(300);
}


