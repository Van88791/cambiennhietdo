var fs = require('fs');
var url = require('url');
var http = require('http');
var WebSocket = require('ws');
var mysql = require('mysql');
var express = require('express');
var app = express();
const path = require('path');
const bodyparser = require('body-parser');
const session = require('express-session');
const router = require('./router');
const con = require('./config/connection');


app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(bodyparser.json());
app.use(bodyparser.urlencoded({
    extended: true
}));

//use session
app.use(session({
    secret: 'secret',
    resave: false,
    saveUninitialized: true,
}));


// sử dụng router.js
app.use('/', router);

    
// create http server
var server = http.createServer(app);
var ws = new WebSocket.Server({
    server
});
var clients = [];

function broadcast(socket, data) {
    // console.log(clients.length);
    for (var i = 0; i < clients.length; i++) {
        if (clients[i] != socket) {
            clients[i].send(data);
        }
    }
}
ws.on('connection', function(socket, req) {
    clients.push(socket);
    socket.on('message', function(message) {
        // console.log('received: %s', message);
        broadcast(socket, message);
        var value = message.toString();
        if(parseInt(value)>=100)
            con.query("INSERT INTO cambien (noidung) VALUES (?)", value, ()=> {});
    });
    socket.on('close', function() {
        var index = clients.indexOf(socket);
        clients.splice(index, 1);
        console.log('disconnected');
    });
});

server.listen(3000);
console.log('Server listening on port 3000');