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

#include "NetworkWrapper.hpp"

/** @brief Constructs an empty socket object ready for general use.
 *
 * This constructor performs no initialization, since Connect() and Listen()
 * will themselves generate sockets for our use.
 *
 */
CS2Net::Socket::Socket()
{
    // We mark the socket as disconnected to remind us that we can't use it yet.
    // We'll mark it as connected or listener when we need to.
    this->fd = -1;
    this->state = SOCKETSTATE_DISCONNECTED;
}

/** @brief Wraps a Socket object around an existing socket file descriptor.
 *
 * This function is used by listener sockets to generate new Socket objects
 * for sockets corresponding to accepted connections.
 *
 * @warning INTERNAL USE ONLY. Do NOT attempt to invoke in your own code,
 * unless you know exactly what you're doing!
 *
 */

CS2Net::Socket::Socket(int sock_fd, SocketState sock_state_override)
{
    this->fd = sock_fd;
    if(this->fd == -1)
    {
        ERROR("Invalid socket file descriptor! %s", strerror(errno));
        // Mark this socket object as invalid
        this->state = SOCKETSTATE_BADSTATE;
        // There is no point in continuing if we don't have a real OS socket available!
        return;
    }
    this->state = sock_state_override;
}

/** @brief Deconstructs a Socket object.
 *
 * This function makes sure the socket has been closed cleanly before
 * the Socket wrapper object is deallocated.
 *
 */

CS2Net::Socket::~Socket()
{
    int err;
    // Make sure we shut down any open connection
    if(this->state == SOCKETSTATE_CONNECTED)
    {
        err = ::shutdown(this->fd, SHUT_RDWR);
        PREFER(err != -1, "Error encountered shutting down socket: %s", strerror(errno));
    }

    // Then close the socket file descriptor
    if(this->state != SOCKETSTATE_DISCONNECTED)
    {
        err = ::close(this->fd);
        PREFER(err != -1, "Error encountered closing socket file descriptor: %s", strerror(errno));
    }

    // All done!
}

/** @brief Connects a Socket object to a remote host.
 *
 * This function is magic: it does the hostname to IP address mapping,
 * converts everything to the correct byte order, and then attempts
 * to connect to a remote host. If it cannot connect to a remote host,
 * it returns -1 and sets errno to whatever socket() or connect() threw.
 *
 * @param[in] hostname the hostname to connect to.
 * @param[in] port the port to connect to.
 *
 * @bug Only the errno for the last entry attempted is preserved in the
 * case of a connection failure. This may not be informative by itself.
 *
 * @return 0 if successful. -1 and errno set on OS error on connect.
 *         -2 if connecting doesn't make sense with current state.
 *         -3 if couldn't even attempt to connect because lookup failed.
 */

int CS2Net::Socket::Connect(std::string * hostname, uint16_t port)
{
    // We should steer clear of any badstate sockets
    if(this->state == SOCKETSTATE_BADSTATE)
    {
        ERROR("Cannot invoke Connect() on a socket in a known bad state!");
        return -2;
    }

    // There is no point in connecting if we're already connected.
    if(this->state == SOCKETSTATE_CONNECTED || this->state == SOCKETSTATE_SSL)
    {
        ERROR("Cannot invoke Connect() on a connected socket");
        return -2;
    }

    // There is no point in invoking Connect() on a listener socket
    if(this->state == SOCKETSTATE_LISTENER)
    {
        ERROR("Cannot invoke Connect() on a listener socket");
        return -2;
    }

    struct addrinfo hints, *result, *rp;
    int s, sfd, __errno;

    // First do the hostname lookup!
    // We do this with the getaddrinfo() function.
    // This function returns every possible address corresponding to a given host.

    // Set up the hints structure.
    // This structure restricts the type of addresses we will be able to query for.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* Stream (TCP) socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;              /* Any protocol */

    // getaddrinfo() wants the port number as a string (const char *) for whatever reason.
    char port_str[8];
    snprintf(port_str, 8, "%hu", port);

    // Let's get ourselves some addrinfo.
    s = getaddrinfo(hostname->c_str(), port_str, &hints, &result);
    if(s != 0)
    {
        // If we couldn't get some addrinfo, tell us why and then error out.
        ERROR("Error doing hostname lookup: %s", gai_strerror(s));
        errno = s;
        return -3;
    }

    // Now we have a list of addrinfo structs that we can try to connect() to.
    // For each of them...
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        // First, we try to generate a socket of the specified type
        sfd = ::socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
        {
            // If we couldn't, then try the next entry
            continue;
        }

        // Then, we try to open a connection
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            // We opened a connection, yay.
            this->fd = sfd;
            this->state = SOCKETSTATE_CONNECTED;
            break;                  /* Success */
        }

        __errno = errno;

        // If we couldn't open a connection, get rid of the socket.
        close(sfd);
    }

    freeaddrinfo(result);

    // We check to see if we have any connection?
    if (this->fd == -1)
    {
        // We couldn't connect!
        ERROR("Couldn't connect to any host!");
        errno = __errno;
        return -1;
    }

    // We're connected now and can be happy.
    return 0;
}

