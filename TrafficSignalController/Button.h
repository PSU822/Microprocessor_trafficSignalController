#define WEST_BUTTON_PIN 54   // 서쪽 방향 버튼 pb 1
#define SOUTH_BUTTON_PIN 55  // 남쪽 방향 버튼 pb 2
#define EAST_BUTTON_PIN  56  // 동쪽 방향 버튼 pb 3
#define NORTH_BUTTON_PIN 57 // 북쪽 방향 버튼 pb 4


// 메인 코드에서 참조될 변수들

extern volatile boolean g_bWestExtend;
extern volatile boolean g_bSouthExtend;
extern volatile boolean g_bEastExtend;
extern volatile boolean g_bNorthExtend;

// 버튼 초기화
void buttonInit();

// 버튼 피드백(위의 외부 참조된 변수들) 상태 변경 함수
void checkButton();