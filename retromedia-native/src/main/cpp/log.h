/*
 *  Copyright (c) 2014 The linux_utility_wrapper project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef LOG_H
#define LOG_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Trace log priority values, in ascending priority order.
 */
enum {
	TRACE_LOG_UNKNOWN = 0,
	TRACE_LOG_DEFAULT, /* only for SetMinPriority() */
	TRACE_LOG_VERBOSE,
	TRACE_LOG_DEBUG,
	TRACE_LOG_INFO,
	TRACE_LOG_WARN,
	TRACE_LOG_ERROR,
	TRACE_LOG_FATAL,
	TRACE_LOG_SILENT, /* only for SetMinPriority(); must be last */
};

typedef int (*trace_log_write)(int prio, const char *tag, const char *msg);

/*
 * Redirect log output
 */
int __trace_log_redirect(trace_log_write);

/*
 * Send a formatted string to the log, used like printf(fmt,...)
 */
int __trace_log_print(int prio, const char *tag, const char *fmt, ...)
#if defined(__GNUC__)
		__attribute__ ((format(printf, 3, 4)))
#endif
		;

/*
 * Log an assertion failure and SIGTRAP the process to have a chance
 * to inspect it, if a debugger is attached. This uses the FATAL priority.
 */
void __trace_log_assert(const char *cond, const char *tag, const char *fmt, ...)
#if defined(__GNUC__)
		__attribute__ ((noreturn))
		__attribute__ ((format(printf, 3, 4)))
#endif
		;

/*
 * Write log to file and set the file size.
 * OK return 0, otherwise -1
 */
int __trace_set_log_file(const char *file, unsigned max_size);

/*
 *Set log level with return last level
 */
int __trace_set_log_level(int level);


#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
