# PHP Inotify

Inotify bindings for PHP 5, PHP 7, and PHP 8

This extension exposes the inotify API and some additional functions.

## Install

* PHP 8: pecl install inotify
* PHP 7: pecl install inotify
* PHP 5: pecl install inotify-0.1.6

## Documentation

Documentation is available at https://php.net/inotify

## Goal

The goal of this extension is to expose the raw inotify API to PHP, while being memory safe and preventing resource leaks.

## Streams

As the C inotify API returns file descriptors, this extension returns PHP streams.

This is useful for the following reasons:

### I/O Polling

The streams can be used with polling mechanisms such as ``stream_select()`` or event loops such as ReactPHP or AMPHP. It's also possible to make the streams unblocking with ``stream_set_blocking()``.

### Resource management

As the inotify file descriptors are owned by PHP streams, they are managed by PHP. This ensures that the file descriptors are eventually closed, which prevents descriptor leaks.
