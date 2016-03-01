/**
* @file error.hpp
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

#ifndef ERROR_H
#define ERROR_H

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

// Error levels
#define ERR_FATAL 0
#define ERR_ERROR 1
#define ERR_WARN  2
#define ERR_INFO  3
#define ERR_DEBUG 4

// What level of error messages do we want?
#define ERRORLEVEL ERR_DEBUG

// Error display functions
void vprintf_error(const char * file, int lineno, int level, const char * msg, va_list fmtargs);
inline void printf_error(const char * file, int lineno, int level, const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    vprintf_error(file, lineno, level, msg, args);
    va_end(args);
}

#if ERRORLEVEL >= ERR_FATAL
#define FATAL(args...) printf_error(__FILE__, __LINE__, ERR_FATAL, args)
#define VFATAL(msg, file, line, fmtargs) vprintf_error(file, line, ERR_FATAL, msg, fmtargs)
#else
#define FATAL(args...)
#define VFATAL(msg, file, line, fmtargs)
#endif

#if ERRORLEVEL >= ERR_ERROR
#define ERROR(args...) printf_error(__FILE__, __LINE__, ERR_ERROR, args)
#define VERROR(msg, file, line, fmtargs) vprintf_error(file, line, ERR_ERROR, msg, fmtargs)
#else
#define ERROR(args...)
#define VERROR(msg, file, line, fmtargs)
#endif

#if ERRORLEVEL >= ERR_WARN
#define WARN(args...) printf_error(__FILE__, __LINE__, ERR_WARN, args)
#define VWARN(msg, file, line, fmtargs) vprintf_error(file, line, ERR_WARN, msg, fmtargs)
#else
#define WARN(args...)
#define VWARN(msg, file, line, fmtargs)
#endif

#if ERRORLEVEL >= ERR_INFO
#define INFO(args...) printf_error(__FILE__, __LINE__, ERR_INFO, args)
#define VINFO(msg, file, line, fmtargs) vprintf_error(file, line, ERR_INFO, msg, fmtargs)
#else
#define INFO(args...)
#define VINFO(msg, file, line, fmtargs)
#endif

#if ERRORLEVEL >= ERR_DEBUG
#define DEBUG(args...) printf_error(__FILE__, __LINE__, ERR_DEBUG, args)
#define VDEBUG(msg, file, line, fmtargs) vprintf_error(file, line, ERR_DEBUG, msg, fmtargs)
#else
#define DEBUG(args...)
#define VDEBUG(msg, file, line, fmtargs)
#endif

// Convenience inline functions.
/**
 * @brief Terminates if stmt is not true.
 *
 * This is the strongest error-printing shortcut,
 * and should be used only for absolutely unrecoverable errors.
 * It terminates the entire program if stmt is not true.
 *
 * @param stmt the statement to check.
 * @param msg a helpful message to print out.
 *
 * @return the value of stmt.
 *
 */
inline bool __REQUIRE(bool stmt, const char * file, int line, const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    if(!(stmt))
    {
        VFATAL(msg, file, line, args);
        exit(1);
    }
    va_end(args);
    return stmt;
}

/**
 * @brief Prints an error message if stmt is not true.
 *
 * This function should be used for errors that cause a state where
 * an individual routine may not be able to proceed, but where the
 * rest of the program may continue or handle the error.
 *
 * @param stmt the statement to check.
 * @param msg a helpful message to print out.
 *
 * @return the value of stmt.
 *
 */
inline bool __ASSERT(bool stmt, const char * file, int line, const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    if(!(stmt))
    {
        VERROR(msg, file, line, args);
    }
    va_end(args);
    return stmt;
}

/**
 * @brief Prints a warning if stmt is not true.
 *
 * This function should be used for errors through which an
 * individual routine may be able to continue.
 * This is the weakest convenience function.
 *
 * @param stmt the statement to check.
 * @param msg a helpful message to print out.
 *
 * @return the value of stmt.
 *
 */
inline bool __PREFER(bool stmt, const char * file, int line, const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    if(!(stmt))
    {
        VWARN(msg, file, line, args);
    }
    va_end(args);
    return stmt;
}

#define REQUIRE(stmt, args...) __REQUIRE(stmt, __FILE__, __LINE__, args)
#define ASSERT(stmt, args...) __ASSERT(stmt, __FILE__, __LINE__, args)
#define PREFER(stmt, args...) __PREFER(stmt, __FILE__, __LINE__, args)

#endif
