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

	read_pos += data_size;
	number_of_copies += data_size;

	return data_size;
}

int RingBuffer::remain_data()
{
	return abs(read_pos - write_pos);
}

bool RingBuffer::is_empty()
{
	if (number_of_copies == BUFFER_SIZE)
		return true;
	else
		return false;
}
