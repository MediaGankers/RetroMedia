/*
 *  Copyright (c) 2014 The linux_utility_wrapper project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#define LOG_BUF_SIZE 1024
#ifndef NULL
#define NULL 0
#endif
#ifdef __ANDROID__
#include <android/log.h>
#endif

static trace_log_write trace_log_fun;
static int trace_log_level = TRACE_LOG_DEFAULT;
static FILE *trace_log_fd = NULL;
static int trace_log_fd_size = 1024 * 1024 * 10;
static int trace_log_header_offset = 0;
static const char vtab[TRACE_LOG_SILENT + 1] =
		{ 'U', 'd', 'V', 'D','I', 'W', 'E', 'F', 'S' };

int __trace_set_log_file(const char *file, unsigned max_size)
{
	int rev = -1;
	FILE *fd = trace_log_fd;
	trace_log_fd = NULL;
	if (fd) {
		fflush(fd);
		fclose(fd);
	}

	if (file) {
		fd = fopen(file, "w+");
		if (fd) {
			time_t now;
			time(&now);
			fprintf(fd, "%s\nCreated By: %s%s\nDate: %s%s\n",
					"=============SnailPlayer Trace log file=================",
					getenv("USER"),".snailgame.com",ctime(&now),
					"========================================================");
		}
		trace_log_fd = fd;
		trace_log_fd_size = max_size;
		trace_log_header_offset = ftell(fd);
		rev = 0;
	}
	return rev;
}

/*
 *Set log level with return last level
 */
int __trace_set_log_level(int level)
{
	int old = trace_log_level;
	trace_log_level = level;
	return old;
}

#ifndef __ANDROID__
static int __trace_log_write(int prio, const char *tag, const char *msg){
	int ret = -1;
	if (prio >= TRACE_LOG_UNKNOWN && prio <= TRACE_LOG_SILENT) {
		ret = printf("%c/%s: %s\n", vtab[prio], tag ? tag : "", msg ? msg : "");
	}
	return ret;
}
#endif

int __trace_log_redirect(trace_log_write pfun){
	trace_log_fun = pfun;
	return 0;
}

static int __trace_internal_write(int prio, const char *tag,
		const char *msg) {
	trace_log_write pfun = NULL;
	if (trace_log_fd) {
		long size = ftell(trace_log_fd);
		if (size > trace_log_fd_size) {
			fseek(trace_log_fd, trace_log_header_offset, 0);
		}
		fprintf(trace_log_fd, "%c/%s %s\n",vtab[prio], tag, msg);
		return 0;
	}
#ifdef __ANDROID__
	pfun = trace_log_fun ?trace_log_fun: __android_log_write;
#else
	pfun = trace_log_fun ? trace_log_fun : __trace_log_write;
#endif
	return (*pfun)(prio, tag, msg);
}

int __trace_log_print(int prio, const char *tag, const char *fmt, ...) {
	va_list ap;
	char buf[LOG_BUF_SIZE] = {0};

	if (prio < trace_log_level) {
		return 0;
	}

	va_start(ap, fmt);
	vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
	va_end(ap);
	return __trace_internal_write(prio, tag, buf);
}

void __trace_log_assert(const char *cond, const char *tag, const char *fmt, ...) {
	char buf[LOG_BUF_SIZE];

	if (fmt) {
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
		va_end(ap);
	} else {
		/* Msg not provided, log condition.  N.B. Do not use cond directly as
		 * format string as it could contain spurious '%' syntax (e.g.
		 * "%d" in "blocks%devs == 0").
		 */
		if (cond)
			snprintf(buf, LOG_BUF_SIZE, "Assertion failed: %s", cond);
		else
			strcpy(buf, "Unspecified assertion failed");
	}
	(void) (__trace_internal_write(TRACE_LOG_FATAL, tag, buf));
	__trace_set_log_file(NULL, 0);
	__builtin_trap(); /* trap so we have a chance to debug the situation */
}
