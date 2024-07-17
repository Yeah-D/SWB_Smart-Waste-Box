#include <Servo.h>

Servo SV;

int trig = 8;
int echo = 9;
int ledPin = 11;            // LED를 연결한 핀
int photoresistorPin = A0;  // 조도센서를 연결한 아날로그 핀
int threshold = 600;        // 조도센서 임계값 설정 (600 이상일 때 어두운 것으로 판단)

bool objectDetected = false;  // 물체 감지 여부를 저장하는 변수
bool lightOn = false;         // 불빛 상태를 저장하는 변수

void setup() {
  SV.attach(10);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledPin, OUTPUT);  // LED 핀을 출력으로 설정
  SV.write(10);             // 서보 모터 초기 위치 설정 (예: 10도)
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = (340 * duration) / 2 / 10000;

  int lightLevel = analogRead(photoresistorPin);

  // 물체를 감지한 경우
  if (distance <= 10) {
    if (lightLevel >= threshold && !lightOn) {
      // 모터 작동 (예: 100도)
      SV.write(100);
      for (int i = 0; i <= 10; i++) {
        analogWrite(ledPin, i * 25);  // 밝기 서서히 증가 (10단계)
        delay(50);                    // 0.5초 동안 유지
      }
      delay(3000);   // 0.5초 동안 유지
      SV.write(10);  // 서보 모터 초기 위치로 되돌리기 (10도)
      for (int i = 10; i >= 0; i--) {
        analogWrite(ledPin, i * 25);  // 밝기 서서히 감소 (10단계)
        delay(15);                    // 0.2초 동안 유지
      }
      analogWrite(ledPin, 0);  // 불빛 완전히 끄기
      objectDetected = true;   // 물체 감지 상태 저장
      lightOn = false;         // 불빛 상태 저장
    } else {
      SV.write(100);
      delay(3000);
      SV.write(10);               // 서보 모터 초기 위치로 되돌리기 (10도)
      digitalWrite(ledPin, LOW);  // 불빛 끄기
      lightOn = false;            // 불빛 상태 저장
      objectDetected = false;     // 물체 감지 상태 저장
    }
  }
}
