#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>
#include <stdlib.h>
#include <time.h>

void gotoxy(int x, int y);
void tableRender(int turnG);
void drawAllDice(int* dice, int* keep);
void drawPoints(int* points, int* state, int playerTurn);

int GetPoint(int y, int* endDice);
int diceCursor(int mvCNT, int playerTurn);
int tableCursor(int mvCNT, int gameTurn);

char* getName(int type, int  P)
{
	static char id[2][15] = { "Player1", "Player2" };
	/*
	if (type == 0)
	{
		printf("Please Type Player%d Name: ", P + 1);
		Sleep(30);
		scanf_s("%s", &id[P], sizeof(id));
		fflush(stdin);
		return 0;
	}
	else
	{
		return id[P];
	}
	*/

	return id[P];
}

void checkDice(int* dice, int* keep, int* endDice)
{
	for (int d = 0; d < 5; d++)
	{

		if (*(dice + d) != 0)
		{
			*(endDice + d) = *(dice + d);
		}
		else
		{
			*(endDice + d) = *(keep + d);
		}
	}
}

int main(void)
{
	//statup
	system("mode con Cols=200 Lines=50");
	system("cls");
	system("title Yacht Basic");
	srand(time(NULL));

	int points[12][2] = { 0, }; //point type, player
	int total[2] = { 0, };
	int state[12][2] = { 0, }; // save or not
	int dice[5];
	int keep[5];
	int endDice[5];
	int playerTurn;
	int diceKey, tableKey;
	int tableX[2] = { 21, 36 };
	int tableY[12] = { 4, 6, 8, 10, 12, 14, 17, 20, 22, 24, 26, 28 };
	int reroll;
	int mv;


	printf("Welcome to the Yacht\n");

	//getName(0, 0); getName(0, 1);
	system("cls");
	printf("%s vs %s", getName(1, 0), getName(1, 1));
	for (int d = 0; d < 3; d++)
	{
		Sleep(400);
		printf(".");
	}

	//Gmae start
	for (int gameTurn = 0; gameTurn < 24; gameTurn++)
	{
		playerTurn = gameTurn & 1; // 이진 곱 연산자 // equal to gameTurn % 2;
		tableRender(gameTurn);
		//roll dice first time of turn
		for (int roll = 0; roll < 5; roll++)
		{
			dice[roll] = 1 + rand() % 6;
			keep[roll] = 0;
		}
		drawAllDice(&dice[0], &keep[0]);
		checkDice(&dice[0], &keep[0], &endDice[0]);
		Sleep(100);
		mv = 0;
		reroll = 2;

		system("cls");

		while (1)
		{
			//draws
			drawAllDice(&dice[0], &keep[0]);
			tableRender(gameTurn);
			diceCursor(mv, playerTurn);
			tableCursor(mv, playerTurn);
			for (int n = 0; n < 12; n++)
			{
				if (state[n][0] == 1)
				{
					gotoxy(tableX[0], tableY[n]);
					printf("%2d", points[n][0]);
				}
				if (state[n][1] == 1)
				{
					gotoxy(tableX[1], tableY[n]);
					printf("%2d", points[n][1]);
				}
			}
			gotoxy(tableX[0], 32); printf("%3d", total[0]);
			gotoxy(tableX[1], 32); printf("%3d", total[1]);
			gotoxy(65, 2); printf("reroll chance: %d", reroll);

			mv = 1;
			//frames
			Sleep(160);
			//controls
			if (GetAsyncKeyState(playerTurn ? VK_RSHIFT : 0x46))//0x46 == F key //if( (GetAsyncKeyState(0x46) && playerTurn == 0) || (GetAsyncKeyState(VK_RSHIFT) && playerTurn == 1) )
			{
				diceKey = diceCursor(-1, playerTurn);

				//swap
				if (dice[diceKey] == 0)
				{
					dice[diceKey] = keep[diceKey];
					keep[diceKey] = 0;
				}
				else
				{
					keep[diceKey] = dice[diceKey];
					dice[diceKey] = 0;
				}
			}
			else if (GetAsyncKeyState(playerTurn ? VK_RCONTROL : 0x45)) //E key //if( (GetAsyncKeyState(0x45) && playerTurn == 0) || (GetAsyncKeyState(VK_RCONTROL) && playerTurn == 1) )
			{
				tableKey = tableCursor(-1, gameTurn);

				if (state[tableKey][playerTurn] == 0)
				{
					goto getPoint;
				}
				else
				{
					continue;
				}
			}
			else if (GetAsyncKeyState(playerTurn ? VK_OEM_2 : 0x51) && reroll > 0) // '/'button and Q key //if( (GetAsyncKeyState(0x51) && playerTurn == 0) || (GetAsyncKeyState(VK_OEM_2) && playerTurn == 1) )
			{
				//reroll
				for (int re = 0; re < 5; re++)
				{
					dice[re] = dice[re] ? 1 + rand() % 6 : 0;
				}
				for (int re = 0; re < 5; re++)
				{
					dice[re] = dice[re] ? 1 + rand() % 6 : 0;
				}
				reroll--;
			}
			else if ((GetAsyncKeyState(VK_ESCAPE)) && (GetAsyncKeyState(VK_SPACE)))
			{
				system("cls");
				printf("Exit game.");
				for (size_t i = 0; i < 2; i++)
				{
					Sleep(500);
					printf(".");
				}
				return 0;
			}
		}
	getPoint:
		checkDice(&dice[0], &keep[0], &endDice[0]);
		state[tableKey][playerTurn] = 1;
		points[tableKey][playerTurn] = GetPoint(tableKey, &endDice[0]);
		total[playerTurn] = total[playerTurn] + points[tableKey][playerTurn];
	}

	tableRender(24);
	for (int n = 0; n < 12; n++)
	{
		gotoxy(tableX[0], tableY[n]);
		printf("%2d", points[n][0]);
		gotoxy(tableX[1], tableY[n]);
		printf("%2d", points[n][1]);
	}
	gotoxy(tableX[0], 32); printf("%3d", total[0]);
	gotoxy(tableX[1], 32); printf("%3d", total[1]);

	gotoxy(0, 34); system("pause");
	system("cls");
	if (total[0] == total[1])
	{
		printf("Tied...\n");
	}
	else
	{
		printf("Winner is %s\n", total[0] > total[1] ? getName(1, 0) : getName(1, 1));
		printf("score: %d vs %d\n", total[0], total[1]);
	}
	system("pause");
	return 0;
}

