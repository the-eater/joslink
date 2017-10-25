#define JOSLINK_VERSION "0.1"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "transport.h"



int main(void) {
    struct Transport *info;

    printf("Welcome to JOSLINK %s\n", JOSLINK_VERSION);
    printf("Fetching Transport\n");

    // Create transport
    info = transport_create();
    if (info == NULL) {
        printf("Failed to get Transport quiting...\n");
        return 1;
    }

    // init transport
    transport_init(info);
    printf("Done init, going in forever sleep");

    // Go in sleep while JACK thread does the callback
    while (1) {
        sleep(1);
    }

    printf("Exiting JOSLINK\n");
    transport_free(info);
    return 0;
}
