#include <iostream>
#include <fstream>
#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define DARKYELLOW 6
#define WHITE 7
#define GREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define PINK 13
#define YELLOW 14
#define LIGHTWHITE 15

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
bool enemyFlag[3];
char car[4][4] = {
	' ',
	'\xb1',
	'\xb1',
	' ',
	'\xb1',
	'\xb1',
	'\xb1',
	'\xb1',
	' ',
	'\xb1',
	'\xb1',
	' ',
	'\xb1',
	'\xb1',
	'\xb1',
	'\xb1',
};

// posisi awal mobil di tengah secara horizontal layar
int carPosX = WIN_WIDTH / 2;
// posisi awal mobil di bawah secara vertikal layar
int carPosY = 22;

// score awal
int score = 0;

void textcolor(int color)
{
	SetConsoleTextAttribute(console, color);
}

void gotoxy(int x, int y)
{
	/*
		pindahkan kursor ke koordinat x horizontal atau kolom
		dan koordinat y vertikal atau baris
	*/
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder()
{
	textcolor(PURPLE);
	// bikin border kiri
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			gotoxy(0 + j, i);
			cout << "±";
			gotoxy(WIN_WIDTH - j, i);
			cout << "±";
		}
	}
	// lebar arena 36
	// bikin border kanan
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		gotoxy(SCREEN_WIDTH, i);
		cout << "±";
	}
	textcolor(WHITE);
}

void genEnemy(int ind)
{
	/*
		bikin posisi mobil lawan di koordinat horizontal x
		minimal di x = 17, karena lebar border kiri adalah 17
		maksimal 49, karena 49 adalah posisi bagian kiri mobil
	*/
	enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind)
{
	/*
		gambar mobil lawan pada horizontal enemyX[ind]
		dan vertikal enemyY[ind]
	*/
	if (enemyFlag[ind] == true)
	{
		textcolor(RED);
		gotoxy(enemyX[ind], enemyY[ind]);
		cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 1);
		cout << " ** ";
		gotoxy(enemyX[ind], enemyY[ind] + 2);
		cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 3);
		cout << " ** ";
		textcolor(WHITE);
	}
}

void eraseEnemy(int ind)
{
	if (enemyFlag[ind] == true)
	{
		gotoxy(enemyX[ind], enemyY[ind]);
		cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 1);
		cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 2);
		cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 3);
		cout << "    ";
	}
}

void resetEnemy(int ind)
{
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}

void drawCar()
{
	textcolor(GREEN);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gotoxy(j + carPosX, i + carPosY);
			cout << car[i][j];
		}
	}
	textcolor(WHITE);
}

void eraseCar()
{
	/*
		hapus bekas pergeseran mobil
	*/
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gotoxy(j + carPosX, i + carPosY);
			cout << " ";
		}
	}
}

bool collision(int ind)
{
	/*
		kalo terjadi tabrakan return true, else false
	*/

	// klo posisi mobil lawan ke-0, berada pada baris tempat mobil
	// berada, terdapat kemungkinan tabrakan terjadi
	if (enemyY[ind] + 4 - carPosY >= 0 && enemyY[ind] + 4 - carPosY < 9)
	{
		if (enemyX[ind] + 4 - carPosX >= 0 && enemyX[ind] + 4 - carPosX < 9)
		{
			return true;
		}
	}
	return false;
}

void gameover()
{
	string text;

	ofstream HallOfFameFile("game.dat", ios_base::app);

	system("cls");
	textcolor(CYAN);
	cout << endl;
	cout << "\t\t--------------------------" << endl;
	cout << "\t\t-------- Game Over -------" << endl;
	cout << "\t\t--------------------------" << endl
		 << endl;
	cout << "\t\tYour name: ";
	cin >> text;
	HallOfFameFile << text << " " << score << endl;
	cout << "\t\tPress any key to go back to menu.";
	textcolor(WHITE);
	getch();

	HallOfFameFile.close();
}

void updateScore()
{
	gotoxy(WIN_WIDTH + 7, 5);
	textcolor(CYAN);
	cout << "Score: " << score << endl;
	textcolor(WHITE);
}

void hall_of_fame(void)
{
	string text;

	ifstream HallOfFameFile("game.dat");

	system("cls");
	textcolor(DARKYELLOW);
	cout << "Hall of Fame";
	cout << "\n----------------";
	while (getline(HallOfFameFile, text))
	{
		cout << "\n " << text;
	}
	cout << "\n\nPress any key to go back to menu";
	textcolor(WHITE);
	getch();

	HallOfFameFile.close();
}

void instructions()
{
	system("cls");
	textcolor(DARKYELLOW);
	cout << "Instructions";
	cout << "\n----------------";
	cout << "\n Avoid Cars by moving left or right and up or down. ";
	cout << "\n\n Press 'a' to move left";
	cout << "\n Press 'd' to move right";
	cout << "\n Press 'w' to move up";
	cout << "\n Press 's' to move down";
	cout << "\n Press 'escape' to exit";
	cout << "\n\nPress any key to go back to menu";
	textcolor(WHITE);
	getch();
}

