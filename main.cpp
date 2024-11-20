#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <fstream>  // For file handling
#pragma comment(lib, "Winmm.lib") // Chỉ định liên kết thư viện Winmm.lib
using namespace std;

class CONRAN; // Forward declaration
void gotoxy(int column, int line);
void TaoMoi(int& x_moi, int& y_moi, CONRAN& r);
void XoaConTro();
void VeTuong(int Mode);
void NhapNhay(CONRAN& r);
void MenuBatDauChoi();
void CheDoCoDien(int& level, int& score, int& highscore);
void CheDoTuDo(int& level, int& score, int& highscore);
void ChonCheDoChoi();
void ChonMucDoChoi(int& level);
bool CheckRanDeMoi(CONRAN& r, int x_moi, int y_moi);
void ReadHighScore(int& highscore);
void WriteHighScore(int highscore);
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
		gotoxy(A[0].x, A[0].y);
		cout << "O";
		for (int i = 1; i < DoDai; i++) {
			gotoxy(A[i].x, A[i].y);
			if (A[0].x == A[i].x && A[0].y == A[i].y) cout << "O";
			else cout << "-";  // In ký tự "-" để vẽ con rắn
		}
	}

	void DiChuyen(int Huong, int& x_moi, int& y_moi, CONRAN& r, int Mode, int Level, int& score) {
		// Di chuyển con rắn và kiểm tra ăn mồi
		for (int i = DoDai - 1; i > 0; i--) {
			A[i] = A[i - 1];
		}

		// Cập nhật vị trí đầu con rắn theo hướng
		if (Huong == 0) A[0].x = A[0].x + 1; // Phải
		if (Huong == 1) A[0].y = A[0].y + 1; // Xuống
		if (Huong == 2) A[0].x = A[0].x - 1; // Trái
		if (Huong == 3) A[0].y = A[0].y - 1; // Lên

		if (Mode == 2) {
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
			if (Mode == 1) {
				if (Level == 1) score += 2;
				else if (Level == 2) score += 4;
				else if (Level == 3) score += 6;
			}
			else if (Mode == 2) {
				if (Level == 1) score += 1;
				else if (Level == 2) score += 2;
				else if (Level == 3) score += 3;
			}
		}

		// Kiểm tra việc đụng tường
		if (Mode == 1) {
			if (A[0].x == 0 || A[0].x == 100 || A[0].y == 0 || A[0].y == 25) {
				PlayGameOverSound();
				NhapNhay(r);
				system("cls");
				cout << "Game Over!" << endl;
				Sleep(2000);
				WriteHighScore(score);  // Cập nhật điểm cao nếu cần
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
				WriteHighScore(score);  // Cập nhật điểm cao nếu cần
				MenuBatDauChoi();
			}
		}
	}
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
	ReadHighScore(highscore);  // Đọc điểm cao từ file
	cout << "PLEASE CHOOSE GAME MODE" << endl;
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
	cout << "PLEASE CHOOSE GAME LEVEL" << endl;
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

void CheDoCoDien(int& Level, int& score, int& highscore) {
	XoaConTro();
	srand(time(0)); // Khởi tạo seed cho hàm rand
	CONRAN r;
	int Huong = 0, x_moi = 0, y_moi = 0;
	char t;

	// Tạo tốc độ rắn theo từng mức độ
	if (Level == 1) r.TocDoRan = 200;
	else if (Level == 2) r.TocDoRan = 100;
	else if (Level == 3) r.TocDoRan = 50;

	// Tạo mồi ban đầu
	TaoMoi(x_moi, y_moi, r);

	VeTuong(1);

	while (1) {
		if (_kbhit()) {
			t = _getch();  // Nhận phím từ bàn phím
			if (t == -32) {
				t = _getch();
				if (t == 75 && Huong != 0) Huong = 2; // Mũi tên trái
				if (t == 72 && Huong != 1) Huong = 3; // Mũi tên lên
				if (t == 77 && Huong != 2) Huong = 0; // Mũi tên phải
				if (t == 80 && Huong != 3) Huong = 1; // Mũi tên xuống
			}
		}

		// Xóa con rắn cũ
		for (int i = 0; i < r.DoDai; i++) {
			gotoxy(r.A[i].x, r.A[i].y);
			cout << " "; // Xóa phần thân cũ của con rắn
		}

		// Di chuyển con rắn và kiểm tra ăn mồi
		r.DiChuyen(Huong, x_moi, y_moi, r, 1, Level, score);

		// Vẽ lại con rắn ở vị trí mới
		r.Ve();

		// Hiển thị điểm ở ngoài khu vực sân chơi
		gotoxy(102, 1);
		cout << "Score: " << score;
		gotoxy(102, 2);
		cout << "Highscore: " << highscore;

		// Tốc độ di chuyển của con rắn
		Sleep(r.TocDoRan);
	}
}

