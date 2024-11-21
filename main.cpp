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
class CONRAN; 

void GoToXY(int column, int line);
void TaoMoi(int& x_moi, int& y_moi, CONRAN& r);
void XoaConTro();
void VeTuong(int gameMode);
void NhapNhay(CONRAN& r);
void MenuBatDauChoi();
void CheDoCoDien(int& level, int& score, vector<HighScoreEntry>& highscore);
void CheDoTuDo(int& level, int& score, vector<HighScoreEntry>& highscore);
void ChonCheDoChoi();
void ChonMucDoChoi(int& level);
bool CheckRanDeMoi(CONRAN& r, int x_moi, int y_moi);
void ReadHighScore(vector<HighScoreEntry>& highScores);
void WriteHighScore(vector<HighScoreEntry>& highScores);
void ThemDiemCao(int score);
void HienThiDiemCao(int x, int y);
void PlayEatSound();
void PlayGameOverSound();

struct Point {
    int x, y;
};

class CONRAN {
public:
    struct Point A[100];
    int DoDai;
    int TocDoRan;

    CONRAN() {
        DoDai = 3;
        TocDoRan = 0;
        A[0].x = 8; A[0].y = 10;
        A[1].x = 7; A[1].y = 10;
        A[2].x = 6; A[2].y = 10;
    }

    void Ve() {
        // Vẽ con rắn ở các vị trí mới
        GoToXY(A[0].x, A[0].y);
        cout << "O";
        for (int i = 1; i < DoDai; i++) {
            GoToXY(A[i].x, A[i].y);
            if (A[0].x == A[i].x && A[0].y == A[i].y) cout << "O";
            else cout << "-";  // In ký tự "-" để vẽ con rắn
        }
    }

    void DiChuyen(int direction, int& x_moi, int& y_moi, CONRAN& r, int gameMode, int level, int& score) {
        // Di chuyển con rắn và kiểm tra ăn mồi
        for (int i = DoDai - 1; i > 0; i--) {
            A[i] = A[i - 1];
        }

        // Cập nhật vị trí đầu con rắn theo hướng
        if (direction == 0) A[0].x = A[0].x + 1; // Phải
        if (direction == 1) A[0].y = A[0].y + 1; // Xuống
        if (direction == 2) A[0].x = A[0].x - 1; // Trái
        if (direction == 3) A[0].y = A[0].y - 1; // Lên

        if (gameMode == 2) {
            if (A[0].x > 100) A[0].x = 0;
            else if (A[0].x < 0) A[0].x = 100;
            if (A[0].y > 25) A[0].y = 0;
            else if (A[0].y < 0) A[0].y = 25;
            VeTuong(2);
        }

        // Kiểm tra nếu con rắn ăn mồi
        if (A[0].x == x_moi && A[0].y == y_moi) {
            PlayEatSound();
            DoDai++;  // Tăng độ dài con rắn
            TaoMoi(x_moi, y_moi, r); // Gọi hàm TaoMoi để tạo mồi mới
            TocDoRan--; //Mỗi khi tăng chiều dài tốc độ rắn tăng dần
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
            if (A[0].x == 0 || A[0].x == 100 || A[0].y == 0 || A[0].y == 25) {
                PlayGameOverSound();
                NhapNhay(r);
                system("cls");
                cout << "Game Over!" << endl;
                Sleep(2000);
                ThemDiemCao(score);  // Cập nhật điểm cao nếu cần
                MenuBatDauChoi();
            }
        }

        // Kiểm tra việc cắn trúng thân
        for (int i = DoDai - 1; i > 0; i--) {
            if (A[0].x == A[i].x && A[0].y == A[i].y) {
                PlayGameOverSound();
                NhapNhay(r);
                system("cls");
                cout << "Game Over!" << endl;
                Sleep(2000);
                ThemDiemCao(score);  // Cập nhật điểm cao nếu cần
                MenuBatDauChoi();
            }
        }
    }
};

struct HighScoreEntry {
	int score=0;
	string name="None";
};

