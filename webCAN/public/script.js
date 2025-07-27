const socket = new WebSocket(`ws://${location.host}`);

function sendCANMessage(id, data) {
  socket.send(JSON.stringify({ id, data }));
}

function sendExternalCall(floor, direction) {
  sendCANMessage(0x400, [floor, direction]); // 외부 호출
}

function sendInternalSelect(floor) {
  sendCANMessage(0x102, [floor]); // 내부 선택
}

function sendDoorCommand(cmd) {
  sendCANMessage(0x103, [cmd]); // 문 열기/닫기
}

// 버튼 UI 생성
const floors = [1, 2, 3, 4, 5];
const externalPanel = document.getElementById("external-panel");
const internalPanel = document.getElementById("internal-panel");

for (let f of floors) {
  if (f < 5) {
    const upBtn = document.createElement("button");
    upBtn.className = "btn";
    upBtn.innerText = `${f}층 ▲`;
    upBtn.onclick = () => sendExternalCall(f, 1);
    externalPanel.appendChild(upBtn);
  }
  if (f > 1) {
    const downBtn = document.createElement("button");
    downBtn.className = "btn";
    downBtn.innerText = `${f}층 ▼`;
    downBtn.onclick = () => sendExternalCall(f, 0);
    externalPanel.appendChild(downBtn);
  }

  const floorBtn = document.createElement("button");
  floorBtn.className = "btn";
  floorBtn.innerText = `${f}층`;
  floorBtn.onclick = () => sendInternalSelect(f);
  internalPanel.appendChild(floorBtn);
}
