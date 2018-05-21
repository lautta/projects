var express = require('express');
var router = express.Router();
var mysql = require('../db.js');
var Quiche = require('quiche');

const MonthList = {"Jan":1, "Feb":2, "Mar": 3, "Apr": 4, "May": 5, "Jun": 6, "Jul": 7, "Aug":8, "Sep": 9, "Oct":10, "Nov":11, "Dec":12};
router.get('/:id', function(req, res, next) {
    var context = [];
    var id = req.params.id;
    var timeList = id.split("-");
    var year = parseInt(timeList[0]);
    var month = parseInt(timeList[1]);
  var jsonobject = {};
  mysql.query("SELECT recipient, awarded FROM award WHERE type_id = 1;", function(err, rows, fields){
     if (err) {
         next(err);
         return;
     }

     var timeList=[];

       for(var i = 0; i < rows.length; i++) {
         var time = rows[i].awarded;
         var timeStr = String(time);
         var timeSplit = timeStr.split(" ");
         timeList.push(timeStr);
         timeList.push(timeSplit[3]);
         timeList.push(timeSplit[1]);
         timeList.push(MonthList[timeSplit[1]]);
         if (parseInt(timeSplit[3]) == year && MonthList[timeSplit[1]] == month){
             if (!jsonobject.hasOwnProperty(rows[i].recipient)) {
                 jsonobject[rows[i].recipient] = 1;
             } else {
                jsonobject[rows[i].recipient] = jsonobject[rows[i].recipient] + 1;
             }
         }
     }
     for (var i in jsonobject) {
         var object = {};
         object["name"] = i;
         object["number"] = jsonobject[i];
         context.push(object);
     }

     var newJson = JSON.stringify(jsonobject);
     var numList = [];
     var pie = new Quiche('pie');
     pie.setTransparentBackground();
     for(var i in jsonobject) {
         pie.addData(jsonobject[i], i);
         numList.push(i);
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
     res.render("chartmonth", {result:context, imageUrl:imageUrl, winner: result});
  });




});


module.exports = router;
