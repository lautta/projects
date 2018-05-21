<?php
require("connect.php");
require("auth-user.php");
require("header.html");
?>
<body>
<?php
require("nav.html");
?>

<div class="jumbotron text-center">
    <h1 class="display-3">User Account</h1>
    <p class="lead">Change user name or delete awards given</p>
</div>

<div class="container">
    <div class="col-sm-offset-1 col-sm-10">
        <h3>Account Details</h3>
        <table class="table">
            <tbody>
            <?php
            $stmt = $mysqli->prepare("SELECT user.email, user.name, user.role, user.creation, user.signature
                                                FROM user
                                                WHERE id = '" . $_SESSION['user_id'] . "'");
            $stmt->execute();
            $stmt->bind_result($u_email, $u_name, $u_role, $u_creation, $u_sig);
            while ($stmt->fetch()) {
                echo '<tr><th scope="row"> Email</th><td> ' . $u_email . '</td></tr>';
                echo '<tr><th scope="row"> Name</th><td> ' . $u_name . '</td></tr>';
                echo '<tr><th scope="row"> Role</th><td> ' . $u_role . '</td></tr>';
                echo '<tr><th scope="row"> Creation Date</th><td> ' . $u_creation . '</td></tr>';
                echo '<tr><th scope="row"> Signature Image</th><td><img src="' . $u_sig . '"></td></tr>';
            }
            $stmt->close();
            ?>
            </tbody>
        </table>
    </div>
</div>
<div class="container">
    <div class="container">
        <form class="form-horizontal" method="POST" action="update-name.php">
            <div class="form-group">
                <label class="control-label col-sm-2" for="name">Update Name</label>
                <div class="col-sm-8">
                    <input type="text" class="form-control" id="name" placeholder="Enter new name" name="name" pattern="^[A-Za-z -]+$"
                           required>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <button type="submit" class="btn btn-dark">Update!</button>
                </div>
            </div>
        </form>
    </div>
    <div class="container">
        <form class="form-horizontal" method="POST" action="delete-award.php">
            <div class="form-group">
                <label class="control-label col-sm-2" for="award">Delete Award</label>
                <div class="col-sm-8">
                    <select class="form-control" id="award" name="award">
                        <?php
                        $stmt = $mysqli->prepare("SELECT award.id, award.recipient, award.awarded, award_type.name
                                                FROM award
                                                JOIN user ON award.awarder_id = user.id
                                                JOIN award_type ON award.type_id = award_type.id
                                                WHERE awarder_id = '" . $_SESSION['user_id'] . "'
                                                ORDER BY  award.awarded DESC");
                        $stmt->execute();
                        $stmt->bind_result($a_id, $recipient, $timedate, $a_type);
                        while ($stmt->fetch()) {
                            echo '<option value=" ' . $a_id . ' "> ' . $recipient . ': ' . $a_type . ' ' . $timedate . '</option>\n';
                        }
                        $stmt->close();
                        ?>
                    </select>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <button type="submit" class="btn btn-dark">Delete!</button>
                </div>
            </div>
        </form>
    </div>
    <div id="success">
        <?php
        if (!empty($_SESSION['success'])) {
            echo '<p class="text-center"><span class="glyphicon glyphicon-ok"></span> ' . $_SESSION['success'] . '</p>';
        }
        ?>
    </div>
    <?php
    unset($_SESSION['success']);
    ?>
</div>
</body>
</html>
