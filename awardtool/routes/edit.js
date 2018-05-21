var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var bcrypt = require('bcryptjs');
var cloudinary = require('cloudinary');
const fileUpload = require('express-fileupload');
cloudinary.config({
    cloud_name: '',
    api_key: '',
    api_secret: ''
});


router.use('/', fileUpload());

router.get('/', function(req, res, next) {
    res.render('edit');
});

router.get('/:id', function(req, res, next) {
    var context = {};
    mysql.query("SELECT id, name, role, email, password FROM user WHERE id=?;", [req.params.id], function(err, rows, fields) {
        if (err) {
            next(err);
            return;
        }
        context.type = rows[0].role;
        context.userid = rows[0].id;
        context.name = rows[0].name;
        context.email = rows[0].email;
        context.password = rows[0].password;
        res.render('edit', context);
    });
});

router.post('/', function(req, res, next) {
    if (req.body['submit'] == "update") {
        var pass = '';
        mysql.query("SELECT password FROM user WHERE id=?;", [req.body.userid], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
            pass = rows[0].password;
            if (pass == req.body.password) {
                mysql.query("UPDATE user SET name=?, email=?, role=? WHERE id=?;", [req.body.name || null, req.body.email, req.body.type, req.body.userid], function(err, rows, fields) {
                    if (err) {
                        next(err);
                        return;
                    }
                });
            }
            else {
                var hash = bcrypt.hashSync(req.body.password);
                mysql.query("UPDATE user SET name=?, email=?, password=?, role=? WHERE id=?;", [req.body.name || null, req.body.email, hash, req.body.type, req.body.userid], function(err, rows, fields) {
                    if (err) {
                        next(err);
                        return;
                    }
                });
            }
            if (req.body.type == "user") {
                if (req.files.signature) {
                    var id = req.body.userid;
                    var imageId = "./images/" + id + ".jpg";
                    var file = req.files.signature;
                    file.mv(imageId, function(err) {
                        if (err)
                            return;
                    });
                    var imgUrl;
                    cloudinary.uploader.upload(imageId, function(result) {
                        imgUrl = result['url'];
                        mysql.query("UPDATE user SET signature=? WHERE id=?;", [imgUrl, id], function(err, rows, fields) {
                            if (err) {
                                next(err);
                                return;
                            }
                            res.redirect('/viewuser');
                        });
                    });
                }
                else {
                    res.redirect('/viewuser');
                }
            }
            else {
                res.redirect('/viewuser');
            }
        });
    }
    else if (req.body['submit'] == "delete") {
        mysql.query("DELETE FROM user WHERE id=?;", [req.body.userid], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
            res.redirect('/viewuser');
        });
    }
    else {
        res.redirect('/viewuser');
    }
});

module.exports = router;
