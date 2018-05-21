<?php
require("connect.php");
session_start();

$email = '';
$email2 = '';
$email = stripslashes($_POST['email']);
$email = mysqli_real_escape_string($mysqli, $email);
$email2 = stripslashes($_POST['confirm-email']);
$email2 = mysqli_real_escape_string($mysqli, $email2);

if ($email == $email2) {
    $stmt = $mysqli->prepare("SELECT email, password, name FROM user WHERE email=?");
    $stmt->bind_param("s", $email);
    $stmt->execute();
    $stmt->bind_result($email, $password, $name);
    $stmt->store_result();
    if ($stmt->num_rows == 1) {
        if ($stmt->fetch()) {
            $body = "Hi $name, \r\nThis is your current password: $password";
            $ch = curl_init();
            curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_setopt($ch, CURLOPT_USERPWD, 'api:key-932f6646b8c07617f8019d585b3e5cd9');
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
            curl_setopt($ch, CURLOPT_URL, 
              'https://api.mailgun.net/v3/sandboxba0853012d9d4d27b46ad9e45099b239.mailgun.org/messages');
            curl_setopt($ch, CURLOPT_POSTFIELDS, 
                array('from' => 'admin@company.com',
                      'to' => $email,
                      'subject' => 'Octans Password',
                      'text' => $body));
            $success = curl_exec($ch);
            if (!$success) {
                $_SESSION['status'] = '<span class="glyphicon glyphicon-remove"></span> ERROR SENDING EMAIL, TRY AGAIN';
            } else {
                $_SESSION['status'] = '<span class="glyphicon glyphicon-ok"></span> PASSWORD HAS BEEN SENT TO EMAIL ADDRESS';
            }
        }
    } else {
        $_SESSION['status'] = '<span class="glyphicon glyphicon-remove"></span> EMAIL NOT FOUND';
    }
    $stmt->close();
} else {
    $_SESSION['status'] = '<span class="glyphicon glyphicon-remove"></span> EMAIL ADDRESSES DO NOT MATCH, PLEASE RECONFIRM';
}
header('Location: recover-pass.php');
exit;

?>
