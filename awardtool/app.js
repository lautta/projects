var express = require('express');
var path = require('path');
var http = require('http');
var bodyParser = require('body-parser');
var mysql = require('./db.js');
var handlebars = require('express-handlebars').create({ defaultLayout: `main` });
var session = require('express-session');
var RedisStore = require('connect-redis')(session);
var flash = require('connect-flash');

var helper = require('./helper.js');
var viewuser = require('./routes/viewuser.js');
var addaward = require('./routes/addaward.js');
var password = require('./routes/password.js');
var login = require('./routes/login.js');
var adduser = require('./routes/adduser.js');
var useraccount = require('./routes/useraccount.js');
var award = require('./routes/award.js');
var edit = require('./routes/edit.js');
var viewaward = require('./routes/viewaward.js');
var search = require('./routes/search.js');
var chartweek = require('./routes/chartweek.js');
var chartmonth = require('./routes/chartmonth.js');

var app = express();
app.use(flash());
app.use(express.static('client/'));

//setup sessions with redis store using default redis server settings @ 127.0.0.1:6379
app.use(session({
    store: new RedisStore(),
    secret: 'hush',
    resave: false,
    saveUninitialized: false
}));

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.use('/', login);
app.use('/login', login);
app.use('/password', password);
app.use('/addaward', helper.authUser, addaward);
app.use('/useraccount', helper.authUser, useraccount);
app.use('/viewuser', helper.authAdmin, viewuser);
app.use('/adduser', helper.authAdmin, adduser);
app.use('/award', helper.authAdmin, award);
app.use('/edit', helper.authAdmin, edit);
app.use('/viewaward', helper.authAdmin, viewaward);
app.use('/search', helper.authAdmin, search);
app.use('/chartweek', helper.authAdmin, chartweek);
app.use('/chartmonth', helper.authAdmin, chartmonth);


//test server
app.get('/test', function(req, res) {
    res.type('text/plain');
    res.send('Testing!');
});

//destroy session and redirect for logout route
app.get('/logout', function(req, res, next) {
    req.session.destroy();
    res.redirect('/');
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});


// error handler
app.use(function(err, req, res, next) {
    context = {};
    context.status = err.status || 500;
    context.message = err.message || 'Server Error';
    res.render('error.handlebars', context);
});


var server = http.createServer(app);
server.timeout = 0;
server.listen(8080, "0.0.0.0", function() {
    var addr = server.address();
    console.log("Server listening at", addr.address + ":" + addr.port);
});

module.exports = app;
