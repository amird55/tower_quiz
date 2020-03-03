#include <NewPing.h>

#define TRIGGER_PIN  D2  
#define ECHO_PIN     D3  

#define RED_PIN   D5
#define GREEN_PIN D6
#define BTN_PIN   D7
#define RELAY_PIN D0

#define MAX_DISTANCE 60
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int gameStatus;
#define READY_TO_START  11
#define GAME_IS_ON      12
#define FINISHED        13

void startNewGame(){
  gameStatus=READY_TO_START;
  bool t=makeServerReadyForNewGame();
}
void setup() {
  pinMode(BTN_PIN,INPUT_PULLUP);
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(RELAY_PIN,OUTPUT);

//  digitalWrite(ECHO_PIN, HIGH);

  Serial.begin(9600);
  Serial.println("started");
  wifiSetup();
  startNewGame();
}
bool btnPressed=false;
int btn_val;
void loop() {
  btn_val=digitalRead(BTN_PIN);
  if(btn_val == HIGH){
    btnPressed=false;
  }
  switch(gameStatus){
    case READY_TO_START:waitingHandler();break;
    case GAME_IS_ON:gameHandler();break;
    case FINISHED:finishedHandler();break;
  }
  delay(100);
}
void unlockBox(){
  digitalWrite(RELAY_PIN,HIGH);
  delay(4000);  
  digitalWrite(RELAY_PIN,LOW);
}
void lightRed(){
  digitalWrite(RED_PIN,HIGH);
  digitalWrite(GREEN_PIN,LOW);
}
void lightGreen(){
  digitalWrite(RED_PIN,LOW);
  digitalWrite(GREEN_PIN,HIGH);
}
void lightBoth(){
  digitalWrite(RED_PIN,HIGH);
  digitalWrite(GREEN_PIN,HIGH);
}
void lightOFF(){
  digitalWrite(RED_PIN,LOW);
  digitalWrite(GREEN_PIN,LOW);
}
void waitingHandler() {
  lightBoth();
  if((!btnPressed)&&(btn_val == LOW)){
    btnPressed=true;
    bool t=makeServerStartNewGame();
    gameStatus=GAME_IS_ON;
    lightOFF();
  }
}
void finishedHandler() {
  if((millis()/1000)%2 == 1){
    lightGreen();
  } else {
    lightRed();
  }
  if((!btnPressed)&&(btn_val == LOW)){
    btnPressed=true;
    bool t=makeServerReadyForNewGame();
    gameStatus=READY_TO_START;
  }
}
void gameHandler() {
  int srvr_rslt;
  short user_ans;
  if((!btnPressed)&&(btn_val == LOW)){
    btnPressed=true;
    user_ans=getAnswer();
    Serial.print("getAnswer=");
    Serial.println(user_ans);
    srvr_rslt=sendAnsToServer(user_ans);
    switch(srvr_rslt){
      case 1:lightGreen();delay(700);lightOFF();break;
      case 2:lightRed();  delay(700);lightOFF();break;
      case 3:gameStatus=FINISHED;break;
      case 4:
            gameStatus=FINISHED;
            bool t=makeServerFinishGame();
            unlockBox();  
            break;
    }
  }
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
