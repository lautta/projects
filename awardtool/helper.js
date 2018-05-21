//User portal/login help functions
module.exports = {
    //check that session is authorized for admin route
    authAdmin: function(req, res, next) {
        if (!req.session.authenticated || req.session.role != 'admin') {
            res.redirect('/');
            return;
        }
        next();
    },
    //check that session is authorized for user route
    authUser: function(req, res, next) {
        if (!req.session.authenticated || req.session.role != 'user') {
            res.redirect('/');
            return;
        }
        next();
    },
    //check form input string is not null or only whitespace
    isEmpty: function(input) {
        return !input || !input.trim();
    },
    //generate random password for password reset
    genPass: function(len) {
        var password = '';
        var charset = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-=+;:,.?';
        while (len > 0) {
            password += charset[Math.round(Math.random() * (charset.length - 1))];
            len--;
        }
        return password;
    }
};
