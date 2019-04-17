var Entry = require('../lib/entry');
var redis   = require('redis');
var R_db  = redis.createClient();


exports.list = function(req, res, next){
  var page = req.page;
  Entry.getRange(page.from, page.to, function(err, entries) {
    if (err) return next(err);
    res.render('good', {
      title: 'Movie',
      entries: entries,
    });
  });
};
exports.main =function(req,res){
  res.render('mainpage',{title: 'Movie DB'});
  var levelup = require('levelup'); 
  var leveldown = require('leveldown'); 

};
exports.vip =function(req,res){
   res.render('vip',{title: 'Movie DB'});
};
exports.star =function(req,res){
  var rating = req.body.star;
  var id = req.session.uid;
  var movie_id = req.body.movie_id;
  console.log(rating);
  console.log(id);
  console.log(movie_id);
   var net = require('net');
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      
       var HOST = '10.44.100.63';
      var PORT = 7777;
      var client = new net.Socket();
      client.connect(PORT, HOST, function() {
      console.log('CONNECTED TO: ' + HOST + ':' + PORT);
      var message = id+','+movie_id+','+rating;
      client.write(message);
    })
      client.on('data', function(data) {
      var Data = String(data)
      console.log('DATA: ' + data);
      client.destroy();
      })
      client.on('close', function() {
      console.log('Connection closed');
      });
    }
exports.form = function(req, res){
  var img_path = 'images/2.jpg'
  var mv_content ='fdsfds'
  res.render('good', { 
    title: 'movie' ,
    path: img_path,
    content: mv_content,
    name :'ds'
  });
}
exports.full2 = function(req,res){
  var data = req.body.ok;
  var mv_content;
   var img_path;
   var movie_name;
      var first1 = '/web_server/contents/'
      var first2 = '/web_server/images/'
      var last = '.txt';
      var road1 = first1+data +last;
      var road2 = first2+data +last;
      var fs = require('fs');
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      levelup(leveldown('/web_server/name'), function (err, db) {
      if (err) throw err
      db.get(data, function (err, value) {
      if (err) return console.log('Ooops!', err) 
      movie_name= value.toString();
      db.close();
      console.log(movie_name);
      levelup(leveldown('/web_server/tags'), function (err, dc) {
      if (err) throw err
      var number = Number(data);
      dc.get(number, function (err, TAGS) {
      if (err) return console.log('Ooops!', err) 
      var tags = new Array();
      var Tags = TAGS.toString();
      var regex = /;/
      console.log(Tags[1]);
      tags=Tags.split(regex);
      movie_name= value.toString();
      fs.readFile(road1 , 'utf-8', function (err, mv_content) {
      if (err) {
        console.log(err);
      } else {
      fs.readFile(road2, 'utf-8', function (err, img_path) {
         if (err) {
           console.log(err);
        } else{
          res.render('good', { 
            movie_id : data,
          title: 'movie' ,
          path: img_path,
          content: mv_content,
          name : movie_name,
          tags : tags
       });
        }
      });
       }
     });
      dc.close();
    });
  });

    })

})
    }
 

exports.full = function(req,res){
   console.log('have a message');
   var data = req.body.full;
   var value_ ;
   if(data.a != '0'){
    value_ =  data.a;
   }
   if(data.b != '0'){
    value_ = data.b;
   }
   if(data.c != '0'){
    value_ = data.c;
   }
   if(data.d != '0'){
     value_ = data.d;
   }
   if(data.e != '0'){
     value_ = data.e;
   }
   if(data.f != '0'){
     value_ = data.f;
   }
   if(data.g != '0'){
     value_ = data.g;
   }
   if(data.h != '0'){
     value_ = data.h;
   }
   if(data.i != '0'){
     value_ = data.i;
   }
   if(data.j != '0'){
     value_ = data.j;
   }
   var mv_content;
   var img_path;
   var movie_name;
      var first1 = '/web_server/contents/'
      var first2 = '/web_server/images/'
      var last = '.txt';
      var road1 = first1+value_ +last;
      var road2 = first2+value_ +last;
      var fs = require('fs');
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      levelup(leveldown('/web_server/name'), function (err, db) {
      if (err) throw err
      db.get(value_, function (err, value) {
      if (err) return console.log('Ooops!', err) 
      movie_name= value.toString();
      db.close();
      console.log(movie_name);
      levelup(leveldown('/web_server/tags'), function (err, dc) {
      if (err) throw err
      var number = Number(value_);

      dc.get(number, function (err, TAGS) {
      if (err) return console.log('Ooops!', err) 
      var tags = new Array();
      var Tags = TAGS.toString();
      var regex = /;/
      console.log(Tags[1]);
      tags=Tags.split(regex);
      movie_name= value.toString();
      fs.readFile(road1 , 'utf-8', function (err, mv_content) {
      if (err) {
        console.log(err);
      } else {
      fs.readFile(road2, 'utf-8', function (err, img_path) {
         if (err) {
           console.log(err);
        } else{
          res.render('good', { 
          movie_id : value_,
          title: 'movie' ,
          path: img_path,
          content: mv_content,
          name : movie_name,
          tags : tags
       });
        }
      });
       }
     });
      dc.close();
    });
  });

    })

})
    }
