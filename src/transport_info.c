//
// Created by eater on 10/15/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <SLAudio.h>
#include <jack/jack.h>
#include "transport_info.h"

#define create(name, what) what *name = malloc(sizeof(what));


int GetInfo(struct TransportInfo* info) {
    printf("Creating SLAudio Context...\n");
    info->SteamContext = SLAudio_CreateContext();
    if (info->SteamContext == NULL) {
        // Nothing to free;
        printf("Failed creating SLAudio Context...\n");
        return -1;
    }

    printf("Getting speaker count: ");
    info->ChannelNum = SLAudio_GetSpeakerCount(info->SteamContext);
    printf("%d\n", info->ChannelNum);

    create(status_t, jack_status_t);
    info->JackClient = jack_client_open("JOSLINK", JackNullOption, status_t);

    int status = (int)status_t;
    free(status_t);

    if (status < 0) {
        printf("Failed to create JACK client error code: %d\n", status);
        FreeInfo(info);
        return -1;
    }

    return 0;
}

void FreeInfo(struct TransportInfo* info) {
    if (info->JackClient != NULL) {
        jack_client_close(info->JackClient);
        jack_free(info->JackClient);
    }

    if (info->SteamContext != NULL) {
        SLAudio_FreeContext(info->SteamContext);
    }

    free(info);
}