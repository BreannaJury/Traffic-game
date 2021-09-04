#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 7
#define NUM_COLS 18

#define SPACE 0
#define WALL -1
#define EXIT -2

void InitialiseRoad(int road[NUM_ROWS][NUM_COLS], char side, int pos)
{
	//Set up road array
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (i == 0 || i == (NUM_ROWS - 1) || j == 0 || (j == NUM_COLS - 1)) {
				//set up edges
				road[i][j] = WALL;
			}
			else {
				//set up middle
				road[i][j] = SPACE;
			}

		}
	}
	//setup exit
	if (side == 'N') {
		road[0][pos] = EXIT;
	}
	else if (side == 'E') {
		road[pos][NUM_COLS-1] = EXIT;
	}
	else if (side == 'S') {
		road[NUM_ROWS-1][pos] = EXIT;
	}
	else if (side == 'W') {
		road[pos][0] = EXIT;
	}
}

void PrintRoad(int road[NUM_ROWS][NUM_COLS])
{
	//display road
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			//# for walls
			if (road[i][j] == WALL) {
				printf("#");
			}
			// nothing for space
			else if (road[i][j] == SPACE) {
				printf(" ");
			}
			// O for exit
			else if (road[i][j] == EXIT) {
				printf("O");
			}
			//print letters for cars
			else if (road[i][j] > SPACE) {
				//convert number to letter
				char car = 'A' + (road[i][j] - 1);
				printf("%c", car);
			}
		}
		printf("\n");
	}
}

double PercentUsed(int road[NUM_ROWS][NUM_COLS]) {
	//calculate percentage of road taken up by cars
	//total number of spaces
	int total = 0;
	//number of spaces used
	double count = 0.0;

	for (int i = 1; i < NUM_ROWS - 1; i++) {
		for (int j = 1; j < NUM_COLS - 1; j++) {
			//add one to total for every array position
			total++;
			//add one to count for every time array position isn't a space
			if (road[i][j] != SPACE) {
				count++;
			}
		}
	}
	//calculate percent used
	double percentUsed = (count / total) * 100;
	return percentUsed;
}

void AddCar(int road[NUM_ROWS][NUM_COLS], int row, int col, int size) {
	//add cars to array
	//search for highest number currently used in grid
	int max = 0;
	for (int i = 1; i < NUM_ROWS - 1; i++) {
		for (int j = 1; j < NUM_COLS - 1; j++) {
			if (road[i][j] > max) {
				max = road[i][j];
			}
		}
	}
	//use number one higher than current max
	int useNumber = max + 1;

	//check that all positions are empty
	int count = 0;
	int safe = 1;
	if (size > 0) { //work from left to right (increase col)
		while (count < size) {
			if ((road[row][col] != SPACE) || (road[row][col + count] != SPACE)) {
				safe = 0;
			}
			count++;
		}
	}
	else if (size < 0) { //work from top to bottom (decrease row)
		int newSize = size - size * 2;
		while (count < newSize) {
			if ((road[row][col] != SPACE) || (road[row + count][col] != SPACE)) {
				safe = 0;
			}
			count++;
		}
	}

	//if positions are clear, assign cars
	count = 0;
	if (safe != 0 && size < 0) { //vertical
		int newSize = size - size * 2;
		while (count < newSize) {
			road[row + count][col] = useNumber;
			count++;
		}
	}
	else if (safe != 0 && size > 0) { //horizontal
		while (count < size) {
			road[row][col + count] = useNumber;
			count++;
		}
	}
}

void FindCar(int road[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int *rowEnd, int *colEnd) {
	//FInd car in array
	//convert letter to number to find car
	int car = move - 'A' + 1;

	//search for last occurance of car
	for (int i = 1; i < NUM_ROWS - 1; i++) {
		for (int j = 1; j < NUM_COLS - 1; j++) {
			if (road[i][j] == car) {
				*rowEnd = i;
				*colEnd = j;
			}
		}
	}

	//check to the left
	if (road[*rowEnd][*colEnd - 1] != car) {
		//look above
		int ypos = *rowEnd;
		while (road[ypos][*colEnd] == car) {
			ypos--;
		}
		*colStart = *colEnd;
		*rowStart = ypos + 1;
	}
	else {
		//look to the left
		int xpos = *colEnd;
		while (road[*rowEnd][xpos] == car) {
			xpos--;
		}
		*colStart = xpos + 1;
		*rowStart = *rowEnd;
	}
	
}

int MoveCar(int road[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1) 
{
	//move car in the array
	//check orientation of car
	if (r0 != r1) {
		//car is vertical
		//check if there is space above
		if (road[r0 - 1][c0] == SPACE) {
			//move upwards until stop
			int rpos = r0;
			int count = 0; 
			while (road[rpos - 1][c0] == SPACE) {
				//set free space to letter stored in current space
				road[rpos - 1][c0] = road[rpos][c0];
				//set last position to empty
				road[r1 - count][c0] = SPACE;
				//increase count and decrease row number to check
				count++;
				rpos--;
			}
			//return 1 if car has reached exit
			if (road[rpos - 1][c0] == EXIT) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			//move downwards until stop
			int rpos = r1;
			int count = 0;
			while (road[rpos + 1][c0] == SPACE) {
				//set free space to letter stored in current space
				road[rpos + 1][c0] = road[rpos][c0];
				//set last position to 0
				road[r0 + count][c0] = 0;
				//increase count and decrease row number to check
				count++;
				rpos++;
			}
			//return 1 if car has reached exit
			if (road[rpos + 1][c0] == EXIT) {
				return 1;
			}
			else {
				return 0;
			}
		}

	}
	else {
		//car is horizontal
		//check if there is space to the left
		if (road[r0][c0 - 1] == SPACE) {
			//move left
			int cpos = c0;
			int count = 0;
			while (road[r0][cpos - 1] == SPACE) {
				//set free space to letter stored in current space
				road[r0][cpos - 1] = road[r0][cpos];
				//set last position to space
				road[r0][c1 - count] = SPACE;
				//increase count and decrease row number to check
				count++;
				cpos--;
			}
			//return 1 if car has reached exit
			if (road[r0][cpos - 1] == EXIT) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			//move right until stop
			int cpos = c1;
			int count = 0;
			while (road[r0][cpos + 1] == SPACE) {
				//set free space to letter stored in current space
				road[r0][cpos + 1] = road[r0][cpos];
				//set last position to 0
				road[r0][c0 + count] = 0;
				//increase count and decrease row number to check
				count++;
				cpos++;
			}
			//return 1 if car has reached exit
			if (road[r0][cpos + 1] == EXIT) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}

}


/***********************************************************/
/***********************************************************/
/******* A SIMPLE MAIN FUNCTION TO TEST YOUR PROGRESS ******/
/***********************************************************/
/***********************************************************/

/* You should add your own tests in here */
int main(void)
{
	int road[NUM_ROWS][NUM_COLS];

	/* Simple test code for Tasks 1 and 2 */
	InitialiseRoad(road, 'S', 6);
	AddCar(road, 2, 6, -3);
	AddCar(road, 4, 14, -2);
	AddCar(road, 5, 3, 9);
	PrintRoad(road);

	/*
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			printf("%3d", road[i][j]);
		}
		printf("\n");
	} */
	
	return 0;
}
