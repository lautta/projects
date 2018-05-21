<?php
require("connect.php");
require("auth-admin.php");
require("header.html");
?>

<body>
<div class="jumbotron text-center">
    <h1 class="display-3">Admin Portal</h1>
    <p class="lead">Welcome <?php echo $_SESSION['email']; ?> to the Admin portal!</p>
</div>
<div id="admin">
    <p class="text-center">
        <!–– Link to Ivy's app here -->
        <a href="https://octans-lautta.c9users.io:8081"> Continue to admin portal</a><br>
        <a href="logout.php"> Logout</a>
    </p>
</div>
</body>
</html>
