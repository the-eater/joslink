//
// Created by eater on 10/15/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <SLAudio.h>
#include <jack/jack.h>
#include <stdbool.h>
#include <string.h>
#include "transport.h"

#define create(name, what) what *name = malloc(sizeof(what));


struct Transport *transport_create() {
    create(transport, struct Transport);
    printf("Creating SLAudio Context...\n");

    // Create steam context
    transport->steam_context = SLAudio_CreateContext();

    if (transport->steam_context == NULL) {
        // Failed creating steam context
        // Nothing to free;
        printf("Failed creating SLAudio Context...\n");
        transport_free(transport);
        return NULL;
    }

    // Add some additional logging
    SLAudio_SetLogLevel(k_ESLAudioLogDebug);

    printf("Getting speaker count: ");

    // Get amount of channels we need to create
    transport->channel_num = SLAudio_GetSpeakerCount(transport->steam_context);
    printf("%d\n", transport->channel_num);

    create(status_t, jack_status_t);

    // Open jack client
    transport->jack_client = jack_client_open("JOSLINK", JackNullOption, status_t);

    if (*status_t & JackFailure) {
        // Failed creating jack client, so free it

        printf("Failed to create JACK client error code: %d\n", *status_t);
        transport_free(transport);
        return NULL;
    }

    return transport;
}

void transport_free(struct Transport *info) {
    int i;

    if (info->channel_num > 0) {
        for (i = 0; i < info->channel_num; ++i) {
            jack_port_unregister(info->jack_client, info->jack_ports[i]);
        }
    }

    if (info->jack_client != NULL) {
        printf("Closing jack client\n");
        jack_client_close(info->jack_client);
    }

    if (info->steam_context != NULL) {
        printf("Freeing steam context\n");
        // ? Generates SEGFAULT for some reason?
        // SLAudio_FreeContext(info->steam_context);
    }
    // ? Generates SEGFAULT for some reason?
    // free(info);
}

void transport_flush_buffer(struct Transport *info) {
    // Start frame
    int16_t *buffer = SLAudio_BeginFrame(info->steam_stream);

    // Copy our buffer to it
    memcpy(buffer, info->buffer, (size_t)info->frame_size);

    // Close frame
    SLAudio_SubmitFrame(info->steam_stream);

    // Reset pointer
    info->buffer_pointer = 0;
}

int transport_process(jack_nframes_t nframes, void * arg) {
    int i;
    int j;
    struct Transport *info = (struct Transport *)arg;
    float *waveform[6];

    // Map buffers to channels
    for (i = 0; i < info->channel_num; ++i) {
        waveform[i] = jack_port_get_buffer(info->jack_ports[i], nframes);
    }

    // Go through each frame and channel and add it to the buffer
    for (i = 0; i < nframes; ++i) {
        for (j = 0; j < info->channel_num; ++j) {
            // Change 32-bit float to 16 bit int very crudely
            // We could calculate our position in the buffer, but using an counter is way easier
            // Also way safer
            info->buffer[info->buffer_pointer++] = (int16_t)(waveform[j][i] * 32768);
        }

        // Buffer is full and ready to be flushed
        if (info->buffer_pointer >= info->buffer_size) {
            transport_flush_buffer(info);
        }
    }

    return 0;
}

void transport_init(struct Transport *info) {
    int i;
    char *str = malloc(40);

    printf("Jack has a sample rate of %d\n", jack_get_sample_rate(info->jack_client));

    printf("Creating %d Jack ports\n", info->channel_num);
    for (i = 0; i < info->channel_num; ++i) {
        sprintf(str, "channel_%d", i + 1);
        printf("Creating Jack port '%s'\n", str);
        info->jack_ports[i] = jack_port_register(info->jack_client, str, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput | JackPortIsTerminal, 0);
    }

    // Define buffer size
    info->buffer_size = (jack_get_sample_rate(info->jack_client) / 1000) * info->channel_num * 50;

    // Define frame size in bytes
    info->frame_size = info->buffer_size  * sizeof(int16_t);

    printf("Sending frames of %d bytes to SteamLink audio\n", info->frame_size);

    // alloc buffer
    info->buffer = malloc((size_t)info->frame_size);

    // Create audio stream, not bLowLatency because audio will get choppy? (what is it for?)
    info->steam_stream = SLAudio_CreateStream(info->steam_context, jack_get_sample_rate(info->jack_client), info->channel_num, info->frame_size, false);

    // Die when we failed in opening the audio stream
    if (info->steam_stream == NULL) {
        printf("Failed to open Audio stream\n");
        exit(1);
    }

    printf("Successfully opened SteamLink Audio Stream\n");

    // Set process callback
    jack_set_process_callback(info->jack_client, transport_process, info);

    printf("Set process callback on JackClient\n");

    // Active our client
    jack_activate(info->jack_client);

    printf("Activated JackClient\n");

    // Free channel string
    free(str);

    printf("Done init\n");
}