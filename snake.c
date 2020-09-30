#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<stdbool.h>
#include<conio.h>

#define lineX 25
#define lineY 25

typedef  struct SnackBody {
	unsigned short x;
	unsigned short y;
	struct SnackBody * ptrNext;
}snackBody;

typedef struct Control {
	bool touch;
	bool pause;
	bool gameOver;
	bool inputCorrect;
	float speed;

}Control;

typedef struct Food {
	short x;
	short y;
	int n;
}Food;

static snackBody * head;
static snackBody * node;
static snackBody * end;
static bool foodexist = false;
static Control control;
static Food food;

void initgame();//初始化
void drawEdge();//初始化边框,并画出边框
void initSnack();//初始化蛇身
void drawSnack();//画出蛇身
void touch();//碰撞检测
void drawFood();//随机画出食物
void changedirection(int);//改变蛇的前进方向,以及暂停，改变速度
void gotoxy(unsigned short x, unsigned short y);//光标去到指定位置
void runGame();//判断是否有敲击键盘
void drawMassage();//展示信息
void closegame();//释放内存

HANDLE handle;

int main(void)
{

	initgame();//初始化

	while (!control.gameOver) {
		control.touch = false;
		drawEdge();//画出边框
		initSnack();//初始化蛇身
		drawSnack();//画出蛇身
		drawFood();//随机画出食物
		drawMassage();//展示信息
		runGame();
		closegame();
		system("cls");
	}
	CloseHandle(handle);//结束句柄

	return 0;
}
//初始化
void initgame() {
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // 光标信息

	handle = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取控制台输出句柄
	SetConsoleCursorInfo(handle, &cursorInfo);  // 设置光标隐藏
	memset(&control, 0, sizeof(Control));  // 全部置0

	control.speed = 0.1f;//速度
	control.pause = false;//暂停
	control.touch = false;//碰撞
	control.gameOver = false;//结束
	control.inputCorrect = false;//判断输入
	food.n = 0;//吃了几顿
}
//光标去到指定位置
void gotoxy(unsigned short x, unsigned short y) {
	static COORD cd;
	cd.X = 2 * x - 1;
	cd.Y = y;
	SetConsoleCursorPosition(handle, cd);
}
//画出边框
void drawEdge() {
	int i;

	SetConsoleTextAttribute(handle, 0xff);
	for (i = 0; i<lineX; i++) {
		gotoxy(i + 1, 1);
		printf("%2s", "");
		gotoxy(i + 1, lineY);
		printf("%2s", "");
	}
	for (i = 1; i<lineY; i++) {
		gotoxy(1, i);
		printf("%2s", "");
		gotoxy(lineX, i);
		printf("%2s", "");
	}
}
//初始化蛇身
void initSnack() {
	head = (snackBody *)malloc(sizeof(snackBody));
	head->ptrNext = (snackBody *)malloc(sizeof(snackBody));
	end = (snackBody *)malloc(sizeof(snackBody));
	head->ptrNext->ptrNext = end;
	end->ptrNext = NULL;

	head->x = (unsigned short)(lineX / 2);
	head->y = (unsigned short)(lineY / 2);

	head->ptrNext->x = (unsigned short)(lineX / 2);
	head->ptrNext->y = (unsigned short)(lineY / 2) + 1;

	head->ptrNext->ptrNext->x = (unsigned short)(lineX / 2);
	head->ptrNext->ptrNext->y = (unsigned short)(lineY / 2) + 2;
}
//释放内存
void closegame() {
	snackBody * q;
	snackBody * p = head;
	while (p) {
		q = p;
		p = q->ptrNext;
		free(q);
	}
	food.n = 0;
	foodexist = false;
}
//hitbord，以及蛇的停留时间,改变蛇的方向
void runGame() {
	while (!control.touch) {
		drawSnack();

		Sleep(control.speed * 1000);
		if (_kbhit()) {
			changedirection(_getch());
			drawMassage();
		}
		else
			changedirection(0);
		if (control.gameOver)
			break;
		touch();
		drawFood();
	}
}
void drawSnack() {
	snackBody * nodebody = head;
	SetConsoleTextAttribute(handle, 0xff);
	//BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY
	gotoxy(nodebody->x, nodebody->y);//头
	printf("%2s", "");
	do {//身
		nodebody = nodebody->ptrNext;
		gotoxy(nodebody->x, nodebody->y);
		printf("%2s", "");
	} while ((nodebody->ptrNext) != NULL);
	SetConsoleTextAttribute(handle, 0x00);
	gotoxy(nodebody->x, nodebody->y);
	printf("%2s", "");
}
//改变蛇的前进方向
void changedirection(int key) {
	switch (key)//筛选输入key值
	{
	case 'a':case 's':case 'd':case 'w':
	case 'A':case 'S':case 'D':case 'W':
	case 'p':
	case 'v':
	case 'o':
		break;
	default:
		key = 0;
	}

	if (key == 'p') {//暂停
		control.pause = !control.pause;
		while (control.pause) {
			SetConsoleTextAttribute(handle, 0xf);
			gotoxy(lineX / 2, lineY / 2);
			printf("游戏暂停");
			Sleep(control.speed * 1000);
			if (_kbhit()) {
				int k = _getch();
				if (k == 'p') {
					control.pause = !control.pause;
				}
			}
		}
		SetConsoleTextAttribute(handle, 0x00);
		gotoxy(lineX / 2, lineY / 2);
		printf("        ");
		key = 0;
	}
	if (key == 'v') {//改变速度
		float speed = 2.0;
		while ((speed<0 || speed>1)) {
			SetConsoleTextAttribute(handle, 0xf);
			gotoxy(lineX / 4 + lineX, lineY / 4 + 15);
			printf("请输入小于1的数值：");
			if (1 == scanf_s("%f", &speed)) {
				control.speed = speed;
			}
			else {
				char ch;
				while ((ch = getchar()) != '\n');
			}
			SetConsoleTextAttribute(handle, 0x00);
			gotoxy(lineX / 4 + lineX, lineY / 4 + 15);
			printf("000000000000000000000000000000000000000000");
		}
		key = 0;
	}
	if (key == 'o') {
		control.gameOver = true;
		key = 0;
	}
	int x1, x2, y1, y2, x3, y3;
	bool full = true;
	x1 = y1 = x2 = y2 = x3 = y3 = 0;
	x1 = head->x;
	y1 = head->y;
	node = head->ptrNext;
	x3 = (head->x) - (node->x);
	y3 = (head->y) - (node->y);
	while (node != NULL) {
		if (full) {
			x2 = node->x;
			y2 = node->y;
			node->x = x1;
			node->y = y1;
		}
		else
		{
			x1 = node->x;
			y1 = node->y;
			node->x = x2;
			node->y = y2;
		}
		full = !full;
		node = node->ptrNext;
	}
	if (key == 'w' || key == 's' || key == 'W' || key == 'S') {
		if (y3 != 0)
			key = 0;
	}
	if (key == 'a' || key == 'd' || key == 'A' || key == 'D') {
		if (x3 != 0)
			key = 0;
	}
	if (key == 0) {//直线跑
		if (y3 == -1)//上(0,-1)
			head->y--;
		else if (y3 == 1)//下
			head->y++;
		else if (x3 == -1)//左
			head->x--;
		else if (x3 == 1)//右
			head->x++;
		return ;
	}
	switch (key) {
		//case 72://上(0,-1)
	case 'w':
	case 'W':
		head->x = head->x;
		head->y = head->y - 1;
		break;
		//case 75: //左(-1,0)
	case 'a':
	case 'A':
		head->x = head->x - 1;
		head->y = head->y;
		break;
		//case 77://右(1,0)
	case 'd':
	case 'D':
		head->x = head->x + 1;
		head->y = head->y;
		break;
		//case 80://下(0,1)
	case 's':
	case 'S':
		head->x = head->x;
		head->y = head->y + 1;
		break;
	}
}
//随机画出食物
void drawFood() {
	if (foodexist)
		return;
	else
	{
		bool foodOnSnackBody = false;
		do {
			node = head;
			food.x = (rand() % (lineX - 2)) + 2;
			food.y = (rand() % (lineY - 2)) + 2;
			foodOnSnackBody = false;

			while (node != NULL) {
				if (node->x == food.x&&node->y == food.y) {
					foodOnSnackBody = true;
					break;
				}
				node = node->ptrNext;
			}
		} while (foodOnSnackBody);
		foodexist = true;
		SetConsoleTextAttribute(handle, BACKGROUND_RED);
		gotoxy(food.x, food.y);
		printf("%2s", "");
	}
}
//碰撞检测
void touch() {
	//吃到食物
	if (head->x == food.x&&head->y == food.y) {
		foodexist = false;
		food.n++;
		//插入蛇身；
		end->ptrNext = (snackBody *)malloc(sizeof(snackBody));
		end->ptrNext->x = end->x;
		end->ptrNext->y = end->y;
		end = end->ptrNext;
		end->ptrNext = NULL;
	}
	if ((food.n + 3) >= ((lineX - 2)*(lineY - 2)))
		control.touch = true;

	if (head->x == 1 || head->x == lineX)
		control.touch = true;
	if (head->y == 1 || head->y == lineY)
		control.touch = true;
}
void drawMassage() {
	if (food.n>0) {
		SetConsoleTextAttribute(handle, BACKGROUND_RED);
		gotoxy(food.x, food.y);
		printf("%2s", "");
	}
	SetConsoleTextAttribute(handle, 0xf);
	gotoxy(lineX / 4 + lineX, lineY / 4 - 2);
	printf("贪吃蛇  ");
	gotoxy(lineX / 4 + lineX, lineY / 4);
	printf("得到食物：%d", food.n);
	gotoxy(lineX / 4 + lineX, lineY / 4 + 1);
	printf("每个方块停留时间：%.2f", control.speed);
	gotoxy(lineX / 4 + lineX, lineY / 4 + 2);
	printf("暂停/开始游戏: p");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 3);
	printf("改变蛇的速度: v");
	gotoxy(lineX / 4 + lineX, lineY - 1);
	printf("结束游戏: o");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 7);
	printf("方向键：");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 8);
	printf("向上：w&W");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 9);
	printf("向下：s&S");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 10);
	printf("向左：a&A");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 11);
	printf("向右：d&D");
	gotoxy(lineX / 4 + lineX, lineY / 4 + 13);
	printf("请注意大小写输入:)");
}
