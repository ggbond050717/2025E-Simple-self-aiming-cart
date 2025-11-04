#ifndef __TRACK_H
#define __TRACK_H

#include "board.h"

extern unsigned char Digtal;


void Read_Tracking_Sensors1(uint8_t *sensor_values);
void Track_printf(uint8_t *sensor_values);

#endif
