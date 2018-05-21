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
    res.render('adduser');
});


router.post('/', function(req, res, next) {
    if (req.body['submit'] == "submit") {
        var type = "user";
        if (req.body.accounttype == '1') {
            type = "user";
        }
        else {
            type = "admin";
        }
        mysql.query("SELECT id FROM user WHERE email=?", [req.body.email], function(err, rows, fields) {
            if (err) {
                next(err);
                return;
            }
            if (rows.length != 0) {
                res.redirect('/adduser');
            }
            else {
                var hash = bcrypt.hashSync(req.body.password);
                mysql.query("INSERT INTO user (name, role, email, password) VALUES (?,?, ?, ?);", [req.body.name || null, type, req.body.email, hash], function(err, rows, fields) {
                    if (err) {
                        next(err);
                        return;
                    }
                    if (type == "user") {
                        mysql.query("SELECT id FROM user WHERE email=?", req.body.email, function(err, rows, fields) {
                            if (err) {
                                next(err);
                                return;
                            }
                            if (req.files.signature) {
                                var id = rows[0].id;
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
                        });
                    }
                    else {
                        res.redirect('/viewuser');
                    }
                });
            }
        });
    }
    else {
        res.redirect('/viewuser');
    }

});

module.exports = router;
