#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "can_receive.h"
#include "can_message.h"

int ReadCAN(int s, struct can_frame *frame)
{
	return read(s, frame, sizeof(struct can_frame));
}

union gps_data_u GetGPSLatLonData(struct can_frame frame, int nbytes)
{
	union gps_data_u gps_u;
	
	if(nbytes > 0)
	{
		printf("can_id = 0x%X\r\ncan_dlc = %d \r\n", frame.can_id, frame.can_dlc);
		for(int i = 0; i < 8; i++)
		{
			gps_u.buf[i] = frame.data[i];
			printf("data[%d] = %d\r\n", i, frame.data[i]);
		}
	}
	return gps_u;
}
