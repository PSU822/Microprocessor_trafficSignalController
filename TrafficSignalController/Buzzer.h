
#define BUZZER_PIN 47        

// 부저 초기화
void buzzerInit(){
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW); 
    
};

// 버튼 눌림시 피드백 부저 소리
void buzzerClick(){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);  // 1초간 울림         /millis() 너무길면 이걸로 변경
    digitalWrite(BUZZER_PIN, LOW);
};

// passive buzzer시
// void buzzerClick() {
//     tone(BUZZER_PIN, 1000, 100);
//     delay(120); 
//     noTone(BUZZER_PIN);
// }


// 보행 보조용 부저 소리
void buzzerWalkSignal(){
    for (int i = 0; i < 5; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);
    }
};

// passive buzzer시g
// void buzzerWalkSignal() {
//     // 짧게 두 번 삑삑 (800Hz, 80ms x 2)
//     for (int i = 0; i < 2; i++) {
//         tone(BUZZER_PIN, 800, 80);
//         delay(100);
//         noTone(BUZZER_PIN);
//         delay(60); // 삑 사이 간격
//     }
// }


// 신호 변경시 부저 소리
void buzzerChangeSignal(){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);  // 1초간 울림
    digitalWrite(BUZZER_PIN, LOW);
};

//passive buzzer 일때 
// void buzzerChangeSignal() {
//     // 길게 삑 (1500Hz, 300ms)
//     tone(BUZZER_PIN, 1500, 300);
//     delay(320);
//     noTone(BUZZER_PIN);
// }

