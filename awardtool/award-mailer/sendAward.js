
var fs = require('fs');
var mysql = require('mysql');
const latex = require('node-latex')
const path = require('path');

setInterval(function() {
//uses the mysql package to connect to the mysql database
    var connection = mysql.createConnection({
        host     : 'localhost',
        user     : 'lautta',
        password : '',
        database : 'c9'
    });

    connection.connect();
    //queries for awards created in the last minute
    var query = connection.query("SELECT award.id as id,recipient,rec_email, type_id,awarded,name,user.id as userid FROM award join user on award.awarder_id = user.id WHERE AddTime <= now() AND AddTime >= now() - interval 1 minute");
    query
        .on('error', function(err) {
            console.log(err);
        })
        .on('fields', function(fields) {
        })
        .on('result', function(row) {

            console.log(row)
            //each award created in the last minute is its own row
            //for each row returned, creates a .tex file, converts the .tex file to pdf, and then uses mailgun to email it to the winner

  function getFormattedDate(date) {
     var year = date.getFullYear();
     var month = (1 + date.getMonth()).toString();
     month = month.length > 1 ? month : '0' + month;
     var day = date.getDate().toString();
     day = day.length > 1 ? day : '0' + day;
     return month + '-' + day + '-' + year;
        };
            var awardDate = getFormattedDate(row.awarded);
            var sendName = row.name;
            var recName = row.recipient;
            var texFile = "certificate" + row.id + ".tex";
            var pdfFile = "certificate" + row.id + ".pdf";

var certType
if (row.type_id == 1) {
    certType = 'Employee of the Month';
} else {
    certType = 'Employee of the Week';
}
//this latex is what appears in the .tex file
            var docString = "\\documentclass{article}\n\\usepackage[english]{babel}\n\\usepackage{graphicx}\n\\graphicspath{{/home/ubuntu/workspace/images/}}\n\\pagenumbering{gobble}\n\\date{}\n\\title{\\huge "+ certType +"}\n\\begin{document}\n\\maketitle\n\\section*{\\centering Presented to " + recName + " by " + sendName + " on " + awardDate + " for outstanding performance}\n\\centering\n\\includegraphics[width=2in]{" + row.userid + "}\n\\end{document}";
            var doc = [
                docString
            ];
            fs.writeFile(texFile, doc, function(err) {
                if(err) {
                    return console.log(err);
                }


                 const fs = require('fs')

                 const input = fs.createReadStream(texFile)
                 const output = fs.createWriteStream(pdfFile)

                 latex(input).pipe(output)
                 output.on('close', function(){
                 console.log("wrote pdf")
                 console.log(texFile)
//input mailgun api_key and verified domain here
                 var api_key = '';
                 var domain = '';
                 var mailgun = require('mailgun-js')({apiKey: api_key, domain: domain});
                 var filepath = path.join('', pdfFile);

                 var data = {
                 from: 'Admin <admin@octans.com>',
                 to: row.rec_email,
                 subject: 'Thanks for all your hard work!',
                 text: 'Your manager has sent you an employee recognition award.',
                 attachment: filepath
                 };

                 mailgun.messages().send(data, function (error, body) {
                 console.log(body);
                 });


                 });


                 });

                             })
                .on('end', function() {
                    console.log("1 minute interval has completed")

                    connection.end()

                });

        }, 60 * 1000);
        //runs once a minute
        //set to run forever with foreverjs
