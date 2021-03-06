/************************************************************************************//**
// Copyright (c) 2006-2015 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
****************************************************************************************/
#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
    extern "C"
#endif

void error_output(const char* fmt, ...);

#ifdef __GNUC__
#define warnf(args)
#define errorf(args)
#define assertf(cond, args)
#else
#define warnf(args) do { \
        error_output("warning: %s: %d: ", __FILE__, __LINE__); \
        error_output args; \
        error_output("\n"); \
    } while(0)

#define errorf(args) do { \
        error_output("error: %s: %d: ", __FILE__, __LINE__); \
        error_output args; \
        error_output("\n"); \
    } while(0)

#define assertf(cond, args) do { \
        if (!(cond)) { \
            error_output("assert: %s: %d: ", __FILE__, __LINE__); \
            error_output args; \
            error_output("\n"); \
            exit(1); \
        } \
    } while(0)
#endif

#ifndef ERROR_NDEBUG
#ifdef __GNUC__
#define debugf(args)
#else
#define debugf(args) do { \
        const char *s = strrchr(__FILE__, '/'); \
        if(!s) \
            s = strrchr(__FILE__, '\\'); \
        if(s) \
            s++; \
        else \
            s = __FILE__; \
        error_output("debug: %s: %d: ", s, __LINE__); \
        error_output args; \
        error_output("\n"); \
    } while(0)
#endif
#else
#define debugf
#endif

#endif
