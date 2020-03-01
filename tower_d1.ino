#include <NewPing.h>

#define TRIGGER_PIN  D2  
#define ECHO_PIN     D3  

#define RED_PIN   D5
#define GREEN_PIN D6
#define BTN_PIN   D7
#define RELAY_PIN D0

#define MAX_DISTANCE 60
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  pinMode(BTN_PIN,INPUT_PULLUP);
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(RELAY_PIN,OUTPUT);

//  digitalWrite(ECHO_PIN, HIGH);

Serial.begin(9600);
Serial.println("started");
}
bool btnPressed=false;
void loop() {
  short user_ans;
  int btn_val=digitalRead(BTN_PIN);
  if((!btnPressed)&&(btn_val == LOW)){
    btnPressed=true;
    user_ans=getAnswer();
    Serial.print("getAnswer=");
    Serial.println(user_ans);
  }
  else if(btn_val == HIGH){
    btnPressed=false;
  }
  delay(100);
}
short getAnswer(){
  int dist=sonar.ping_cm();
  short ans;
  if(dist<=0){
    ans=127;
  }else {
    ans=floor(dist/8);
  }
  return ans;
}
