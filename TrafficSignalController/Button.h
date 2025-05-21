#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PIN 2 // 버튼 pb 1

extern volatile boolean g_bExtendSignal;


void buttonISR() {
  static unsigned long last_time = 0;
  unsigned long now = millis();

  if (now - last_time > 500) {
    g_bExtendSignal = true;
    Serial.println("인터럽트 발생 - 신호 연장");
  }
  
  last_time = now;
}

// ▶ 버튼 핀 초기화 함수
void buttonInit() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void checkButton(){
  if (digitalRead(BUTTON_PIN) == LOW) {
    g_bExtendSignal = true;
    Serial.println("디버그용 : 버튼 인터럽트 발생 - 신호 연장");
  }
}

#endif
