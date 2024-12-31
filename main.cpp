#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>  // For file handling
#pragma comment(lib, "Winmm.lib") // Chỉ định liên kết thư viện Winmm.lib
using namespace std;

// Forward declaration
struct Point;
struct HighScoreEntry;
class SNAKE;

void GoToXY(int column, int line);
void GenerateFood(int& x_food, int& y_food, SNAKE& r);
void HideCursor();
void DrawBoundary(int gameMode);
void Blink(SNAKE& r);
void StartMenu();
void ClassicMode(int& level, int& score, vector<HighScoreEntry>& highscore);
void FreeMode(int& level, int& score, vector<HighScoreEntry>& highscore);
void SelectGameMode();
void SelectDifficultyLevel(int& level);
bool CheckFoodCollision(SNAKE& r, int x_food, int y_food);
void ReadHighScores(vector<HighScoreEntry>& highScores);
void WriteHighScores(vector<HighScoreEntry>& highScores);
void AddHighScore(int score);
void DisplayHighScore(int x, int y);
void PlayFoodSound();
void PlayGameOverSound();
void Countdown();

struct Point {
    int x, y;
};

class SNAKE {
public:
    struct Point body[100];
    int length;
    int speed;

    SNAKE() {
        length = 3;
        speed = 0;
        body[0].x = 8; body[0].y = 10;
        body[1].x = 7; body[1].y = 10;
        body[2].x = 6; body[2].y = 10;
    }

    void Ve() {
        // Vẽ con rắn ở các vị trí mới
        GoToXY(body[0].x, body[0].y);
        cout << "O";
        for (int i = 1; i < length; i++) {
            GoToXY(body[i].x, body[i].y);
            if (body[0].x == body[i].x && body[0].y == body[i].y) cout << "O";
            else cout << "-";  // In ký tự "-" để vẽ con rắn
        }
    }

    void DiChuyen(int direction, int& x_food, int& y_food, SNAKE& r, int gameMode, int level, int& score) {
        // Di chuyển con rắn và kiểm tra ăn mồi
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        // Cập nhật vị trí đầu con rắn theo hướng
        if (direction == 0) body[0].x = body[0].x + 1; // Phải
        if (direction == 1) body[0].y = body[0].y + 1; // Xuống
        if (direction == 2) body[0].x = body[0].x - 1; // Trái
        if (direction == 3) body[0].y = body[0].y - 1; // Lên

        if (gameMode == 2) {
            if (body[0].x > 100) body[0].x = 0;
            else if (body[0].x < 0) body[0].x = 100;
            if (body[0].y > 25) body[0].y = 0;
            else if (body[0].y < 0) body[0].y = 25;
            DrawBoundary(2);
        }

        // Kiểm tra nếu con rắn ăn mồi
        if (body[0].x == x_food && body[0].y == y_food) {
            PlayFoodSound();
            length++;  // Tăng độ dài con rắn
            GenerateFood(x_food, y_food, r); // Gọi hàm GenerateFood để tạo mồi mới
            speed--; //Mỗi khi tăng chiều dài tốc độ rắn tăng dần
            //Tính điểm
            if (gameMode == 1) {
                if (level == 1) score += 2;
                else if (level == 2) score += 4;
                else if (level == 3) score += 6;
            }
            else if (gameMode == 2) {
                if (level == 1) score += 1;
                else if (level == 2) score += 2;
                else if (level == 3) score += 3;
            }
        }

        // Kiểm tra việc đụng tường
        if (gameMode == 1) {
            if (body[0].x == 0 || body[0].x == 100 || body[0].y == 0 || body[0].y == 25) {
                PlayGameOverSound();
                Blink(r);
                system("cls");
                cout << "Game Over!" << endl;
                Sleep(2000);
                AddHighScore(score);  // Cập nhật điểm cao nếu cần
                StartMenu();
            }
        }

        // Kiểm tra việc cắn trúng thân
        for (int i = length - 1; i > 0; i--) {
            if (body[0].x == body[i].x && body[0].y == body[i].y) {
                PlayGameOverSound();
                Blink(r);
                system("cls");
                cout << "Game Over!" << endl;
                Sleep(2000);
                AddHighScore(score);  // Cập nhật điểm cao nếu cần
                StartMenu();
            }
        }
    }
};

struct HighScoreEntry {
    int score = 0;
    string name = "None";
};

int main() {
    StartMenu();
    Beep(1000, 200);
    return 0;
}

void StartMenu() {
    int choose = 0;
    cout << "WELCOME TO SNAKE GAME" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Start" << endl;
    cout << "2. Quit" << endl;
    cout << "You choose: ";
    cin >> choose;
    if (choose == 1) {
        system("cls");
        SelectGameMode();
    }
    else {
        cout << "Good bye!!!" << endl;
        exit(0);
    }
}

