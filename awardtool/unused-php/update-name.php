<?php
require("connect.php");
session_start();

$stmt = $mysqli->prepare("UPDATE user SET name = ? WHERE id = '" . $_SESSION['user_id'] . "'");
$stmt->bind_param("s", $_POST['name']);
$stmt->execute();
$stmt->close();
$_SESSION['success'] = 'User Name Updated!';
header("Location: user-account.php");
exit();
?>
