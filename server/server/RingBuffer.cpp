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

	if (write_pos + data_size > BUFFER_SIZE) {
		std::cout << "enqueue buffer end\n";
		int temp = BUFFER_SIZE - write_pos;
		std::cout << "temp: " << temp << std::endl;
		std::cout << "write pos: " << write_pos << std::endl;
		memcpy(buffer + write_pos, data, temp);
		//for (int i{}; i < temp; ++i)
		//	printf("buffer[%d]: %d\n", write_pos + i, buffer[write_pos + i]);

		write_pos = 0;
		memcpy(buffer + write_pos, data + temp, data_size - temp);
		//for (int i{}; i < data_size - temp; ++i)
		//	printf("buffer[%d]: %d\n", write_pos + i, buffer[write_pos + i]);

		write_pos += data_size - temp;

		//std::cout << "write pos: " << write_pos << std::endl;
	} else {
		memcpy(buffer + write_pos, data, data_size);

		write_pos += data_size;
		if (BUFFER_SIZE == write_pos)
			write_pos = 0;
	}

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
	
	if (read_pos + data_size > BUFFER_SIZE) {
		int temp = BUFFER_SIZE - read_pos;
		memcpy(destination, buffer + read_pos, temp);
		memset(buffer + read_pos, NULL, temp);
		read_pos = 0;
		memcpy(destination + temp, buffer + read_pos, data_size - temp);
		memset(buffer + read_pos, NULL, data_size - temp);
		read_pos += data_size - temp;
	} else {
		memcpy(destination, buffer + read_pos, data_size);
		memset(buffer + read_pos, NULL, data_size);
		read_pos += data_size;
		if (BUFFER_SIZE == read_pos)
			read_pos = 0;
	}

	number_of_copies += data_size;

	return data_size;
}

int RingBuffer::remain_data()
{
	if (write_pos >= read_pos)
		return write_pos - read_pos;

	return BUFFER_SIZE - read_pos + write_pos;
}

char RingBuffer::peek_front()
{
	if (BUFFER_SIZE == read_pos)
		return buffer[0];

	return buffer[read_pos];
}