// const express = require('express');
// const app = express();
// const port = 3000;

// const can = require('socketcan');
// let channel = can.createRawChannel("can0", true); // can0 인터페이스를 사용합니다.
// channel.start();

// app.use(express.json());
// app.use(express.static("public"));

// app.post('/send', (req, res) => {
//   const { id, data } = req.body;

//   try {
//     const canMessage = {
//       id: parseInt(id, 16),
//       data: Buffer.from(data.split(' ').map(byte => parseInt(byte, 16)))
//     };

//     channel.send(canMessage);
//     console.log("송신된 CAN 메시지:", canMessage);
//     res.send("메시지 전송 완료");
//   } catch (e) {
//     console.error("전송 실패:", e);
//     res.status(400).send("메시지 포맷 오류");
//   }
// });

// app.listen(port, () => {
//   console.log(`웹 서버 실행 중: http://localhost:${port}`);
// });

const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');
const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.static('public'));

app.post('/send', (req, res) => {
  const { can_id, data } = req.body;
  console.log(`수신된 메시지 → CAN ID: ${can_id}, DATA: ${data}`);
  res.sendStatus(200);
});

app.listen(port, () => {
  console.log(`웹 서버 실행 중: http://localhost:${port}`);
});
