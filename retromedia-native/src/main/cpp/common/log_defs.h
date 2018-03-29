/*
 *  Copyright (c) 2014 The linux_utility_wrapper project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef _LOGD_H_
#define _LOGD_H_
#ifndef NULL
#define NULL 0
#endif  
#include "log.h"
#ifdef __cplusplus
#include <string.h>
#include <strings.h>
#include <sstream>
extern "C"
{
#endif

// ---------------------------------------------------------------------

/*
 * Normally we strip ALOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define LOG_NDEBUG 0"
 * at the top of your source file) to change that behavior.
 */
#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

/*
 * This is the local tag used for the following simplified
 * logging macros.  You can change this preprocessor definition
 * before using the other macros to change the tag.
 */
#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

// ---------------------------------------------------------------------

/*
 * Simplified macro to send a verbose log message using the current LOG_TAG.
 */
#ifndef ALOGV
#if LOG_NDEBUG
#define ALOGV(...)   ((void)0)
#else
#define ALOGV(...) ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#endif
#endif

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#ifndef ALOGV_IF
#if LOG_NDEBUG
#define ALOGV_IF(cond, ...)   ((void)0)
#else
#define ALOGV_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a debug log message using the current LOG_TAG.
 */
#ifndef ALOGD
#define ALOGD(...) ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGD_IF
#define ALOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an info log message using the current LOG_TAG.
 */
#ifndef ALOGI
#define ALOGI(...) ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGI_IF
#define ALOGI_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning log message using the current LOG_TAG.
 */
#ifndef ALOGW
#define ALOGW(...) ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGW_IF
#define ALOGW_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message using the current LOG_TAG.
 */
#ifndef ALOGE
#define ALOGE(...) ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGE_IF
#define ALOGE_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

// ---------------------------------------------------------------------

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * verbose priority.
 */
#ifndef IF_ALOGV
#if LOG_NDEBUG
#define IF_ALOGV() if (false)
#else
#define IF_ALOGV() IF_ALOG(LOG_VERBOSE, LOG_TAG)
#endif
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * debug priority.
 */
#ifndef IF_ALOGD
#define IF_ALOGD() IF_ALOG(LOG_DEBUG, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * info priority.
 */
#ifndef IF_ALOGI
#define IF_ALOGI() IF_ALOG(LOG_INFO, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * warn priority.
 */
#ifndef IF_ALOGW
#define IF_ALOGW() IF_ALOG(LOG_WARN, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * error priority.
 */
#ifndef IF_ALOGE
#define IF_ALOGE() IF_ALOG(LOG_ERROR, LOG_TAG)
#endif

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

/*
 * Log a fatal error.  If the given condition fails, this stops program
 * execution like a normal assertion, but also generating the given message.
 * It is NOT stripped from release builds.  Note that the condition test
 * is -inverted- from the normal assert() semantics.
 */
#ifndef LOG_ALWAYS_FATAL_IF
#define LOG_ALWAYS_FATAL_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)trace_printAssert(#cond, LOG_TAG, ## __VA_ARGS__)) \
    : (void)0 )
#endif

