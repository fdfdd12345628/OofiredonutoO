#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEPTH 8
#define INF 0x7FFFFFFF
#define NEGINF 0x80000000
#define NEXTMOVENUM 1000
#define WHITE 1
#define BLACK 2
#define NEXTBRIDGENUM 100

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
	int computed_prohibit_tile;
	int computed_connected;

} board;

int init_board(board* b) {
	b->color = -1;
	memcpy(b->tile, init_board_content, sizeof(init_board_content));
	memcpy(b->bridge, init_board_content, sizeof(init_board_content));
	memcpy(b->prohibit_tile, init_board_content, sizeof(init_board_content));
	memcpy(b->connected_tile, init_board_content, sizeof(init_board_content));
	b->computed_prohibit_tile = 0;
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

void print_board(board* b) {
	printf("current color: %d", b->color);
	printf("tile:\n");
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->tile[i][0], b->tile[i][1], b->tile[i][2], b->tile[i][3], b->tile[i][4], b->tile[i][5], b->tile[i][6], b->tile[i][7], b->tile[i][8], b->tile[i][9]);
	}
	printf("bridge:\n");
	for (i = 0; i < 10; i++) {
		// printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", b->bridge[i][0], b->bridge[i][1], b->bridge[i][2], b->bridge[i][3], b->bridge[i][4], b->bridge[i][5], b->bridge[i][6], b->bridge[i][7], b->bridge[i][8], b->bridge[i][9]);
	}

	printf("\n\n\n");

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
		// printf("");
	}
	b->computed_connected = 1;
	return 0;
}

int mark_prohibit_tile(board* b) {
	if (b->computed_prohibit_tile) return 0;
	mark_prohibit_tile_by_tile(b);
	mark_prohibit_tile_by_bridge(b);
	b->computed_prohibit_tile = 1;
	return 0;
}

int mark_prohibit_tile_by_tile(board* b) {
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
						if (j > 0 && b->tile[i - 1][j - 1] == 0) {
							b->prohibit_tile[i - 1][j - 1] = 1;
							finded[i - 1][j - 1] = 1;
						}
						if (j < 9 && b->tile[i - 1][j + 1] == 0) {
							b->prohibit_tile[i - 1][j + 1] = 1;
							finded[i - 1][j + 1] = 1;
						}
					}
					if (j > 0 && b->tile[i][j - 1] == 0) {
						b->prohibit_tile[i][j - 1] = 1;
						finded[i][j - 1] = 1;

					}
					if (i < 9 && b->tile[i + 1][j] == 0) {
						b->prohibit_tile[i + 1][j] = 1;
						finded[i + 1][j] = 1;
						if (j > 0 && b->tile[i + 1][j - 1] == 0) {
							b->prohibit_tile[i + 1][j - 1] = 1;
							finded[i + 1][j - 1] = 1;
						}
						if (j < 9 && b->tile[i + 1][j + 1] == 0) {
							b->prohibit_tile[i + 1][j + 1] = 1;
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
	return 0;
}

int mark_prohibit_tile_by_bridge(board* b) {
	int used_bridge[51];
	int i, j;
	memcpy(used_bridge, init_board_content, sizeof(used_bridge));

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (b->bridge[i][j] != 0) {
				int bridge_num = b->bridge[i][j];
				if (used_bridge[bridge_num] == -1) {
					if (i > 1 && b->bridge[i - 2][j] == bridge_num) {
						b->prohibit_tile[i - 1][j] = 1;
					}
					else if (i > 1 && j > 0 && b->bridge[i - 2][j - 1] == bridge_num) {
						b->prohibit_tile[i - 1][j] = 1;
						b->prohibit_tile[i - 1][j - 1] = 1;
					}
					else if (i > 1 && j > 1 && b->bridge[i - 2][j - 2] == bridge_num) {
						b->prohibit_tile[i - 1][j - 1] = 1;
					}
					else if (i > 0 && j > 1 && b->bridge[i - 1][j - 2] == bridge_num) {
						b->prohibit_tile[i - 1][j - 1] = 1;
						b->prohibit_tile[i][j - 1] = 1;
					}
					else if (j > 1 && b->bridge[i][j - 2] == bridge_num) {
						b->prohibit_tile[i][j - 1] = 1;
					}
					else if (i < 9 && j > 1 && b->bridge[i + 1][j - 2] == bridge_num) {
						b->prohibit_tile[i + 1][j - 1] = 1;
						b->prohibit_tile[i][j - 1] = 1;
					}
					else if (i < 8 && j > 1 && b->bridge[i + 2][j - 2] == bridge_num) {
						b->prohibit_tile[i + 1][j - 1] = 1;
					}
					else if (i < 8 && j > 0 && b->bridge[i + 2][j - 1] == bridge_num) {
						b->prohibit_tile[i + 1][j - 1] = 1;
						b->prohibit_tile[i + 1][j] = 1;
					}
					else if (i < 8 && b->bridge[i + 2][j] == bridge_num) {
						b->prohibit_tile[i + 1][j] = 1;
					}
					else if (i < 8 && j < 9 && b->bridge[i + 2][j + 1] == bridge_num) {
						b->prohibit_tile[i + 1][j] = 1;
						b->prohibit_tile[i + 1][j + 1] = 1;
					}
					else if (i < 8 && j < 8 && b->bridge[i + 2][j + 2] == bridge_num) {
						b->prohibit_tile[i + 1][j + 1] = 1;
					}
					else if (i < 9 && j < 8 && b->bridge[i + 1][j + 2] == bridge_num) {
						b->prohibit_tile[i + 1][j + 1] = 1;
						b->prohibit_tile[i][j + 1] = 1;
					}
					else if (j < 8 && b->bridge[i][j + 2] == bridge_num) {
						b->prohibit_tile[i][j + 1] = 1;

					}
					else if (i > 0 && j < 8 && b->bridge[i - 1][j + 2] == bridge_num) {
						b->prohibit_tile[i][j + 1] = 1;
						b->prohibit_tile[i - 1][j + 1] = 1;
					}
					else if (i > 1 && j < 8 && b->bridge[i - 2][j + 2] == bridge_num) {
						b->prohibit_tile[i - 1][j + 1] = 1;

					}
					else if (i > 1 && j < 9 && b->bridge[i - 2][j + 1] == bridge_num) {
						b->prohibit_tile[i - 1][j + 1] = 1;
						b->prohibit_tile[i - 1][j] = 1;
					}
					used_bridge[bridge_num] = 1;
				}
			}
		}
	}
}

