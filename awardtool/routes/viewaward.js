var express = require('express');
var router = express.Router();
var mysql = require('../db.js');

router.get('/', function(req, res, next) {
    res.render('viewaward');
});
router.get('/:id', function(req, res, next) {
    var context = {};
    mysql.query("SELECT id, recipient, rec_email, awarder_id, type_id, awarded FROM award WHERE id=?;", [req.params.id], function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        if (rows.length != 0) {
            context.awarder_id = rows[0].awarder_id;
            mysql.query("SELECT name FROM award_type WHERE id=?;", [rows[0].type_id], function(err, types, fields) {
                if (err) {
                    next(err);
                    return;
                }
                context.type = types[0].name;
                var awarder = rows[0].awarder_id;
                mysql.query("SELECT name FROM user WHERE id=?;", [awarder], function(err, users, fields) {
                    if (err) {
                        next(err);
                        return;
                    }
                    context.awarder = users[0].name;
                    context.id = rows[0].id;
                    context.recipient = rows[0].recipient;
                    context.email = rows[0].rec_email;
                    res.render('viewaward', context);
                });
            });
        } else {
            context.id = req.params.id;
            res.render('viewaward', context);
        }


    });
});




module.exports = router;
