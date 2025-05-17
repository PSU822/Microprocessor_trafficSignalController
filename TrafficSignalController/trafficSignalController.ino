// extended from TrafficSignalControl.ino

// 각 팀원별 구현 파트 세분화
#include "Button.h"
#include "Seg7.h"
#include "Buzzer.h"

// 연장값 안전장치용
#define MAX_EXTEND_TIME 120

// 방향 정의
#define WEST    0
#define SOUTH   1
#define EAST    2
#define NORTH   3

// Traffic signal
#define RED_LIGHT         0x01
#define YELLOW_LIGHT      0x02
#define LEFT_GREEN_LIGHT  0x04
#define GREEN_LIGHT       0x08
#define WALK_GREEN_LIGHT  0x10
#define WALK_RED_LIGHT    0x20

// LED 핀 정의 (서쪽 방향)
#define WEST_RED_PIN          14
#define WEST_YELLOW_PIN       15
#define WEST_LEFT_GREEN_PIN   16
#define WEST_GREEN_PIN        17
#define WEST_WALK_RED_PIN     18
#define WEST_WALK_GREEN_PIN   19

// LED 핀 정의 (남쪽 방향)
#define SOUTH_RED_PIN         22
#define SOUTH_YELLOW_PIN      23
#define SOUTH_LEFT_GREEN_PIN  24
#define SOUTH_GREEN_PIN       25
#define SOUTH_WALK_RED_PIN    26
#define SOUTH_WALK_GREEN_PIN  27

// LED 핀 정의 (동쪽 방향)
#define EAST_RED_PIN          30
#define EAST_YELLOW_PIN       31
#define EAST_LEFT_GREEN_PIN   32
#define EAST_GREEN_PIN        33
#define EAST_WALK_RED_PIN     34
#define EAST_WALK_GREEN_PIN   35

// LED 핀 정의 (북쪽 방향)
#define NORTH_RED_PIN         38
#define NORTH_YELLOW_PIN      39
#define NORTH_LEFT_GREEN_PIN  40
#define NORTH_GREEN_PIN       41
#define NORTH_WALK_RED_PIN    42
#define NORTH_WALK_GREEN_PIN  43

// 전역 변수 정의
unsigned char g_ucLED_west_data = RED_LIGHT | WALK_RED_LIGHT;
unsigned char g_ucLED_south_data = RED_LIGHT | WALK_RED_LIGHT;
unsigned char g_ucLED_east_data = RED_LIGHT | WALK_RED_LIGHT;
unsigned char g_ucLED_north_data = RED_LIGHT | WALK_RED_LIGHT;
volatile unsigned int g_unActionCounter = 0;  // 1 second counter
unsigned int g_unSel_dir = 0;  // direction selection

const int g_nBaseTime = 10;     // 기본 신호등 시간
const int g_nExtendTime = 30;   // 버튼 입력 감지시 추가되는 기본 시간
volatile int g_nCurrentExtendTime = 0;  // 연장시간 계산용 함수


volatile boolean g_bWestExtend = false;
volatile boolean g_bSouthExtend = false;
volatile boolean g_bEastExtend = false;
volatile boolean g_bNorthExtend = false;

// LED 초기화 함수
void LED_Init(void)
{
    // 서쪽 방향 LED 핀 설정
    pinMode(WEST_RED_PIN, OUTPUT);
    pinMode(WEST_YELLOW_PIN, OUTPUT);
    pinMode(WEST_LEFT_GREEN_PIN, OUTPUT);
    pinMode(WEST_GREEN_PIN, OUTPUT);
    pinMode(WEST_WALK_RED_PIN, OUTPUT);
    pinMode(WEST_WALK_GREEN_PIN, OUTPUT);
    
    // 남쪽 방향 LED 핀 설정
    pinMode(SOUTH_RED_PIN, OUTPUT);
    pinMode(SOUTH_YELLOW_PIN, OUTPUT);
    pinMode(SOUTH_LEFT_GREEN_PIN, OUTPUT);
    pinMode(SOUTH_GREEN_PIN, OUTPUT);
    pinMode(SOUTH_WALK_RED_PIN, OUTPUT);
    pinMode(SOUTH_WALK_GREEN_PIN, OUTPUT);
    
    // 동쪽 방향 LED 핀 설정
    pinMode(EAST_RED_PIN, OUTPUT);
    pinMode(EAST_YELLOW_PIN, OUTPUT);
    pinMode(EAST_LEFT_GREEN_PIN, OUTPUT);
    pinMode(EAST_GREEN_PIN, OUTPUT);
    pinMode(EAST_WALK_RED_PIN, OUTPUT);
    pinMode(EAST_WALK_GREEN_PIN, OUTPUT);
    
    // 북쪽 방향 LED 핀 설정
    pinMode(NORTH_RED_PIN, OUTPUT);
    pinMode(NORTH_YELLOW_PIN, OUTPUT);
    pinMode(NORTH_LEFT_GREEN_PIN, OUTPUT);
    pinMode(NORTH_GREEN_PIN, OUTPUT);
    pinMode(NORTH_WALK_RED_PIN, OUTPUT);
    pinMode(NORTH_WALK_GREEN_PIN, OUTPUT);
    
    // 초기 LED 상태 설정
    displayLED();
}