/** @brief Disconnects this socket from its remote server.
 *
 * This function shuts down any open connection and then closes the
 * underlying OS socket.
 *
 * @return 0 on success; -1 and errno set on underlying error;
 * -2 if closing this socket does not make sense.
 */

int CS2Net::Socket::Disconnect()
{
    // There is no point in disconnecting a socket that already is.
    if (this->state == SOCKETSTATE_DISCONNECTED)
    {
        ERROR("Cannot invoke Disconnect() on a disconnected socket!");
        return -2;
    }

    // There is no point in disconnecting a socket in a bad state.
    if (this->state == SOCKETSTATE_BADSTATE)
    {
        ERROR("Cannot invoke Disconnect() on a socket in bad state!");
        return -2;
    }

    // There is no point in disconnecting a listener socket.
    if (this->state == SOCKETSTATE_LISTENER)
    {
        ERROR("Cannot invoke Disconnect() on a listener socket!");
        return -2;
    }

    int e;

    // Shutdown the open connection
    e = ::shutdown(this->fd, SHUT_RDWR);
    if(e == -1)
    {
        ERROR("Error shutting down open connection: %s", strerror(errno));
        //return -1;
    }

    // Close the socket
    e = ::close(this->fd);
    if(e == -1)
    {
        ERROR("Error closing socket: %s", strerror(errno));
        //return -1;
    }

    // All done!
    this->fd = -1;
    this->state = SOCKETSTATE_DISCONNECTED;
    return 0;

}

/** @brief Sends a string on an open connection.
 *
 * This function sends the contents of a std::string to this socket
 * if there is an open connection.
 *
 * This function blocks until all of the data can be sent.
 * Non-blocking I/O is outside the scope of the assignment this wrapper
 * was intended for, and thus is not supported by the wrapper.
 *
 * This function does _not_ deallocate the string. That operation is
 * the responsibility of the caller.
 *
 * @param[in] to_send the string to send.
 *
 * @return the number of bytes transferred, -1 on error,
 * -2 if sending does not make sense.
 */

int CS2Net::Socket::Send(std::string * to_send)
{
    // There is no point in sending data on disconnected socket.
    if (this->state == SOCKETSTATE_DISCONNECTED)
    {
        ERROR("Cannot invoke Send() on a disconnected socket!");
        return -2;
    }

    // There is no point in sending on a socket in a bad state.
    if (this->state == SOCKETSTATE_BADSTATE)
    {
        ERROR("Cannot invoke Send() on a socket in bad state!");
        return -2;
    }

    // There is no point in sending on a listener socket.
    if (this->state == SOCKETSTATE_LISTENER)
    {
        ERROR("Cannot invoke Send() on a listener socket!");
        return -2;
    }

    ssize_t sent;

    // Let's send some message.
    sent = ::send(this->fd, to_send->data(), to_send->length(), 0);

    // Print obvious errors.
    if(sent == -1)
    {
        ERROR("Error on Send(): %s", strerror(errno));
    }
    // Catch not-so-obvious errors.
    else if(sent < to_send->length())
    {
        WARN("Send() reports partial send (%d of %d bytes). %s", sent, to_send->length(), strerror(errno));
    }
    else {
        // Nothing to worry about here.
    }

    // All done!
    return sent;

}

/** @brief Receives a string of the specified length on an open connection.
 *
 * This function pulls in the requested amount of data and stores it in
 * a new std::string that must be deallocated when no longer used.
 *
 * This function blocks until any data is received.
 * Optionally, it may block until all requested data is received or an
 * error occurs.
 *
 * @param[in] length the amount of data requested.
 * @param[in] block_while_buffer whether or not to wait for all requested data.
 *
 * @return a new std::string with the requested data, or NULL (and set errno) on error.
 *
 */
