#ifndef SEG7_H
#define SEG7_H

#include <Arduino.h>

// 7-세그먼트 핀 정의
static const byte seg7_sel[6] = {8, 9 , 10, 11, 12, 13}; // 선택 핀
static const byte seg7_dp[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // 데이터 핀

// 0~9, A~F 패턴
static const byte seg7_data[16] = {
    0x3F, 0x06, 0x5B, 0x4F, // 0, 1, 2, 3
    0x66, 0x6D, 0x7D, 0x07, // 4, 5, 6, 7
    0x7F, 0x67, 0x77, 0x7C, // 8, 9, A, B
    0x39, 0x5E, 0x79, 0x71  // C, D, E, F
};

// 세븐 세그먼트 초기화. HIGH: 꺼짐, LOW: 켜짐
inline void sevenSegmentInit() {
    // seg7_sel을 출력 모드로 설정 (HIGH로 초기화)
    for (int i = 0; i < 6; i++) {
        pinMode(seg7_sel[i], OUTPUT);
        digitalWrite(seg7_sel[i], HIGH);
    }

    // seg7_dp를 출력 모드로 설정
    for (int i = 0; i < 8; i++) {
        pinMode(seg7_dp[i], OUTPUT);
    }
}

// 한 자리(pos)에 pattern를 표시하는 함수
inline void FndDisplay(int pos, byte pattern) {
    // 먼저 모든 자리 비활성화
    for (int i = 0; i < 6; i++) {
        digitalWrite(seg7_sel[i], HIGH);
    }
    // 선택된 자리만 LOW로 활성화
    digitalWrite(seg7_sel[pos], LOW);

    // 데이터 핀에 패턴 전송
    for (int bit = 0; bit < 8; bit++) {
        // digitalWrite(seg7_dp[bit], bitRead(pattern, bit));
        digitalWrite(seg7_dp[bit], (pattern >> bit) & 0x01);
    }
}

// 남은 시간을 오른쪽 두 자리에 표시
// time은 두 자리 정수로 가정
inline void displayTime(int time) {
    // digits[0]는 10의 자리, digits[1]은 1의 자리
    int digits[2] = { (time / 10) % 10, time % 10 };

    // 맨 오른쪽 두 자리 표시 (4, 5번 자리)
    for (int idx = 0; idx < 2; idx++) {
        FndDisplay(idx + 4, seg7_data[digits[idx]]);
        delayMicroseconds(1000); // 1ms 대기: 멀티플렉싱 안정화
    }

    // 나머지 자리(0~3)는 꺼짐
    for (int idx = 0; idx < 4; idx++) {
        FndDisplay(idx, 0x00);
        delayMicroseconds(500); // 0.5ms 대기
    }
}

#endif // SEG7_H
