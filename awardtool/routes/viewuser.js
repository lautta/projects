var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var cheerio = require('cheerio');

router.get('/', function(req, res, next) {
    var context = {};
    mysql.query("SELECT id, name, role, email, creation, signature FROM user;", function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        context.result = rows;
        res.render('viewuser', context);
    });

});

router.get('/user', function(req, res, next) {
    cheerio.load(req);
    var context = {};
    mysql.query("SELECT  id, name, role, email,  creation, signature FROM user where role='user';", function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        context.result = rows;
        res.render('viewuser', context);
    });
});

router.get('/admin', function(req, res, next) {
    var context = {};
    mysql.query("SELECT  id, role, email,  creation, signature FROM user where role='admin';", function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        context.result = rows;
        res.render('viewuser', context);
    });
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "view") {
        //var url = '/award/' + req.body.id;
        res.send(req.body.id);
    }
    else if (req.body['submit'] == "edit") {
        res.redirect('/edit');
    }
    else {
        var id = req.params.id;
        console.log(id);
        mysql.query("DELETE FROM user WHERE id=?;", [req.body.id], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
        });
        res.redirect("/viewuser");
    }

});

module.exports = router;
