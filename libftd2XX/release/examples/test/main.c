/*
    Simple example to read a large amount of data from a BM device.
    Device must have bitbang capabilities to enable this to work

    To build use the following gcc statement
    (assuming you have the d2xx library in the /usr/local/lib directory).
    gcc -o largeread main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include "../ftd2xx.h"
#include <time.h>
#define BUF_SIZE 0x11 // 1 Megabyte
#define MAX_DEVICES 5
int main(int argc, char *argv[])
{
    FT_STATUS ftStatus;
    char *pcBufRead;
    FT_HANDLE ftHandle;
    char cBufLD[MAX_DEVICES][64];
    DWORD dwBytesRead;

    ftStatus = FT_OpenEx("C", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("FT_Open(%s) failed\n", cBufLD[0]);
        return 1;
    }

    __uint8_t a[BUF_SIZE];
    FT_SetBaudRate(ftHandle, 4000000);
    FT_SetTimeouts(ftHandle, 0, 0); // infinite timeouts
    UCHAR LatencyTimer = 1;
    ftStatus = FT_SetLatencyTimer(ftHandle, LatencyTimer);
    if (ftStatus != FT_OK)
    {
        printf("failed");
        return 1;
    }
    int j = 0;
    struct timespec ts, st;
    clock_gettime(CLOCK_REALTIME, &st);
    while (j < 100000)
    {
        // printf("1\n");
        FT_Read(ftHandle, a, BUF_SIZE, &dwBytesRead);
        clock_gettime(CLOCK_REALTIME, &ts);
        // printf("2\n");
        printf("[%6.6f ms] ", (ts.tv_sec-st.tv_sec)*1000+ts.tv_nsec/1000000.0);
        for (size_t i = 0; i < BUF_SIZE; i++)
        {
            printf("0x%02X ", a[i]);
        }
        printf("\n");
        j++;
    }

    // fwrite(pcBufRead, 1, dwBytesRead, fh);
    FT_Close(ftHandle);
    return 0;
}
