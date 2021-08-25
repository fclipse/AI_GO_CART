const int but1 = 28;
const int but2 = 29;

int var1 = 0;
int var2 = 0;
int var3 = 0;

// 스텝 모터 제어
const int enA = 10;  // 구동 여부 결정, 10
const int stepPin = 11; // 스텝 펄스, 11
const int dirPinLR = 12;  // 좌우 회전, 12
const int rst = 5; //리셋, LOW 상태로 유지함, 5

const int STEPS_PER_REV = 1200; // 400에 모터 2회전, 점퍼는 off-on-off로 세팅함(200pulse/rev)
//rotateLimit 설정 ---------------------------------------------------------------
int rotatePos = 10;
int rotateMid = 10;
const int rotateLeftLimit = 8;       
const int rotateRightLimit = 12;      //right가 left보다 더 많이 돎
const int rotate_L = 1;
const int rotate_S = 1;

const int rotate = 1;

// 드라이브 모터 제어
//계속 redeclared 오류남
const int PWR = 8; // 파워
const int PWM = 9; // 신호 1
int velocity = 30;

// 페달 제어_전진, 후진 스위치 센싱
int pedalF = 6;
boolean pedalFVal = 0;

int pedalB = 7;
boolean pedalBVal = 0;

const int ground = 4;

// 페달 제어_페달 센싱
const int pedalSensor = A0;
int pedalVal = 0;

// 수동 모드, 앱 제어모드 변경, 0은 앱제어모드, 1은 수동 모드
boolean modeState = 1;

// 속도값 초기화
int i = 0;
char state = 'S';
char SetMode = 'q';
char Mode = 'q';
char lol = 'a';
// 입력 문자, 입력 문자 백업
//----------------------------------------------변수들
void forward(){
  //드라이브 모터가 앞으로 회전하도록 신호부여
  digitalWrite(PWR,LOW); 
  analogWrite(PWM, i);

  if (i != velocity) {
    for (i = 0; i < velocity; i = i + 10) {
      analogWrite(PWM, i);
      delay(50);
    }
  }
  //정렬 코드
  if (rotateMid > rotatePos) {
    int j = rotateMid - rotatePos;
    for (int k = 0; k < j; k++) {
      right();
    }
    //right();
    //left();
  } 
  else if (rotateMid < rotatePos) {
    int j = rotatePos - rotateMid;
    for (int k = 0; k < j; k++) {
      left();
    }
    //left();
    //right();
  }
}

void backward(){
  ////드라이브 모터가 뒤로 회전하도록 신호부여
  digitalWrite(PWR,HIGH);
  analogWrite(PWM, i);
  
  if(i != velocity) {
    for (i = 0; i < velocity; i = i + 10) {
      analogWrite(PWM, i);
      delay(100);
    }
  }
}

void left() {
  // 조향 모터가 '반시계방향'으로 회전하도록 신호부여
  digitalWrite(dirPinLR,HIGH); 
  
  if (rotatePos > rotateLeftLimit) {
    // 1000마이크로초 주기로 모터 축이 1.5회전하는 코드
    // 1:50 기어박스 내장되어 있으므로, 모터 1회전에 바퀴 7.2도 회전함
    // 따라서, 모터가 1.5회전하면 바퀴가 10.8도 회전함
    for(int x = 0; x < STEPS_PER_REV*rotate_L; x++) {//5번, right보다 1 많게
      digitalWrite(enA,HIGH);
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500); 
    }
    rotatePos = rotatePos - 1;
  } else {
    rotatePos = rotateLeftLimit;
  }
  //Serial.println(rotatePos);
}
void right() {
  // 조향 모터가 '시계방향'으로 회전하도록 신호부여
  digitalWrite(dirPinLR,LOW); 
  
  if (rotatePos < rotateRightLimit) {
    for(int x = 0; x < STEPS_PER_REV*rotate_L; x++) {
      digitalWrite(enA,HIGH);
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500); 
    }
    rotatePos = rotatePos + 1;
  } else {
    rotatePos = rotateRightLimit;
  }
}
void motorStop(){
  
  digitalWrite(PWR,LOW);
  analogWrite(PWM, 0);
  //delay(100);
}
//-----------------setSerial------------------
/*
void setSerial(){
  if(Serial.available()){
    //cmd = Serial.read();
  }
}
*/
//------------------setMode--------------------
/*    //구버전 serial input
void setMode(){
  // 아두이노 메가를 쓸 때는 Serial3를 그대로 사용하고, 아두이노 우노를 쓸 때는 Serial3를 mySerial로 수정해주세요. 
  // 아두이노 메가를 쓸 때는 SW6 스위치를 3번쪽으로 옮기고, 아두이노 우노를 쓸 때는 SW6 스위치를 1번쪽으로 옮겨주세요.
  if (Serial.available()){        // 블루투스 통신에 데이터가 있을 경우
    SetMode = Serial.read();     // 블루투스의 데이터(문자 한 글자)를 '' 변수에 저장
    // cmd 변수의 데이터가 m이면 수동모드로, i면 앱모드로 modeState 변수의 상태를 바꿈
    switch(SetMode){
      case 'q':
        modeState = 1;
        break;
      case 'e':
        modeState = 0;
        break;
     }
  }
}
*/
int var;
//var1 = 0;
//modeState 변경, 자율주행/수동조종 제어, 28번핀 사용
void BUTTON1(){
  var = digitalRead(but1);
  if(var - var1 == 1){
    modeState = 1 - modeState;
  }
  var1 = var;
}

