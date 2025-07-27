const express = require("express");
const WebSocket = require("ws");
const can = require("socketcan");
const http = require("http");

// Express 서버 설정
const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// 정적 파일 서빙 (index.html, script.js 등)
app.use(express.static("public"));

// WebSocket 연결 처리
wss.on("connection", (ws) => {
  console.log("🔌 Web client connected");

  ws.on("message", (message) => {
    const msg = JSON.parse(message);
    console.log("📤 Web -> CAN:", msg);

    // CAN 메시지 전송
    const frame = {
      id: msg.id,
      ext: false,
      data: Buffer.from(msg.data)
    };
    canChannel.send(frame);
  });
});

// socketCAN 설정
const canChannel = can.createRawChannel("vcan0", true);

canChannel.addListener("onMessage", (msg) => {
  console.log(`📥 CAN -> Web: ID=0x${msg.id.toString(16)} DATA=[${[...msg.data].join(",")}]`);

  // 클라이언트에게 브로드캐스트
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify({
        id: msg.id,
        data: [...msg.data]
      }));
    }
  });
});

canChannel.start();

// 서버 실행
const PORT = 3000;
server.listen(PORT, () => {
  console.log(`🚀 Server listening on http://localhost:${PORT}`);
});
