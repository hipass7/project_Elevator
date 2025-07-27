sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
candump vcan0

webCAN directory에서
1)
npm install
npm start

or

2)
npm init -y
npm install ws socketcan express

node server.js
http://localhost:3000 접속 (localhost는 서버 IP 사용)