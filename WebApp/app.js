/**
 * Module dependencies.
 */

var api = require('./routes/api');
var entries = require('./routes/entries');
var express = require('express');
var routes = require('./routes');
var http = require('http');
var path = require('path');
var register = require('./routes/register');
var messages = require('./lib/messages');
var login = require('./routes/login');
var user = require('./lib/middleware/user');
var validate = require('./lib/middleware/validate');
var page = require('./lib/middleware/page');
var Entry = require('./lib/entry');

var app = express();

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.cookieParser('your secret here'));
app.use(express.session());
app.use(express.static(path.join(__dirname, 'public')));
app.use('/api', api.auth);
app.use(user);
app.use(messages);
app.use(app.router);
app.use(routes.notfound);
app.use(routes.error);

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/register', register.form);
app.get('/rcm', entries.recommend);
app.post('/register', register.submit);
app.post('/rcm',entries.full);
//app.post('/index', entries.search);
app.get('/login', login.form);
app.post('/login', login.submit);
app.post('/', entries.search);
app.post('/vip', entries.search);
app.post('/good', entries.star);
app.post('/result',entries.full2);
app.get('/logout', login.logout);
app.get('/vip', entries.vip);
app.get('/movie', entries.form);
app.get('/', entries.main);
app.post(
   '/movie',
   validate.required('entry[title]'),
   validate.lengthAbove('entry[title]', 4),
   entries.submit
);
app.get('/api/user/:id', api.user);
app.post('/api/entry', entries.submit);
app.get('/api/entries/:page?', page(Entry.count), api.entries);
app.get('/movie:page?', page(Entry.count, 5), entries.list);

if (process.env.ERROR_ROUTE) {
  app.get('/dev/error', function(req, res, next){
    var err = new Error('database connection failed');
    err.type = 'database';
    next(err);
  });
}

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});