exports.recommend = function(req,res){
      var Name = new Array(20);
      var Path = new Array(20);
      var id = req.session.uid;
      R_db.hget('user:'+id ,'recommend',function(err,value) {
      if (err) throw err;
      var RCM = new Array(20);
      var regex = /,/;
      splitArray=value.split(regex);
      console.log(splitArray[1]);
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      var i = 0;
      var j = 10 ;
      var k = 10;
      levelup(leveldown('/web_server/Link'), function (err, db) {
      if (err) throw err
      levelup(leveldown('/web_server/name'), function (err, dc) {
      if (err) throw err;
      for(i=10; i<20;i++){
        console.log(i);
        var key = splitArray[i] * 1;
      db.get(key, function (err, value) {
      if (err) return console.log('Ooops!', err) 
      var a = value.toString();
      RCM[j] = a;
      console.log(RCM[j]);
      var key2 = a;
      var first = '/movie_images/'
      var last = '.jpg';
      Path[j] = first+a+last;
      j++;
      dc.get(key2, function (err, value) {
      if (err) return console.log('Ooops!', err) // likely the key was not found
      movie_name= value.toString();
      Name[k] = movie_name;
      k++;
      while(k == 20){
        for(var i= 10 ;i<20;i++){
           console.log(Path[i]);
        }
        dc.close();
        db.close();
      res.render('recommand', { 
          path1: Path[10],
          path2: Path[11],
          path3: Path[12],
          path4: Path[13],
          path5: Path[14],
          path6: Path[15],
          path7: Path[16],
          path8: Path[17],
          path9: Path[18],
          path10: Path[19],
          name1: Name[10],
          name2: Name[11],
          name3: Name[12],
          name4: Name[13],
          name5: Name[14],
          name6: Name[15],
          name7: Name[16],
          name8: Name[17],
          name9: Name[18],
          name10: Name[19],
          value1:RCM[10],
          value2:RCM[11],
          value3:RCM[12],
          value4:RCM[13],
          value5:RCM[14],
          value6:RCM[15],
          value7:RCM[16],
          value8:RCM[17],
          value9:RCM[18],
          value10:RCM[19]
       });
      return;
    }
     })
    });
    };
    });
  });
    });
    }
exports.search = function(req,res){
      var data = req.body.search;
      var keyword = data.movie;
      var net = require('net');
      var HOST = '10.44.100.63';
      var Data;
      var Path = new Array();
      var Search = new Array();
      var PORT = 8888;
      var client = new net.Socket();
      client.connect(PORT, HOST, function() {
      console.log('CONNECTED TO: ' + HOST + ':' + PORT);
      client.write(keyword);
      });
      client.on('data', function(data) {
      var regex = /,/;
      Data = data.toString();
      Search=Data.split(regex);
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      var j = 0;
      var len = Search.length;
      console.log('length'+len);
      var S_N = new Array();
      levelup(leveldown('/web_server/tags'), function (err, db) {
      if (err) throw err
      for(i=0; i<len; i++){
        var key = Search[i];
      db.get(key, function (err, value) {
      if (err) {
        console.log(couting);
        if(couting )
        return console.log('Ooops!', err) 
      }
      var a = value.toString();
      S_N[j] = a;
      var first = '/movie_images/'
      var last = '.jpg';
      Path[j] = first+Search[j]+last;
      j++;
      while(j == len){
        db.close();
        console.log('begin');
        var Result = [];
        for(var k=0; k<j ;k++){
          console.log(S_N[k]);
          var J_N = i + "";
          var J_N ={
            'link' : Search[k],
            'name' : S_N[k],
            'path' : Path[k]
          }
          Result.push(J_N);
        }
           res.render('result', { 
          number : j,
          Result : Result
       });
           return;
        }
      })
    }
  })
      console.log('DATA: ' + data);
      client.destroy();
      })
      client.on('close', function() {
      console.log('Connection closed');
      });
    }
/*

   var mv_content;
   var img_path;
   var movie_name;
      var first1 = '/web_server/contents/'
      var first2 = '/web_server/images/'
      var last = '.txt';
      var no = 100;
      var road1 = first1+no +last;
      var road2 = first2+no +last;
      var fs = require('fs');
      var levelup = require('levelup'); 
      var leveldown = require('leveldown'); 
      levelup(leveldown('/web_server/name'), function (err, db) {
      if (err) throw err
      db.get(no, function (err, value) {
      if (err) return console.log('Ooops!', err) // likely the key was not found
      movie_name= value.toString();
    console.log(movie_name);
    fs.readFile(road1 , 'utf-8', function (err, mv_content) {
      if (err) {
        console.log(err);
      } else {
         fs.readFile(road2, 'utf-8', function (err, img_path) {
         if (err) {
           console.log(err);
        } else{
          res.render('good', { 
          title: 'movie' ,
          path: img_path,
          content: mv_content,
          name : movie_name
       });
        }
      });
       }
     });
    })
})
    }*/

      //.get(no, function (err, value) {
      //if (err) return console.log('Ooops!', err) // likely the key was not found
      //var movie_name = value.toString();
      //console.log(a);
      // ta da!
      
   // })
   // db.close();
    // }) 
    
   /*db.createReadStream()
  .on('data', function (data) {
    console.log(data.key, '=', data.value)
  })
  .on('error', function (err) {
    console.log('Oh my!', err)
  })
  .on('close', function () {
    console.log('Stream closed')
  })
  .on('end', function () {
    console.log('Stream ended')
  })*/
exports.submit = function(req, res, next){
  var data = req.body.entry;
  var entry = new Entry({
    "username": res.locals.user.name,
    "title": data.title,
    "body": data.body
  });

  entry.save(function(err) {
    if (err) return next(err);
    if (req.remoteUser) {
      res.json({message: 'Entry added.'});
    } else {
      res.redirect('/');
    }
  });
};