void tableRender(int turnG)
{
	gotoxy(0, 0); printf("|	Turn: %d : 12 | %s VS %s\n", turnG / 2 + 1, getName(1, 0), getName(1, 1));
	printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
	printf("!Categories  ||                          !\n");
	printf("|----------------------------------------|\n");
	printf("|Aces        ||            \\             |\n");//1
	printf("|----------------------------------------|\n");
	printf("|Deuces      ||            \\             |\n");//2
	printf("|----------------------------------------|\n");
	printf("|Threes      ||            \\             |\n");//3
	printf("|----------------------------------------|\n");
	printf("|Fours       ||            \\             |\n");//4
	printf("|----------------------------------------|\n");
	printf("|Fives       ||            \\             |\n");//5
	printf("|----------------------------------------|\n");
	printf("!Sixes       ||            \\             |\n");//6
	printf("|========================================|\n");
	printf("|----------------------------------------|\n");
	printf("|Choice      ||            \\             |\n");//7
	printf("|----------------------------------------|\n");
	printf("|----------------------------------------|\n");
	printf("|4 of a Kind ||            \\             |\n");//8
	printf("|----------------------------------------|\n");
	printf("|Full House  ||            \\             |\n");//9
	printf("|----------------------------------------|\n");
	printf("|S. Straight ||            \\             |\n");//10
	printf("|----------------------------------------|\n");
	printf("|L. Straight ||            \\             |\n");//11
	printf("|----------------------------------------|\n");
	printf("|Yacht       ||            \\             |\n");//12
	printf("|----------------------------------------|\n");
	printf("\n");
	printf("|========================================|\n");
	printf("! Total      ||            \\             |\n");//13
	printf("|========================================|\n");
}

