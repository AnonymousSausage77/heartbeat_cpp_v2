#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

//GPS data structures

struct gps_data1_s33
{
	uint8_t mess_id; //message ID 33
	uint8_t pad; //padding not used
	uint16_t hdg;
	int32_t lat;
};

struct gps_data2_s34 //message ID 34
{
	uint8_t mess_id; //message ID 33
	uint8_t pad; //padding not used
	uint16_t spd;
	int32_t lon;
};

union gps_data_u //Message ID - 33 or 34
{
	uint8_t buf[8];
	uint8_t mess_id;
	struct gps_data1_s33 gps_d_s33;
	struct gps_data2_s34 gps_d_s34;
};

struct gps_message_s
{
	uint32_t can_id; //Message ID = 33/35 for lat/long or 34/36 for hdg/spd
	uint8_t can_dlc; //Payload length
	uint8_t padding; //Not used
	uint8_t reserve; //not used
	uint8_t ext_dlc; //Extended payload length (not used)
	union gps_data_u gps_d_u;
};

//CAN data structures

struct can_id_s
{
	uint32_t can_id: 28; //28 bit CAN identifier
	uint32_t error_mess: 1; //error message frame flag (0 = data frame, 1 = error message)
	uint32_t remote_tx: 1; //remote transmission request flag (1 = rtr frame)
	uint32_t frame_format: 1; //frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
};

union can_id_u
{
	uint32_t can_id;
	struct can_id_s can_id_bits;
};

union can_message_u
{
	char buf[16];
	struct gps_message_s gps_m_s;
	struct can_frame frame;
};

#endif
