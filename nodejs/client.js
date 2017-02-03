var net = require('net');

var args = process.argv.slice(2);
console.log(args)
var url = args.split(':');
//var port = args.split(':')[1] || 80;
//port = port.split('/')[0] || port;
//var path = port.split('/')[1] || "index.html";

//console.log(args);
//console.log(port);
//console.log(path);

var client = new net.Socket();
client.connect(9003, function() {
	console.log('Connected');
	client.write('GET http://www.google.com/ HTTP/1.1\n\n');
});

client.on('data', function(data) {
	console.log('Received: ' + data);
	client.destroy();
});

client.on('close', function() {
	console.log('Connection closed');
});