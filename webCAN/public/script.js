const socket = new WebSocket(`ws://${location.host}`);

const floors = [5, 4, 3, 2, 1];
const logEl = document.getElementById("log");

// 로그 기록
function log(msg) {
  logEl.textContent += `[${new Date().toLocaleTimeString()}] ${msg}\n`;
  logEl.scrollTop = logEl.scrollHeight;
}

socket.onopen = () => log("✅ WebSocket 연결됨");
socket.onerror = (e) => log("❌ WebSocket 오류 발생");

socket.onmessage = (event) => {
  const message = JSON.parse(event.data);

  if (message.type === "status") {
    const elevatorId = message.elevatorId; // CAN ID 대신 elevatorId 사용
    const floor = message.floor;
    const status = message.status;
    const door = message.door;
    const direction = message.direction;

    if (elevatorId >= 0x000 && elevatorId <= 0x0FF && status == 0x01) {
      // 상태 텍스트 갱신
      const status = document.getElementById(`status-${elevatorId}`);
      const doorStr = door === 1 ? "열림" : "닫힘";
      const dirStr = direction === 1 ? "▲" : (direction === 255 ? "▼" : "■");
      status.textContent = `층: ${floor}, 문: ${doorStr}, 방향: ${dirStr}`;

      // 캐빈 위치 이동
      const cab = document.getElementById(`cab-${elevatorId}`);
      const topPx = (5 - floor) * 40; // 40px per floor
      cab.style.top = `${topPx}px`;
      cab.className = `cab ${door === 1 ? "open" : "closed"}`;
    }
    
  }
};

function sendCANMessage(id, data) {
  const msg = { id, data };
  socket.send(JSON.stringify(msg));
  log(`📤 전송: ${JSON.stringify(msg)}`);
}

function sendExternalCall(floor, direction, elevatorId) {
  sendCANMessage(0x100 + elevatorId, [floor, direction]);
}

function sendInternalSelect(floor, elevatorId) {
  sendCANMessage(0x100 + elevatorId, [floor]);
}

function sendDoorCommand(cmd, elevatorId) {
  sendCANMessage(0x200 + elevatorId, [cmd]);
}

// 외부 호출 UI 생성
const externalPanel = document.getElementById("external-panel");
const elevatorId = 1; // 외부 호출이 연결될 엘리베이터 ID
floors.forEach(f => {
  const row = document.createElement("div");
  row.className = "floor";

  const label = document.createElement("div");
  label.className = "floor-label";
  label.innerText = `${f}층`;
  row.appendChild(label);

  if (f < 5) {
    const upBtn = document.createElement("button");
    upBtn.className = "btn";
    upBtn.innerText = "▲";
    upBtn.onclick = () => sendExternalCall(f, 1, elevatorId);
    row.appendChild(upBtn);
  }

  if (f > 1) {
    const downBtn = document.createElement("button");
    downBtn.className = "btn";
    downBtn.innerText = "▼";
    downBtn.onclick = () => sendExternalCall(f, 0, elevatorId);
    row.appendChild(downBtn);
  }

  externalPanel.appendChild(row);
});

// 내부 패널 UI 생성
[1, 2].forEach(elevatorId => {
  const panel = document.getElementById(`internal-panel-${elevatorId}`);
  for (let f = 1; f <= 5; f++) {
    const btn = document.createElement("button");
    btn.className = "btn";
    btn.innerText = `${f}층`;
    btn.onclick = () => sendInternalSelect(f, elevatorId);
    panel.appendChild(btn);
  }
});
