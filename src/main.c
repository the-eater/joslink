#define JOSLINK_VERSION "0.1"

#include <stdio.h>
#include <stdlib.h>

#include "transport_info.h"



int main(void) {
    int errno = 0;
    struct TransportInfo *info = malloc(sizeof(struct TransportInfo));

    printf("Welcome to JOSLINK %s\n", JOSLINK_VERSION);
    printf("Fetching TransportInfo\n");

    errno = GetInfo(info);
    if (errno != 0) {
        printf("Failed to get TransportInfo quiting...\n");
        return 1;
    }

    printf("Exiting JOSLINK\n");
    return 0;
}
