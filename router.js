const { Router } = require('express');
const e = require('express')
var express = require('express');
const con = require('./config/connection');
var router = express.Router();


router.get('/', (req, res) => {
    if(req.session.user) {
        res.render('index', {
            user: req.session.user,
        });
    } else {
        res.render('index',{
            user: req.session.user,
        });
    }
});

router.get('/login', (req, res) => {
    res.render('login');
});



router.get('/logout', (req, res) => {
    req.session.destroy(function(err) {
        if(err) {
            console.log(err)
        } else {
           res.render('logout');
        }
    });    
});

router.get('/database', (req, res) => {
    var sql = "SELECT * FROM cambien";
    con.query(sql, function(err, results) {
    if (err) throw err;
    res.render('database',{
        results: results
    });
  });
});


router.post('/login', function(req, res) {
    var username = req.body.username;
	var password = req.body.password;
	if (username && password) {
		con.query('SELECT * FROM user WHERE username = ? AND password = ?', [username, password], function(error, results, fields) {
			if (results.length > 0) {
				req.session.loggedin = true;
				req.session.user = req.body.username;
				res.redirect('/');
			} else {
				res.send('Incorrect Username and/or Password!');
			}			
			res.end();
		});
	} else {
		res.send('Please enter Username and Password!');
		res.end();
	}
});




module.exports = router;