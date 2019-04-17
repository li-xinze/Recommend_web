var levelup = require('levelup'); 
var leveldown = require('leveldown'); 
levelup(leveldown('/web_server/name'), function (err, db) {
      if (err) throw err
      var key = Search[i];
      db.get('6972', function (err, value) {
      	if (err) return console.log('Ooops!', err)
      		movie_name= value.toString();
      	console.log(movie_name);
      })
      db.close();
  })

