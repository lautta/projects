var express = require('express');
var router = express.Router();
var mysql = require('../db.js');

router.get('/', function(req, res, next) {
    res.render('search');
});

router.post('/', function(req, res, next){
    if (req.body['submit'] == 'search') {
        mysql.query('SELECT * FROM award WHERE id=?', [req.body.award], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
            if (rows.length != 0) {
                var url = '/viewaward/' + req.body.award;
                res.redirect(url);
            }  else {
                var url = '/viewaward';
                res.redirect(url);
            }
        });
    }
});

module.exports = router;
