var express = require('express');
var router = express.Router();
var mysql = require('../db.js');

router.get('/', function(req, res, next) {
    var context = {};
    res.render('award', context);

});

router.get('/:id', function(req, res, next) {
    var context = {};
    mysql.query("SELECT id, recipient, rec_email, type_id, awarded FROM award WHERE awarder_id=?", [req.params.id], function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        context.result = rows;
        res.render('award', context);
    });
});


module.exports = router;
