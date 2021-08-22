// 아두이노 우노 보드의 하드웨어 시리얼 포트(0, 1)를 사용합니다. 
//nano 33 ble sense -> RGB 22, 23, 24
const int red = 22;     //13
const int blue = 23;    //12
const int green = 24;   //11


void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);

  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
  digitalWrite(green, HIGH);
}
//nano 33 ble sense > LOW가 키는것.
void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    
    if (input == 'a') {
      digitalWrite(red, LOW);
      delay(500);
      digitalWrite(red, HIGH);
    } else if (input == 'b') {
      digitalWrite(blue, LOW);
      delay(500);
      digitalWrite(blue, HIGH);
    } else if (input == 'c') {
      digitalWrite(green, LOW);
      delay(500);
      digitalWrite(green, HIGH);
    }
  }
}
