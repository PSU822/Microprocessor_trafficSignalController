
// 세븐 세그먼트 셀렉트 핀, 데이터핀, 데이터 배열

int seg7_sel[6] = {8, 9 , 10, 11, 12, 13};  // 핀 매칭
int seg7_dp[8] = {0, 1, 2, 3, 4, 5, 6, 7};  // 핀 ㅐㅁ칭
int seg7_data[16] = {}; // 세그먼트 데이터 추가 필요

// 세븐 세그먼트 초기화 함수
void sevenSegmentInit();

// 남은 시간 표기
void displayTime(int Time);

