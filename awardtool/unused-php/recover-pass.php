<?php
require("connect.php");
session_start();
require("header.html");
?>

<body>
<div class="jumbotron text-center">
    <h1 class="display-3" id="company-brand">Octans</h1>
</div>

<div class="container">
    <div class="well">
        <h2 class="text-center">Recover Password</h2>
        <form class="form-horizontal" method="POST" action="email-pass.php">
            <div class="form-group">
                <label class="control-label col-sm-2" for="email">Email</label>
                <div class="col-sm-8">
                    <input type="email" id="email" name="email" class="form-control" placeholder="Email address"
                           required
                           autofocus>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-sm-2" for="confirm-email">Confirm</label>
                <div class="col-sm-8">
                    <input type="email" id="confirm-email" name="confirm-email" class="form-control"
                           placeholder="Email address" required>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <a href="index.php">Return to login</a>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <button class="btn btn-dark" type="submit">Recover!</button>
                </div>
            </div>
        </form>
    </div>
    <div id="status">
        <?php
        if (!empty($_SESSION['status'])) {
            echo '<p class="text-center">' . $_SESSION['status'] . '</p>';
        }
        ?>
    </div>
    <?php
    unset($_SESSION['status']);
    ?>
</div>
</body>
</html>