void CheDoTuDo(int& Level, int& score, int& highscore) {
	XoaConTro();
	srand(time(0)); // Khởi tạo seed cho hàm rand
	CONRAN r;
	int Huong = 0, x_moi = 0, y_moi = 0;
	char t;

	// Tạo tốc độ rắn theo từng mức độ
	if (Level == 1) r.TocDoRan = 200;
	else if (Level == 2) r.TocDoRan = 100;
	else if (Level == 3) r.TocDoRan = 50;

	// Tạo mồi ban đầu
	TaoMoi(x_moi, y_moi, r);

	VeTuong(2);

	while (1) {
		if (_kbhit()) {
			t = _getch();  // Nhận phím từ bàn phím
			if (t == -32) {
				t = _getch();
				if (t == 75 && Huong != 0) Huong = 2; // Mũi tên trái
				if (t == 72 && Huong != 1) Huong = 3; // Mũi tên lên
				if (t == 77 && Huong != 2) Huong = 0; // Mũi tên phải
				if (t == 80 && Huong != 3) Huong = 1; // Mũi tên xuống
			}
		}

		// Xóa con rắn cũ
		for (int i = 0; i < r.DoDai; i++) {
			gotoxy(r.A[i].x, r.A[i].y);
			cout << " "; // Xóa phần thân cũ của con rắn
		}

		// Di chuyển con rắn và kiểm tra ăn mồi
		r.DiChuyen(Huong, x_moi, y_moi, r, 2, Level, score);

		// Vẽ lại con rắn ở vị trí mới
		r.Ve();

		// Hiển thị điểm ở ngoài khu vực sân chơi
		gotoxy(102, 1);
		cout << "Score: " << score;
		gotoxy(102, 2);
		cout << "Highscore: " << highscore;

		// Tốc độ di chuyển của con rắn
		Sleep(r.TocDoRan);
	}
}

void NhapNhay(CONRAN& r) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < r.DoDai; j++) {
			gotoxy(r.A[j].x, r.A[j].y);
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
	gotoxy(x_moi, y_moi);
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
	CONSOLE_CURSOR_INFO Info{};
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void VeTuong(int Mode) {
	if (Mode == 1) {
		for (int i = 0; i <= 100; i++) {
			gotoxy(i, 0);
			cout << "#";
			gotoxy(i, 25);
			cout << "#";
		}
		for (int i = 0; i <= 25; i++) {
			gotoxy(0, i);
			cout << "#";
			gotoxy(100, i);
			cout << "#";
		}
	}
	else {
		for (int i = 0; i <= 100; i++) {
			gotoxy(i, 0);
			cout << ".";
			gotoxy(i, 25);
			cout << ".";
		}
		for (int i = 0; i <= 25; i++) {
			gotoxy(0, i);
			cout << ".";
			gotoxy(100, i);
			cout << ".";
		}
	}
}

void gotoxy(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

void ReadHighScore(int& highscore) {
	ifstream infile("highscore.txt");
	if (infile) {
		infile >> highscore;
	}
	else {
		highscore = 0;  // Default highscore if file does not exist
	}
	infile.close();
}

void WriteHighScore(int highscore) {
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

void PlayEatSound() {
	PlaySound(TEXT("eatFood.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void PlayGameOverSound() {
	PlaySound(TEXT("endGame.wav"), NULL, SND_FILENAME | SND_ASYNC);
}