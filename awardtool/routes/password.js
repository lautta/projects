var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var bcrypt = require('bcryptjs');
var helper = require('../helper.js');
var api_key = '';
var domain = '';
var mailgun = require('mailgun-js')({ apiKey: api_key, domain: domain });

router.get('/', function(req, res, next) {
    res.render('password', {
        layout: 'user.handlebars',
        success: req.flash('success'),
        error: req.flash('error')
    });
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "submit") {
        if (req.body.remail == req.body.cemail) {
            mysql.query("SELECT email, name FROM user WHERE email=?;", [req.body.remail], function(err, rows, fields) {
                if (err) {
                    next(err);
                    return;
                }
                if (rows.length == 1) {
                    var name = rows[0].name;
                    var email = rows[0].email;
                    //admin has no name
                    if (!name) {
                        name = email;
                    }
                    var password = helper.genPass(8);
                    var hash = bcrypt.hashSync(password);
                    mysql.query("UPDATE user SET password = ? WHERE email = ?;", [hash, req.body.remail], function(err, rows, fields) {
                        if (err) {
                            next(err);
                            return;
                        }
                    });
                    var data = {
                        from: 'admin@company.com',
                        to: email,
                        subject: 'Octans Award Tool Password Reset',
                        text: 'Hi ' + name + ', \r\n\r\nSomeone requested your password to be reset.\r\nThis is now your current password: ' + password + '\r\nPlease make a note of it or contact an admin to change it.\r\n\r\nThanks!'
                    };
                    mailgun.messages().send(data, function(error, body) {
                        console.log(body);
                    });
                    req.flash('success', 'Password sent to ' + email + '!');
                    res.redirect('/password');
                }
                else {
                    req.flash('error', 'Account: ' + req.body.remail + ' not found!');
                    res.redirect('/password');
                }
            });
        }
        else {
            req.flash('error', 'Non-matching emails, please reconfirm!');
            res.redirect('/password');
        }
    }
});

module.exports = router;
