#include <stdint.h>

uint32_t pack_coordinates(int32_t degrees, uint32_t minutes, float seconds);

char get_message_type();

uint32_t get_coordinates_latitude();
uint32_t get_coordinates_longitude();

void get_input(char* message);

void pack_message(char* message, void* new_block);
