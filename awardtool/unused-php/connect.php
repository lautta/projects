<?php
ini_set('display_errors', 'On');
$mysqli = new mysqli("127.0.0.1", "lautta", "", "c9");
if ($mysqli->connect_errno) {
    echo "Connection error " . $mysqli->connect_errno . " " . $mysqli->connect_error;
}
?>