// 타이머 초기화 함수 - 타이머1 사용
void Timer_Init(void)
{
    // 타이머1 설정 (Timer0 대신 Timer1 사용)
    noInterrupts(); // 인터럽트 비활성화
    
    // 타이머1 초기화
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    
    // 비교 일치 값 설정 (1초마다 인터럽트)
    OCR1A = 15624; // 16MHz / (1024 * 15625) = 1Hz
    
    // CTC 모드 및 1024 프리스케일러 설정
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    
    // 타이머1 비교 일치 인터럽트 활성화
    TIMSK1 |= (1 << OCIE1A);
    
    interrupts(); // 인터럽트 활성화
}

// LED 표시 함수
void displayLED(void)
{
    // 서쪽 방향 LED 표시
    digitalWrite(WEST_RED_PIN, (g_ucLED_west_data & RED_LIGHT) ? HIGH : LOW);
    digitalWrite(WEST_YELLOW_PIN, (g_ucLED_west_data & YELLOW_LIGHT) ? HIGH : LOW);
    digitalWrite(WEST_LEFT_GREEN_PIN, (g_ucLED_west_data & LEFT_GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(WEST_GREEN_PIN, (g_ucLED_west_data & GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(WEST_WALK_RED_PIN, (g_ucLED_west_data & WALK_RED_LIGHT) ? HIGH : LOW);
    digitalWrite(WEST_WALK_GREEN_PIN, (g_ucLED_west_data & WALK_GREEN_LIGHT) ? HIGH : LOW);
    
    // 남쪽 방향 LED 표시
    digitalWrite(SOUTH_RED_PIN, (g_ucLED_south_data & RED_LIGHT) ? HIGH : LOW);
    digitalWrite(SOUTH_YELLOW_PIN, (g_ucLED_south_data & YELLOW_LIGHT) ? HIGH : LOW);
    digitalWrite(SOUTH_LEFT_GREEN_PIN, (g_ucLED_south_data & LEFT_GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(SOUTH_GREEN_PIN, (g_ucLED_south_data & GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(SOUTH_WALK_RED_PIN, (g_ucLED_south_data & WALK_RED_LIGHT) ? HIGH : LOW);
    digitalWrite(SOUTH_WALK_GREEN_PIN, (g_ucLED_south_data & WALK_GREEN_LIGHT) ? HIGH : LOW);
    
    // 동쪽 방향 LED 표시
    digitalWrite(EAST_RED_PIN, (g_ucLED_east_data & RED_LIGHT) ? HIGH : LOW);
    digitalWrite(EAST_YELLOW_PIN, (g_ucLED_east_data & YELLOW_LIGHT) ? HIGH : LOW);
    digitalWrite(EAST_LEFT_GREEN_PIN, (g_ucLED_east_data & LEFT_GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(EAST_GREEN_PIN, (g_ucLED_east_data & GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(EAST_WALK_RED_PIN, (g_ucLED_east_data & WALK_RED_LIGHT) ? HIGH : LOW);
    digitalWrite(EAST_WALK_GREEN_PIN, (g_ucLED_east_data & WALK_GREEN_LIGHT) ? HIGH : LOW);
    
    // 북쪽 방향 LED 표시
    digitalWrite(NORTH_RED_PIN, (g_ucLED_north_data & RED_LIGHT) ? HIGH : LOW);
    digitalWrite(NORTH_YELLOW_PIN, (g_ucLED_north_data & YELLOW_LIGHT) ? HIGH : LOW);
    digitalWrite(NORTH_LEFT_GREEN_PIN, (g_ucLED_north_data & LEFT_GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(NORTH_GREEN_PIN, (g_ucLED_north_data & GREEN_LIGHT) ? HIGH : LOW);
    digitalWrite(NORTH_WALK_RED_PIN, (g_ucLED_north_data & WALK_RED_LIGHT) ? HIGH : LOW);
    digitalWrite(NORTH_WALK_GREEN_PIN, (g_ucLED_north_data & WALK_GREEN_LIGHT) ? HIGH : LOW);
}

// 방향 선택 함수
void selectDirection(unsigned char sel_dir, unsigned char data)
{
    switch (sel_dir)
    {
        case WEST:
            g_ucLED_west_data = data;
            break;
        case SOUTH:
            g_ucLED_south_data = data;
            break;
        case EAST:
            g_ucLED_east_data = data;
            break;
        case NORTH:
            g_ucLED_north_data = data;
            break;
        default:
            break;
    }
}

// 방향 변경 함수
void changeDirection(unsigned char sel_dir)
{

    int extendTime = 0; // 연장 시간 확인
    int totalTime = g_nBaseTime;

    switch (sel_dir){
        case WEST:
            if (g_bWestExtend && g_nCurrentExtendTime == 0) {
                extendTime = g_nExtendTime;
                buzzerClick();
                Serial.println("서쪽 보행자 신호 연장");
                g_bWestExtend = false;
            }
            break;
        case SOUTH:
            if (g_bSouthExtend && g_nCurrentExtendTime == 0) {
                extendTime = g_nExtendTime;
                buzzerClick();
                Serial.println("남쪽 보행자 신호 연장");
                g_bSouthExtend = false;
            }
            break;
        case EAST:
            if (g_bEastExtend && g_nCurrentExtendTime == 0) {
                extendTime = g_nExtendTime;
                buzzerClick();
                Serial.println("동쪽 보행자 신호 연장");
                g_bEastExtend = false;
            }
            break;
        case NORTH:
            if (g_bNorthExtend && g_nCurrentExtendTime == 0) {
                extendTime = g_nExtendTime;
                buzzerClick();
                Serial.println("북쪽 보행자 신호 연장");
                g_bNorthExtend = false;
            }
            break;
    }

    if(extendTime > MAX_EXTEND_TIME) extendTime = MAX_EXTEND_TIME;

    // 기존 트래픽 문제가 생기던 문제 수정
    if (g_unActionCounter == 1)
    {
        selectDirection(sel_dir, GREEN_LIGHT | LEFT_GREEN_LIGHT | WALK_RED_LIGHT);
        // 모든 보행자 신호 붉은색
        selectDirection((sel_dir+3)%4, RED_LIGHT | WALK_RED_LIGHT);
        selectDirection((sel_dir+2)%4, RED_LIGHT | WALK_RED_LIGHT); 
        selectDirection((sel_dir+1)%4, RED_LIGHT | WALK_RED_LIGHT); 

        g_nCurrentExtendTime = extendTime;
    }
    // 2초 안전시간 뒤 반대편 보행자 신호 초록으로 변경
    if(g_unActionCounter == 3){
        selectDirection((sel_dir+2)%4, RED_LIGHT | WALK_GREEN_LIGHT);
        buzzerChangeSignal();
    }
    // 7세그먼트 표기 함수에 넘길 시간 raw
    totalTime += g_nCurrentExtendTime;

    // 3초 ~ 5초동안
    if (g_unActionCounter >= 3 && g_unActionCounter <= totalTime + 2) {
        int remainTime = totalTime + 3 - g_unActionCounter;

        if(remainTime >= 0) displayTime(remainTime);

        if (g_nCurrentExtendTime > 0 && g_unActionCounter % 2 == 0) buzzerWalkSignal();
    }

    if (g_unActionCounter == totalTime + 3) // 시간 추가를 고려한 조건 변경
    {
    selectDirection(sel_dir, YELLOW_LIGHT | WALK_RED_LIGHT);
    
    // 모든 보행자 신호를 빨간색으로 변경
    selectDirection((sel_dir+3)%4, RED_LIGHT | WALK_RED_LIGHT);
    selectDirection((sel_dir+2)%4, RED_LIGHT | WALK_RED_LIGHT); // 반대편 보행자 신호 끄기
    selectDirection((sel_dir+1)%4, RED_LIGHT | WALK_RED_LIGHT);

    buzzerChangeSignal();
    }
    
    if (g_unActionCounter == totalTime + 4)
    {
        selectDirection(sel_dir, RED_LIGHT | WALK_RED_LIGHT); // 기준 신호등
        g_unSel_dir++;
        if (g_unSel_dir > 3)
            g_unSel_dir = 0;
        g_unActionCounter = 0;
        g_nCurrentExtendTime = 0;
    }
}

// 타이머1 비교 일치 인터럽트 서비스 루틴
ISR(TIMER1_COMPA_vect) {
  g_unActionCounter++;  // 1초마다 카운터 증가
}

void setup() {
  Serial.begin(9600);
  Serial.println("교통제어 +보행 보조기능 시스템 동작 시작");
  
  // 각 모듈 초기화
  sevenSegmentInit();
  buttonInit();
  buzzerInit();

  LED_Init();
  Timer_Init();
}

void loop() {
  // 버튼 상황 감지
  checkButton();

  // 방향 변경 처리
  changeDirection(g_unSel_dir);
  
  // LED 표시 업데이트
  displayLED();
}