# Demo TCP echo client and server

Copyright © 2012 Bart Massey

This is a little demo I wrote in my networking class to
illustrate UNIX/Linux TCP programming. It's just like the
hundreds like it all over the Internet. Enjoy.

Server usage:

            echoserver

Client usage:

            echoclient <server-ip-address> <message-to-echo>

The server IP address may be given to the client numerically
or via DNS name, and is usually localhost. The server port
is hardcoded to 2041. Both the client and the server can
connect via either IPv4 or IPv6.

This work is available under the MIT License.  Please see
the file COPYING in this distribution for license terms.