// exclude cross bridge of a straight bridge
int straight_bridge(int bridge[10][10], int start[2], int end[2]) {
	// if can place return 1, else reture 0
	int y_diff = start[0] - end[0];
	int x_diff = start[1] - end[1];
	int y_center = (start[0] + end[0]) / 2;
	int x_center = (start[1] + end[1]) / 2;
	int i, j;
	/*for (i = 0; i < 10; i++) {
		for (j = 0; j < 9; j++) {
			if (bridge[i][j] == 1) {
				printf("y: %d, x: %d\n", i, j);
			}
		}
	}*/
	/*
	find place

	@ @ @
	b 0 b
	@ @ @

	*/
	for (i = 0; i < 3; i++) {
		int exist_brdige_num = { 0 };
		for (j = 0; j < 3; j++) {
			if (x_diff) {
				int x1 = x_center - 1 + i;
				int x2 = x_center - 1 + j;
				int y1 = y_center - 1;
				int y2 = y_center + 1;
				if (x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9 && y1 >= 0 && y1 <= 9 && y2 >= 0 && y2 <= 9) {
					if (bridge[y1][x1] != 0 && (bridge[y1][x1] == bridge[y2][x2])) {
						return 0;
					}
				}
			}
			else if (y_diff)
			{
				int x1 = x_center - 1;
				int x2 = x_center + 1;
				int y1 = y_center - 1 + i;
				int y2 = y_center - 1 + j;
				if (x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9 && y1 >= 0 && y1 <= 9 && y2 >= 0 && y2 <= 9) {
					// printf("%d %d\n", bridge[x1][y1], bridge[x2][y2]);
					if (bridge[y1][x1] != 0 && (bridge[y1][x1] == bridge[y2][x2])) {
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

int L_bridge(int bridge[10][10], int start[2], int end[2]) {
	// if can place return 1, else reture 0
	int x_diff = start[1] - end[1];
	int y_diff = start[0] - end[0];
	int is_vertical = abs(x_diff) > abs(y_diff) ? 0 : 1;
	int up_point[2];
	int down_point[2];
	int i, j;
	if (start[0] < end[0]) {
		memcpy(up_point, start, sizeof(up_point));
		memcpy(down_point, end, sizeof(down_point));
	}
	else {
		memcpy(up_point, end, sizeof(up_point));
		memcpy(down_point, start, sizeof(down_point));
	}
	int is_slash = -1;
	if (up_point[1] > down_point[1]) {
		is_slash = 1;
	}
	else
	{
		is_slash = 0;
	}
	int bias = is_slash ? -1 : 1;
	if (is_vertical) {
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 2; j++) {
				//int bias = is_slash ? -1 : 1;
				int x1 = up_point[1] + (1 + i) * bias;
				int x2 = down_point[1] - (1 + j) * bias;
				int y1 = up_point[0];
				int y2 = down_point[0];
				if (x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9 && y1 >= 0 && y1 <= 9 && y2 >= 0 && y2 <= 9) {
					if (bridge[y1][x1] != 0 && (bridge[y1][x1] == bridge[y2][x2])) {
						return 0;
					}
				}
			}
		}
	}
	else {
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 2; j++) {
				//int bias = is_slash ? -1 : 1;
				int x1 = up_point[1];// + (1 + i) * bias;
				int x2 = down_point[1];// + (1 + j) * bias;
				int y1 = up_point[0] - (1 + i) * bias;
				int y2 = down_point[0] + (1 + j) * bias;
				if (x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9 && y1 >= 0 && y1 <= 9 && y2 >= 0 && y2 <= 9) {
					if (bridge[y1][x1] != 0 && (bridge[y1][x1] == bridge[y2][x2])) {
						return 0;
					}
				}
			}
		}
	}
	return 1;

}

int cross_bridge(int bridge[10][10], int start[2], int end[2]) {
	// if can place return 1, else reture 0
	int x_diff = start[1] - end[1];
	int y_diff = start[0] - end[0];
	int up_point[2];
	int down_point[2];
	if (start[0] < end[0]) {
		memcpy(up_point, start, sizeof(up_point));
		memcpy(down_point, end, sizeof(down_point));
	}
	else {
		memcpy(up_point, end, sizeof(up_point));
		memcpy(down_point, start, sizeof(down_point));
	}
	int is_slash = -1;
	if (up_point[1] > down_point[1]) {
		is_slash = 1;
	}
	else
	{
		is_slash = 0;
	}
	int upper_place[5][2] = { 0 };
	int down_place[5][2] = { 0 };
	generate_cross_check_place(up_point, is_slash, upper_place, down_place);
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			int x1 = upper_place[i][1];
			int x2 = down_place[j][1];
			int y1 = upper_place[i][0];
			int y2 = down_place[j][0];
			if (x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9 && y1 >= 0 && y1 <= 9 && y2 >= 0 && y2 <= 9) {
				if (bridge[y1][x1] != 0 && (bridge[y1][x1] == bridge[y2][x2])) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int generate_cross_check_place(int up_point[2], int is_slash, int upper_place[5][2], int down_place[5][2]) {
	int bias = is_slash ? -1 : 1;
	upper_place[0][0] = up_point[0] - 1;
	upper_place[0][1] = up_point[1] + (1) * bias;

	upper_place[1][0] = up_point[0];
	upper_place[1][1] = up_point[1] - (1) * bias;

	upper_place[2][0] = up_point[0];
	upper_place[2][1] = up_point[1] + (2) * bias;

	upper_place[3][0] = up_point[0] + 1;
	upper_place[3][1] = up_point[1] + (2) * bias;

	upper_place[0][0] = up_point[0] + 1;
	upper_place[0][1] = up_point[1] + (3) * bias;

	down_place[0][0] = up_point[0] + 1;
	down_place[0][1] = up_point[1] + (-1) * bias;

	down_place[1][0] = up_point[0] + 1;
	down_place[1][1] = up_point[1];

	down_place[2][0] = up_point[0] + 2;
	down_place[2][1] = up_point[1];

	down_place[3][0] = up_point[0] + 2;
	down_place[3][1] = up_point[1] + (1) * bias;

	down_place[4][0] = up_point[0] + 3;
	down_place[4][1] = up_point[1] + (1) * bias;

	return 0;
}

int next_bridge_move(board* b, int next_bridge[NEXTBRIDGENUM][2]) {
	int i, j;
	int tile_color_num;
	if (b->color == WHITE) {
		tile_color_num = WHITE;
	}
	else if (b->color == BLACK)
	{
		tile_color_num = BLACK;
	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (b->bridge[i][j] == 0 && b->tile[i][j] == tile_color_num) {
				int bridge_num = b->bridge[i][j];

				if (i > 1 && b->bridge[i - 2][j] == bridge_num) {
					b->prohibit_tile[i - 1][j] = 1;
				}
				else if (i > 1 && j > 0 && b->bridge[i - 2][j - 1] == bridge_num) {
					b->prohibit_tile[i - 1][j] = 1;
					b->prohibit_tile[i - 1][j - 1] = 1;
				}
				else if (i > 1 && j > 1 && b->bridge[i - 2][j - 2] == bridge_num) {
					b->prohibit_tile[i - 1][j - 1] = 1;
				}
				else if (i > 0 && j > 1 && b->bridge[i - 1][j - 2] == bridge_num) {
					b->prohibit_tile[i - 1][j - 1] = 1;
					b->prohibit_tile[i][j - 1] = 1;
				}
				else if (j > 1 && b->bridge[i][j - 2] == bridge_num) {
					b->prohibit_tile[i][j - 1] = 1;
				}
				else if (i < 9 && j > 1 && b->bridge[i + 1][j - 2] == bridge_num) {
					b->prohibit_tile[i + 1][j - 1] = 1;
					b->prohibit_tile[i][j - 1] = 1;
				}
				else if (i < 8 && j > 1 && b->bridge[i + 2][j - 2] == bridge_num) {
					b->prohibit_tile[i + 1][j - 1] = 1;
				}
				else if (i < 8 && j > 0 && b->bridge[i + 2][j - 1] == bridge_num) {
					b->prohibit_tile[i + 1][j - 1] = 1;
					b->prohibit_tile[i + 1][j] = 1;
				}
				else if (i < 8 && b->bridge[i + 2][j] == bridge_num) {
					b->prohibit_tile[i + 1][j] = 1;
				}
				else if (i < 8 && j < 9 && b->bridge[i + 2][j + 1] == bridge_num) {
					b->prohibit_tile[i + 1][j] = 1;
					b->prohibit_tile[i + 1][j + 1] = 1;
				}
				else if (i < 8 && j < 8 && b->bridge[i + 2][j + 2] == bridge_num) {
					b->prohibit_tile[i + 1][j + 1] = 1;
				}
				else if (i < 9 && j < 8 && b->bridge[i + 1][j + 2] == bridge_num) {
					b->prohibit_tile[i + 1][j + 1] = 1;
					b->prohibit_tile[i][j + 1] = 1;
				}
				else if (j < 8 && b->bridge[i][j + 2] == bridge_num) {
					b->prohibit_tile[i][j + 1] = 1;

				}
				else if (i > 0 && j < 8 && b->bridge[i - 1][j + 2] == bridge_num) {
					b->prohibit_tile[i][j + 1] = 1;
					b->prohibit_tile[i - 1][j + 1] = 1;
				}
				else if (i > 1 && j < 8 && b->bridge[i - 2][j + 2] == bridge_num) {
					b->prohibit_tile[i - 1][j + 1] = 1;

				}
				else if (i > 1 && j < 9 && b->bridge[i - 2][j + 1] == bridge_num) {
					b->prohibit_tile[i - 1][j + 1] = 1;
					b->prohibit_tile[i - 1][j] = 1;
				}

			}
		}
	}
}

int next_tile_move(board* b, int x, int y) {
	// return 1 for success

	if (!b->computed_prohibit_tile) {
		mark_prohibit_tile(b);
	}
	if (b->prohibit_tile[y][x] == 1) {
		return 0;
	}
	else
	{
		b->tile[y][x] = b->color;
		b->computed_prohibit_tile = 0;
		mark_prohibit_tile(b);
		return 1;
	}
}

int next_move(board* b, int next_tile[NEXTMOVENUM][10][10], int next_bridge[NEXTMOVENUM][10][10]) {
	int i, j;
	for (i = 0; i < NEXTMOVENUM; i++) {
		memcpy(next_tile[i], init_board_content, sizeof(init_board_content));
		memcpy(next_bridge[i], init_board_content, sizeof(init_board_content));
	}

	// record current index of next move
	int m=0;
	// generate tile move

	// first tile
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			board temp_b = *b;
			int result1 = next_tile_move(&temp_b, i, j);
			if (result1) {

				// second tile
				int i2, j2;
				for (i2 = 0; i2 < 10; i2++) {
					for (j2 = 0; j2 < 10; j2++) {
						board temp_b2 = temp_b;
						int result2 = next_tile_move(&temp_b2, i2, j2);
						if (result2) {
							if (m < NEXTMOVENUM) {
								memcpy(next_tile[m], temp_b2.tile, sizeof(temp_b2.tile));
								m++;
								// print_board(&temp_b2);
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	// generate bridge move
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			board temp_b = *b;
			if (temp_b.bridge[i][j] == 0 && temp_b.tile[i][j] == temp_b.color) {
				int bridge_num = temp_b.bridge[i][j];

				if (i > 1 && temp_b.bridge[i - 2][j] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j] = 1;
				}
				else if (i > 1 && j > 0 && temp_b.bridge[i - 2][j - 1] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j] = 1;
					temp_b.prohibit_tile[i - 1][j - 1] = 1;
				}
				else if (i > 1 && j > 1 && temp_b.bridge[i - 2][j - 2] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j - 1] = 1;
				}
				else if (i > 0 && j > 1 && temp_b.bridge[i - 1][j - 2] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j - 1] = 1;
					temp_b.prohibit_tile[i][j - 1] = 1;
				}
				else if (j > 1 && temp_b.bridge[i][j - 2] == bridge_num) {
					temp_b.prohibit_tile[i][j - 1] = 1;
				}
				else if (i < 9 && j > 1 && temp_b.bridge[i + 1][j - 2] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j - 1] = 1;
					temp_b.prohibit_tile[i][j - 1] = 1;
				}
				else if (i < 8 && j > 1 && temp_b.bridge[i + 2][j - 2] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j - 1] = 1;
				}
				else if (i < 8 && j > 0 && temp_b.bridge[i + 2][j - 1] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j - 1] = 1;
					temp_b.prohibit_tile[i + 1][j] = 1;
				}
				else if (i < 8 && temp_b.bridge[i + 2][j] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j] = 1;
				}
				else if (i < 8 && j < 9 && temp_b.bridge[i + 2][j + 1] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j] = 1;
					temp_b.prohibit_tile[i + 1][j + 1] = 1;
				}
				else if (i < 8 && j < 8 && temp_b.bridge[i + 2][j + 2] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j + 1] = 1;
				}
				else if (i < 9 && j < 8 && temp_b.bridge[i + 1][j + 2] == bridge_num) {
					temp_b.prohibit_tile[i + 1][j + 1] = 1;
					temp_b.prohibit_tile[i][j + 1] = 1;
				}
				else if (j < 8 && temp_b.bridge[i][j + 2] == bridge_num) {
					temp_b.prohibit_tile[i][j + 1] = 1;

				}
				else if (i > 0 && j < 8 && temp_b.bridge[i - 1][j + 2] == bridge_num) {
					temp_b.prohibit_tile[i][j + 1] = 1;
					temp_b.prohibit_tile[i - 1][j + 1] = 1;
				}
				else if (i > 1 && j < 8 && temp_b.bridge[i - 2][j + 2] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j + 1] = 1;

				}
				else if (i > 1 && j < 9 && temp_b.bridge[i - 2][j + 1] == bridge_num) {
					temp_b.prohibit_tile[i - 1][j + 1] = 1;
					temp_b.prohibit_tile[i - 1][j] = 1;
				}

			}
		}
	}

	return -1;
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
	//mark_prohibit_by_tile(&a);
	int next_tile[NEXTMOVENUM][10][10] = { 0 };
	int next_bridge[NEXTMOVENUM][10][10] = { 0 };
	next_move(&a, next_tile, next_bridge);

	printf("count donw\n");
	print_board(&a);
	return 0;
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