void drawEmpty(int x, int y) {
	gotoxy(x, y);  printf("                         ");
	gotoxy(x, y + 1);  printf("                         ");
	gotoxy(x, y + 2);  printf("                         ");
	gotoxy(x, y + 3);  printf("                         ");
	gotoxy(x, y + 4);  printf("                         ");
	gotoxy(x, y + 5);  printf("                         ");
	gotoxy(x, y + 6);  printf("                         ");
	gotoxy(x, y + 7); printf("                         ");
	gotoxy(x, y + 8); printf("                         ");
	gotoxy(x, y + 9); printf("                         ");

}
void draw1(int x, int y) {
	gotoxy(x, y);  printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2);  printf("*                       *");
	gotoxy(x, y + 3);  printf("*                       *");
	gotoxy(x, y + 4);  printf("*          $$$          *");
	gotoxy(x, y + 5);  printf("*          $$$          *");
	gotoxy(x, y + 6);  printf("*                       *");
	gotoxy(x, y + 7); printf("*                       *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");

}
void draw2(int x, int y) {
	gotoxy(x, y); printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2); printf("*   $$$                 *");
	gotoxy(x, y + 3); printf("*   $$$                 *");
	gotoxy(x, y + 4); printf("*                       *");
	gotoxy(x, y + 5); printf("*                       *");
	gotoxy(x, y + 6); printf("*                 $$$   *");
	gotoxy(x, y + 7); printf("*                 $$$   *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");

}
void draw3(int x, int y) {
	gotoxy(x, y);  printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2);  printf("*   $$$                 *");
	gotoxy(x, y + 3);  printf("*   $$$                 *");
	gotoxy(x, y + 4);  printf("*          $$$          *");
	gotoxy(x, y + 5);  printf("*          $$$          *");
	gotoxy(x, y + 6);  printf("*                 $$$   *");
	gotoxy(x, y + 7); printf("*                 $$$   *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");

}
void draw4(int x, int y) {
	gotoxy(x, y);  printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 3);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 4);  printf("*                       *");
	gotoxy(x, y + 5);  printf("*                       *");
	gotoxy(x, y + 6);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 7); printf("*   $$$           $$$   *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");

}
void draw5(int x, int y) {
	gotoxy(x, y);  printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 3);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 4);  printf("*          $$$          *");
	gotoxy(x, y + 5);  printf("*          $$$          *");
	gotoxy(x, y + 6);  printf("*   $$$           $$$   *");
	gotoxy(x, y + 7); printf("*   $$$           $$$   *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");

}
void draw6(int x, int y) {
	gotoxy(x, y);  printf("*************************");
	gotoxy(x, y + 1);  printf("*                       *");
	gotoxy(x, y + 2);  printf("*   $$$    $$$    $$$   *");
	gotoxy(x, y + 3);  printf("*   $$$    $$$    $$$   *");
	gotoxy(x, y + 4);  printf("*                       *");
	gotoxy(x, y + 5);  printf("*                       *");
	gotoxy(x, y + 6);  printf("*   $$$    $$$    $$$   *");
	gotoxy(x, y + 7); printf("*   $$$    $$$    $$$   *");
	gotoxy(x, y + 8); printf("*                       *");
	gotoxy(x, y + 9); printf("*************************");
}

void draw_dice(int number, int x) {
	int y = 22;
	if (number == 1) draw1(x, y);
	else if (number == 2) draw2(x, y);
	else if (number == 3) draw3(x, y);
	else if (number == 4) draw4(x, y);
	else if (number == 5) draw5(x, y);
	else if (number == 6) draw6(x, y);
	else drawEmpty(x, y);
}
void draw_keep(int number, int x) {
	int y = 7;
	if (number == 1) draw1(x, y);
	else if (number == 2) draw2(x, y);
	else if (number == 3) draw3(x, y);
	else if (number == 4) draw4(x, y);
	else if (number == 5) draw5(x, y);
	else if (number == 6) draw6(x, y);
	else drawEmpty(x, y);
}

void drawAllDice(int* dice, int* keep)
{
	for (int diceCount = 0; diceCount < 5; diceCount++)
	{
		draw_keep(*(keep + diceCount), diceCount * 28 + 46);
		draw_dice(*(dice + diceCount), diceCount * 28 + 46);
	}
}

int diceCursor(int mvCNT, int playerTurn)
{
	static int Cols;
	int diceX[5] = { 46, 74, 102, 130, 158 };
	static int c;

	if (mvCNT == 0)
	{
		c = 0;
	}
	else if (mvCNT == -1)
	{
		return c;
	}

	//system("cls");
	gotoxy(46, 19); printf("                                                                                                                                                          ");
	gotoxy(46, 20); printf("                                                                                                                                                          ");

	Cols = diceX[c];
	gotoxy(Cols + 11, 19);
	printf("/\\");
	gotoxy(Cols + 11, 20);
	printf("\\/");
	if (GetAsyncKeyState(playerTurn ? VK_LEFT : 0x41) && c != 0) // A key //if( (GetAsyncKeyState(0x41) && playerTurn == 0) || (GetAsyncKeyState(VK_LEFT) && playerTurn == 1) )
	{
		c--;
	}
	else if (GetAsyncKeyState(playerTurn ? VK_RIGHT : 0x44) && c != 4) //D key //if( (GetAsyncKeyState(0x44) && playerTurn == 0) || (GetAsyncKeyState(VK_RIGHT) && playerTurn == 1) )
	{
		c++;
	}

	return 0;
}

int tableCursor(int mvCNT, int gameTurn)
{
	int tableY[12] = { 4, 6, 8, 10, 12, 14, 17, 20, 22, 24, 26, 28 };
	int tableX[2] = { 20, 35 };
	static int y;

	if (mvCNT == 0)
	{
		y = 0;
	}
	else if (mvCNT == -1)
	{
		return y;
	}
	gotoxy(13, tableY[y]);
	printf("<<");
	gotoxy(tableX[gameTurn], tableY[y]);
	printf("<  >");
	if (GetAsyncKeyState(gameTurn ? VK_UP : 0x57) && y != 0) // W key //if( (GetAsyncKeyState(0x57) && playerTurn == 0) || (GetAsyncKeyState(VK_UP) && playerTurn == 1) )
	{
		y--;
	}
	else if (GetAsyncKeyState(gameTurn ? VK_DOWN : 0x53) && y != 11) //S key
	{
		y++;
	}

	return 0;
}

