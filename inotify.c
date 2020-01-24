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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_inotify.h"

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_inotify_init, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_inotify_add_watch, 0, ZEND_RETURN_VALUE, 3)
	ZEND_ARG_INFO(0, inotify_instance)
	ZEND_ARG_INFO(0, pathname)
	ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_inotify_rm_watch, 0, ZEND_RETURN_VALUE, 2)
	ZEND_ARG_INFO(0, inotify_instance)
	ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_inotify_queue_len, 0, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, inotify_instance)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_inotify_read, 0, ZEND_RETURN_VALUE, 1)
	ZEND_ARG_INFO(0, inotify_instance)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ inotify_functions[]
 */
#if ZEND_MODULE_API_NO >= 20071006
const 
#endif
zend_function_entry inotify_functions[] = {

	PHP_FE(inotify_init,			arginfo_inotify_init)
	PHP_FE(inotify_add_watch,		arginfo_inotify_add_watch)
	PHP_FE(inotify_rm_watch,		arginfo_inotify_rm_watch)
	PHP_FE(inotify_queue_len,		arginfo_inotify_queue_len)
	PHP_FE(inotify_read,			arginfo_inotify_read)
	{NULL, NULL, NULL}	/* Must be the last line in inotify_functions[] */
};
/* }}} */

/* {{{ inotify_module_entry
 */
zend_module_entry inotify_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"inotify",
	inotify_functions,
	PHP_MINIT(inotify),
	PHP_MSHUTDOWN(inotify),
	NULL,		/* Replace with NULL if there's nothing to do at request start */
	NULL,		/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(inotify),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_INOTIFY_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_INOTIFY
ZEND_GET_MODULE(inotify)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(inotify)
{
	/* the following are legal, implemented events that user-space can watch for */
	REGISTER_LONG_CONSTANT("IN_ACCESS", IN_ACCESS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MODIFY", IN_MODIFY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_ATTRIB", IN_ATTRIB, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_CLOSE_WRITE", IN_CLOSE_WRITE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_CLOSE_NOWRITE", IN_CLOSE_NOWRITE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_OPEN", IN_OPEN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MOVED_FROM", IN_MOVED_FROM, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MOVED_TO", IN_MOVED_TO, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_CREATE", IN_CREATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_DELETE", IN_DELETE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_DELETE_SELF", IN_DELETE_SELF, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MOVE_SELF", IN_MOVE_SELF, CONST_CS | CONST_PERSISTENT);

	/* the following are legal events.  they are sent as needed to any watch */
	REGISTER_LONG_CONSTANT("IN_UNMOUNT", IN_UNMOUNT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_Q_OVERFLOW", IN_Q_OVERFLOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_IGNORED", IN_IGNORED, CONST_CS | CONST_PERSISTENT);

	/* helper events */
	REGISTER_LONG_CONSTANT("IN_CLOSE", IN_CLOSE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MOVE", IN_MOVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_ALL_EVENTS", IN_ALL_EVENTS, CONST_CS | CONST_PERSISTENT);

	/* special flags */
	REGISTER_LONG_CONSTANT("IN_ONLYDIR", IN_ONLYDIR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_DONT_FOLLOW", IN_DONT_FOLLOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_MASK_ADD", IN_MASK_ADD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_ISDIR", IN_ISDIR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IN_ONESHOT", IN_ONESHOT, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(inotify)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(inotify)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Version", PHP_INOTIFY_VERSION);
	php_info_print_table_end();
}
/* }}} */

static int php_inotify_queue_len(const int fd) /* {{{ */
{
	int ret;
	int queue_len;

	ret = ioctl(fd, FIONREAD, &queue_len);
	if (ret < 0) {
		php_error_docref(NULL, E_WARNING, "%s", strerror(errno));
		return 0;
	}
	return queue_len;
} /* }}} */

/* {{{ proto resource inotify_init()
   Initializes a new inotify instance and returns an inotify resource associated with the new inotify event queue */
PHP_FUNCTION(inotify_init)
{
	php_stream *stream;
	int fd;

	fd = inotify_init();

	if (fd == -1) {
		switch(errno) {
			INOTIFY_ERROR_CASE(INIT,EMFILE);
			INOTIFY_ERROR_CASE(INIT,ENFILE);
			INOTIFY_ERROR_CASE(INIT,ENOMEM);
			INOTIFY_DEFAULT_ERROR(errno);
		}
		RETURN_FALSE;
	}

	stream = php_stream_fopen_from_fd(fd, "r", NULL);
	stream->flags |= PHP_STREAM_FLAG_NO_SEEK;

	php_stream_to_zval(stream, return_value);
}
/* }}} */

/* {{{ proto int inotify_add_watch(resource inofity_instance, string pathname, int mask)
   Adds a watch to an initialized inotify instance */
PHP_FUNCTION(inotify_add_watch)
{
	zval *zstream;
	php_stream *stream;
	char *pathname;
	size_t pathname_len;
	long mask, wd;
	int fd;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rsl", &zstream, &pathname, &pathname_len, &mask) == FAILURE) {
		return;
	}

	if (php_check_open_basedir(pathname)) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, zstream);
	INOTIFY_FD(stream, fd);

	wd = inotify_add_watch(fd, pathname, mask);

	if (wd == -1) {
		switch(errno) {
			INOTIFY_ERROR_CASE(ADD_WATCH,EACCES);
			INOTIFY_ERROR_CASE(ADD_WATCH,EBADF);
			INOTIFY_ERROR_CASE(ADD_WATCH,EINVAL);
			INOTIFY_ERROR_CASE(ADD_WATCH,ENOMEM);
			INOTIFY_ERROR_CASE(ADD_WATCH,ENOSPC);
			INOTIFY_DEFAULT_ERROR(errno);
		}
		RETURN_FALSE;
	}

	RETURN_LONG(wd);
}
/* }}} */

