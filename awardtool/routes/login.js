var express = require('express');
var router = express.Router();
var bcrypt = require('bcryptjs');
var mysql = require('../db.js');


router.get('/', function(req, res, next) {
    if (req.session.authenticated && req.session.role == 'admin') {
        res.redirect('/viewuser');
    }
    else if (req.session.authenticated && req.session.role == 'user') {
        res.redirect('/addaward');
    }
    else {
        res.render('login', {
            layout: 'user.handlebars',
            error: req.flash('error')
        });
    }
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "login") {
        mysql.query("SELECT id, email, password, role FROM user WHERE email=?;", [req.body.uemail], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
            var numRows = rows.length;

            if (numRows == 1) {
                var hash = rows[0].password;
                if (bcrypt.compareSync(req.body.upassword, hash)) {
                    var user_id = rows[0].id;
                    var email = rows[0].email;
                    var role = rows[0].role;
                    req.session.authenticated = true;
                    req.session.role = role;
                    req.session.user_id = user_id;
                    req.session.email = email;

                    if (role == "admin") {
                        res.redirect('/viewuser');
                    }
                    else if (role == "user") {
                        res.redirect('/addaward');
                    }
                }
                else {
                    req.flash('error', 'Invalid Password!');
                    res.redirect('/');
                }
            }
            else {
                req.flash('error', 'Email not found!');
                res.redirect('/');
            }
        });
    }
});

module.exports = router;
