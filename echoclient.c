/*
 * Copyright © 2012 Bart Massey
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/*
 * TCP Echo Client
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "echo.h"

int main(int argc, char **argv) {
    int r, s;
    struct sockaddr_in addr;
    char buf[512];

    /* Check for proper usage. */
    assert(argc == 3);

    /* Create the socket. */
    s = socket(AF_INET, SOCK_STREAM, 0);
    assert(s != -1);

    /* Connect the client to the server. */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    /* XXX Numeric IP address only for now. */
    r = inet_aton(argv[1], &addr.sin_addr);
    assert(r != -1);
    r = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    assert(r != -1);
    printf("Got connection...\n");

    /* Write the message to the server. */
    r = write(s, argv[2], strlen(argv[2]) + 1);
    assert(r != -1);
    printf("Wrote arg...\n");

    /* Read and print the echo response. */
    r = read(s, buf, 512);
    assert(r != -1);
    printf("%s\n", buf);

    return 0;
}