void setup() {
  //모니터링을 위한 시리얼 통신 설정
  Serial.begin(9600); // 시리얼 통신
  //Serial3는 아두이노 메가를 사용할 때 사용하는 시리얼입니다. 아두이노 우노를 사용하면 주석처리 하세요.
  Serial3.begin(9600);     
  //버튼 입력 설정
  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  
  // 스텝모터 핀 모드 설정
  pinMode(dirPinLR,OUTPUT);
  pinMode(stepPin,OUTPUT); 
  pinMode(enA, OUTPUT);
  digitalWrite(enA, HIGH);
  pinMode(rst, OUTPUT);
  digitalWrite(rst, LOW);

  // 드라이브모터 핀 모드 설정
  pinMode(PWM,OUTPUT); 
  pinMode(PWR, OUTPUT);
         
  // 페달모드 전진 후진 신호
  pinMode(pedalF, INPUT_PULLUP);
  pinMode(pedalB, INPUT_PULLUP);
  pinMode(ground, OUTPUT);
  digitalWrite(ground, LOW);

  Serial.println("AI Go-Kart is Ready!");
  //Serial3.println("Bluetooth Connection Check");
}
//---------------------------------------------함수선언

void loop() {
  
  while(modeState == 1) {
    //setMode();
    BUTTON1();
    // 전진, 후진 스위치 값 저장
    pedalFVal = digitalRead(pedalF);
    pedalBVal = digitalRead(pedalB);

    // 페달값 센싱-매핑-한계범위설정
    pedalVal = analogRead(pedalSensor);
    pedalVal = map(pedalVal, 230, 850, 0, 255);
    pedalVal = constrain(pedalVal, 0, 255);

    // 페달 신호가 0이면 브레이킹
    if (pedalVal == 0) {
      digitalWrite(PWR,LOW);   
      analogWrite(PWM, 0);
      delay(100);
    }

    // 전진, 후진 스위치 값에 따른 페달 동작, 이상했음
    if (pedalFVal == 1 && pedalBVal == 1) {
      digitalWrite(PWR, HIGH); 
      analogWrite(PWM, pedalVal);
      //Serial.println("RRRR");
    } else if (pedalFVal == 1 && pedalBVal == 0) {
      digitalWrite(PWR,LOW);
      analogWrite(PWM, pedalVal);
      //Serial.println("FFFF");
    } else {
      digitalWrite(PWR, LOW);
      analogWrite(PWM, 0);
      //Serial.println("SSSS");
    }
  }
  //modestate가 0이면 자동운전 모드로 수행 : e
  while(modeState == 0){
    //setMode();
    BUTTON1();
    if(Serial.available()){
      Mode = Serial.read();
    }
    if(Mode == 'W'){
      forward();
    }
    else if(Mode == 'S'){
      motorStop();
    }
    else if(Mode == 'A'){
      //반대로 해줘야 제대로 감
      right();
      right();
      //delay(500);
      //딜레이 제거 고려 , 0823
    }
    else if(Mode == 'D'){
      //반대로 해줘야 제대로 감    
      left();
      left();
      //delay(500);
    }
    else if(Mode == 'X')
      backward();
    else if(Mode == 'o'){
      velocity -= 30;
      if(velocity < 30)
        velocity = 30;
    }
    else if(Mode == 'p'){
      velocity += 30;
      if(velocity > 250)
        velocity = 250;
    }
    //Serial.println(rotatePos);
  }
}