#ifndef LOG_ALWAYS_FATAL
#define LOG_ALWAYS_FATAL(...) \
    ( ((void)trace_printAssert(NULL, LOG_TAG, ## __VA_ARGS__)) )
#endif

/*
 * Versions of LOG_ALWAYS_FATAL_IF and LOG_ALWAYS_FATAL that
 * are stripped out of release builds.
 */
#if LOG_NDEBUG

#ifndef LOG_FATAL_IF
#define LOG_FATAL_IF(cond, ...) ((void)0)
#endif
#ifndef LOG_FATAL
#define LOG_FATAL(...) ((void)0)
#endif

#else

#ifndef LOG_FATAL_IF
#define LOG_FATAL_IF(cond, ...) LOG_ALWAYS_FATAL_IF(cond, ## __VA_ARGS__)
#endif
#ifndef LOG_FATAL
#define LOG_FATAL(...) LOG_ALWAYS_FATAL(__VA_ARGS__)
#endif

#endif

/*
 * Assertion that generates a log message when the assertion fails.
 * Stripped out of release builds.  Uses the current LOG_TAG.
 */
#ifndef ALOG_ASSERT
#define ALOG_ASSERT(cond, ...) LOG_FATAL_IF(!(cond), ## __VA_ARGS__)
//#define ALOG_ASSERT(cond) LOG_FATAL_IF(!(cond), "Assertion failed: " #cond)
#endif

// ---------------------------------------------------------------------

/*
 * Basic log message macro.
 *
 * Example:
 *  ALOG(LOG_WARN, NULL, "Failed with error %d", errno);
 *
 * The second argument may be NULL or "" to indicate the "global" tag.
 */

/*
 * Log macro that allows you to specify a number for the priority.
 */
#ifndef LOG_PRI
#define LOG_PRI(priority, tag, ...) \
    trace_printLog(priority, tag, __VA_ARGS__)
#endif

#ifndef ALOG
#define ALOG(priority, tag, ...) \
    LOG_PRI(TRACE_##priority, tag, __VA_ARGS__)
#endif

/*
 * Conditional given a desired logging priority and tag.
 */
#ifndef IF_ALOG
#define IF_ALOG(priority, tag) \
    if (trace_testLog(ANDROID_##priority, tag))
#endif

/* TODO: something for LIST */

/*
 * ===========================================================================
 *
 * The stuff in the rest of this file should not be used directly.
 */

#define trace_printLog(prio, tag, fmt...) \
    __trace_log_print(prio, tag, fmt)

/* XXX Macros to work around syntax errors in places where format string
 * arg is not passed to ALOG_ASSERT, LOG_ALWAYS_FATAL or LOG_ALWAYS_FATAL_IF
 * (happens only in debug builds).
 */

/* Returns 2nd arg.  Used to substitute default value if caller's vararg list
 * is empty.
 */
#define __trace_second(dummy, second, ...)     second

/* If passed multiple args, returns ',' followed by all but 1st arg, otherwise
 * returns nothing.
 */
#define __trace_rest(first, ...)               , ## __VA_ARGS__

#define trace_printAssert(cond, tag, fmt...) \
    __trace_log_assert(cond, tag, \
        __trace_second(0, ## fmt, NULL) __trace_rest(fmt))

#define trace_testLog(prio, tag) (1)

#define LITERAL_TO_STRING_INTERNAL(x)    #x
#define LITERAL_TO_STRING(x) LITERAL_TO_STRING_INTERNAL(x)

#ifdef CHECK
#undef CHECK
#endif

#define CHECK(condition)                                \
        LOG_ALWAYS_FATAL_IF(                                \
                            !(condition),                               \
                            "%s",                                       \
                            __FILE__ ":" LITERAL_TO_STRING(__LINE__)    \
                            " CHECK(" #condition ") failed.")

#ifdef TRESPASS
#undef TRESPASS
#endif

#define TRESPASS() \
                LOG_ALWAYS_FATAL(                                       \
                                    __FILE__ ":" LITERAL_TO_STRING(__LINE__)            \
                                        " Should not be here.");

#ifdef __cplusplus
}
#endif

#ifdef MAKE_COMPARATOR
#undef MAKE_COMPARATOR
#endif

#ifdef CHECK_OP
#undef CHECK_OP
#endif

#ifdef CHECK_EQ
#undef CHECK_EQ
#endif

#ifdef CHECK_NE
#undef CHECK_NE
#endif

#ifdef CHECK_LE
#undef CHECK_LE
#endif

#ifdef CHECK_LT
#undef CHECK_LT
#endif

#ifdef CHECK_GE
#undef CHECK_GT
#endif

#ifdef __cplusplus
#define MAKE_COMPARATOR(suffix,op)                          \
template<class A, class B>                              \
 void Compare_##suffix(const A &a, const B &b, std::string &ret) {      \
  std::ostringstream  res;                                        \
    if (!(a op b)) {                                    \
        res << a;                                  \
        res <<" vs. ";                            \
        res << b;                                  \
    }                                                   \
    ret = res.str();                                        \
}

MAKE_COMPARATOR(EQ, ==)
MAKE_COMPARATOR(NE, !=)
MAKE_COMPARATOR(LE, <=)
MAKE_COMPARATOR(GE, >=)
MAKE_COMPARATOR(LT, <)
MAKE_COMPARATOR(GT, >)

#define CHECK_OP(x,y,suffix,op)                                         \
        do {                                                                \
            std::string  __res;                                             \
            Compare_##suffix(x, y,__res);                                   \
            if (!__res.empty()) {                                          \
                std::string ___full =                                           \
                __FILE__ ":" LITERAL_TO_STRING(__LINE__)                \
                " CHECK_" #suffix "( " #x "," #y ") failed: ";      \
                ___full += __res ;                                     \
                LOG_ALWAYS_FATAL("%s", ___full.c_str());                    \
            }                                                               \
        } while (false)
#else
#define CHECK_OP(x,y,suffix,op) CHECK(((x) op (y)))
#endif

//define CHECK_XX supported
#define CHECK_EQ(x,y)   CHECK_OP(x,y,EQ,==)
#define CHECK_NE(x,y)   CHECK_OP(x,y,NE,!=)
#define CHECK_LE(x,y)   CHECK_OP(x,y,LE,<=)
#define CHECK_LT(x,y)   CHECK_OP(x,y,LT,<)
#define CHECK_GE(x,y)   CHECK_OP(x,y,GE,>=)
#define CHECK_GT(x,y)   CHECK_OP(x,y,GT,>)

#endif // LOGD_H
