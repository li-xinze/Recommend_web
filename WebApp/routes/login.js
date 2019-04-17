var User = require('../lib/user');
var redis   = require('redis');
var R_db  = redis.createClient();

exports.form = function(req, res){
  res.render('login', { title: 'Login' });
};

exports.submit = function(req, res, next){
  var data = req.body.user;
  User.authenticate(data.name, data.pass, function(err, user){
    if (err) return next(err);
    if (user) {
      req.session.uid = user.id;
      res.redirect('/vip');
        var net = require('net');
      var HOST = '10.44.100.63';
      var PORT = 6666;
      var client = new net.Socket();
      client.connect(PORT, HOST, function() {
      console.log('CONNECTED TO: ' + HOST + ':' + PORT);
      client.write(user.id);
      });
      client.on('data', function(data) {
      var Data = String(data)
      console.log('DATA: ' + data);
      R_db.hmset('user:'+user.id ,{
        'recommend': data 
      }, redis.print);
      client.destroy();
      })
     
      client.on('close', function() {
      console.log('Connection closed');
      });
      
      }
else {
      res.error("帐号或密码错误");
      res.redirect('back');
    }

  });
};

exports.logout = function(req, res){
  req.session.destroy(function(err) {
    if (err) throw err;
    res.redirect('/');
  });
};
