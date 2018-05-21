<?php
require("connect.php");
session_start();
if (isset($_SESSION["email"]) && $_SESSION["role"] == "admin") {
    header("Location: admin.php");
    exit();
} elseif (isset($_SESSION["email"]) && $_SESSION["role"] == "user") {
    header("Location: award-tool.php");
    exit();
}
require("header.html");
?>

<body>

<div class="jumbotron text-center">
    <h1 class="display-3" id="company-brand">Octans</h1>
</div>

<div class="container">
    <div class="well">
        <h2 class="text-center">Award Tool Login</h2>
        <form class="form-horizontal" method="POST" action="login.php">
            <div class="form-group">
                <label class="control-label col-sm-2" for="login-email">Email</label>
                <div class="col-sm-8">
                    <input type="email" name="login-email" id="login-email" class="form-control"
                           placeholder="Email address" required autofocus>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-sm-2" for="login-password">Password</label>
                <div class="col-sm-8">
                    <input type="password" name="login-password" id="login-password" class="form-control"
                           placeholder="Password" required>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <a href="recover-pass.php">Forgot password?</a>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <button class="btn btn-dark" name="submit" type="submit">Login!</button>
                </div>
            </div>
        </form>
    </div>
    <div id="error">
        <?php
        if (!empty($_SESSION['error'])) {
            echo '<p class="text-center"><span class="glyphicon glyphicon-remove"></span> ' . $_SESSION['error'] . '</p>';
        }
        ?>
    </div>
    <?php
    unset($_SESSION['error']);
    ?>
</div>
</body>
</html>
