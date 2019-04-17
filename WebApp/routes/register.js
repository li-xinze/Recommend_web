var User = require('../lib/user');


exports.form = function(req, res){
  res.render('register', { title: '注册' });
};

exports.submit = function(req, res, next){
  var data = req.body.user;
  User.getByName(data.name, function(err, user){
    if (err) return next(err);

    if (user.id) {
      res.error("用户名已被注册，请重新输入!");
      res.redirect('back');
     }
    if(data.pass1 != data.pass2){
      res.error("两次密码不一致，请重新输入!");
      res.redirect('back');
     }
     else {
      user = new User({
        name: data.name,
        pass: data.pass1,
        email:data.email,
        sex:data.sex,
        recommend:'null'
      });

      user.save(function(err){
        if (err) return next(err);
        req.session.uid = user.id;
        res.redirect('/');
      });
    }
  });
};
