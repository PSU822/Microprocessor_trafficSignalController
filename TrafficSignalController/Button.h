#ifndef BUTTON_H
#define BUTTON_H

#define WEST_BUTTON_PIN 54 // 서쪽 방향 버튼 pb 1
#define SOUTH_BUTTON_PIN 55 // 남쪽 방향 버튼 pb 2
#define EAST_BUTTON_PIN 56 // 동쪽 방향 버튼 pb 3
#define NORTH_BUTTON_PIN 57 // 북쪽 방향 버튼 pb 4


// 메인 코드에서 참조될 변수들

extern volatile boolean g_bWestExtend;
extern volatile boolean g_bSouthExtend;
extern volatile boolean g_bEastExtend;
extern volatile boolean g_bNorthExtend;

// ▶ 버튼 핀 초기화 함수
void buttonInit() {
  pinMode(WEST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SOUTH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EAST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NORTH_BUTTON_PIN, INPUT_PULLUP);
}

// ▶ 버튼 상태 체크 함수 (디바운싱 구현)
void checkButton() {
  // 서쪽 버튼
  static int westPrev = HIGH;
  static unsigned long westLast = 0;
  int westCurrent = digitalRead(WEST_BUTTON_PIN);
  if (westCurrent != westPrev) westLast = millis();
  if (millis() - westLast > 50) {
    if (westCurrent == LOW && westPrev == HIGH) g_bWestExtend = true;
    westPrev = westCurrent;
  }

  // 남쪽 버튼
  static int southPrev = HIGH;
  static unsigned long southLast = 0;
  int southCurrent = digitalRead(SOUTH_BUTTON_PIN);
  if (southCurrent != southPrev) southLast = millis();
  if (millis() - southLast > 50) {
    if (southCurrent == LOW && southPrev == HIGH) g_bSouthExtend = true;
    southPrev = southCurrent;
  }

  // 동쪽 버튼
  static int eastPrev = HIGH;
  static unsigned long eastLast = 0;
  int eastCurrent = digitalRead(EAST_BUTTON_PIN);
  if (eastCurrent != eastPrev) eastLast = millis();
  if (millis() - eastLast > 50) {
    if (eastCurrent == LOW && eastPrev == HIGH) g_bEastExtend = true;
    eastPrev = eastCurrent;
  }

  // 북쪽 버튼
  static int northPrev = HIGH;
  static unsigned long northLast = 0;
  int northCurrent = digitalRead(NORTH_BUTTON_PIN);
  if (northCurrent != northPrev) northLast = millis();
  if (millis() - northLast > 50) {
    if (northCurrent == LOW && northPrev == HIGH) g_bNorthExtend = true;
    northPrev = northCurrent;
  }
}

#endif
