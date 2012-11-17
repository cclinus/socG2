<?php

exec("rm -rf setting");

$ourHead = $_GET['ourHead'];
$ourTail = $_GET['ourTail'];
$enemyHead = $_GET['enemyHead'];
$enemyTail = $_GET['enemyTail'];
$gate = $_GET['gate'];

$myFile = "setting";
$fh = fopen($myFile, 'w') or die("can't open file");
$stringData = "$gate\n$ourHead\n$ourTail\n$enemyHead\n$enemyTail\n";
fwrite($fh, $stringData);
fclose($fh);

?>
