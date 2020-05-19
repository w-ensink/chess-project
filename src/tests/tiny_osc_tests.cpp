//
// Created by Wouter Ensink on 13/05/2020.
//

#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#include <csignal>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <variant>

#include "../tinyosc/osc_interface.h"

static volatile bool keepRunning = true;

// handle Ctrl+C
static void sigintHandler(int x) {
    keepRunning = false;
}




/**
 * A basic program to listen to port 9000 and print received OSC packets.
 */
int main (int argc, char *argv[])
{
/*
    char buffer[2048]; // declare a 2Kb buffer to read packet data into

    printf("Starting write tests:\n");
    int len = 0;
    unsigned char blob[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    len = tosc_writeMessage(buffer, sizeof(buffer), "/address", "fsibTFNI",
                            1.0f, "hello world", -1, sizeof(blob), blob);

    tosc_printOscBuffer(buffer, len);

    printf("done.\n");

    // register the SIGINT handler (Ctrl+C)
    signal(SIGINT, &sigintHandler); // only for ctrl+c

    // open a socket to listen for datagrams (i.e. UDP packets) on port 9000
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    fcntl(fd, F_SETFL, O_NONBLOCK); // set the socket to non-blocking

    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(9000);
    sin.sin_addr.s_addr = INADDR_ANY;
    //struct sockaddr_out sout;

    bind(fd, (struct sockaddr *) &sin, sizeof(struct sockaddr_in));


    printf("tinyosc is now listening on port 9000.\n");
    printf("Press Ctrl+C to stop.\n");

    while (keepRunning)
    {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(fd, &readSet);
        struct timeval timeout = {1, 0}; // select times out after 1 second
        if (select(fd+1, &readSet, nullptr, nullptr, &timeout) > 0)
        {
            struct sockaddr sa; // can be safely cast to sockaddr_in
            socklen_t sa_len = sizeof(struct sockaddr_in);
            int len = 0;

            while ((len = (int) recvfrom(fd, buffer, sizeof(buffer), 0, &sa, &sa_len)) > 0)
            {
                if (tosc_isBundle(buffer))
                {
                    tosc_bundle bundle;
                    tosc_parseBundle(&bundle, buffer, len);
                    const uint64_t timetag = tosc_getTimetag(&bundle);
                    tosc_message osc;
                    while (tosc_getNextMessage(&bundle, &osc))
                    {
                        tosc_printMessage(&osc);
                    }
                }
                else
                {
                    tosc_message osc;
                    tosc_parseMessage(&osc, buffer, len);
                    tosc_printMessage(&osc);
                }
            }
        }
    }

    // close the UDP socket
    close(fd);
    */

    Local_OSC_Server server {9000};

    server.startListening ([] (std::unique_ptr<OSC_Message> m)
    {
        std::cout << "address: " << m->address << ", types: " << m->types << ", arguments: [";

        for (size_t i = 0; i < m->arguments.size(); ++i)
            std::cout << m->getIndexAsString (i) << ", ";

        std::cout << "]\n";
    });

    //std::cin.get();
    while(true)
    {
        sleep(5);
        std::cout << "Engine cycle\n";
    }
    server.stopListening();

    return 0;
}
