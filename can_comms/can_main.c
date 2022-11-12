/*
 * can_main.c
 * 
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "can_control.h"


int main()
{
	union can_message_u can_mess_tx, can_mess_rx;
	union gps_data_u my_gps_d_u;

	int s0, s1;
	int frame_len;
	char *can_error;
	struct can_frame frame;

	memset(&can_mess_tx.frame, 0, sizeof(struct can_frame));
	memset(&can_mess_rx.frame, 0, sizeof(struct can_frame));

	can_error = InitialiseCAN(can0, &s0);
	if (can_error)
	{
		printf("Can0 Initialisation failed: %s", can_error);
		return 1;
	}

	can_error = InitialiseCAN(can1, &s1);
	if (can_error)
	{
		printf("Can1 Initialisation failed: %s", can_error);
		return 1;
	}

	//Send message 33
	can_mess_tx.gps_m_s.can_id = 0x8A083414;
	can_mess_tx.gps_m_s.can_dlc = 8;
	can_mess_tx.gps_m_s.gps_d_u.gps_d_s33.mess_id = 33;
	can_mess_tx.gps_m_s.gps_d_u.gps_d_s33.lat = 35;
	can_mess_tx.gps_m_s.gps_d_u.gps_d_s33.hdg = 20;
	
	printf("Send CAN data fo message 33\n");
	can_error = SendCAN(can_mess_tx.buf, s1);
	if (can_error)
	{
		printf("Can0 failed to send: %s", can_error);
		return 1;
	}

	//Get CAN data and decode if data is a command message otherwise ignore
	//frame_len = ReadCAN(s0, &can_mess_rx.frame);
	frame_len = read(s0, &frame, sizeof(struct can_frame));
	printf("frame length = %d\n", frame_len);
	if (frame_len > 0)
	{
		printf("Data Received\n");
		printf("Message Receive:\nframe: can ID = %#.8X; can length = %d\n", frame.can_id, frame.can_dlc);
		for (int i = 0; i < 8; i++)
		{
			printf("frame: data[%d] = %d\n", i, frame.data[i]);
		}
		if (frame.can_id == 0x8A08AA14)
		{
			printf("GPS Message Received\n");
			my_gps_d_u = GetGPSLatLonData(frame, frame_len);
			if (my_gps_d_u.mess_id == 33)
			{
				printf("GPS Lat = %d, GPS Lon = %d\n", my_gps_d_u.gps_d_s33.lat, my_gps_d_u.gps_d_s33.hdg);
			}
			else if (my_gps_d_u.mess_id == 34)
			{
				printf("GPS Lat = %d, GPS Lon = %d\n", my_gps_d_u.gps_d_s34.lon, my_gps_d_u.gps_d_s34.spd);
			}
			else printf("Unknown message");
		}
		else printf ("invalid CAN ID\n");
	}
	else printf("No Data Received");
	
	CloseCAN(can0, s0);
	CloseCAN(can1, s1);

	return 0;
}