void drawPoints(int* points, int* state, int playerTurn)
{
	int tableY[12] = { 4, 6, 8, 10, 12, 14, 17, 20, 22, 24, 26, 28 };

	for (int y = 0; y < 11; y++)
	{
		if (*(state + playerTurn) == 1)
		{
			gotoxy(21, tableY[y]); printf("%2d", *(points + 0) + y);
			gotoxy(36, tableY[y]); printf("%2d", *(points + 1) + y);
		}
		else
		{
			continue;
		}
	}
}

int GetPoint(int y, int* endDice)
{
	int points[12] = { 0, }; //[point][player]
	int dices[5];
	int count[6] = { 0, };

	for (int i = 0; i < 5; i++)
	{
		dices[i] = *(endDice + i);
	}

	for (int i = 0; i < 5; i++)
	{
		if (dices[i] == 1)
		{
			count[0]++;
		}
		else if (dices[i] == 2)
		{
			count[1]++;
		}
		else if (dices[i] == 3)
		{
			count[2]++;
		}
		else if (dices[i] == 4)
		{
			count[3]++;
		}
		else if (dices[i] == 5)
		{
			count[4]++;
		}
		else
		{
			count[5]++;
		}
	}

	int ace = count[0] * 1;
	points[0] = ace;

	int deuce = count[1] * 2;
	points[1] = deuce;

	int three = count[2] * 3;
	points[2] = three;

	int four = count[3] * 4;
	points[3] = four;

	int five = count[4] * 5;
	points[4] = five;

	int six = count[5] * 6;
	points[5] = six;

	int choice = ace + deuce + three + four + five + six;
	points[6] = choice;

	int fourOfKind = ace + deuce + three + four + five + six;
	int fullHouse = ace + deuce + three + four + five + six;
	const int yacht = 50;
	const int largeStraight = 30;
	const int smallStraight = 15;
	const int bonus = 35;

	if ((count[0] == 5) || (count[1] == 5) || (count[2] == 5) || (count[3] == 5) || (count[4] == 5) || (count[5] == 5))
	{
		//yacht, 4 of kind, Full House
		points[11] = yacht;
		points[7] = fourOfKind;
		points[8] = fullHouse;
	}
	else if (
		((count[0] == 3) && ((count[1] == 2) || (count[2] == 2) || (count[3] == 2) || (count[4] == 2) || (count[5] == 2)))
		|| ((count[1] == 3) && ((count[0] == 2) || (count[2] == 2) || (count[3] == 2) || (count[4] == 2) || (count[5] == 2)))
		|| ((count[2] == 3) && ((count[0] == 2) || (count[1] == 2) || (count[3] == 2) || (count[4] == 2) || (count[5] == 2)))
		|| ((count[3] == 3) && ((count[0] == 2) || (count[1] == 2) || (count[2] == 2) || (count[4] == 2) || (count[5] == 2)))
		|| ((count[4] == 3) && ((count[0] == 2) || (count[1] == 2) || (count[2] == 2) || (count[3] == 2) || (count[5] == 2)))
		|| ((count[5] == 3) && ((count[0] == 2) || (count[1] == 2) || (count[2] == 2) || (count[3] == 2) || (count[4] == 2)))
		)
	{
		points[8] = fullHouse;
	}
	//4 of kind
	if ((count[0] == 4) || (count[1] == 4) || (count[2] == 4) || (count[3] == 4) || (count[4] == 4) || (count[5] == 4)) // 앞선 조건문에서 처리함 (4보다 큰경우를)
	{
		points[7] = fourOfKind;
	}
	//straights
	if (((count[0] >= 1) && (count[1] >= 1) && (count[2] >= 1) && (count[3] >= 1))
		|| ((count[1] >= 1) && (count[2] >= 1) && (count[3] >= 1) && (count[4] >= 1))
		|| ((count[2] >= 1) && (count[3] >= 1) && (count[4] >= 1) && (count[5] >= 1)))
	{
		//Large Straight, Small Straight

		points[9] = smallStraight;
	}
	else if (((count[0] >= 1) && (count[1] >= 1) && (count[2] >= 1) && (count[3] >= 1) && (count[4] >= 1))
		|| ((count[1] >= 1) && (count[2] >= 1) && (count[3] >= 1) && (count[4] >= 1) && (count[5] >= 1)))
	{
		//Small Straight
		points[9] = smallStraight;
		points[10] = largeStraight;
	}
	return points[y];
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
