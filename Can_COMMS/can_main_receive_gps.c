/*
 * can_main_receive_gps.c
 * 
 * Receives GPS CAN messages from can0
 */


/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>*/

#include "can_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>


int main()
{
	union can_message_u can_mess_rx;
	union gps_data_u my_gps_d_u;

	int s0;
	int frame_len;
	char *can_error;
	struct can_frame frame;

	memset(&can_mess_rx.frame, 0, sizeof(struct can_frame));

	can_error = InitialiseCAN(can0, &s0);
	if (can_error)
	{
		printf("Can1 Initialisation failed: %s", can_error);
		return 1;
	}

	//Get CAN data and decode if data is a command message otherwise ignore
	printf("Start Receive CAN Data Loop\n");
	while (1)
	{
		printf("Receive CAN Data\n");
		frame_len = read(s0, &frame, sizeof(frame));
		//frame_len = ReadCAN(s0, &frame);
		printf("Data Receive Frame Length = %d\n", frame_len);
		if (frame_len > 0)
		{
			my_gps_d_u = GetGPSLatLonData(frame, frame_len);
			printf("GPS Lat = %d, GPS Heading = %d\n", my_gps_d_u.gps_d_s33.lat, my_gps_d_u.gps_d_s33.hdg);
			if (frame.can_id == 33)
			{
				printf("GPS Message Received\n");
				break;
			}
		}
		else printf("No Data Received\n");
	}
	
	CloseCAN(can0, s0);

	return 0;
}
