/*
 * Copyright © 2012 Bart Massey
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/*
 * TCP Echo Service
 */

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "echo.h"

/* Handle a SIGCHLD by collecting the child. */
void sigchild(int sig) {
    pid_t pid = wait(0);
    assert(pid != -1);
    printf("Server reaped child %d\n", pid);
}

int main(int argc, char **argv) {
    int r, s;
    struct sockaddr_in addr;
    void (*sr)(int);

    /* Check for proper usage. */
    assert(argc == 1);

    /* Set up the sigchild handler. */
    sr = signal(SIGCHLD, sigchild);
    assert(sr != SIG_ERR);

    /* Create the master socket. */
    s = socket(AF_INET, SOCK_STREAM, 0);
    assert(s != -1);

    /* Bind the socket to an IP address. */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    r = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    assert(r != -1);

    /* Set up the socket to listen, with a queue depth of 99. */
    r = listen(s, 99);
    assert(r != -1);

    /* Server loop: spawn a child to handle each connection. */
    printf("Starting server...\n");
    while (1) {
        /* Wait for a connection. */
        int pid;
        int s0 = accept(s, 0, 0);
        assert(s0 != -1);
        printf("Got connection...\n");

        /* Fork a child to handle the connection. */
        pid = fork();
        assert(pid != -1);
        if (pid == 0) {
            /* Fork is in child process: handle connection. */
            pid = getpid();
            printf("Child %d\n", pid);

            /* Echo loop: Receive and retransmit characters. */
            while(1) {
                /* Read a buffer full of characters. */
                char buf[512];
                r = read(s0, buf, sizeof(buf));
                assert(r != -1);
                printf("Child %d: read %d bytes\n", pid, r);

                /* If the read returned no characters, the
                   connection has been closed. */
                if (r == 0)
                    break;

                /* Transmit back the characters received. */
                r = write(s0, buf, r);
                assert(r != -1);
                printf("Child %d: wrote %d bytes\n", pid, r);
            }

            /* When the other end closes, close our end. */
            printf("Child %d: connection closed\n", pid);
            (void) close(s0);

            /* Exits our child process. */
            return 0;
        }
    }
    return 0;
}
