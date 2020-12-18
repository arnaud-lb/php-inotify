#!/bin/sh

echo "Building php-inotify with PHP version:"
php --version

if [ $MEMORY_CHECK -eq 1 ]; then
    PHP_RDKAFKA_CFLAGS="-Wall -Werror -Wno-deprecated-declarations"
fi

cd php-inotify
phpize
CFLAGS="$PHP_INOTIFY_CFLAGS" ./configure
make

echo "extension=$(pwd)/modules/inotify.so"|sudo tee /usr/local/etc/php/inotify.ini >/dev/null
