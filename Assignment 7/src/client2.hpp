/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
The client executable (including GUI).

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

#ifndef __CLIENT2_H__
#define __CLIENT2_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <list>

#include <gtk/gtk.h>

#include "NetworkWrapper.hpp"
#include "CS2ChatProtocol.hpp"

/* TODO: declare any extra classes here */

// Function is declared here for unit test purposes and to prevent the additon
// of a whole new source file.
/**
 * @brief Creates and returns an encoded network message.
 *
 * Given a message type and a message payload, this function constructs a
 * string to send over the network.  This function is unit tested by the
 * accompanying testsuite.
 *
 * @attention This function is student-implemented.
 * @return string The encoded network message to send over the network.
 */
std::string EncodeNetworkMessage(MESSAGE_TYPE type, std::string * payload)
{
    // TODO: fix this.
    return std::string();
}

#endif
