#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <netinet/in.h>
#include "generic_context.h"


void receive_message(char *packed_buffer, size_t buffer_size, struct sockaddr_in client_info, int client_socket);
short process_message(char *packed_buffer, size_t buffer_size);
void unpack_message(char *packed_buffer, size_t buffer_size, struct gs_generic_protocol_t* new_message_st);

#endif