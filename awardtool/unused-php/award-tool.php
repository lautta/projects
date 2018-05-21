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
    <h1 class="display-3">Create Awards</h1>
    <p class="lead">Use this tool to create a new award!</p>
</div>

<div class="container">
    <div class="award">
        <img src="award.jpg">
    </div>
    <div class="container">
        <form class="form-horizontal" method="POST" action="create-award.php">
            <div class="form-group">
                <label class="control-label col-sm-2" for="r_name">Recipient's Name</label>
                <div class="col-sm-8">
                    <input type="text" class="form-control" id="r_name" placeholder="Enter name" name="r_name" pattern="^[A-Za-z -]+$"
                           required>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-sm-2" for="r_email">Recipient's Email</label>
                <div class="col-sm-8">
                    <input type="email" class="form-control" id="r_email" placeholder="Enter email" name="r_email"
                           required>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-sm-2" for="created">Award Date and Time</label>
                <div class="col-sm-8">
                    <input type="datetime-local" class="form-control" id="created" placeholder="Enter date and time"
                           name="created" required>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-sm-2" for="award_type">Award Type</label>
                <div class="col-sm-8">
                    <select class="form-control" id="award_type" name="award_type">
                        <?php
                        $stmt = $mysqli->prepare("SELECT id, name FROM award_type");
                        $stmt->execute();
                        $stmt->bind_result($t_id, $t_name);
                        while ($stmt->fetch()) {
                            echo '<option value=" ' . $t_id . ' "> ' . $t_name . '</option>\n';
                        }
                        $stmt->close();
                        ?>
                    </select>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-2 col-sm-8">
                    <button type="submit" name="submit" class="btn btn-dark">Create!</button>
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
