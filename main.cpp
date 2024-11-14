#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
using namespace std;

class CONRAN; // Forward declaration
void gotoxy(int column, int line);
void TaoMoi(int& x_moi, int& y_moi, CONRAN& r);
void xoacontro();
void VeTuong();
bool check_ran_de_moi(CONRAN& r, int x_moi, int y_moi);  // Hàm kiểm tra mồi có trùng với rắn hay không

struct Point {
    int x, y;
};

class CONRAN {
public:
    struct Point A[100];
    int DoDai;

    CONRAN() {
        DoDai = 3;
        A[0].x = 8; A[0].y = 10;
        A[1].x = 9; A[1].y = 10;
        A[2].x = 10; A[2].y = 10;
    }

    void Ve() {
        // Vẽ con rắn ở các vị trí mới
        gotoxy(A[0].x, A[0].y);
        cout << "O";
        for (int i = 1; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            cout << "-";  // In ký tự "-" để vẽ con rắn
        }
    }

    void DiChuyen(int Huong, int& x_moi, int& y_moi, CONRAN& r) {
        // Di chuyển con rắn và kiểm tra ăn mồi
        for (int i = DoDai - 1; i > 0; i--) {
            A[i] = A[i - 1];
        }

        // Cập nhật vị trí đầu con rắn theo hướng
        if (Huong == 0) A[0].x = A[0].x + 1; // Phải
        if (Huong == 1) A[0].y = A[0].y + 1; // Xuống
        if (Huong == 2) A[0].x = A[0].x - 1; // Trái
        if (Huong == 3) A[0].y = A[0].y - 1; // Lên

        // Kiểm tra nếu con rắn ăn mồi
        if (A[0].x == x_moi && A[0].y == y_moi) {
            DoDai++;  // Tăng độ dài con rắn
            TaoMoi(x_moi, y_moi, r);  // Gọi hàm TaoMoi để tạo mồi mới
        }

        //Kiểm tra việc đụng tường
        if (A[0].x == 0 || A[0].x == 100 || A[0].y == 0 || A[0].y == 25) {
            system("cls");
            cout << "Game Over!";
            Sleep(2000);
            exit(0);
        }
                
    }
};

int main() {
    xoacontro();
    srand(time(0)); // Khởi tạo seed cho hàm rand
    CONRAN r;
    int Huong = 0, x_moi = 0, y_moi = 0;
    char t;

    // Tạo mồi ban đầu
    TaoMoi(x_moi, y_moi, r);

    VeTuong();

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
        r.DiChuyen(Huong, x_moi, y_moi, r);

        // Vẽ lại con rắn ở vị trí mới
        r.Ve();


        Sleep(100);  // Tốc độ di chuyển của con rắn
    }

    return 0;
}

void TaoMoi(int& x_moi, int& y_moi, CONRAN& r) {
    do {
        // Tạo tọa độ ngẫu nhiên cho mồi
        x_moi = rand() % 99 + 1;
        y_moi = rand() % 24 + 1;
    } while (check_ran_de_moi(r, x_moi, y_moi));  // Kiểm tra xem mồi có trùng với vị trí con rắn không
    gotoxy(x_moi, y_moi);
    cout << "o";  // Hiển thị mồi
}

bool check_ran_de_moi(CONRAN& r, int x_moi, int y_moi) {
    // Kiểm tra mồi có trùng với các điểm trên thân rắn hay không
    for (int i = 0; i < r.DoDai; i++) {
        if (r.A[i].x == x_moi && r.A[i].y == y_moi) {
            return true;
        }
    }
    return false;
}

void xoacontro() {
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void VeTuong() {
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

void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}
