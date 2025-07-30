#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include "generic_context.h"

uint32_t pack_coordinates(int32_t degrees, uint32_t minutes, float seconds);

char get_message_type();

float get_coordinates_latitude();
float get_coordinates_longitude();

void pack_message(const struct gs_generic_protocol_t* message_st, char* message_str);
void get_message(struct gs_generic_protocol_t* new_message);

char get_input(char* message);

uint16_t get_checksum(const char* message, uint32_t message_size);

#endif

