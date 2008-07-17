#!/usr/bin/php
<?php
/**
 * Simple tail program using the inotify extension
 * Usage: ./tail.php file
 */

function main_loop($file, $file_fd) {

	$inotify = inotify_init();
	if ($inotify === false) {
		fprintf(STDERR, "Failed to obtain an inotify instance\n");
		return 1;
	}

	$watch = inotify_add_watch($inotify, $file, IN_MODIFY);
	if ($watch === false) {
		fprintf(STDERR, "Failed to watch file '%s'", $file);
		return 1;
	}

	while (($events = inotify_read($inotify)) !== false) {

		echo "Event received !\n";

		foreach($events as $event) {
			if (!($event['mask'] & IN_MODIFY)) continue;

			echo stream_get_contents($file_fd);

			break;
		}
	}

	// May not happen

	inotify_rm_watch($inotify, $watch);
	fclose($inotify);
	fclose($file_fd);

	return 0;
}

ob_implicit_flush(1);

if (!extension_loaded('inotify')) {
	fprintf(STDERR, "Inotify extension not loaded !\n");
	exit(1);
}

if ($argc < 2) {
	fprintf(STDERR, "Usage: " . $argv[0] . " [FILE]\n");
	exit(1);
}

$file = $argv[1];
if (!file_exists($file) || ($fd = fopen($file, "r")) === false) {
	fprintf(STDERR, "File '%s' does not exists or is not readable\n", $file);
	exit(1);
}
fseek($fd, 0, SEEK_END);

exit(main_loop($file, $fd));

?>
