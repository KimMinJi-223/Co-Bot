#pragma once

// 패킷 재조립을 위해 작성
// - 버퍼가 꽉 찼을 경우: 오래된 데이터부터 덮어쓰기

// #include <array>

#include <string.h>

constexpr int BUFFER_SIZE = 50000;

class RingBuffer
{
private:
	int write_point;
	int read_point;
	// std::array<char, BUFFER_SIZE> buffer;
	char buffer[BUFFER_SIZE];

public:
	int enqueue(char* data, int data_size); // 데이터 넣기
	int dequeue(char* dest, int data_size); // 데이터 꺼내기

	int diff();
	bool empty();

	void move_read_pos(char move);

public:
	RingBuffer();
	~RingBuffer();
};

// 현재까지 테스트에서는 아무 문제 없음
// 이론상으로도 문제 없을 것 같음
// 그래도 추후에 예상치 못한 오류를 일으킬 가능성은 있을 듯
// 오류가 터진다면 한번 들여다 보기
// 현재 dequeue를 쓰지 않음
// -> 억지로 쓰면 쓸 수 있지만 굳이?? 코드 구현도 복잡해지고 괜히 오류 일으킬 가능성만 높일 수 있다.
// 교수님과 다른 코드 방식으로 구현하기 위해 링버퍼로 패킷 재조립을 구현했지만 아직까지 필요성을 못 느낌
// 구현이 어려운 건 아니지만 실습때 교수님이 사용하셨던 방법이 더 간단해보임
// 인터넷 찾아보니 인터럽트 때문이라는데 그거면 교수님의 코드로도 방어가 가능하다.
// 그래도 패킷 재조립에 링버퍼가 필수는 아니지만 많이 유용하게 쓰이는 것 같다.
// 링버퍼의 작동방식은 이해했지만 사용 이유가 명확하게 이해가지 않음 -> 더 살펴봐야 할 것 같다.

// 링버퍼
// 링버퍼의 연산은 모두 O(1)의 시간 복잡도를 가진다.