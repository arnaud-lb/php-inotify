--TEST--
inotify IN_EXCL_UNLINK flag
--SKIPIF--
<?php if (!extension_loaded("inotify")) print "skip: inotify extension not loaded"; ?>
--FILE--
<?php 

// Open an inotify instance
$fd = inotify_init();
// make unblocking
stream_set_blocking($fd, 0);

$wd = inotify_add_watch($fd, __DIR__, IN_MODIFY|IN_EXCL_UNLINK);

$file = __DIR__ . '/inotify_in_excl_unlink_tmpfile';

$tmpfp = fopen($file, 'w');
unlink($file);

fwrite($tmpfp, 'foobar');
fclose($tmpfp);

$events = inotify_read($fd);
var_dump($events);

?>
--EXPECT--
bool(false)
--CLEAN--
<?php
$file = __DIR__ . '/inotify_in_excl_unlink_tmpfile';
if (is_file($file)) {
    unlink($file);
}
?>
