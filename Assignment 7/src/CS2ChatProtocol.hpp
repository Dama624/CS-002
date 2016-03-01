/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
The chat protocol description.

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

#ifndef __CS2_CHAT_PROTOCOL_H__
#define __CS2_CHAT_PROTOCOL_H__

typedef unsigned char MESSAGE_TYPE;

// This message is a message from the server. The payload is a text string.
// These messages should be prepended with "Server Message", then displayed.
#define MSG_SERVER_MESSAGE 0x00

// This message is a message from the client. The payload is a text string.
// The payload is the username the client wishes to use;
// it must contain no newlines.
#define MSG_AUTH_USERNAME 0x10

// This message is a message from the server. There is no payload.
// This message signals that the username requested is valid.
#define MSG_AUTH_OK 0x18

// This message is a message from the server. The payload is a text string.
// This message signals that the username requested is invalid.
#define MSG_AUTH_ERROR 0x19

// This message is a message from either side. The payload is a text string.
// The payload of this message should be displayed verbatim on its own line.
#define MSG_CHATMSG 0x20

// This message is a message from the server. The payload is a text string.
// The payload contains a newline-delimited list of usernames.
// The payload should be parsed, then loaded into the client user list.
#define MSG_USERLIST 0x21

// This message is a message from the server. The payload is a text string.
// These messages should be prepended with "Server Error", then displayed.
#define MSG_GENERAL_ERROR 0xFF

#endif



