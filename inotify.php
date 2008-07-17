<?php
/*
The inotify extension exposes the inotify functions inotify_init(), 
inotify_add_watch() and inotify_rm_watch(), which work just like the C
ones.

As the C inotify_init() function returns a file descriptor, PHP's 
inotify_init() returns a stream resource, so you can use the standard
stream functions on it, like stream_select(), stream_set_blocking(), 
and also fclose(), used to close an inotify instance and all its 
watches.

It also exports two additional functions:

inotify_queue_len(resource $inotify_instance), which returns a number
upper than zero if their are waiting events

inotify_read(resource $inotify_instance), which replaces the C way
of reading inotify events. It returns arrays of events, in which keys
represents the members of the inotify_event structure.
*/

// Open an inotify instance
$fd = inotify_init();

// Watch __FILE__ for metadata changes (e.g. mtime)
$watch_descriptor = inotify_add_watch($fd, __FILE__, IN_ATTRIB);

// generate an event
touch(__FILE__);

// Read events
$events = inotify_read($fd);
// it may return something like this:
array(
  array(
    'wd' => 1, // $watch_descriptor
    'mask' => 4, // IN_ATTRIB bit is set
    'cookie' => 0, // unique id to connect related events (e.g. 
                   // IN_MOVE_FROM and IN_MOVE_TO events)
    'name' => '', // the name of a file (e.g. if we monitored changes
                  // in a directory)
  ),
);
print_r($events);

// If we do not want to block on inotify_read(), we may

// - Use stream_select() on $fd:
$read = array($fd);
$write = null;
$except = null;
stream_select($read,$write,$except,0);

// - Use stream_set_blocking() on $fd
stream_set_blocking($fd, 0);
inotify_read($fd); // Does no block, and return false if no events are pending

// - Use inotify_queue_len() to check if event queue is not empty
$queue_len = inotify_queue_len($fd); // If > 0, we can be assured that
                                     // inotify_read() will not block

// We do not need anymore to watch events, closing them

// Stop watching __FILE__ for metadata changes
inotify_rm_watch($fd, $watch_descriptor);

// Close our inotify instance
// This may have closed all watches if we haven't already closed them
fclose($fd);

?>
