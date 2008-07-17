--TEST--
inotify
--SKIPIF--
<?php if (!extension_loaded("inotify")) print "skip: inotify extension not loaded"; ?>
--FILE--
<?php 

$ino = inotify_init();
echo 'inotify_init(): ';
var_dump($ino);

$ret = fclose($ino);
echo 'fclose($ino): ';
var_dump($ret);
echo '$ino: ';
var_dump($ino);

$ino = inotify_init();
echo 'inotify_init(): ';
var_dump($ino);

echo 'inotify_queue_len($ino): ';
var_dump(inotify_queue_len($ino));

$watch = inotify_add_watch($ino, __FILE__, IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE);
echo 'inotify_add_watch($ino, __FILE__, IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE): ';
var_dump($watch);

$watch_dir = inotify_add_watch($ino, dirname(__FILE__), IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE);
echo 'inotify_add_watch($ino, dirname(__FILE__), IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE): ';
var_dump($watch_dir);

touch(__FILE__);
$fd = fopen(__FILE__,"r");
fclose($fd);

echo 'inotify_queue_len($ino): ';
var_dump(inotify_queue_len($ino));

$event_count = 6;
$all_events = array();

while ($event_count > 0) {
	$events = inotify_read($ino);
	if ($events === false) break;
	if (is_array($events)) {
		$all_events = array_merge($all_events, $events);
		$event_count -= count($events);
	}
}

function mysort($a,$b) {
	$a = print_r($a,true);
	$b = print_r($b,true);
	if ($a > $b) return 1;
	if ($a < $b) return -1;
	return 0;
}
// events do not always arrive in the same order, 
// we need to sort them to have a reproductible output
usort($all_events,'mysort');

echo 'inotify_read($ino): ';
var_dump($all_events);

$ret = inotify_rm_watch($ino,$watch);
echo 'inofity_rm_watch($ino,$watch): ';
var_dump($ret);
echo '$watch: ';
var_dump($watch);

$ret = fclose($ino);
echo 'fclose($ino): ';
var_dump($ret);
echo '$ino: ';
var_dump($ino);
?>
--EXPECTF--
inotify_init(): resource(%d) of type (stream)
fclose($ino): bool(true)
$ino: resource(%d) of type (Unknown)
inotify_init(): resource(%d) of type (stream)
inotify_queue_len($ino): int(0)
inotify_add_watch($ino, __FILE__, IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE): int(%d)
inotify_add_watch($ino, dirname(__FILE__), IN_ATTRIB|IN_OPEN|IN_CLOSE_NOWRITE): int(%d)
inotify_queue_len($ino): int(%d)
inotify_read($ino): array(6) {
  [0]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(16)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(0) ""
  }
  [1]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(32)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(0) ""
  }
  [2]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(4)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(0) ""
  }
  [3]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(16)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(7) "002.php"
  }
  [4]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(32)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(7) "002.php"
  }
  [5]=>
  array(4) {
    ["wd"]=>
    int(%d)
    ["mask"]=>
    int(4)
    ["cookie"]=>
    int(0)
    ["name"]=>
    string(7) "002.php"
  }
}
inofity_rm_watch($ino,$watch): bool(true)
$watch: int(%d)
fclose($ino): bool(true)
$ino: resource(%d) of type (Unknown)
