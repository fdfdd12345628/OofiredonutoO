#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEPTH 8
#define INF 0x7FFFFFFF
#define NEGINF 0x80000000
#define NEXTMOVENUM 100
#define WHITE 1
#define BLACK 2

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
	int color;
	int tile[10][10];
	int bridge[10][10];
	int prohibit_tile[10][10];
	int connected_tile[10][10];
	int computed_prohibit;
	int computed_connected;

} board;

int init_board(board* b) {
	b->color = -1;
	memcpy(b->tile, init_board_content, sizeof(init_board_content));
	memcpy(b->bridge, init_board_content, sizeof(init_board_content));
	memcpy(b->prohibit_tile, init_board_content, sizeof(init_board_content));
	memcpy(b->connected_tile, init_board_content, sizeof(init_board_content));
	b->computed_prohibit = 0;
	b->computed_connected = 0;
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
	FILE* c_file = fopen(chess_location, "w");
	FILE* b_file = fopen(bridge_location, "w");
	if (c_file == NULL || b_file == NULL) {
		printf("open file failed\n");
		return -1;
	}
	int i = 0;
	for (i = 0; i < 10; i++) {
		fprintf(c_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->tile[i][0], b->tile[i][1], b->tile[i][2], b->tile[i][3], b->tile[i][4], b->tile[i][5], b->tile[i][6], b->tile[i][7], b->tile[i][8], b->tile[i][9]);
	}
	for (i = 0; i < 10; i++) {
		fprintf(b_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->bridge[i][0], b->bridge[i][1], b->bridge[i][2], b->bridge[i][3], b->bridge[i][4], b->bridge[i][5], b->bridge[i][6], b->bridge[i][7], b->bridge[i][8], b->bridge[i][9]);
	}
	return 0;
}

int count_surround_tile(int tile_board[10][10], int x, int y, int depth, int color, int searched[10][10]) {

	if (searched[x][y] == 1) {
		return 0;
	}
	if (tile_board[x][y] == color) {
		searched[x][y] = 1;
		if (depth == 0) {
			return 1;
		}
		int r = 1;

		if (x > 0) {
			r += count_surround_tile(tile_board, x - 1, y, depth - 1, color, searched);
		}
		if (y > 0) {
			r += count_surround_tile(tile_board, x, y - 1, depth - 1, color, searched);
		}
		if (x < 9) {
			r += count_surround_tile(tile_board, x + 1, y, depth - 1, color, searched);
		}
		if (y < 9) {
			r += count_surround_tile(tile_board, x, y + 1, depth - 1, color, searched);
		}

		return r;
	}
	return 0;
}

int count_all_connect_tile(board* b) {
	int i, j, tile_color_num = -1;
	if (b->color == WHITE) {
		tile_color_num = WHITE;

	}
	else if (b->color == BLACK)
	{
		tile_color_num = BLACK;

	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			int search[10][10] = { 0 };
			b->connected_tile[i][j] = count_surround_tile(b->tile, i, j, 3, tile_color_num, search);
			
		}
		printf("");
	}
	b->computed_connected = 1;
	return 0;
}

int mark_prohibit_by_tile(board* b) {
	int tile_color_num, opposite_colot_num;
	if (b->color == WHITE) {
		tile_color_num = WHITE;
		opposite_colot_num = BLACK;
	}
	else if (b->color == BLACK)
	{
		tile_color_num = BLACK;
		opposite_colot_num = WHITE;
	}
	if (b->computed_prohibit) return 0;
	if (!b->computed_connected) { 
		count_all_connect_tile(b); 
	}
	int finded[10][10];
	memcpy(finded, init_board_content, sizeof(init_board_content));
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {

			// our color surround
			if (b->tile[i][j] == tile_color_num) {
				if (b->connected_tile[i][j] == 4) {
					
					if (i > 0 && b->tile[i - 1][j] == 0) {
						b->prohibit_tile[i - 1][j] = 1;
						finded[i - 1][j] = 1;
						if (j > 0 && b->tile[i-1][j - 1] == 0) {
							b->prohibit_tile[i-1][j - 1] = 1;
							finded[i-1][j - 1] = 1;
						}
						if (j < 9 && b->tile[i-1][j + 1] == 0) {
							b->prohibit_tile[i-1][j + 1] = 1;
							finded[i-1][j + 1] = 1;
						}
					}
					if (j > 0 && b->tile[i][j - 1] == 0) {
						b->prohibit_tile[i][j - 1] = 1;
						finded[i][j - 1] = 1;
						
					}
					if (i < 9 && b->tile[i + 1][j] == 0) {
						b->prohibit_tile[i + 1][j] = 1;
						finded[i + 1][j] = 1;
						if (j > 0 && b->tile[i+1][j - 1] == 0) {
							b->prohibit_tile[i+1][j - 1] = 1;
							finded[i + 1][j - 1] = 1;
						}
						if (j < 9 && b->tile[i+1][j + 1] == 0) {
							b->prohibit_tile[i+1][j + 1] = 1;
							finded[i + 1][j + 1] = 1;
						}
					}
					if (j < 9 && b->tile[i][j + 1] == 0) {
						b->prohibit_tile[i][j + 1] = 1;
						finded[i][j + 1] = 1;
					}
					
				}

				b->prohibit_tile[i][j] = 1;
			}

			// enemy color 
			else if (b->tile[i][j] == opposite_colot_num)
			{
				b->prohibit_tile[i][j] = 1;
				finded[i][j] = 1;
			}
			else {
				if (finded[i][j] == -1) {
					b->prohibit_tile[i][j] = 0;
				}
			}
			finded[i][j] = 1;
		}
	}
}

int mark_prohibit_by_bridge(board* b) {

}

int next_move(board* b, int next_tile[NEXTMOVENUM][10][10], int next_bridge[NEXTMOVENUM][10][10]) {
	int i = 0;
	for (i = 0; i < NEXTMOVENUM; i++) {
		memcpy(next_tile[i], init_board_content, sizeof(init_board_content));
		memcpy(next_bridge[i], init_board_content, sizeof(init_board_content));

	}



	return -1;
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
	print_board(&a);
	a.color = BLACK;

	/*int searched[10][10] = {0};
	int r = count_surround_tile(a.tile, 2, 2, 3, BLACK, searched);
	printf("check_island = %d\n", r);
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", searched[i][0], searched[i][1], searched[i][2], searched[i][3], searched[i][4], searched[i][5], searched[i][6], searched[i][7], searched[i][8], searched[i][9]);
	}
	
	count_all_connect_tile(&a);
	*/
	mark_prohibit_by_tile(&a);
	printf("count donw\n");
	print_board(&a);
}

int main(int argc, char* argv[])
{
	test();
	printf("Hello world!\n");
	board a = {
		BLACK,
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
