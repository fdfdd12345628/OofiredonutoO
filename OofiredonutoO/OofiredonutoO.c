#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEPTH 8
#define INF 0x7FFFFFFF
#define NEGINF 0x80000000

static int init_board_content[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

typedef struct board
{
	enum { white, black } color;
	int tile[10][10];
	int bridge[10][10];


} board;

int init_board(board* b) {
	b->color = -1;
	memcpy(b->tile, init_board_content, sizeof(init_board_content));
	memcpy(b->bridge, init_board_content, sizeof(init_board_content));
	return 0;
}

int read_board(char* chess_location, char* bridge_location, board* b) {
	FILE* c_file = fopen(chess_location, "r");
	FILE* b_file = fopen(bridge_location, "r");
	if (c_file == NULL || b_file == NULL) {
		printf("open file failed\n");
		return -1;
	}
	int i = 0;
	for (i = 0; i < 10; i++) {
		fscanf(c_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &(b->tile[i][0]), &(b->tile[i][1]), &(b->tile[i][2]), &(b->tile[i][3]), &(b->tile[i][4]), &(b->tile[i][5]), &(b->tile[i][6]), &(b->tile[i][7]), &(b->tile[i][8]), &(b->tile[i][9]));
	}
	for (i = 0; i < 10; i++) {
		fscanf(b_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &(b->bridge[i][0]), &(b->bridge[i][1]), &(b->bridge[i][2]), &(b->bridge[i][3]), &(b->bridge[i][4]), &(b->bridge[i][5]), &(b->bridge[i][6]), &(b->bridge[i][7]), &(b->bridge[i][8]), &(b->bridge[i][9]));
	}
	return 0;

}

int write_board(char* chess_location, char* bridge_location, board* b) {

}

void print_board(board* b) {
	printf("current color: %d", b->color);
	printf("tile:\n");
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->tile[i][0], b->tile[i][1], b->tile[i][2], b->tile[i][3], b->tile[i][4], b->tile[i][5], b->tile[i][6], b->tile[i][7], b->tile[i][8], b->tile[i][9]);
	}
	printf("bridge:\n");
	for (i = 0; i < 10; i++) {
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->bridge[i][0], b->bridge[i][1], b->bridge[i][2], b->bridge[i][3], b->bridge[i][4], b->bridge[i][5], b->bridge[i][6], b->bridge[i][7], b->bridge[i][8], b->bridge[i][9]);
	}

}

int AI(int argc, char* argv[]) {
	if (argc != 6) {
		printf("error parameters input count\n");
		return -1;
	}
	board init_board;
	int r = read_board(argv[3], argv[4], &init_board);
	if (r != 0) {
		printf("error read board\n");
		return -1;
	}

}

int test() {
	board a;
	init_board(&a);
	print_board(&a);
	read_board("chess.txt", "bridge.txt", &a);
	// print_board(&a);
}

int main(int argc, char* argv[])
{
	test();
	printf("Hello world!\n");
	board a = {
		black,
		{0},
		{0},
	};

	if (argc == 0) {
		printf("must input correct parameters\n");
	}
	else
	{
		AI(argc, argv);
	}

}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
