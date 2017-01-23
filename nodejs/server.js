/*
var http = require('http');
var static = require('node-static');
var file = new static.Server('./public');

http.createServer(function (request, response) {
    console.log(`${request.method} ${request.url}`);
    file.serve(request, response);
}).listen(9005);
*/

var fs = require('fs'),
    http = require('http');

http.createServer(function (req, res) {
  fs.readFile('./public/' + (req.url == '/' ? '/index.html' : req.url), function (err,data) {
    if (err) {
      res.writeHead(404);
      res.end("Not Found");
      return;
    }
    res.writeHead(200);
    res.end(data);
  });
}).listen(7897);
