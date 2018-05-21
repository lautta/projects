var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var moment = require('moment');
var validator = require('validator');
var helper = require('../helper.js');


router.get('/', function(req, res, next) {
    mysql.query("SELECT id, name FROM award_type;", function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        res.render('addaward', {
            layout: 'user.handlebars',
            result: rows,
            useremail: req.session.email,
            success: req.flash('success'),
            error: req.flash('error')
        });
    });
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "submit") {
        //form validation
        //check for valid mysql datetime range or else it will set to null
        var m = moment(req.body.created, moment.ISO_8601);
        if (m.isValid && m.isBetween('1000-01-01T01:00:00.000',
                '9999-12-31T23:59:59.000')) {
            if (validator.isEmail(req.body.r_email) && !helper.isEmpty(req.body.r_email)) {
                if (!helper.isEmpty(req.body.r_name)) {
                    mysql.query("INSERT INTO award (recipient, rec_email, awarder_id, type_id, awarded) VALUES (?, ?, ?, ?, ?);", [req.body.r_name, req.body.r_email, req.session.user_id, req.body.award_type, req.body.created], function(err, rows, fields) {
                        if (err) {
                            next(err);
                            return;
                        }
                    });
                    req.flash('success', 'Award successfully created!');
                    res.redirect('/addaward');
                }
                else {
                    req.flash('error', 'Empty name, award not created!');
                    res.redirect('/addaward');
                }
            }
            else {
                req.flash('error', 'Invalid or empty email, award not created!');
                res.redirect('/addaward');
            }
        }
        else {
            req.flash('error', 'Invalid date, award not created!');
            res.redirect('/addaward');
        }
    }
    else {
        res.redirect('/addaward');
    }
});

module.exports = router;
