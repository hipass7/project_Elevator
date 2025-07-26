// server.js
const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 3000 });

// 실제 CAN 송신 부분 - socketcan 설치된 경우 사용
// const can = require('socketcan');
// const channel = can.createRawChannel("can0", true);
// channel.start();

function sendCANMock(id, data) {
  console.log(`[CAN-SEND] ID: 0x${id.toString(16)} | Data:`, data);
  // 실제 CAN 사용시: channel.send({ id, data: Buffer.from(data) });
}

wss.on('connection', ws => {
  ws.on('message', msg => {
    const { id, data } = JSON.parse(msg);
    sendCANMock(id, data);
  });
  console.log("🔌 Web client connected.");
});
