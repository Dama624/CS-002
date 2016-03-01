/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
A wrapper around OS-specific networking routines,
intended to simplify network programming for new students.

@section License

Copyright (c) 2012-2013 California Institute of Technology.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the California Institute of Technology.

*/

#ifndef __CS2NET_NETWORKWRAPPER_H__
#define __CS2NET_NETWORKWRAPPER_H__

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <unistd.h>

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <poll.h>
#include <errno.h>

#include "error.hpp"

namespace CS2Net
{

    enum SocketState
    {
        SOCKETSTATE_DISCONNECTED,
        SOCKETSTATE_CONNECTED,
        SOCKETSTATE_LISTENER,
        SOCKETSTATE_SSL,
        SOCKETSTATE_BADSTATE,
    };

    class Socket
    {

    private:
        int fd;
        SocketState state;

    public:
        Socket();
        Socket(int sock_fd, SocketState sock_state_override = SOCKETSTATE_CONNECTED);

        ~Socket();

        int Connect(std::string * hostname, uint16_t port);
        int Disconnect();

        int Send(std::string * to_send);
        std::string * Recv(size_t length, bool block_while_buffer);

        std::string * GetRemoteAddr();

        int Bind(uint16_t port, int backlog);
        Socket * Accept();

        int GetSocketFileDescriptor() { return fd; }

    };

    /** @brief A wrapping class around a Socket object used for the Poll function.
     *
     */
    struct PollFD
    {
        Socket * sock;          //! a Socket object to poll
        short requested_events; //! a bitfield designating which events to request
        short returned_events;  //! a bitfield containing the results of a Poll() call

        PollFD()
        {
            this->sock = NULL;
            this->requested_events = 0;
            this->returned_events = 0;
        }

        PollFD(Socket * sock, short requested_events)
        {
            this->sock = sock;
            this->requested_events = requested_events;
            this->returned_events = 0;
        }

        void SetRead(bool r)
        {
            if(r)
            {
                this->requested_events |= POLLIN;
            }
            else
            {
                this->requested_events &= (~POLLIN);
            }
        }
        void SetPriRead(bool r)
        {
            if(r)
            {
                this->requested_events |= POLLPRI;
            }
            else
            {
                this->requested_events &= (~POLLPRI);
            }
        }
        void SetWrite(bool w)
        {
            if(w)
            {
                this->requested_events |= POLLOUT;
            }
            else
            {
                this->requested_events &= (~POLLOUT);
            }
        }
        bool CanRead()
        {
            return (this->returned_events & POLLIN);
        }
        bool CanPriRead()
        {
            return (this->returned_events & POLLPRI);
        }
        bool CanWrite()
        {
            return (this->returned_events & POLLOUT);
        }
        bool HasError()
        {
            return (this->returned_events & POLLERR);
        }
        bool HasHangup()
        {
            return (this->returned_events & POLLHUP);
        }

    };

    int Poll(std::vector<PollFD> * to_poll, int timeout_ms);

}

#endif