int main() {
    MenuBatDauChoi();
    Beep(1000, 200);
    return 0;
}

void MenuBatDauChoi() {
    int choose = 0;
    cout << "WELCOME TO SNAKE GAME" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Start" << endl;
    cout << "2. Quit" << endl;
    cout << "You choose: ";
    cin >> choose;
    if (choose == 1) {
        system("cls");
        ChonCheDoChoi();
    }
    else {
        cout << "Good bye!!!" << endl;
        exit(0);
    }
}

void ChonCheDoChoi() {
    int choose = 0, level = 0, score = 0, highscore = 0;
    ThemDiemCao(highscore);  // Đọc điểm cao từ file
    cout << "PLEASE CHOOSE GAME Mode" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Classic Mode" << endl;
    cout << "2. Free Mode" << endl;
    cout << "3. Back" << endl;
    cout << "You choose: ";
    cin >> choose;
    if (choose == 1) {
        system("cls");
        ChonMucDoChoi(level);
        CheDoCoDien(level, score, highscore);
    }
    else if (choose == 2) {
        system("cls");
        ChonMucDoChoi(level);
        CheDoTuDo(level, score, highscore);
    }
    else {
        system("cls");
        MenuBatDauChoi();
    }
}

void ChonMucDoChoi(int& level) {
    cout << "PLEASE CHOOSE GAME level" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "4. Back" << endl;
    cout << "You choose: ";
    cin >> level;
    system("cls");
    if (level == 4) ChonCheDoChoi();
}

void CheDoCoDien(int& level, int& score, int& highscore) {
    XoaConTro();
    srand(time(0)); // Khởi tạo seed cho hàm rand
    CONRAN r;
    int direction = 0, x_moi = 0, y_moi = 0;
    char t;

    // Tạo tốc độ rắn theo từng mức độ
    if (level == 1) r.TocDoRan = 200;
    else if (level == 2) r.TocDoRan = 100;
    else if (level == 3) r.TocDoRan = 50;

    // Tạo mồi ban đầu
    TaoMoi(x_moi, y_moi, r);

    VeTuong(1);

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
        for (int i = 0; i < r.DoDai; i++) {
            GoToXY(r.A[i].x, r.A[i].y);
            cout << " "; // Xóa phần thân cũ của con rắn
        }

        // Di chuyển con rắn và kiểm tra ăn mồi
        r.DiChuyen(direction, x_moi, y_moi, r, 1, level, score);

        // Vẽ lại con rắn ở vị trí mới
        r.Ve();

		// Hiển thị điểm hiện tại
		GoToXY(102, 1);
        cout << "Score: " << score;

		// Hiển thị điểm cao nhất
		DisplayHighScore(102, 2);

        // Tốc độ di chuyển của con rắn
        Sleep(r.TocDoRan);
    }
}

void CheDoTuDo(int& level, int& score, int& highscore) {
    XoaConTro();
    srand(time(0)); // Khởi tạo seed cho hàm rand
    CONRAN r;
    int direction = 0, x_moi = 0, y_moi = 0;
    char t;

    // Tạo tốc độ rắn theo từng mức độ
    if (level == 1) r.TocDoRan = 200;
    else if (level == 2) r.TocDoRan = 100;
    else if (level == 3) r.TocDoRan = 50;

    // Tạo mồi ban đầu
    TaoMoi(x_moi, y_moi, r);

    VeTuong(2);

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
        for (int i = 0; i < r.DoDai; i++) {
            GoToXY(r.A[i].x, r.A[i].y);
            cout << " "; // Xóa phần thân cũ của con rắn
        }

        // Di chuyển con rắn và kiểm tra ăn mồi
        r.DiChuyen(direction, x_moi, y_moi, r, 2, level, score);

        // Vẽ lại con rắn ở vị trí mới
        r.Ve();

		// Hiển thị điểm hiện tại
		GoToXY(102, 1);
        cout << "Score: " << score;

		// Hiển thị điểm cao nhất
		DisplayHighScore(102, 2);

        // Tốc độ di chuyển của con rắn
        Sleep(r.TocDoRan);
    }
}

