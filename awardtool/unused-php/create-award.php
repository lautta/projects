<?php
require("connect.php");
session_start();

$stmt = $mysqli->prepare("INSERT INTO award (recipient, rec_email, awarder_id, type_id, awarded) VALUES (?, ?, ?, ?, ?)");
$stmt->bind_param("ssiis", $_POST['r_name'], $_POST['r_email'], $_SESSION['user_id'], $_POST['award_type'], $_POST['created']);
$stmt->execute();
$stmt->close();
$_SESSION['success'] = 'Award Created!';
header("Location: award-tool.php");
exit();
?>