void play()
{
	carPosX = -1 + WIN_WIDTH / 2;
	carPosY = 22;
	score = 0;
	enemyFlag[0] = true;
	enemyFlag[1] = false;
	enemyY[0] = enemyY[1] = 1;

	system("cls");
	drawBorder();
	updateScore();
	// bikin mobil lawan ke 0
	genEnemy(0);
	// bikin mobil lawan ke 1
	genEnemy(1);

	textcolor(LIGHTCYAN);
	gotoxy(WIN_WIDTH + 7, 2);
	cout << "Car Game";
	gotoxy(WIN_WIDTH + 6, 4);
	cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6);
	cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12);
	cout << "Control ";
	gotoxy(WIN_WIDTH + 6, 13);
	cout << "--------- ";
	gotoxy(WIN_WIDTH + 3, 14);
	cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 3, 15);
	cout << " D Key - Right";
	gotoxy(WIN_WIDTH + 3, 16);
	cout << " W Key - Up";
	gotoxy(WIN_WIDTH + 3, 17);
	cout << " S Key - Down";

	gotoxy(18, 5);
	cout << "Press any key to start";
	textcolor(WHITE);
	getch();
	gotoxy(18, 5);
	cout << "                      ";

	while (true)
	{
		if (kbhit())
		{
			char ch = getch();
			if (ch == 'a' || ch == 'A' || ch == KEY_LEFT)
			{
				// 18 adalah batas border kiri
				if (carPosX > 18)
					carPosX -= 4;
			}
			else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT)
			{
				// 54 adalah batas border kanan
				// 50 karena lebar mobil 4 jadi dikurang 4
				if (carPosX < 50)
					carPosX += 4;
			}
			else if (ch == 'w' || ch == 'W' || ch == KEY_UP)
			{
				// 0 adalah batas atas vertikal layar
				if (carPosY > 2)
					carPosY -= 4;
			}
			else if (ch == 's' || ch == 'S' || ch == KEY_DOWN)
			{
				// 22 + 4 adalah batas bawah vertikal layar
				if (carPosY < 22)
					carPosY += 4;
			}
			// kalo pencet ESC, kembali ke menu utama
			else if (ch == 27)
			{
				break;
			}
		}

		// gambar mobil
		drawCar();

		// gambar mobil lawan ke-i klo ada
		for (int i = 0; i < 2; i++)
		{
			drawEnemy(i);
		}

		// cek jika tabrakan mobil dengan mobil lawan, maka kalah
		for (int i = 0; i < 2; i++)
		{
			if (collision(i) == true)
			{
				gameover();
				return;
			}
		}

		Sleep(50);

		// hapus gambar mobil
		eraseCar();

		// hapus gambar mobil lawan
		for (int i = 0; i < 2; i++)
		{
			eraseEnemy(i);
		}

		// jika posisi mobil lawan ke-0 berada pada vertikal 10
		if (enemyY[0] == 10)
			// jika mobil lawan ke-1 tidak ada, maka buat jadi ada
			if (enemyFlag[1] == false)
				enemyFlag[1] = true;

		// jalankan mobil lawan ke-i, 1 vertikal ke bawah
		for (int i = 0; i < 2; i++)
		{
			if (enemyFlag[i] == true)
				enemyY[i] += 1;
		}

		// mobil lawan ke-i sampe batas bawah screen maka score nambah 1
		// dan mobil lawan ke-i di hapus, lalu buat baru
		for (int i = 0; i < 2; i++)
		{
			if (enemyY[i] > SCREEN_HEIGHT - 4)
			{
				resetEnemy(i);
				score++;
				updateScore();
			}
		}
	}
}

int main()
{
	setcursor(0, 0);
	srand((unsigned)time(NULL));

	do
	{
		system("cls");
		textcolor(LIGHTCYAN);
		gotoxy(10, 5);
		cout << " -------------------------- ";
		gotoxy(10, 6);
		cout << " |        Car Game        | ";
		gotoxy(10, 7);
		cout << " --------------------------";
		gotoxy(10, 9);
		cout << "1. Start Game";
		gotoxy(10, 10);
		cout << "2. Instructions";
		gotoxy(10, 11);
		cout << "3. Hall of Fame";
		gotoxy(10, 12);
		cout << "4. Quit";
		gotoxy(10, 14);
		cout << "Select option: ";
		textcolor(WHITE);
		char op = getche();

		if (op == '1')
			play();
		else if (op == '2')
			instructions();
		else if (op == '3')
			hall_of_fame();
		else if (op == '4')
			exit(0);

	} while (true);

	return 0;
}