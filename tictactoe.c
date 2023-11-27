#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

struct pii {
	int R, C;
};

char brd[5][5];
bool sys, win;
bool flld[5][5];
struct pii mvu[5], mvc[5];

//setting a random seed for rand
void set_random_seed() {
	time_t s;
	time(&s);
	srand(s);
}

//the function that adopts the game to user's OS
void check_sys() {
	int mode;
	printf("Choose: Windows(0), Other(1)\n");
	scanf("%d", &mode);
	sys = mode;
}

//fills the board with ' '
void fill_brd() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			brd[i][j] = ' ';
}

//shows the board
void sh_board() {

	system((sys)? "clear" : "cls");
	printf("   1   2   3\n");
	printf("A  %c | %c | %c \n", brd[0][0], brd[0][1], brd[0][2]);
	printf("  ---+---+---\n");
	printf("B  %c | %c | %c \n", brd[1][0], brd[1][1], brd[1][2]);	
	printf("  ---+---+---\n");
	printf("C  %c | %c | %c \n", brd[2][0], brd[2][1], brd[2][2]);
	
	printf("\n\n");

}

//checks if user made a valid move
bool isval(int r, int c) {
	if (r < 0 || r > 2 || c < 0 || c > 2) {
		printf("Not a valid move\n");
		return false;
	}
	if (brd[r][c] != ' ') {
		printf("Not a valid move\n");
		return false;
	}
	return true;
}

//updates the flld array
void update_flld(int r, int c) {
	
	if (r == c) {
		flld[2][0] = true;
	}
	if (r + c == 2) {
		flld[2][1] = true;
	}

	flld[0][r] = true;
	flld[1][c] = true;

}

//waits for x seconds
void wait(int x) {

	time_t b, e;
	time(&b);
	time(&e);

	while(e - b < x)
		time(&e);

}

//inputs user's moves
void usr_move(int num) {

	sh_board();
	printf("It's your turn!\n");
	
	bool mode = false;
	char in1, in2;
	int r, c;
	while (!mode) {

		scanf(" %c %c", &in1, &in2);
		r = in1 - 'A', c = in2 - '1';
		mode = isval(r, c);

	}

	brd[r][c] = 'X';

	mvu[num].R = r, mvu[num].C = c;	
	update_flld(r, c);
	
	sh_board();

}

//checks if three pieces are in a row, column or diameter
bool iswin(struct pii a,struct pii b, struct pii c) {
	if (a.R == a.C && b.R == b.C && c.R == c.C)
		return true;
	if (a.R + a.C == 2 && b.R + b.C == 2 && c.R + c.C == 2)
		return true;
	if (a.R == b.R && b.R == c.R)
		return true;
	if (a.C == b.C && b.C == c.C)
		return true;
	return false;
}

//considers computer's move
void move(int numc, int numu) {


	if (numc == 2) {
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				
				if (brd[r][c] != ' ')
					continue;

				struct pii tmp = {r, c};
				if (iswin(mvc[0], mvc[1], tmp)) {
					brd[r][c] = 'O';
					win = true;
					return;
				}

			}
		}
	}


	if (numu == 1) {
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				
				if (brd[r][c] != ' ')
					continue;
				
				struct pii tmp = {r, c};	
				if (iswin(mvu[0], mvu[1], tmp)) {
					brd[r][c] = 'O';
					mvc[numc].R = r, mvc[numc].C = c;
					return;
				}

			}
		}
	}
	
	
	int mxpoint = -1;
	struct pii res;

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			
			if (brd[r][c] != ' ')
				continue;
				
			int point = 0;
			
			if (r == c && !flld[2][0]) {
				if (numc == 1 && mvc[0].R == mvc[0].C)
					point = 5;
				point++;
			}
			if (r + c == 2 && !flld[2][1]) {
				if (numc == 1 && mvc[0].R + mvc[0].C == 2)
					point = 5;
				point++;
			}
			if (!flld[0][r]) {
				if (numc == 1 && mvc[0].R == r)
					point = 5;
				point++;
			}
			if (!flld[1][c]) {
				if (numc == 1 && mvc[0].C == c)
					point = 5;
				point++;
			}
	
			if (point > mxpoint) {
				mxpoint = point;
				res.R = r, res.C = c;			
			}

		}
	}

	brd[res.R][res.C] = 'O'; 
	mvc[numc].R = res.R, mvc[numc].C = res.C;

}


void play(bool first) {
	
	if (first) {

		printf("You are the second player!\n");
		wait(2);
		
		int r = rand() % 3, c = rand() % 3;
		brd[r][c] = 'O';
		mvc[0].R = r, mvc[0].C = c;

		for(int i = 0; i < 2; i++) {
			usr_move(i);
			move(i + 1, i);
		}

		if (!win)
			usr_move(2);		

	}

	else {
		
		printf("You are the first player!\n");
		wait(2);
		
		for (int i = 0; i < 3; i++) {
			usr_move(i);
			move(i, i);
		}

	}
		
}


void show_results() {
	
	sh_board();

	if (win) {
		for (int i = 0; i < 4; i++) {
			printf("-");
            fflush(stdout);
			wait(1);
		}
		printf("GameOver");
        fflush(stdout);
		for (int i = 0; i < 4; i++) {
			printf("-");
            fflush(stdout);
			wait(1);
		}
		printf("\n");
		wait(5);
	}
	
	else {
		printf("Draw\n");
		wait(2);
		printf("Well Played!\n");
		wait(5);
	}
	
}


int main() {
	
	set_random_seed();
	
	check_sys();//check the user's OS

	system((sys)? "clear" : "cls");

	printf("Welcome to tic tac toe ^_^\n");
	fflush(stdout);
	wait(2);
	printf("We each place 3 pieces.\n");
	fflush(stdout);
	wait(2);

	fill_brd(), sh_board(); //make the board ready, show the empty board to user

	play(rand() % 2);

	show_results();
	
	return 0;
}
