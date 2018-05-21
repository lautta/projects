<?php
require("connect.php");
session_start();

$stmt = $mysqli->prepare("DELETE FROM award WHERE id = ?");
$stmt->bind_param("i", $_POST['award']);
$stmt->execute();
$stmt->close();
$_SESSION['success'] = 'Award Deleted!';
header("Location: user-account.php");
exit();
?>
