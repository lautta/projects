var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var helper = require('../helper.js');


router.get('/', function(req, res, next) {
    //for user detail table data
    mysql.query("SELECT email, name, role, creation, signature FROM user WHERE id = ?;", [req.session.user_id], function(err, userdetails) {
        if (err) {
            next(err);
            return;
        }
        //for delete user awards selection
        mysql.query("SELECT award.id, award.recipient, award.awarded, award_type.name FROM award JOIN user ON award.awarder_id = user.id JOIN award_type ON award.type_id = award_type.id WHERE awarder_id = ? ORDER BY  award.awarded DESC;", [req.session.user_id], function(err, userawards) {
            if (err) {
                next(err);
                return;
            }
            res.render('useraccount', {
                layout: 'user.handlebars',
                data1: userdetails,
                data2: userawards,
                useremail: req.session.email,
                success: req.flash('success'),
                error: req.flash('error')
            });
        });
    });
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "update") {
        //validate that name is not empty
        if (!helper.isEmpty(req.body.name)) {
            mysql.query("UPDATE user SET name = ? WHERE id = ?;", [req.body.name, req.session.user_id], function(err, rows, fields) {
                if (err) {
                    next(err);
                    return;
                }
            });
            req.flash('success', 'Name successfully updated!');
            res.redirect('/useraccount');
        }
        else {
            req.flash('error', 'Empty name, name not updated!');
            res.redirect('/useraccount');
        }
    }
    else if (req.body['submit'] == "delete") {
        mysql.query("DELETE FROM award WHERE id = ?;", [req.body.award], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
        });
        req.flash('success', 'Award successfully deleted!');
        res.redirect('/useraccount');
    }
    else {
        res.redirect('/useraccount');
    }
});

module.exports = router;
