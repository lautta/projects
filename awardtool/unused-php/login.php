<?php
require("connect.php");
session_start();

$email = '';
$password = '';
$email = stripslashes($_POST['login-email']);
$email = mysqli_real_escape_string($mysqli, $email);
$password = stripslashes($_POST['login-password']);
$password = mysqli_real_escape_string($mysqli, $password);
$stmt = $mysqli->prepare("SELECT id, email, password, role FROM user WHERE email=? AND password=?");
$stmt->bind_param("ss", $email, $password);
$stmt->execute();
$stmt->bind_result($uid, $email, $password, $role);
$stmt->store_result();
if ($stmt->num_rows == 1) {
    if ($stmt->fetch()) {
        $_SESSION['email'] = $email;
        $_SESSION['role'] = $role;
        $_SESSION['user_id'] = $uid;
        if ($role == 'admin') {
            header('Location: admin.php');
            exit;
        } elseif ($role == 'user') {
            header('Location: award-tool.php');
            exit;
        }
    }
} else {
    $_SESSION['error'] = 'INVALID EMAIL/PASSWORD';
}
$stmt->close();
header('Location: index.php');
exit;
?>