void NhapNhay(CONRAN& r) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < r.DoDai; j++) {
            GoToXY(r.A[j].x, r.A[j].y);
            cout << " ";
        }
        Sleep(500);
        r.Ve();
        Sleep(500);
    }
}

void TaoMoi(int& x_moi, int& y_moi, CONRAN& r) {
    do {
        // Tạo tọa độ ngẫu nhiên cho mồi
        x_moi = rand() % 99 + 1;
        y_moi = rand() % 24 + 1;
    } while (CheckRanDeMoi(r, x_moi, y_moi));  // Kiểm tra xem mồi có trùng với vị trí con rắn không
    GoToXY(x_moi, y_moi);
    cout << "o";  // Hiển thị mồi
}

bool CheckRanDeMoi(CONRAN& r, int x_moi, int y_moi) {
    // Kiểm tra mồi có trùng với các điểm trên thân rắn hay không
    for (int i = 0; i < r.DoDai; i++) {
        if (r.A[i].x == x_moi && r.A[i].y == y_moi) {
            return true;
        }
    }
    return false;
}

void XoaConTro() {
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void VeTuong(int gameMode) {
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

<<<<<<< HEAD
void DocDiemCao(int& highscore) {
    ifstream infile("highscore.txt");
    if (infile) {
        infile >> highscore;
    }
    else {
        highscore = 0;  // Default highscore if file does not exist
    }
    infile.close();
}

void VietDiemCao(int highscore) {
    ifstream infile("highscore.txt");
    int currentHighScore;

    if (infile) {
        infile >> currentHighScore;  // Đọc kỷ lục hiện tại từ file
        infile.close();

        // So sánh điểm hiện tại với kỷ lục đã lưu
        if (highscore > currentHighScore) {
            // Hiển thị thông báo chúc mừng nếu phá kỷ lục
            system("cls");
            cout << "Chuc mung! Ban da pha ky luc voi so diem: " << highscore << "!" << endl;
            Sleep(2000);  // Dừng lại 2 giây để người chơi xem thông báo

            ofstream outfile("highscore.txt"); // Ghi đè tệp nếu có kỷ lục mới
            outfile << highscore;
            outfile.close();
        }
    }
    else {
        // Nếu tệp không tồn tại, tạo mới và ghi kỷ lục
        ofstream outfile("highscore.txt");
        outfile << highscore;
        outfile.close();
    }
}
=======

// Hàm đọc danh sách top 10 điểm số
void ReadHighScore(vector<HighScoreEntry>& highScores) {
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
void WriteHighScore(vector<HighScoreEntry>& highScores) {
	ofstream outfile("highscore.txt");
	for (const auto& entry : highScores) {
		outfile << entry.name << " " << entry.score << endl;
	}
	outfile.close();
}

// Hàm thêm điểm mới vào bảng xếp hạng
void ThemDiemCao(int score) {
	vector<HighScoreEntry> highScores;
	ReadHighScore(highScores);

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
		WriteHighScore(highScores);
	}
}

void HienThiDiemCao(int x, int y) {
	// Đọc danh sách điểm cao từ file
	vector<HighScoreEntry> highScores;
	ReadHighScore(highScores);

	// Lấy điểm cao nhất và tên tương ứng
	int highscore = (highScores.empty()) ? 0 : highScores.front().score;
	string highscoreName = (highScores.empty()) ? "None" : highScores.front().name;

	// Hiển thị điểm cao nhất
	GoToXY(x, y);
	cout << "Highscore: " << highscore << " (" << highscoreName << ")";
}


>>>>>>> 5b3c2285d6507a39df51314887e1b489dbf1504d
void PlayEatSound() {
    PlaySound(TEXT("eatFood.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void PlayGameOverSound() {
    PlaySound(TEXT("endGame.wav"), NULL, SND_FILENAME | SND_ASYNC);
}