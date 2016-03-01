/**
@file
@author Solomon Chang
@date 2013
@copyright 2-clause BSD; see License section

@brief
Performs tests of the EncodeNetworkMessage function.

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

#include "client2.hpp"
#include <stdio.h>
#include <string>

int main()
{
    // NOTE TO STUDENTS: You should not construct your encoded network messages
    // as done below as we have hard coded the lengths and message contents.
    // We're simply testing that your code produces a known correct result.

    // Test the construction of a server message.
    std::string message_contents("four");
    std::string result =
      EncodeNetworkMessage(MSG_SERVER_MESSAGE, &message_contents);
    // The message contents is of length 4, 4 in little endian format is
    // 04 00. Additionally, 0x00 is the tag for MSG_SERVER_MESSAGE.
    std::string building_result = {0x00, 0x04, 0x00, 'f', 'o', 'u', 'r'};
    if (result != building_result)
    {
      printf("Incorrect server message encoding!!!\n");
    }
    else
    {
      printf("Correct server message encoding!!!\n");
    }

    // Test the construction of a chat message.
    message_contents = "foo \n bar \t baz /?!@#$%\\qux";
    result = EncodeNetworkMessage(MSG_CHATMSG, &message_contents);
    // The message contents is of length 27, 27 is a little endian format is
    // 1b 00.  Additionally, 0x20 is the tag for MSG_CHATMSG.
    building_result = {0x20, 0x1b, 0x00, 'f', 'o', 'o', ' ', '\n', ' ', 'b',
      'a', 'r', ' ', '\t', ' ', 'b', 'a', 'z', ' ', '/', '?', '!', '@', '#',
      '$', '%', '\\', 'q', 'u', 'x'};
    if (result != building_result)
    {
      printf("Incorrect chat message encoding!!!\n");
    }
    else
    {
      printf("Correct chat message encoding!!!\n");
    }

    return 0;
}
