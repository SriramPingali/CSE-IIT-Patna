#define segA 2
#define segB 3
#define segC 4
#define segD 5
#define segE 6
#define segF 7
#define segG 8

 
int count =0;
 
void setup() {
  int x;
  // put your setup code here, to run once:
 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(7,OUTPUT);
 pinMode(8,OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (count)
   {
    case 0:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,LOW);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,HIGH);

      break;


case 1:
      digitalWrite(segA,HIGH);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,HIGH);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,HIGH);
      digitalWrite(segG,HIGH);

      break;

case 2:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,HIGH);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,LOW);      
      digitalWrite(segF,HIGH);
      digitalWrite(segG,LOW);

      break;

case 3:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,HIGH);
      digitalWrite(segG,LOW);

      break;

case 4:
      digitalWrite(segA,HIGH);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,HIGH);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,LOW);

      break;

case 5:
      digitalWrite(segA,LOW);
      digitalWrite(segB,HIGH); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,LOW);

      break;

case 6:
      digitalWrite(segA,LOW);
      digitalWrite(segB,HIGH); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,LOW);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,LOW);

      break;

case 7:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,HIGH);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,HIGH);
      digitalWrite(segG,HIGH);

      break;

case 8:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,LOW);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,LOW);

      break;

case 9:
      digitalWrite(segA,LOW);
      digitalWrite(segB,LOW); 
      digitalWrite(segC,LOW);     
      digitalWrite(segD,LOW);
      digitalWrite(segE,HIGH);      
      digitalWrite(segF,LOW);
      digitalWrite(segG,LOW);

      break;
    }
    n = (n+1)%10;
    delay(1000)
}      
