var mysql = require('mysql');
var pool = mysql.createPool({
    host: '127.0.0.1',
    user: 'lautta',
    password: '',
    database: 'c9',
    multipleStatements: true
});

module.exports = pool;
