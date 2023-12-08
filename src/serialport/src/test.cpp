/*
	Simple example to read a large amount of data from a BM device.
	Device must have bitbang capabilities to enable this to work

	To build use the following gcc statement
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o largeread main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/ftd2xx.h"
#include <time.h>
#include "../include/livelybot_serial.h"
#define BUF_SIZE 0x11 //
int main(int argc, char *argv[])
{
	char serialNumber[] = "B";
	livelybot_serial ser;
	ser.LD_Open(serialNumber);
	ser.LD_SetBaudRate(4000000);
	ser.LD_SetLatencyTimer(1);
	ser.LD_SetTimeouts(0, 0);
	__uint8_t a[BUF_SIZE];
	int j = 0;
	struct timespec ts, st;
	clock_gettime(CLOCK_REALTIME, &st);
	unsigned int rt = 0;
	while (j < 100000)
	{
		ser.LD_read(a, 2, &rt);
		if (a[0] == 0xFD && a[1] == 0xFE)
		{
			clock_gettime(CLOCK_REALTIME, &ts);
			printf("[%6.6f ms] ", (ts.tv_sec - st.tv_sec) * 1000 + ts.tv_nsec / 1000000.0);
			for (size_t i = 0; i < 2; i++)
			{
				printf("0x%02X ", a[i]);
			}
			ser.LD_read(a, 15, &rt);
			// printf("2\n");
			for (size_t i = 0; i < 15; i++)
			{
				printf("0x%02X ", a[i]);
			}
			printf("\n");
		}

		j++;
	}

	ser.LD_Close();
	return 0;
}