void SelectGameMode() {
    int choose = 0, level = 0, score = 0;
    vector<HighScoreEntry> highscore;
    ReadHighScores(highscore);  // Đọc điểm cao từ file
    cout << "PLEASE CHOOSE GAME Mode" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Classic Mode" << endl;
    cout << "2. Free Mode" << endl;
    cout << "3. Back" << endl;
    cout << "You choose: ";
    cin >> choose;

    try {
        if (choose == 1) {
            system("cls");
            SelectDifficultyLevel(level);
            ClassicMode(level, score, highscore);
        }
        else if (choose == 2) {
            system("cls");
            SelectDifficultyLevel(level);
            FreeMode(level, score, highscore);
        }
        else {
            system("cls");
            StartMenu();
        }
    } catch (const exception& e) {
        system("cls");
        cout << "An error occurred: " << e.what() << endl;
        cout << "Returning to main menu..." << endl;
        Sleep(2000); // Đợi 2 giây để người dùng đọc lỗi
        StartMenu();
    } catch (...) {
        system("cls");
        cout << "An unknown error occurred." << endl;
        cout << "Returning to main menu..." << endl;
        Sleep(2000); // Đợi 2 giây để người dùng đọc lỗi
        StartMenu();
    }
}

void SelectDifficultyLevel(int& level) {
    cout << "PLEASE CHOOSE GAME level" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "4. Back" << endl;
    cout << "You choose: ";
    cin >> level;
    system("cls");
    if (level == 4) SelectGameMode();
}

void ClassicMode(int& level, int& score, vector<HighScoreEntry>& highscore) {
    Countdown();
    HideCursor();
    srand(time(0)); // Khởi tạo seed cho hàm rand
    SNAKE r;
    int direction = 0, x_food = 0, y_food = 0;
    char t;

    // Tạo tốc độ rắn theo từng mức độ
    if (level == 1) r.speed = 200;
    else if (level == 2) r.speed = 100;
    else if (level == 3) r.speed = 50;

    // Tạo mồi ban đầu
    GenerateFood(x_food, y_food, r);

    DrawBoundary(1);

    while (1) {
        if (_kbhit()) {
            t = _getch();  // Nhận phím từ bàn phím
            if (t == -32) {
                t = _getch();
                if (t == 75 && direction != 0) direction = 2; // Mũi tên trái
                if (t == 72 && direction != 1) direction = 3; // Mũi tên lên
                if (t == 77 && direction != 2) direction = 0; // Mũi tên phải
                if (t == 80 && direction != 3) direction = 1; // Mũi tên xuống
            }
        }

        // Xóa con rắn cũ
        for (int i = 0; i < r.length; i++) {
            GoToXY(r.body[i].x, r.body[i].y);
            cout << " "; // Xóa phần thân cũ của con rắn
        }

        // Di chuyển con rắn và kiểm tra ăn mồi
        r.DiChuyen(direction, x_food, y_food, r, 1, level, score);

        // Vẽ lại con rắn ở vị trí mới
        r.Ve();

        // Hiển thị điểm hiện tại
        GoToXY(102, 1);
        cout << "Score: " << score;

        // Hiển thị điểm cao nhất
        DisplayHighScore(102, 2);

        // Tốc độ di chuyển của con rắn
        Sleep(r.speed);
    }
}

void FreeMode(int& level, int& score, vector<HighScoreEntry>& highscore) {
    Countdown();
    HideCursor();
    srand(time(0)); // Khởi tạo seed cho hàm rand
    SNAKE r;
    int direction = 0, x_food = 0, y_food = 0;
    char t;

    // Tạo tốc độ rắn theo từng mức độ
    if (level == 1) r.speed = 200;
    else if (level == 2) r.speed = 100;
    else if (level == 3) r.speed = 50;

    // Tạo mồi ban đầu
    GenerateFood(x_food, y_food, r);

    DrawBoundary(2);

    while (1) {
        if (_kbhit()) {
            t = _getch();  // Nhận phím từ bàn phím
            if (t == -32) {
                t = _getch();
                if (t == 75 && direction != 0) direction = 2; // Mũi tên trái
                if (t == 72 && direction != 1) direction = 3; // Mũi tên lên
                if (t == 77 && direction != 2) direction = 0; // Mũi tên phải
                if (t == 80 && direction != 3) direction = 1; // Mũi tên xuống
            }
        }

        // Xóa con rắn cũ
        for (int i = 0; i < r.length; i++) {
            GoToXY(r.body[i].x, r.body[i].y);
            cout << " "; // Xóa phần thân cũ của con rắn
        }

        // Di chuyển con rắn và kiểm tra ăn mồi
        r.DiChuyen(direction, x_food, y_food, r, 2, level, score);

        // Vẽ lại con rắn ở vị trí mới
        r.Ve();

        // Hiển thị điểm hiện tại
        GoToXY(102, 1);
        cout << "Score: " << score;

        // Hiển thị điểm cao nhất
        DisplayHighScore(102, 2);

        // Tốc độ di chuyển của con rắn
        Sleep(r.speed);
    }
}


