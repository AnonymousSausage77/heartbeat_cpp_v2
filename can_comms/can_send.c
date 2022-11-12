#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "can_message.h"

char *SendCAN(char buf[16], int s)
{
	int nbytes;
	union can_message_u can_data;

	memset(can_data.buf, '\0', sizeof(can_data.buf));
	
	for (int i = 0; i < sizeof(can_data.frame); i++)
	{
		can_data.buf[i] = buf[i];
	}
	
	printf("can_id  = %#.8X\n", can_data.frame.can_id);
	printf("can_dlc = %d\n", can_data.frame.can_dlc);
	for(int i = 0; i < 8; i++)
		printf("buf[%d] = %d; frame data[%d] = %d\n", i, buf[i+8], i, can_data.frame.data[i]);

	nbytes = write(s, &can_data.frame, sizeof(can_data.frame));
	printf("nbytes = %d\n", nbytes); 
	if(nbytes != sizeof(can_data.frame))
	{
		return "Send Error frame[0]!\r\n";
	}
	return 0;
}
