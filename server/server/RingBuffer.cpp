#include "RingBuffer.h"

RingBuffer::RingBuffer()
    : write_point(0)
    , read_point(0)
{
}

RingBuffer::~RingBuffer()
{
}

int RingBuffer::enqueue(char* data, int data_size)
{
    if (data_size > BUFFER_SIZE) // 링버퍼의 크기보다 더 큰 데이터가 들어옴
    {
        return -1;
    }

    int w_p = write_point;
    for (int i{}; i < data_size; ++i)
    {
        if ((w_p == read_point) && !empty()) // 읽지도 않은 데이터가 있어서 더 못 씀. 에러 표시
        {
            return -2; 
        }

        ++w_p;

        if (w_p == BUFFER_SIZE)
        {
            w_p = 0;
        }
    }

    memcpy(buffer + write_point, data, data_size);

    write_point = w_p;

    return data_size;

    //int write_num = 0;

    //// 현재 버퍼를 넘어선다면 계속 채우다가 오래된 데이터부터 덮어쓰기 하기
    //if (BUFFER_SIZE <= write_point + data_size)
    //{
    //    int remain1 = BUFFER_SIZE - write_point;
    //    memcpy(buffer + write_point, data, remain1);
    //    write_num += remain1;
    //    write_point = 0;

    //    int remain2 = data_size - remain1;
    //    memcpy(buffer + write_point, data + remain1, remain2);
    //    write_num += remain2;
    //    write_point += remain2;

    //    return write_num;
    //}

    //memcpy(buffer + write_point, data, data_size);

    //write_point += data_size;

    //return data_size;
}

int RingBuffer::dequeue(char* dest, int data_size)
{
    if (data_size > BUFFER_SIZE) // 버퍼 크기보다 더 큰 크기를 읽으려고 할 때
    {
        return -1;
    }

    int r_p = read_point;
    for (int i{}; i < data_size; ++i)
    {
        if (r_p == write_point) // 더 읽을게 없는데 읽는 다는 것이므로 오류
        {
            return -2;
        }

        ++r_p;

        if (r_p == BUFFER_SIZE)
        {
            r_p = 0;
        }
    }

    memcpy(dest, buffer + read_point, data_size);
    read_point = r_p;

    return data_size;

    //int read_num = 0;

    //// n + 1 로 간다면?
    //if (BUFFER_SIZE <= read_point + data_size)
    //{
    //    int remain1 = BUFFER_SIZE - read_point;
    //    memcpy(dest, buffer + read_point, remain1);
    //    read_num += remain1;
    //    read_point = 0;

    //    int remain2 = data_size - remain1;
    //    memcpy(dest + remain1, buffer + read_point, remain2);
    //    read_num += remain2;
    //    read_point += remain2;

    //    return read_num;
    //}

    //memcpy(dest, buffer + read_point, data_size);
    //read_num += data_size;
    //read_point += data_size;

    //return read_num;
}

int RingBuffer::diff()
{
    int diff;

    if (read_point > write_point)
    {
        diff = (BUFFER_SIZE - read_point) + write_point;
    }
    else
    {
        diff = write_point - read_point;
    }
    return diff;
}

bool RingBuffer::empty()
{
    return (write_point == read_point);
}

void RingBuffer::move_read_pos(char move)
{
    if (BUFFER_SIZE <= read_point + move)
    {
        read_point = move - (BUFFER_SIZE - read_point);
    }
    else
    {
        read_point += move;
    }
}
