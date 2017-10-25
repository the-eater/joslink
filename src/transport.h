//
// Created by eater on 10/15/17.
//

#ifndef JOSLINK_TRANSPORT_INFO_H
#define JOSLINK_TRANSPORT_INFO_H

#include <jack/jack.h>
#include <SLAudio.h>

struct Transport {
    CSLAudioContext* steam_context;
    int channel_num;
    jack_client_t* jack_client;
    CSLAudioStream* steam_stream;
    int buffer_pointer;
    int buffer_size;
    int frame_size;
    jack_port_t* jack_ports[6];
    int16_t* buffer;
};

/**
 * Creates a new transport for this device
 */
struct Transport *transport_create();

/**
 * Free given transport
 */
void transport_free(struct Transport *);

/**
 * Initializes given transport
 */
void transport_init(struct Transport *);
#endif //JOSLINK_TRANSPORT_INFO_H
