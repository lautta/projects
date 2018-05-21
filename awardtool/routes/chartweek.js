var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var Quiche = require('quiche');

const MonthList = {"Jan":1, "Feb":2, "Mar": 3, "Apr": 4, "May": 5, "Jun": 6, "Jul": 7, "Aug":8, "Sep": 9, "Oct":10, "Nov":11, "Dec":12};
router.get('/:id', function(req, res, next) {
    var week = req.params.id;
    var timeList = week.split("-");
    var year = timeList[0];
    var week = timeList[1].slice(1);
    var startDate = new Date(parseInt(year), 0, 1);
    var endDate = new Date(parseInt(year), 0, 1);
    startDate.setDate(startDate.getDate() + (7 * (parseInt(week)-1) + 1));
    endDate.setDate(endDate.getDate() + 7*parseInt(week));
    var jsonobject = {};
      mysql.query("SELECT recipient, awarded FROM award WHERE type_id = 2;", function(err, rows, fields){
         if (err) {
             next(err);
             return;
         }


       for(var i = 0; i < rows.length; i++) {
           var timeString = String(rows[i].awarded);
           var timesplit = timeString.split(" ");
           var year = parseInt(timesplit[3]);
           var month = MonthList[timesplit[1]];
           var day =  parseInt(timesplit[2]);
           var curDate = new Date(year, month - 1, day);
           if (curDate.getTime() >= startDate.getTime() && curDate.getTime() < endDate.getTime()) {
             if (!jsonobject.hasOwnProperty(rows[i].recipient)) {
                 jsonobject[rows[i].recipient] = 1;
             } else {
                jsonobject[rows[i].recipient] = jsonobject[rows[i].recipient] + 1;
             }
           }
     }
     var newJson = JSON.stringify(jsonobject);
     var pie = new Quiche('pie');
     pie.setTransparentBackground();
     var context = [];
     for (var i in jsonobject) {
         var object = {};
         object["name"] = i;
         object["number"] = jsonobject[i];
         context.push(object);
     }
     for(var i in jsonobject) {
         pie.addData(jsonobject[i], i);
     }
     var winner = "";
     var value = 0;
     for(var i in jsonobject) {
         if(jsonobject[i] > value) {
             value = jsonobject[i];
             winner = i;
         }
     }
     var result = []
     for (var i in jsonobject) {
         if (jsonobject[i] == value) {
             result.push(i)
         }
     }
     var imageUrl = pie.getUrl(true);
    res.render("chartweek", {result:context, imageUrl:imageUrl, winner: result});

  });

});


module.exports = router;