std::string * CS2Net::Socket::Recv(size_t length, bool block_while_buffer)
{
    int __errno = 0;

    // There is no point in recving data on disconnected socket.
    if (this->state == SOCKETSTATE_DISCONNECTED)
    {
        ERROR("Cannot invoke Recv() on a disconnected socket!");
        errno = ENOTCONN;
        return NULL;
    }

    // There is no point in recving on a socket in a bad state.
    if (this->state == SOCKETSTATE_BADSTATE)
    {
        ERROR("Cannot invoke Recv() on a socket in bad state!");
        errno = EOPNOTSUPP;
        return NULL;
    }

    // There is no point in recving on a listener socket.
    if (this->state == SOCKETSTATE_LISTENER)
    {
        ERROR("Cannot invoke Recv() on a listener socket!");
        errno = EOPNOTSUPP;
        return NULL;
    }

    // Let's set up our recv() flags
    int flags = (block_while_buffer ? MSG_WAITALL : 0);

    // Set up a temporary buffer to hold our data
    char * buf = new char[length];

    // Now pull in our data
    ssize_t rcvd = ::recv(this->fd, (void *)buf, length, flags);

    // Print obvious errors.
    if(rcvd == -1)
    {
        __errno = errno;
        ERROR("Error on Recv(): %s", strerror(errno));
    }
    // Catch not-so-obvious errors.
    else if(rcvd < length && errno != 0)
    {
        __errno = errno;
        WARN("Recv() received fewer bytes than requested: %s", strerror(errno));
    }
    else
    {
        // Nothing to worry about here.
    }

    // Construct a string for all the received data (if any)
    std::string * to_return = NULL;
    if(rcvd >= 0)
    {
        to_return = new std::string(buf, rcvd);
    }

    // Restore any saved errors
    errno = __errno;

    // Deallocate temp buffers
    delete[] buf;

    // All done!
    return to_return;
}

/** @brief Binds a socket and starts listening.
 *
 * This function attempts to create and bind a socket on every network
 * interface (returned by getaddrinfo() in order). The first successful
 * bind is kept. If no available address could be bound, then an error
 * is returned.
 *
 * @param[in] port the port to which to bind.
 * @param[in] backlog the number of pending connections to allow.
 *
 * @return 0 on success; -1 or errno set on error; -2 if bind does not
 * make sense.
 */
int CS2Net::Socket::Bind(uint16_t port, int backlog)
{
    // There is no point in binding a socket that is not disconnected
    if(this->state != SOCKETSTATE_DISCONNECTED)
    {
        ERROR("Cannot bind a non-disconnected socket!");
        return -2;
    }

    struct addrinfo hints, *result, *rp;
    int sfd, s, err, __errno;

    // The hints struct gives getaddrinfo() clues as to what we want.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* STREAM socket */
    hints.ai_flags = AI_PASSIVE;        /* For wildcard IP address */
    hints.ai_protocol = 0;              /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // getaddrinfo() wants the port number as a string (const char *) for whatever reason.
    char port_str[8];
    snprintf(port_str, 8, "%hu", port);

    DEBUG("port: %s", port_str);

    // Let's get ourselves some addrinfo.
    s = getaddrinfo(NULL, port_str, &hints, &result);
    if(s != 0)
    {
        // If we couldn't get some addrinfo, tell us why and then error out.
        ERROR("Error doing hostname lookup: %s", gai_strerror(s));
        errno = s;
        return -1;
    }

    // Now we have a list of addrinfo structs that we can try to connect() to.
    // For each of them...
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        // First, we try to generate a socket of the specified type
        sfd = ::socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
        {
            // If we couldn't, then try the next entry
            DEBUG("couldn't socket: %s", strerror(errno));
            continue;
        }

        // Set SO_REUSEADDR to avoid issues if we terminate and then
        // immediately restart the server.
        int foo = 1;
        err = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &foo, sizeof(int));
        if(err == -1)
        {
            // could not set SO_REUSEADDR
            __errno = errno;
            DEBUG("couldn't so_reuseaddr: %s", strerror(errno));
        }

        // Then, we try the real bind
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            // We bound a socket, yay.
            // Now try to start listening.
            err = ::listen(sfd, backlog);
            if(err == -1)
            {
                // can't listen; this socket is no good
                __errno = errno;
                DEBUG("couldn't listen: %s", strerror(errno));
            }
            else
            {
                // listening loud and clear
                this->fd = sfd;
                this->state = SOCKETSTATE_LISTENER;
                break;                  /* Success */
            }
        }
        else
        {
            __errno = errno;
        }

        // If we couldn't open a connection, get rid of the socket.
        close(sfd);

    }

    freeaddrinfo(result);

    // We check to see if we have any successful bind?
    if (this->fd == -1)
    {
        // We couldn't bind!
        ERROR("Couldn't bind to any network interface!");
        errno = __errno;
        return -1;
    }

    // We're bound now and can be happy.
    return 0;

}

