const express = require("express");
const WebSocket = require("ws");
const path = require("path");
const can = require("socketcan"); // socketCAN 라이브러리

const app = express();
const port = 3000;

app.use(express.static(path.join(__dirname, "public")));

const server = app.listen(port, () => {
  console.log(`🚀 http://localhost:${port}`);
});

// WebSocket 서버
const wss = new WebSocket.Server({ server });

// CAN 채널 오픈 (vcan0)
const channel = can.createRawChannel("vcan0", true);
channel.start();

wss.on("connection", ws => {
  console.log("📡 클라이언트 연결됨");

  ws.on("message", msg => {
    console.log("수신:", msg.toString());
    try {
      const parsed = JSON.parse(msg.toString());
      // 브라우저에서 보낸 메시지를 vcan0로 전송
      if (parsed.id !== undefined && parsed.data !== undefined) {
        channel.send({ id: parsed.id, data: Buffer.from(parsed.data) });
        console.log(`CAN 전송: ID=0x${parsed.id.toString(16)}, Data=[${parsed.data}]`);
      }
    } catch (e) {
      console.error("메시지 파싱 오류:", e);
    }
  });
});

// vcan0에서 CAN 메시지 수신 → WebSocket으로 브라우저 전송
channel.addListener("onMessage", frame => {
  // frame: { id, data: Buffer }
  const msg = {
    type: "status",
    elevatorId: frame.id,           // 0x000 + elevatorId
    floor: frame.data[0],           // CAN data[0] = floor
    status: frame.data[1],            // CAN data[1] = door 상태
    direction: frame.data[2],        // CAN data[2] = 방향
    door: frame.data[3]
  };

  const msgStr = JSON.stringify(msg);
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) client.send(msgStr);
  });
  console.log("CAN 수신 → 브라우저 전송:", msgStr);
});

console.log("🟢 WebSocket 서버 + vcan0 CAN 채널 준비 완료");
