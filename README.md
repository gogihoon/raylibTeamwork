🎵 Raylib Music Player
C 언어로 만든 간단한 GUI 음악 플레이어입니다.
raylib과 raygui를 이용해 .ogg, .wav 파일을 재생할 수 있습니다.

✨ 기능
music/ 폴더 내 .wav, .ogg 음악 파일 자동 탐색

재생, 일시정지, 이전곡 / 다음곡 이동

반복 재생 ON/OFF

📁 사용 방법
프로젝트 폴더에 music/ 폴더를 만들고 음악 파일(.ogg, .wav)을 넣습니다.

복사
편집
project_root/
├── music/
│   ├── song1.ogg
│   ├── song2.wav
├── test.c
├── raygui.h
└── ...
컴파일:

bash
복사
편집
gcc test.c -o music_player -lraylib -lwinmm -lgdi32 -lraygui
⚠️ raylib과 raygui 헤더 파일이 필요하며, raygui.h는 소스와 함께 있어야 합니다.

실행:

bash
복사
편집
./music_player
🛠️ 구현된 컨트롤
Prev : 이전 곡으로 이동

Play / Pause : 재생 / 일시정지 전환

Next : 다음 곡으로 이동

Repeat: ON/OFF : 반복 재생 여부 전환

현재 재생 중인 곡 제목 표시

📦 의존성
raylib

raygui
