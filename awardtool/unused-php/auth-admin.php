<?php
session_start();
if (!isset($_SESSION["email"]) || $_SESSION["role"] != "admin") {
    header("Location: index.php");
    exit();
}
?>
