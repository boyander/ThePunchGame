var io = require('socket.io').listen(8888);

io.sockets.on('connection', function (socket) {
  socket.emit('handshake');
  socket.on('handshake-ok', function (data) {
    console.log('Login user with ID -> '+ data.user);
    socket.emit('get-shakes');
  });
  socket.on('shake-data', function (data) {
    console.log('User with ID -> '+ data.user + ' has a total of ' + data.accumulated);
    socket.emit('get-shakes');
  });
});
