# project_Elevator
Type : project

Component : Elevator

---

## Contents

Elevator 동작 과정을 나만의 과정으로 해석하여 Python 프로그래밍 언어 프로젝트로 구현한다.
최종목표 : 현재 공부하고 있는 Rust 프로그래밍 언어로 프로젝트 구현하기

[240727]

- main 실행 파일에서 argument로 최대 층수를 입력받도록 설정 완료
- 최대 층수만큼 process를 생성 (각 층마다 한 개의 process)
- 각 층에서 버튼을 누른 경우 main에 trigger가 되도록 하는 기능 구현 (trigger.py)

<br />

[240728]

- main 실행 파일에서 각 층의 process로부터 up, down trigger를 정상적으로 받도록 설정하고 그에 따른 동작 구현 완료
- trigger 실행 파일에 대해서 up, down 기능 구현 완료
- 엘리베이터 이동 알고리즘 외 모두 구현 완료 (현재는 임시로 생성형 AI에 의한 알고리즘 사용중)
- 엘리베이터 내에서 층을 입력했을 때의 상황에 대해서 로직 추가가 필요함

<br />
<br />

---

### Reference

