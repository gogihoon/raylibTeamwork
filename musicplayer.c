#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

#define MUSIC_FOLDER "music/"  // 음악 파일이 들어 있는 폴더

// 지원되는 오디오 파일 형식인지 확인 (.ogg 또는 .wav)
bool IsSupportedAudioFile(const char *filename) {
    const char *ext = GetFileExtension(filename);
    return (TextIsEqual(ext, ".ogg") || TextIsEqual(ext, ".wav"));
}

int currentTrack = 0;       // 현재 재생 중인 트랙 번호
bool isRepeating = false;   // 반복 재생 여부

// 음악을 불러오고 재생하는 함수
void LoadAndPlay(Music *music, const char *filepath) {
    if (IsMusicStreamPlaying(*music)) StopMusicStream(*music);  // 현재 재생 중이면 정지
    UnloadMusicStream(*music);                                   // 이전 스트림 해제
    *music = LoadMusicStream(filepath);                          // 새 음악 로드
    PlayMusicStream(*music);                                     // 음악 재생
}

int main(void)
{
    InitWindow(600, 300, "Music Player");  // 윈도우 생성
    InitAudioDevice();                     // 오디오 장치 초기화

    FilePathList files = LoadDirectoryFiles(MUSIC_FOLDER);  // 폴더에서 파일 목록 불러오기

    int musicCount = 0;                     // 플레이리스트에 담긴 곡 수
    char *playlist[256];                    // 최대 256곡 저장할 수 있는 배열

    // 지원되는 오디오 파일만 플레이리스트에 추가
    for (int i = 0; i < files.count && musicCount < 256; i++) {
        if (IsSupportedAudioFile(files.paths[i])) {
            playlist[musicCount] = TextFormat("%s%s", MUSIC_FOLDER, GetFileName(files.paths[i]));
            musicCount++;
        }
    }

    // 음악 파일이 하나도 없으면 종료
    if (musicCount == 0) {
        TraceLog(LOG_ERROR, "지원되는 음악 파일이 폴더에 없습니다: %s", MUSIC_FOLDER);
        CloseAudioDevice();
        CloseWindow();
        return 1;
    }

    Music music = LoadMusicStream(playlist[currentTrack]);  // 첫 번째 곡 로드
    PlayMusicStream(music);                                 // 재생 시작

    SetTargetFPS(60);  // 60 FPS 설정

    while (!WindowShouldClose())  // 창을 닫을 때까지 반복
    {
        UpdateMusicStream(music);  // 스트림 업데이트 (필수)

        // 음악이 끝났을 때 처리
        if (!IsMusicStreamPlaying(music) && GetMusicTimePlayed(music) >= GetMusicTimeLength(music) - 0.1f) {
            if (isRepeating) PlayMusicStream(music);  // 반복 재생이면 다시 재생
            else {
                currentTrack = (currentTrack + 1) % musicCount;  // 다음 곡으로 이동
                LoadAndPlay(&music, playlist[currentTrack]);
            }
        }

        // 이전 곡 버튼
        if (GuiButton((Rectangle){ 60, 200, 80, 40 }, "Prev")) {
            currentTrack = (currentTrack - 1 + musicCount) % musicCount;
            LoadAndPlay(&music, playlist[currentTrack]);
        }

        // 재생/일시정지 버튼
        if (GuiButton((Rectangle){ 160, 200, 80, 40 }, IsMusicStreamPlaying(music) ? "Pause" : "Play")) {
            if (IsMusicStreamPlaying(music)) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }

        // 다음 곡 버튼
        if (GuiButton((Rectangle){ 260, 200, 80, 40 }, "Next")) {
            currentTrack = (currentTrack + 1) % musicCount;
            LoadAndPlay(&music, playlist[currentTrack]);
        }

        // 반복 재생 ON/OFF 버튼
        if (GuiButton((Rectangle){ 360, 200, 120, 40 }, isRepeating ? "Repeat: ON" : "Repeat: OFF")) {
            isRepeating = !isRepeating;
        }

        // 화면 그리기 시작
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Now playing:", 60, 60, 20, DARKGRAY);  // 텍스트 출력
        DrawText(GetFileName(playlist[currentTrack]), 60, 100, 24, MAROON);

        EndDrawing();  // 화면 그리기 종료
    }

    // 종료 전 자원 해제
    UnloadMusicStream(music);
    UnloadDirectoryFiles(files);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
