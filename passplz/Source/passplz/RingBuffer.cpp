#define _CRT_SECURE_NO_WARNINGS

#include "RingBuffer.h"

int RingBuffer::enqueue(char* data, int data_size)
{
	if (number_of_copies == 0)
		return static_cast<int>(error::full_buffer);

	if (number_of_copies < data_size)
		return static_cast<int>(error::in_data_is_too_big);

	if (BUFFER_SIZE < write_pos + data_size)
		write_pos = 0;

	memcpy(buffer + write_pos, data, data_size);

	write_pos += data_size;
	number_of_copies -= data_size;

	return data_size;
}

int RingBuffer::dequeue(char* destination, int data_size)
{
	if (number_of_copies == BUFFER_SIZE)
		return static_cast<int>(error::no_data_in_buffer);

	if (BUFFER_SIZE - number_of_copies < data_size)
		return static_cast<int>(error::out_data_is_too_big);

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
			if (read_pos + buffer[read_pos] >= BUFFER_SIZE) {
				return 0;
			} else {
				buffer[read_pos + buffer[read_pos] + 1];
				return 0;
			}
		} else
			return write_pos;
	} else
		return abs(write_pos - read_pos);
}

char RingBuffer::peek_front()
{
	if (buffer[read_pos] == NULL)
		return buffer[0];
	else
		return buffer[read_pos];
}