/** @brief Accepts a connection on a bound socket.
 *
 * This function requires that the socket be both bound and listening,
 * though we take care of the listen() portion automatically upon bind.
 *
 * This function blocks until an incoming connection is received.
 * Use Poll() to determine whether a connection is waiting to be accepted.
 *
 * @return a new socket object, or NULL (and errno set) on error.
 *
 */
CS2Net::Socket * CS2Net::Socket::Accept()
{
    int _errno = 0;
    // There is no point in accept()ing on a nonlistener socket
    if(this->state != SOCKETSTATE_LISTENER)
    {
        ERROR("Attempted to accept() on a nonlistener socket!");
        errno = EOPNOTSUPP;
        return NULL;
    }

    int incoming_fd;

    // Try to accept a connection.
    incoming_fd = ::accept(this->fd, NULL, NULL);

    // Did we accept a connection?
    if(incoming_fd == -1)
    {
        _errno = errno;
        ERROR("Failed to accept connection: %s", strerror(errno));
        errno = _errno;
        return NULL;
    }

    // We have accepted a connection. Wrap it in a socket and return
    return new Socket(incoming_fd, SOCKETSTATE_CONNECTED);

}

/** @brief Returns the address of the remote end of a connected socket.
 *
 * This function discovers the address of the other end of the socket
 * and returns it as a string.
 *
 * @return the address as string, or NULL on error.
 */
std::string * CS2Net::Socket::GetRemoteAddr()
{
    // There is no point in querying a socket that is not connected
    if(this->state != SOCKETSTATE_CONNECTED && this->state != SOCKETSTATE_SSL)
    {
        ERROR("Cannot retrieve remote address on a non-connected socket!");
        return NULL;
    }

    struct sockaddr a;
    char * host_buf, * port_buf;
    int err, _errno;
    socklen_t a_len = sizeof(a);

    // Get the host information from the socket
    err = ::getpeername(this->fd, &a, &a_len);
    if(err == -1)
    {
        _errno = errno;
        ERROR("Error retrieving socket remote name: %s", strerror(errno));
        errno = _errno;
        return NULL;
    }

    host_buf = new char[64];
    port_buf = new char[8];

    // Translate the addrinfo to host/port strings
    err = ::getnameinfo(&a, a_len, host_buf, 64, port_buf, 8, NI_NUMERICHOST | NI_NUMERICSERV);
    if(err != 0)
    {
        _errno = errno;
        ERROR("Error looking up host: %s", gai_strerror(err));
        errno = _errno;
        return NULL;
    }

    // Assemble the host/port strings into something readable
    std::string * s = new std::string();
    s->append(host_buf);
    s->append(":");
    s->append(port_buf);

    // Clean up
    delete[] host_buf;
    delete[] port_buf;

    // Return the generated string (all done!)
    return s;

}

/** @brief Polls a set of sockets to see if they are ready for reading or writing.
 *
 * This function takes a vector of PollFD structs (structs that encapsulate file
 * descriptors along with bitfields that specify which events should be listened
 * for, and updates the vector in place, setting the appropriate bits in the
 * bitfield designated for returned events.
 *
 * This function blocks for up to the specified amount of time.
 *
 * @param to_poll the vector of PollFD structs on which to perform polling.
 * @param timeout_ms the maximum duration for which to wait (-1 == infinity)
 *
 * @return the number of file descriptors ready, or -1 on error.
 */
int CS2Net::Poll(std::vector<PollFD> * to_poll, int timeout_ms)
{
    int err, __errno, i;

    // Translate all of the vector elements into an array of the appropriate type
    // this operation does not change the order of elements; will be important later
    struct pollfd * fds = new struct pollfd[to_poll->size()];

    for(i = 0; i < to_poll->size(); i++)
    {
        fds[i].fd       = (*to_poll)[i].sock->GetSocketFileDescriptor();
        fds[i].events   = (*to_poll)[i].requested_events;
        fds[i].revents  = 0;
    }

    // Now do the poll call
    err = ::poll(fds, to_poll->size(), timeout_ms);
    if(err == -1)
    {
        __errno = errno;
        ERROR("Error during poll(): %s", strerror(errno));
        delete[] fds;
        errno = __errno;
        return -1;
    }

    // Now translate the elements of the returned array back to the original vector
    for(i = 0; i < to_poll->size(); i++)
    {
        (*to_poll)[i].returned_events = fds[i].revents;
    }

    // Finally return the number of ready file descriptors
    delete[] fds;
    return err;
}
