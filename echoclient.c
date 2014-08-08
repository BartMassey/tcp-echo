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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "echo.h"

int main(int argc, char **argv) {
    int r, s;
    struct addrinfo *ai;
    char buf[512];
    char port_string[sizeof(uint32_t) + 1];

    /* Check for proper usage. */
    assert(argc == 3);
    assert(strlen(argv[2]) < sizeof(buf));

    /* Look up server address or DNS name. */
    r = snprintf(port_string, sizeof(port_string), "%d", ECHO_PORT);
    assert(r < sizeof(port_string));
    r = getaddrinfo(argv[1], port_string, 0, &ai);
    assert(r != -1);

    /* Create the socket. */
    s = socket(ai->ai_family, SOCK_STREAM, 0);
    assert(s != -1);

    /* Connect the client to the server. */
    r = connect(s, ai->ai_addr, ai->ai_addrlen);
    assert(r != -1);
    fprintf(stderr, "Got connection...\n");

    /* Write the message to the server. */
    r = write(s, argv[2], strlen(argv[2]) + 1);
    assert(r != -1);
    fprintf(stderr, "Wrote arg...\n");

    /* Read and print the echo response. */
    r = read(s, buf, strlen(argv[2]) + 1);
    assert(r == strlen(argv[2]) + 1);
    printf("%s\n", buf);

    return 0;
}
