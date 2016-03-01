/**
* @file error.cpp
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2013
* @copyright see License section
*
* @brief Defines error printing features.
*
* @section License
* Copyright (c) 2012-2013 California Institute of Technology.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The views and conclusions contained in the software and documentation are those
* of the authors and should not be interpreted as representing official policies,
* either expressed or implied, of the California Institute of Technology.
*
*/

#include "error.hpp"

/**
* @brief Prints out an error message at the specified error level.
*
*
*/
void vprintf_error(const char * file, int lineno, int level, const char * msg, va_list fmtargs)
{
    const char * type;
    switch(level)
    {
        case ERR_FATAL:
            type = "FATAL";
            break;
        case ERR_ERROR:
            type = "ERROR";
            break;
        case ERR_WARN:
            type = "WARN";
            break;
        case ERR_INFO:
            type = "INFO";
            break;
        case ERR_DEBUG:
            type = "DEBUG";
            break;
        default:
            type = "?????";
            break;
    }

    fprintf(stderr, "[%5.5s] %s:%d: ", type, file, lineno);
    vfprintf(stderr, msg, fmtargs);
    fprintf(stderr, "\n");
}
