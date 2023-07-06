#define _CRT_SECURE_NO_WARNINGS

#include "RingBuffer.h"

int RingBuffer::enqueue(char* data, int data_size)
{
	if (number_of_copies == 0) {
		std::cout << "enqueue full buffer\n";
		return static_cast<int>(error::full_buffer);
	}

	if (number_of_copies < data_size) {
		std::cout << "enqueue in data is too big\n";
		return static_cast<int>(error::in_data_is_too_big);
	}	

	if (BUFFER_SIZE < write_pos + data_size)
		write_pos = 0;

	memcpy(buffer + write_pos, data, data_size);

	write_pos += data_size;
	number_of_copies -= data_size;

	return data_size;
}

int RingBuffer::dequeue(char* destination, int data_size)
{
	if (number_of_copies == BUFFER_SIZE) {
		std::cout << "dequeue no data in buffer\n";
		return static_cast<int>(error::no_data_in_buffer);
	}

	if (BUFFER_SIZE - number_of_copies < data_size) {
		std::cout << "dequeue out data is too big\n";
		return static_cast<int>(error::out_data_is_too_big);
	}
	           
	if (BUFFER_SIZE < read_pos + data_size) 
		read_pos = 0;

	memcpy(destination, buffer + read_pos, data_size);
	memset(buffer + read_pos, NULL, data_size);

	read_pos += data_size;
	number_of_copies += data_size;

	return data_size;
}

int RingBuffer::remain_data()
{
	if (read_pos > write_pos) {
		if (buffer[read_pos] != NULL) {
			if (read_pos + buffer[read_pos] >= BUFFER_SIZE) { // 이 문제와 
				return 0;
			} else { // 이 문제를 아직 잘 해결하지 못 함
				buffer[read_pos + buffer[read_pos] + 1];
				return 0;
			}
		} else
			return write_pos;
	} else
		return abs(write_pos - read_pos);

	//if (read_pos > write_pos) {
	//	int gap = 0, count = 0;
	//	while (buffer[read_pos + gap] != NULL) {
	//		if (read_pos + buffer[read_pos] > BUFFER_SIZE) {
	//			for (; buffer[read_pos + gap] == NULL; ++gap)
	//				++count;
	//			return count + write_pos;
	//		} else {
	//			if (buffer[buffer[read_pos + gap]])
	//			return buffer[read_pos] + write_pos;
	//		}
	//	}
	//}
}

char RingBuffer::peek_front()
{
	if (buffer[read_pos] == NULL)
		return buffer[0];
	else
		return buffer[read_pos];
}