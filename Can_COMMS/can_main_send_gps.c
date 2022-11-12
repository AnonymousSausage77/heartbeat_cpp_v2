/*
 * can_main_send_gps.c
 * 
 * Sends GPS data over CAN1
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "can_control.h"

int main()
{
	union can_message_u can_mess_tx33, can_mess_tx34;

	int s0;
	char *can_error;

	memset(&can_mess_tx33.frame, '\0', sizeof(struct can_frame));
	memset(&can_mess_tx34.frame, '\0', sizeof(struct can_frame));

	can_mess_tx33.gps_m_s.can_id = 0x8A080014;
	can_mess_tx33.gps_m_s.can_dlc = 8;
	can_mess_tx33.gps_m_s.gps_d_u.gps_d_s33.mess_id = 33;
	can_mess_tx33.gps_m_s.gps_d_u.gps_d_s33.lat = 35;
	can_mess_tx33.gps_m_s.gps_d_u.gps_d_s33.hdg = 22;
	
	can_mess_tx34.gps_m_s.can_id = 0x8A080014;
	can_mess_tx34.gps_m_s.can_dlc = 8;
	can_mess_tx34.gps_m_s.gps_d_u.gps_d_s34.mess_id = 34;
	can_mess_tx34.gps_m_s.gps_d_u.gps_d_s34.lon = -155;
	can_mess_tx34.gps_m_s.gps_d_u.gps_d_s34.spd = 5;

	can_error = InitialiseCAN(can0, &s0);
	if (can_error)
	{
		printf("Can0 Initialisation failed: %s", can_error);
		return 1;
	}
	
	while (1)
	{
		/*for (int i= 0; i < 16; i++)
		{
			printf("Send buf[%d]\n", can_mess_tx.buf[i]);
		}*/
		
		printf("Send CAN data\n");
		can_error = SendCAN(can_mess_tx33.buf, s0);
		if (can_error)
		{
			printf("Can1 failed to send: %s", can_error);
			return 1;
		}
		usleep(2000000);
		can_error = SendCAN(can_mess_tx34.buf, s0);
		if (can_error)
		{
			printf("Can1 failed to send: %s", can_error);
			return 1;
		}
		usleep(2000000);
	}

	CloseCAN(can0, s0);

	return 0;
}
