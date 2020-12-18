/*
  +----------------------------------------------------------------------+
  | inotify                                                              |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Arnaud Le Blanc <arnaud.lb@gmail.com>                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_INOTIFY_H
#define PHP_INOTIFY_H

extern zend_module_entry inotify_module_entry;
#define phpext_inotify_ptr &inotify_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

#include <sys/inotify.h>

PHP_MINIT_FUNCTION(inotify);
PHP_MSHUTDOWN_FUNCTION(inotify);
PHP_MINFO_FUNCTION(inotify);

PHP_FUNCTION(inotify_init);
PHP_FUNCTION(inotify_add_watch);
PHP_FUNCTION(inotify_rm_watch);
PHP_FUNCTION(inotify_queue_len);
PHP_FUNCTION(inotify_read);

#define PHP_INOTIFY_VERSION "3.0.0"

#define INOTIFY_BUF_TOO_SMALL(ret,errno) \
	((ret) == 0 || ((ret) == -1 && (errno) == EINVAL))
#define INOTIFY_FD(stream, fd) \
	php_stream_cast((stream), PHP_STREAM_AS_FD_FOR_SELECT, (void*)&(fd), 1);

/* Define some error messages for the error numbers set by inotify_*() functions, 
 as strerror() messages are not always usefull here */

#define INOTIFY_INIT_EMFILE \
	"The user limit on the total number of inotify instances has been reached"
#define INOTIFY_INIT_ENFILE \
	"The system limit on the total number of file descriptors has been reached"
#define INOTIFY_INIT_ENOMEM \
	"Insufficient kernel memory is available"

#define INOTIFY_ADD_WATCH_EACCES \
	"Read access to the given file is not permitted"
#define INOTIFY_ADD_WATCH_EBADF \
	"The given file descriptor is not valid"
#define INOTIFY_ADD_WATCH_EINVAL \
	"The given event mask contains no valid events; or the given file descriptor is not valid"
#define INOTIFY_ADD_WATCH_ENOMEM \
	"Insufficient kernel memory was available"
#define INOTIFY_ADD_WATCH_ENOSPC \
	"The user limit on the total number of inotify watches was reached or the kernel failed to allocate a needed resource"

#define INOTIFY_RM_WATCH_EINVAL \
	"The file descriptor is not an inotify instance or the watch descriptor is invalid"

#define INOTIFY_ERROR_CASE(func, errno) \
	case (errno): \
		php_error_docref(NULL, E_WARNING, INOTIFY_##func##_##errno); \
		break;
#define INOTIFY_DEFAULT_ERROR(errno) \
	default: \
		php_error_docref(NULL, E_WARNING, "%s", strerror(errno)); \
		break;


#endif	/* PHP_INOTIFY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
