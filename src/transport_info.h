//
// Created by eater on 10/15/17.
//

#ifndef JOSLINK_TRANSPORT_INFO_H
#define JOSLINK_TRANSPORT_INFO_H

#include <jack/jack.h>
#include <SLAudio.h>

struct TransportInfo {
    CSLAudioContext* SteamContext;
    int ChannelNum;
    jack_client_t* JackClient;
};

int GetInfo(struct TransportInfo*);
void FreeInfo(struct TransportInfo*);
#endif //JOSLINK_TRANSPORT_INFO_H
