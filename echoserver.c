/*
 * Copyright © 2012 Bart Massey
 * TCP Echo Service
 */

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

int main() {
    int r;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    assert(s != -1);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2041);
    addr.sin_addr.s_addr = INADDR_ANY;
    r = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    assert(r != -1);
    r = listen(s, 99);
    assert(r != -1);
    printf("Starting server...\n");
    while (1) {
        int s0 = accept(s, 0, 0);
        assert(s0 != -1);
        printf("Got connection...\n");
        while(1) {
            char buf[512];
            r = read(s0, buf, 512);
            assert(r != -1);
            printf("Read %d bytes\n", r);
            if (r == 0)
                break;
            r = write(s0, buf, r);
            assert(r != -1);
            printf("Wrote bytes\n");
        }
        printf("Connection closed\n");
    }
    return 0;
}