void Blink(SNAKE& r) {
    bool visible = true;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < r.length; j++) {
            GoToXY(r.body[j].x, r.body[j].y);
            cout << (visible ? " " : "O");
        }
        visible = !visible;
        Sleep(500);
    }
}

void GenerateFood(int& x_food, int& y_food, SNAKE& r) {
    do {
        // Tạo tọa độ ngẫu nhiên cho mồi
        x_food = rand() % 99 + 1;
        y_food = rand() % 24 + 1;
    } while (CheckFoodCollision(r, x_food, y_food));  // Kiểm tra xem mồi có trùng với vị trí con rắn không
    GoToXY(x_food, y_food);
    cout << "o";  // Hiển thị mồi
}

bool CheckFoodCollision(SNAKE& r, int x_food, int y_food) {
    // Kiểm tra mồi có trùng với các điểm trên thân rắn hay không
    for (int i = 0; i < r.length; i++) {
        if (r.body[i].x == x_food && r.body[i].y == y_food) {
            return true;
        }
    }
    return false;
}

void HideCursor() {
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void DrawBoundary(int gameMode) {
    if (gameMode == 1) {
        for (int i = 0; i <= 100; i++) {
            GoToXY(i, 0);
            cout << "#";
            GoToXY(i, 25);
            cout << "#";
        }
        for (int i = 0; i <= 25; i++) {
            GoToXY(0, i);
            cout << "#";
            GoToXY(100, i);
            cout << "#";
        }
    }
    else {
        for (int i = 0; i <= 100; i++) {
            GoToXY(i, 0);
            cout << ".";
            GoToXY(i, 25);
            cout << ".";
        }
        for (int i = 0; i <= 25; i++) {
            GoToXY(0, i);
            cout << ".";
            GoToXY(100, i);
            cout << ".";
        }
    }
}

void GoToXY(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}


// Hàm đọc danh sách top 10 điểm số
void ReadHighScores(vector<HighScoreEntry>& highScores) {
    ifstream infile("highscore.txt");
    if (infile) {
        HighScoreEntry entry;
        while (infile >> entry.name >> entry.score) {
            highScores.push_back(entry);
        }
    }
    infile.close();
}

// Hàm ghi danh sách top 10 điểm số
void WriteHighScores(vector<HighScoreEntry>& highScores) {
    ofstream outfile("highscore.txt");
    for (const auto& entry : highScores) {
        outfile << entry.name << " " << entry.score << endl;
    }
    outfile.close();
}

// Hàm thêm điểm mới vào bảng xếp hạng
void AddHighScore(int score) {
    vector<HighScoreEntry> highScores;
    ReadHighScores(highScores);

    // Hiển thị thông báo nếu điểm mới lọt top 10
    if (highScores.size() < 10 || score > highScores.back().score) {
        system("cls");
        cout << "Chuc mung! Ban da dat diem cao voi so diem: " << score << "!" << endl;
        cout << "Nhap ten cua ban (toi da 10 ky tu): ";
        string name;
        cin >> name;
        if (name.size() > 10) {
            name = name.substr(0, 10); // Giới hạn tên tối đa 10 ký tự
        }

        // Thêm điểm mới vào danh sách
        highScores.push_back({ score, name });

        // Sắp xếp danh sách theo điểm số giảm dần
        sort(highScores.begin(), highScores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
            return a.score > b.score;
            });

        // Giữ lại top 10
        if (highScores.size() > 10) {
            highScores.pop_back();
        }

        // Ghi lại danh sách mới vào file
        WriteHighScores(highScores);
    }
}

void DisplayHighScore(int x, int y) {
    // Đọc danh sách điểm cao từ file
    vector<HighScoreEntry> highScores;
    ReadHighScores(highScores);

    // Lấy điểm cao nhất và tên tương ứng
    int highscore = (highScores.empty()) ? 0 : highScores.front().score;
    string highscoreName = (highScores.empty()) ? "None" : highScores.front().name;

    // Hiển thị điểm cao nhất
    GoToXY(x, y);
    cout << "Highscore: " << endl;
    GoToXY(102, 3);
    cout << highscore << " (" << highscoreName << ")";
}


void PlayFoodSound() {
    PlaySound(TEXT("eatFood.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void PlayGameOverSound() {
    PlaySound(TEXT("endGame.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void Countdown() {    // Hàm hiển thị đếm ngược
    for (int i = 3; i > 0; i--) {
        system("cls");
        GoToXY(50, 12);  // Đặt vị trí hiển thị ở giữa màn hình
        cout << "Starting in: " << i;
        Sleep(1000);  // Tạm dừng 1 giây
    }
    system("cls");
}
