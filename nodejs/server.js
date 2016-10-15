var static = require('node-static');

var file = new static.Server('./www');

require('http').createServer(function (request, response) {
    request.addListener('end', function () {
        console.log(request.headers);
        file.serve(request, response);
    }).resume();
}).listen(9005);