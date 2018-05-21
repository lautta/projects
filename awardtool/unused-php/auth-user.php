<?php
session_start();
if (!isset($_SESSION["email"]) || $_SESSION["role"] != "user") {
    header("Location: index.php");
    exit();
}
?>