/* {{{ proto bool inotify_rm_watch(resource inotify_instance, int inotify_watch_descriptor)
   Remove an existing watch from the given inotify instance */
PHP_FUNCTION(inotify_rm_watch)
{
	zval *zstream;
	php_stream *stream;
	int fd;
	long wd;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zstream, &wd) == FAILURE) {
		return;
	}

	php_stream_from_zval(stream, zstream);
	INOTIFY_FD(stream, fd);

	if (inotify_rm_watch(fd, wd) == -1) {
		switch(errno) {
			INOTIFY_ERROR_CASE(RM_WATCH,EINVAL);
			INOTIFY_DEFAULT_ERROR(errno);
		}
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int inotify_queue_len(resource inotify_instance)
   Returns an int upper than zero if events are pending */
PHP_FUNCTION(inotify_queue_len)
{
	zval *zstream;
	php_stream *stream;
	int fd;
	long queue_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zstream) == FAILURE) {
		return;
	}

	php_stream_from_zval(stream, zstream);
	INOTIFY_FD(stream, fd);

	queue_len = php_inotify_queue_len(fd);

	RETURN_LONG(queue_len);
}
/* }}} */

/* {{{ proto array inotify_read(resource inotify_instance)
   read()s inotify events */
PHP_FUNCTION(inotify_read)
{
	zval *zstream;
	php_stream *stream;
	char *readbuf = NULL;
	size_t readbuf_size = 0;
	ssize_t readden, i;
	struct inotify_event *event;
	zval event_ary;
	int fd;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zstream) == FAILURE) {
		return;
	}

	php_stream_from_zval(stream, zstream);
	INOTIFY_FD(stream, fd);

	readbuf_size = (double) php_inotify_queue_len(fd) * 1.6;
	if (readbuf_size < 1) {
		readbuf_size = sizeof(struct inotify_event) + 32;
	}

	do {
		readbuf = erealloc(readbuf, readbuf_size);
		readden = read(fd, readbuf, readbuf_size);

		/* If the passed buffer is too small to contain all the
		 * pending events, the kernel may return an error and 
		 * forces us to pass a bigger one. */
		if (INOTIFY_BUF_TOO_SMALL(readden,errno)) {
			readbuf_size *= 1.6;
			continue;
		} else if (readden < 0) {
			if (errno != EAGAIN) {
				php_error_docref(NULL, E_WARNING, "%s", strerror(errno));
			}
			efree(readbuf);
			RETURN_FALSE;
		}
	} while (INOTIFY_BUF_TOO_SMALL(readden,errno));

	array_init(return_value);

	for(i = 0; i < readden; i += sizeof(struct inotify_event) + event->len) {
		event = (struct inotify_event *)&readbuf[i];

		array_init(&event_ary);
		add_assoc_long(&event_ary, "wd", event->wd);
		add_assoc_long(&event_ary, "mask", event->mask);
		add_assoc_long(&event_ary, "cookie", event->cookie);
		add_assoc_string(&event_ary, "name", (event->len > 0 ? event->name : ""));

		add_next_index_zval(return_value, &event_ary);
	}
	efree(readbuf);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
