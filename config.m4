dnl $Id$
dnl config.m4 for extension inotify

PHP_ARG_ENABLE(inotify, whether to enable inotify support,
[  --enable-inotify        Enable inotify support])

if test "$PHP_INOTIFY" != "no"; then

  AC_TRY_RUN([
   #include <sys/inotify.h>
   void testfunc(int (*passedfunc)()) {
   }
   int main() {
    testfunc(inotify_init);
    return 0;
   }
  ],[],[
   AC_MSG_ERROR(Your system does not support inotify)
  ])

  PHP_NEW_EXTENSION(inotify, inotify.c, $ext_shared)
fi
