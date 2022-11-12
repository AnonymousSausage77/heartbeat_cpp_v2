#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "can_control.h"

#define CAN_FRC_MASK 0x1FFF003FU /* extended frame format (EFF) */


char *InitialiseCAN(enum canbus bus, int *s)
{
	int ret;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	switch (bus)
	{
		case 0:
		{
			system("sudo ip link set can0 up type can bitrate 1000000");
			printf("ip link set can0 up type can bitrate 1000000\n");
			system("sudo ifconfig can0 txqueuelen 65536");
			printf("ifconfig can0 txqueuelen 65536\n");
			//system("sudo ifconfig can0 up");
			printf("can0 active\r\n");
			break;
		}
		case 1:
		{
			system("sudo ip link set can1 up type can bitrate 1000000");
			printf("ip link set can1 up type can bitrate 1000000\n");
			system("sudo ifconfig can1 txqueuelen 65536");
			printf("ifconfig can1 txqueuelen 65536\n");
			//system("sudo ifconfig can1 up");
			printf("can1 active\r\n");
			break;
		}
	}

	//1.Create socket
	*s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (*s < 0)
	{
		//perror("socket PF_CAN failed");
		return "socket PF_CAN failed";
	}

	//2.Specify can0 device
	switch (bus)
	{
		case 0:
		{
			strcpy(ifr.ifr_name, "can0");
			break;
		}
		case 1:
		{
			strcpy(ifr.ifr_name, "can1");
			break;
		}
	}
	ret = ioctl(*s, SIOCGIFINDEX, &ifr);
	if (ret < 0)
	{
		//perror("ioctl failed");
		return "ioctl failed";
	}

	//3.Bind the socket to can0
	addr.can_family = PF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	ret = bind(*s, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
	{
		//perror("bind failed");
		return "bind failed";
	}

	//4.Define receive rules
	switch (bus)
	{
		case 0:
		{
			struct can_filter rfilter[1];
			rfilter[0].can_id = 0x8A080014;
			rfilter[0].can_mask = CAN_FRC_MASK;
			setsockopt(*s, SOL_CAN_RAW, CAN_RAW_FILTER | CAN_RAW_LOOPBACK, &rfilter, sizeof(rfilter));
			break;
		}
		case 1:
		{
			struct can_filter rfilter[1];
			rfilter[0].can_id = 0x8A080014;
			rfilter[0].can_mask = CAN_FRC_MASK;
			setsockopt(*s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
			break;
		}
	}
	
	return 0;
}

void CloseCAN(enum canbus bus, int s)
{
	close(s);
	switch (bus)
	{
		case 0:
		//6.Close the socket and can0
			system("sudo ifconfig can0 down");
			printf("can0 closed\n");
			break;
		case 1:
		//6.Close the socket and can1
			system("sudo ifconfig can1 down");
			printf("can1 closed\n");
			break;
	}
}
