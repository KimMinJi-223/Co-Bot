#pragma once
#include <iostream>

#define BUFFER_SIZE 2048

enum class error { full_buffer = -10, no_data_in_buffer, in_data_is_too_big, out_data_is_too_big, remain_err1, remain_err2
};

class RingBuffer
{
private:
	int read_pos;
	int write_pos;
	int number_of_copies;

public:
	char buffer[BUFFER_SIZE];
	RingBuffer() : read_pos(0), write_pos(0), number_of_copies(BUFFER_SIZE)
	{
		for (int i{}; i < BUFFER_SIZE; ++i)
		{
			buffer[i] = NULL;
		}
	};

public:
	int enqueue(char* data, int data_size);
	int dequeue(char* destination, int data_size);
	int remain_data();
	char peek_front();
};