//
// Created by Wouter Ensink on 14/05/2020.
//

#ifndef CHESS_ENGINE_OSC_INTERFACE_H
#define CHESS_ENGINE_OSC_INTERFACE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <thread>
#include <type_traits>
#include <fcntl.h>
#include "tinyosc.h"


class Local_OSC_Client
{
public:

    explicit Local_OSC_Client (unsigned int port) : portNumber {port}, messageBuffer {0}, serverAddress {0}
    {
        if ((serverFileDescriptor = socket (AF_INET, SOCK_DGRAM, 0)) == 0)
        {
            std::cout << "Local_OSC_Client: Error getting Server File Descriptor\n";
            return;
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons (port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        if (connect (serverFileDescriptor, (struct sockaddr*) &serverAddress, sizeof (serverAddress)) < 0)
        {
            std::cout << "Local_OSC_Client: Failed to connect to Server\n";
            return;
        }
    }

    ~Local_OSC_Client()
    {
        close (serverFileDescriptor);
    }

    template <typename... Args>
    auto sendMessage (const std::string& address, const std::string& types, Args... args)
    {
        auto messageLength = tosc_writeMessage (messageBuffer, sizeof (messageBuffer), address.c_str(), types.c_str(), args...);
        send (serverFileDescriptor, messageBuffer, messageLength, 0);

        std::cout << "Local_OSC_Client: Message sent to port " << portNumber << std::endl;
    }


private:

    unsigned int portNumber;
    char messageBuffer [1024];
    int serverFileDescriptor;
    struct sockaddr_in serverAddress;
};


//===================================================================================================


struct OSC_Message
{
    using ArgumentsList = std::vector<std::variant<long, double, std::string>>;

    std::string address;
    std::string types;
    ArgumentsList arguments;


    template <typename T>
    auto getTypeAtIndex (size_t index) noexcept -> T
    {
        if (std::holds_alternative<T> (arguments[index]))
            return std::get<T> (arguments[index]);

        std::cout << "The type you try to index does not exist at index "
            << index << " the types in this message are: " << types << std::endl;

        if constexpr (std::is_same<T, std::string>::value)
            return "[undefined]";
        else
            return static_cast<T> (0);
    }


    auto getIndexAsString (size_t index) -> std::string
    {
        switch (types[index])
        {
            case 'i':
            case 'h': return std::to_string (getTypeAtIndex<long>(index));
            case 'f':
            case 'd': return std::to_string (getTypeAtIndex<double>(index));
            case 's': return getTypeAtIndex<std::string> (index);
            default: return "[impossible]";
        }
    }
};


//===================================================================================================


class Local_OSC_Server
{
public:

    explicit Local_OSC_Server (unsigned int port) : port {port}
    {
        fileDescriptor = socket (AF_INET, SOCK_DGRAM, 0);
        fcntl (fileDescriptor, F_SETFL, O_NONBLOCK);
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons (port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        bind (fileDescriptor, (struct sockaddr*) &serverAddress, sizeof (struct sockaddr_in));
    }


    ~Local_OSC_Server()
    {
        shouldKeepListening = false;
        listener.join();
        close (fileDescriptor);
    }


    void startListening (std::function<void(std::unique_ptr<OSC_Message>)> callback)
    {
        shouldKeepListening = true;

        listener = std::thread ([this, callback = std::move (callback)]
        {
            while (shouldKeepListening)
            {
                fd_set readSet;
                FD_ZERO (&readSet);
                FD_SET (fileDescriptor, &readSet);
                struct timeval timeout = {1, 0};

                if (select (fileDescriptor + 1, &readSet, nullptr, nullptr, &timeout) > 0)
                {
                    struct sockaddr sa {};
                    socklen_t sa_len = sizeof (struct sockaddr_in);
                    int len = 0;

                    while ((len = (int) recvfrom (fileDescriptor, messageBuffer, sizeof (messageBuffer), 0, &sa, &sa_len)) > 0)
                    {
                        if (! tosc_isBundle (messageBuffer))
                        {
                            tosc_message osc;
                            tosc_parseMessage (&osc, messageBuffer, len);
                            callback (parseOSC_Message (osc));
                        }
                    }
                }
            }
        });
    }


    void stopListening()
    {
        shouldKeepListening = false;
    }

private:

    std::atomic<bool> shouldKeepListening {false};
    std::thread listener;
    char messageBuffer [1024] = {0};
    unsigned int port;
    int fileDescriptor;
    struct sockaddr_in serverAddress {};


    static std::unique_ptr<OSC_Message> parseOSC_Message (tosc_message& message)
    {
        std::string types = tosc_getFormat (&message);
        auto m = std::make_unique<OSC_Message>();
        m->address = tosc_getAddress (&message);
        m->types.reserve (types.length());

        for (auto t : types)
        {
            switch (t)
            {
                case 's': { m->arguments.emplace_back(std::string {tosc_getNextString(&message)});  break; }
                case 'i': { m->arguments.emplace_back((long)tosc_getNextInt32(&message));           break; }
                case 'h': { m->arguments.emplace_back((long)tosc_getNextInt64(&message));           break; }
                case 'f': { m->arguments.emplace_back(tosc_getNextFloat(&message));                 break; }
                case 'd': { m->arguments.emplace_back(tosc_getNextDouble(&message));                break; }
                default: break;
            }
        }

        m->types = std::move (types);
        return m;
    }
};

#endif //CHESS_ENGINE_OSC_INTERFACE_H
