const express = require("express");
const WebSocket = require("ws");
const path = require("path");

const app = express();
const port = 3000;

app.use(express.static(path.join(__dirname, "public")));

const server = app.listen(port, () => {
  console.log(`🚀 http://localhost:${port}`);
});

const wss = new WebSocket.Server({ server });

let state = [
  { floor: 1, direction: 0, door: 0 }, // elevator 1
  { floor: 5, direction: 0, door: 0 }  // elevator 2
];

wss.on("connection", ws => {
  console.log("📡 클라이언트 연결됨");

  ws.on("message", msg => {
    console.log("수신:", msg.toString());
  });
});

// 테스트: 상태 가상 변화 시뮬레이션
setInterval(() => {
  for (let i = 0; i < 2; i++) {
    let e = state[i];
    e.floor += e.direction;

    if (e.floor >= 5) { e.floor = 5; e.direction = -1; }
    else if (e.floor <= 1) { e.floor = 1; e.direction = 1; }

    e.door = Math.random() > 0.5 ? 1 : 0;

    const status = {
      type: "status",
      elevatorId: i + 1,
      floor: e.floor,
      door: e.door,
      direction: e.direction
    };

    const msg = JSON.stringify(status);
    wss.clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(msg);
      }
    });
  }
}, 1000);
