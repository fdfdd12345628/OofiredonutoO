#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int total_node = 0;
int out_of_time_node = 0;

#define DEPTH 8
#define INF 0x7FFFFFFF
#define NEGINF 0x80000000

//moveable chart : x[0] for minion, x[1] for us
//Rabbit index : 1
const int Rabbit[2][5][5] = { 
	0,0,0,0,0,
	0,0,0,1,0,
	0,1,0,0,0,
	0,0,0,1,0,
	0,0,0,0,0,-1 };
//Rooster index : 2
const int Rooster[2][5][5] = {
	0,0,0,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,1,1,0,
	0,0,0,0,0,-1
};
//Sheep index : 3
const int Sheep[2][5][5] = {
	0,0,0,0,0,
	0,1,0,1,0,
	0,0,0,0,0,
	0,1,0,1,0,
	0,0,0,0,0,-1
};
//Snake index : 4
const int Snake[2][5][5] = {
	0,0,0,0,0,
	0,1,0,0,0,
	0,0,0,1,0,
	0,1,0,0,0,
	0,0,0,0,0,-1
};
//Tiger index : 5
const int Tiger[2][5][5] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,1,
	0,0,0,0,0,
	0,0,0,0,0,-1
};
//Ox index : 6
const int Ox[2][5][5] = {
	0,0,0,0,0,
	0,0,1,0,0,
	0,0,0,1,0,
	0,0,1,0,0,
	0,0,0,0,0,-1
};
//Boar index : 7
const int Boar[2][5][5] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,1,0,1,0,
	0,0,0,0,0,
	0,0,0,0,0,-1
};
//Dog index : 8
const int Dog[2][5][5] = {
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,-1
};
//Dragon index : 9
const int Dragon[2][5][5] = {
	0,0,0,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,1,1,0,
	0,0,0,0,0,-1
};
//Horse index : 10
const int Horse[2][5][5] = {
	0,0,0,0,0,
	0,0,0,1,0,
	0,0,0,1,0,
	0,0,0,1,0,
	0,0,0,0,0,-1
};
//Monkey index : 11
const int Monkey[2][5][5] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,-1
};
//Mouse index : 12
const int Mouse[2][5][5] = {
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,1,0,
	0,0,0,0,0,
	0,0,1,0,0,-1
};
//Ostrich index : 13
const int Ostrich[2][5][5] = {
	0,0,0,0,1,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,1,

	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,1,0,
	0,0,0,0,0,
	0,0,0,0,0,
};
//Frog index : 14
const int Frog[2][5][5] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,1,0,
	0,0,0,0,0,
	0,0,0,0,0,

	0,0,0,1,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,1,0
};
//Turtle index : 15
const int Turtle[2][5][5] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,1,0,
	0,0,0,0,0,
	0,0,0,0,0,

	0,0,0,0,0,
	0,0,0,0,0,
	0,1,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0
};

int neg_one[2][40][2][2];

typedef struct Game 
{
	int board[5][5];//1 = us minion, 2 = us, -1 = enemy minion, -2 = enemy
	int current_player;//1 = us, -1 = enemy
	int our_card[2];
	int wild_card;
	int enemy_card[2];
}game_body;

char* readRepository(char *filename)
{
	/*
	* Input: absolute file path (file name if it is in the same directory with the file)
	* Ouput: contents of the file
	* Read the whole content of a specified file with its name and return the content
	* as a string pointer
	*
	*
	*/
	FILE *fp;
	char temp[1000];
	char *return_string = "";
	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Can't open file\n");
	}

	char *temp3 = "";
	while (fgets(temp, 1000, fp) != NULL)
	{
		return_string = malloc((strlen(temp) + strlen(return_string)) * sizeof(char));
		strcpy(return_string, temp3);
		temp3 = malloc((strlen(temp) + strlen(temp3)) * sizeof(char));
		strcat(return_string, temp);
		strcpy(temp3, return_string);
	}

	fclose(fp);
	return return_string;
}

int name_to_index(char* name) {
	if (!strcmp(name, "Rabbit")) return 1;
	else if(!strcmp(name, "Rooster")) return 2;
	else if (!strcmp(name, "Sheep")) return 3;
	else if (!strcmp(name, "Snake")) return 4;
	else if (!strcmp(name, "Tiger")) return 5;
	else if (!strcmp(name, "Ox")) return 6;
	else if (!strcmp(name, "Boar")) return 7;
	else if (!strcmp(name, "Dog")) return 8;
	else if (!strcmp(name, "Dragon")) return 9;
	else if (!strcmp(name, "Horse")) return 10;
	else if (!strcmp(name, "Monkey")) return 11;
	else if (!strcmp(name, "Mouse")) return 12;
	else if (!strcmp(name, "Ostrich")) return 13;
	else if (!strcmp(name, "Frog")) return 14;
	else if (!strcmp(name, "Turtle")) return 15;
	else return 0;
}

char* index_to_name(int index, char* name) {
	switch (index)
	{
	case(1):
		strcpy(name, "Rabbit");
		break;
	case(2):
		strcpy(name, "Rooster");
		break;
	case(3):
		strcpy(name, "Sheep");
		break;
	case(4):
		strcpy(name, "Snake");
		break;
	case(5):
		strcpy(name, "Tiger");
		break;
	case(6):
		strcpy(name, "Ox");
		break;
	case(7):
		strcpy(name, "Boar");
		break;
	case(8):
		strcpy(name, "Dog");
		break;
	case(9):
		strcpy(name, "Dragon");
		break;
	case(10):
		strcpy(name, "Horse");
		break;
	case(11):
		strcpy(name, "Monkey");
		break;
	case(12):
		strcpy(name, "Mouse");
		break;
	case(13):
		strcpy(name, "Ostrich");
		break;
	case(14):
		strcpy(name, "Frog");
		break;
	case(15):
		strcpy(name, "Turtle");
		break;
	default:
		strcpy(name, "");
		break;
	}
	return name;
}

int* index_to_area(int index, int area[2][5][5]) {
	int temp[2][5][5] = { 0 };
	int *destiantion = temp;
	switch (index)
	{
	case(1):
		destiantion = Rabbit;
		break;
	case(2):
		destiantion = Rooster;
		break;
	case(3):
		destiantion = Sheep;
		break;
	case(4):
		destiantion = Snake;
		break;
	case(5):
		destiantion = Tiger;
		break;
	case(6):
		destiantion = Ox;
		break;
	case(7):
		destiantion = Boar;
		break;
	case(8):
		destiantion = Dog;
		break;
	case(9):
		destiantion = Dragon;
		break;
	case(10):
		destiantion = Horse;
		break;
	case(11):
		destiantion = Monkey;
		break;
	case(12):
		destiantion = Mouse;
		break;
	case(13):
		destiantion = Ostrich;
		break;
	case(14):
		destiantion = Frog;
		break;
	case(15):
		destiantion = Turtle;
		break;
	default:
		break;
	}
	int i;
	memcpy(area, destiantion, 2 * 5 * 5 * sizeof(*destiantion));
	/*for (i = 0; i < 50; i++) {
		area[i / 50][i / 5][i % 5] = *(destiantion + i);
		/*area[i / 50][i / 5][i % 5] = *(
			destiantion + sizeof(*destiantion) * 50 * (i / 50) +
			sizeof(*destiantion)*(i / 5) +
			sizeof(*destiantion)*(i % 5));
	}*/
	return area;
}

void initial(struct Game* game, char* file_name) {
	if (file_name == NULL) {//UI, training mode
		int temp_board[5][5] = {
			1,0,0,0,-1,
			1,0,0,0,-1,
			2,0,0,0,-2,
			1,0,0,0,-1,
			1,0,0,0,-1
		};
		int i;
		for (i = 0; i < 25; i++) {
			game->board[i / 5][i % 5] = temp_board[i / 5][i % 5];
		}
		int All_card[] = {
			1,2,3,4,5,
			6,7,8,9,10,
			11,12,13,14,15
		};
		srand(time(NULL));//random five card
		//our card
		for (i = 0; i < 2; i++) {
			int temp_card = rand() % 15;//random card
			if (All_card[temp_card] > 0) {//if the card is not used
				game->our_card[i] = temp_card + 1;//*(game).our_card[i]
				All_card[temp_card] = -1;
			}
			else {
				i--;
			}
		}
		//wild card
		for (i = 0; i < 1; i++) {
			int temp_card = rand() % 15;//random card
			if (All_card[temp_card] > 0) {//if the card is not used
				game->wild_card = temp_card + 1;
				All_card[temp_card] = -1;
			}
			else {
				i--;
			}
		}
		//enemy card
		for (i = 0; i < 2; i++) {
			int temp_card = rand() % 15;//random card
			if (All_card[temp_card] > 0) {//if the card is not used
				game->enemy_card[i] = temp_card + 1;
				All_card[temp_card] = -1;
			}
			else {
				i--;
			}
		}
		game->current_player = 1;//our first
	}
	else {//battle mode
		int temp_board[5][5] = { 0 };
		int i;
		for (i = 0; i < 25; i++) {
			game->board[i / 5][i % 5] = temp_board[i / 5][i % 5];
		}
		//start prasing file
		char* original;

		original = readRepository(file_name);
		char file_content[100][100];
		int file_index = 0;
		int each_char = 0;
		file_index = 0;
		int line = 0;
		for (;; each_char++) {
			if (*(original + sizeof(*original)*file_index) == '\n') {
				file_content[line][each_char] = '\0';
				each_char = -1;
				file_index++;
				line++;
				continue;
			}
			else if (*(original + sizeof(*original)*file_index) == '\0') {
				file_content[line][each_char] = '\0';
				break;
			}
			else if (*(original + sizeof(*original)*file_index) == '\r') {
				file_index++;
				continue;
			}
			else {
				file_content[line][each_char] = *(original + sizeof(*original)*file_index);
				file_index++;
				continue;
			}
		}
		//end of prasing file
		for (i = 0; i < 8; i++) {
			printf("%s\n", file_content[i]);
		}

		int player = 0;
		if (strcmp(file_content[0] + sizeof(file_content[0][0])*7, "Red")==0) {
			player = 1;
		}
		else {
			player = -1;
		}
		//prasing RedCard
		int original_card[3][2] = { 0 };//[0] for red, [1] for black, [2] for wild
		char temp[2][20] = { 0 };
		int char_index = 0;
		int card_index = 0;
		int index;
		if (file_content[2][0] != 'R') index = 9;
		else index = 8;
		for (;; index++) {//red card
			if (*(file_content[2] + index) == ',') {
				temp[card_index][char_index] = '\0';
				card_index++;
				char_index = 0;
				continue;
			}
			if (*(file_content[2] + index) == '\0') {
				temp[card_index][char_index] = '\0';
				break;
			}
			else {
				temp[card_index][char_index] = *(file_content[2] + index);
				char_index++;
			}
		}
		original_card[0][0] = name_to_index(temp[0]);
		original_card[0][1] = name_to_index(temp[1]);
		//end of red card

		char_index = 0;
		card_index = 0;
		if (file_content[3][0] != 'B') index=11;
		else index = 10;
		for (;; index++) {//black card
			
			if (*(file_content[3] + index) == ',') {
				temp[card_index][char_index] = '\0';
				card_index++;
				char_index = 0;
				continue;
			}
			if (*(file_content[3] + index) == '\0') {
				temp[card_index][char_index] = '\0';
				break;
			}
			else {
				temp[card_index][char_index] = *(file_content[3] + index);
				char_index++;
			}
		}
		original_card[1][0] = name_to_index(temp[0]);
		original_card[1][1] = name_to_index(temp[1]);
		//end of black card

		original_card[2][0] = name_to_index(file_content[4] + 11);//public card

		//putting card
		if (player == 1) {
			game->our_card[0] = original_card[0][0];
			game->our_card[1] = original_card[0][1];
			game->enemy_card[0] = original_card[1][0];
			game->enemy_card[1] = original_card[1][1];
			game->wild_card = original_card[2][0];
		}
		else {
			game->our_card[0] = original_card[1][0];
			game->our_card[1] = original_card[1][1];
			game->enemy_card[0] = original_card[0][0];
			game->enemy_card[1] = original_card[0][1];
			game->wild_card = original_card[2][0];
		}
		//end of putting card

		//[0] for red, [n][0] for main_pawn , [n][n][0] for x
		int temp_place[2][6][2];
		for (i = 0; i < 12; i++) {
			temp_place[0][i / 2][i % 2] = -1;
			temp_place[1][i / 2][i % 2] = -1;
		}
		if (file_content[5][0] != 'R') {
			temp_place[0][0][0] = *(file_content[5] + 13) - '1';
			temp_place[0][0][1] = *(file_content[5] + 15) - '1';
		}
		else {
			temp_place[0][0][0] = *(file_content[5] + 12) - '1';
			temp_place[0][0][1] = *(file_content[5] + 14) - '1';
		}
		if (file_content[7][0] != 'B') {
			temp_place[1][0][0] = *(file_content[7] + 15) - '1';
			temp_place[1][0][1] = *(file_content[7] + 17) - '1';
		}
		else {
			temp_place[1][0][0] = *(file_content[7] + 14) - '1';
			temp_place[1][0][1] = *(file_content[7] + 16) - '1';
		}
		
		char_index = 0;
		for (index = 1; index < 6;) {
			if (*(file_content[6] + char_index) == '(') {
				temp_place[0][index][0] = *(file_content[6] + char_index + 1) - '1';
				temp_place[0][index][1] = *(file_content[6] + char_index + 3) - '1';
				char_index++;
				index++;
			}
			else if (*(file_content[6] + char_index) == '\0') break;
			char_index++;
		}
		char_index = 0;
		for (index = 1; index < 6;) {
			if (*(file_content[8] + char_index) == '(') {
				temp_place[1][index][0] = *(file_content[8] + char_index + 1) - '1';
				temp_place[1][index][1] = *(file_content[8] + char_index + 3) - '1';
				char_index++;
				index++;
			}
			else if (*(file_content[8] + char_index) == '\0') 
				break;
			char_index++;
		}
		game->board[temp_place[0][0][0]][temp_place[0][0][1]] = 2*player;
		for (i = 1; i < 6; i++) {
			if (temp_place[0][i][0] == -1)break;
			game->board[temp_place[0][i][0]][temp_place[0][i][1]] = 1*player;
		}
		game->board[temp_place[1][0][0]][temp_place[1][0][1]] = -2*player;
		for (i = 1; i < 6; i++) {
			if (temp_place[1][i][0] == -1)break;
			game->board[temp_place[1][i][0]][temp_place[1][i][1]] = -1*player;
		}
		game->current_player = 1;
		//free(original);
	}
}

void print_board(const struct Game game) {
	int i, j;
	for (i = 0; i < 25; i++) {
		if (game.board[i % 5][4 - (i / 5)] < 0)
			printf("%d ", game.board[i % 5][4 - (i / 5)]);
		else
			printf(" %d ", game.board[i % 5][4 - (i / 5)]);
		if (i % 5 == 4) printf("\n");
	}
	char card_name[5][20] = { '\0' };//0 & 1 for us, 2 for wlid, 3 & 4 for enemy
	int all_card[5];
	all_card[0] = game.our_card[0];
	all_card[1] = game.our_card[1];
	all_card[2] = game.wild_card;
	all_card[3] = game.enemy_card[0];
	all_card[4] = game.enemy_card[1];
	for (i = 0; i < 5; i++) {
		char temp_name[20];
		index_to_name(all_card[i], temp_name);
		strcpy(card_name[i], temp_name);
	}
	printf("Our card : %s , %s\n", card_name[0], card_name[1]);
	printf("Wild card : %s\n", card_name[2]);
	printf("Enemy card : %s , %s\n", card_name[3], card_name[4]);
	for (i = 0; i < 5; i++) {
		char temp_name[20];
		index_to_name(all_card[i], temp_name);
		strcpy(card_name[i], temp_name);
		printf("%s\n", temp_name);
		int area[2][5][5] = { 0 };
		index_to_area(all_card[i], area);
		if (area[1][0][0] == -1) {
			for (j = 0; j < 25; j++) {
				printf("%d ", area[0][j % 5][4 - (j / 5)]);
				if (j % 5 == 4) printf("\n");
			}
		}
		else
		{
			printf("For minion :\n");
			for (j = 0; j < 25; j++) {//minion
				printf("%d ", area[0][j % 5][4 - (j / 5)]);
				if (j % 5 == 4) printf("\n");
			}
			printf("For main pawn :\n");
			for (j = 0; j < 25; j++) {//main pawn
				printf("%d ", area[1][j % 5][4 - (j / 5)]);
				if (j % 5 == 4) printf("\n");
			}
		}
		printf("\n");
	}
}

void print_board_min(const game_body game) {
	int i, j;
	for (i = 0; i < 25; i++) {
		if (game.board[i % 5][4 - (i / 5)] < 0)
			printf("%d ", game.board[i % 5][4 - (i / 5)]);
		else
			printf(" %d ", game.board[i % 5][4 - (i / 5)]);
		if (i % 5 == 4) printf("\n");
	}
}

void change_player(struct Game *original) {
	game_body temp_game = *original;
	int temp[5][5];
	int i;
	for (i = 0; i < 25; i++) {
		temp[i / 5][i % 5] = original->board[4 - (i / 5)][4 - (i % 5)];
	}
	memcpy(original->board, temp, 5 * 5 * sizeof(**temp));
	/*for (i = 0; i < 25; i++) {
		original->board[i / 5][i % 5] = temp[i / 5][i % 5];
	}*/
	original->current_player = -temp_game.current_player;
	original->enemy_card[0] = temp_game.our_card[0];
	original->enemy_card[1] = temp_game.our_card[1];
	original->our_card[0] = temp_game.enemy_card[0];
	original->our_card[1] = temp_game.enemy_card[1];
	original->wild_card = temp_game.wild_card;
}

int* all_move(const game_body game, int able[40][2][2], int card) {//[n][0] for x, [n][1] for y, [0] for initial, [1] for final
	int index_eat = 0, index_no_eat = 0;
	//int area[2][5][5];
	//index_to_area(card, area);
	int player = game.current_player;
	int i, x, y, cardx, cardy;
	int eat[40][2][2];
	int no_eat[40][2][2];
	if (card != 15) {
		//able_index/5=y, able_index%5=x
		int able_index[4];// [x / 5][x % 5] count total x
		int total_able;
		int special;//special = minion, out of special = main pawn
		switch (card) {
		case 1:
			able_index[0] = 7;
			able_index[1] = 16;
			able_index[2] = 18;
			total_able = 3;
			special = 0;
			break;
		case 2:
			able_index[0] = 6;
			able_index[1] = 11;
			able_index[2] = 13;
			able_index[3] = 18;
			total_able = 4;
			special = 0;
			break;
		case 3:
			able_index[0] = 6;
			able_index[1] = 8;
			able_index[2] = 16;
			able_index[3] = 18;
			total_able = 4;
			special = 0;
			break;
		case 4:
			able_index[0] = 6;
			able_index[1] = 8;
			able_index[2] = 17;
			total_able = 3;
			special = 0;
			break;
		case 5:
			able_index[0] = 6;
			total_able = 1;
			special = 0;
			break;
		case 6:
			able_index[0] = 11;
			able_index[1] = 13;
			able_index[2] = 17;
			total_able = 3;
			special = 0;
			break;
		case 7:
			able_index[0] = 7;
			able_index[1] = 17;
			total_able = 2;
			special = 0;
			break;
		case 8:
			able_index[0] = 6;
			able_index[1] = 11;
			able_index[2] = 16;
			total_able = 3;
			special = 0;
			break;
		case 9:
			able_index[0] = 11;
			able_index[1] = 13;
			able_index[2] = 16;
			able_index[3] = 18;
			total_able = 4;
			special = 0;
			break;
		case 10:
			able_index[0] = 16;
			able_index[1] = 17;
			able_index[2] = 18;
			total_able = 3;
			special = 0;
			break;
		case 11:
			able_index[0] = 8;
			able_index[1] = 13;
			able_index[2] = 18;
			total_able = 3;
			special = 0;
			break;
		case 12:
			able_index[0] = 10;
			able_index[1] = 14;
			able_index[2] = 17;
			total_able = 3;
			special = 0;
			break;
		case 13:
			able_index[0] = 20;
			able_index[1] = 24;
			able_index[2] = 17;
			total_able = 3;
			special = 2;
			break;
		case 14:
			able_index[0] = 17;
			able_index[1] = 15;
			able_index[2] = 19;
			total_able = 3;
			special = 1;
			break;
		default:
			total_able = 0;
			break;
		}
		if(!special){
			for (i = 0; i < 25; i++) {
				if (!game.board[i / 5][i % 5])continue;
				if (game.board[i / 5][i % 5] * player > 0) {
					for (x = 0; x < total_able; x++) {
						int temp_x = i / 5 + (able_index[x] % 5 - 2);
						int temp_y = i % 5 + (able_index[x] / 5 - 2);
						if (temp_x < 0 || temp_x>4 || temp_y < 0 || temp_y>4) continue;
						if (game.board[temp_x][temp_y] * player > 0)continue;
						//if eat_able
						if (game.board[temp_x][temp_y]) {
							eat[index_eat][0][0] = i / 5;
							eat[index_eat][0][1] = i % 5;
							eat[index_eat][1][0] = temp_x;
							eat[index_eat][1][1] = temp_y;
							index_eat++;
						}
						//if cant eat
						else {
							no_eat[index_no_eat][0][0] = i / 5;
							no_eat[index_no_eat][0][1] = i % 5;
							no_eat[index_no_eat][1][0] = temp_x;
							no_eat[index_no_eat][1][1] = temp_y;
							index_no_eat++;
						}
					}
				}
			}
		}
		else {
			for (i = 0; i < 25; i++) {
				if (!game.board[i / 5][i % 5])continue;
				if (game.board[i / 5][i % 5] * player == 1) {
					for (x = 0; x < special; x++) {
						int temp_x = i / 5 + (able_index[x] % 5 - 2);
						int temp_y = i % 5 + (able_index[x] / 5 - 2);
						if (temp_x < 0 || temp_x>4 || temp_y < 0 || temp_y>4) continue;
						if (game.board[temp_x][temp_y] * player > 0)continue;
						if (game.board[temp_x][temp_y] * player > 0)continue;
						//if eat_able
						if (game.board[temp_x][temp_y]) {
							eat[index_eat][0][0] = i / 5;
							eat[index_eat][0][1] = i % 5;
							eat[index_eat][1][0] = temp_x;
							eat[index_eat][1][1] = temp_y;
							index_eat++;
						}
						//if cant eat
						else {
							no_eat[index_no_eat][0][0] = i / 5;
							no_eat[index_no_eat][0][1] = i % 5;
							no_eat[index_no_eat][1][0] = temp_x;
							no_eat[index_no_eat][1][1] = temp_y;
							index_no_eat++;
						}
					}
				}
				else if (game.board[i / 5][i % 5] * player == 2) {
					for (x = special; x < total_able; x++) {
						int temp_x = i / 5 + (able_index[x] % 5 - 2);
						int temp_y = i % 5 + (able_index[x] / 5 - 2);
						if (temp_x < 0 || temp_x>4 || temp_y < 0 || temp_y>4) continue;
						if (game.board[temp_x][temp_y] * player > 0)continue;
						if (game.board[temp_x][temp_y] * player > 0)continue;
						//if eat_able
						if (game.board[temp_x][temp_y]) {
							eat[index_eat][0][0] = i / 5;
							eat[index_eat][0][1] = i % 5;
							eat[index_eat][1][0] = temp_x;
							eat[index_eat][1][1] = temp_y;
							index_eat++;
						}
						//if cant eat
						else {
							no_eat[index_no_eat][0][0] = i / 5;
							no_eat[index_no_eat][0][1] = i % 5;
							no_eat[index_no_eat][1][0] = temp_x;
							no_eat[index_no_eat][1][1] = temp_y;
							index_no_eat++;
						}
					}
				}
			}
		}
	}
	else if (card == 15) {
		for (x = 0; x < 25; x++) {
			if (!game.board[x / 5][x % 5]) continue;
			if (game.board[x / 5][x % 5] * player == 2 && (x / 5) < 4) {
				if (game.board[x / 5][x % 5 + 1] == 0) {
					no_eat[index_no_eat][0][0] = x / 5;
					no_eat[index_no_eat][0][1] = x % 5;
					no_eat[index_no_eat][1][0] = x / 5;
					no_eat[index_no_eat][1][1] = x % 5 + 1;
					index_no_eat++;
				}
			}
			else if (game.board[x / 5][x % 5] * player == -2 && (x / 5) > 0) {
				no_eat[index_no_eat][0][0] = x / 5;
				no_eat[index_no_eat][0][1] = x % 5;
				no_eat[index_no_eat][1][0] = x / 5;
				no_eat[index_no_eat][1][1] = x % 5 - 1;
				index_no_eat++;
			}
		}
	}
	memcpy(able, eat, sizeof(*eat)*index_eat);
	memcpy(able + index_eat, no_eat, sizeof(*no_eat)*index_no_eat);
	//memcpy(able, no_eat, sizeof(*no_eat)*index_no_eat);
	/*for (x = 0; x < 5; x++) {
		for (y = 0; y < 5; y++) {
			if (game.board[x][y] * game.current_player == 1 && card != 15) {
				for (cardx = 0; cardx < 5; cardx++) {
					for (cardy = 0; cardy < 5; cardy++) {
						if (area[0][cardx][cardy]) {
							if (x + (cardx - 2) < 0 || x + (cardx - 2) > 4 || y + (cardy - 2) < 0 || y + (cardy - 2) > 4) {
								continue;
							}
							if (game.board[x + (cardx - 2)][y + (cardy - 2)] * game.current_player > 0) continue;
							able[index][0][0] = x;
							able[index][0][1] = y;
							able[index][1][0] = x + (cardx - 2);
							able[index][1][1] = y + (cardy - 2);
							index++;
						}
					}
				}

			}
			else if (game.board[x][y] * game.current_player == 2 && card != 15) {
				for (cardx = 0; cardx < 5; cardx++) {
					for (cardy = 0; cardy < 5; cardy++) {
						if (area[1][0][0] == -1) {
							//general condition
							if (area[0][cardx][cardy]) {
								if (x + (cardx - 2) < 0 || x + (cardx - 2) > 4 || y + (cardy - 2) < 0 || y + (cardy - 2) > 4) {
									continue;
								}
								if (game.board[x + (cardx - 2)][y + (cardy - 2)] * game.current_player > 0) continue;
								able[index][0][0] = x;
								able[index][0][1] = y;
								able[index][1][0] = x + (cardx - 2);
								able[index][1][1] = y + (cardy - 2);
								index++;
							}
						}
						else if (area[1][0][0] != -1) {
							//special card
							if (area[1][cardx][cardy]) {
								if (x + (cardx - 2) < 0 || x + (cardx - 2) > 4 || y + (cardy - 2) < 0 || y + (cardy - 2) > 4) {
									continue;
								}
								if (game.board[x + (cardx - 2)][y + (cardy - 2)] * game.current_player > 0) continue;
								able[index][0][0] = x;
								able[index][0][1] = y;
								able[index][1][0] = x + (cardx - 2);
								able[index][1][1] = y + (cardy - 2);
								index++;
							}
						}
					}
				}
			}
			else if (card == 15) {
				if (game.board[x][y] * game.current_player == -2) {
					if (y - 1 < 0) continue;
					if (game.board[x][y - 1] * game.current_player < 0) continue;
					able[index][0][0] = x;
					able[index][0][1] = y;
					able[index][1][0] = x;
					able[index][1][1] = y - 1;
					index++;
					continue;
				}
				for (cardx = 0; cardx < 5; cardx++) {
					for (cardy = 0; cardy < 5; cardy++) {
						if (game.board[x][y] * game.current_player > 0) {
							if (area[0][cardx][cardy]) {
								if (x + (cardx - 2) < 0 || x + (cardx - 2) > 4 || y + (cardy - 2) < 0 || y + (cardy - 2) > 4) {
									continue;
								}
								if (game.board[x + (cardx - 2)][y + (cardy - 2)] * game.board[x][y] > 0) continue;
								able[index][0][0] = x;
								able[index][0][1] = y;
								able[index][1][0] = x + (cardx - 2);
								able[index][1][1] = y + (cardy - 2);
								index++;
							}
						}
						
					}
				}
			}
		}
	}*/
	return able;
}

int* valid_move(const struct Game game, int *current, int *destination, int card) {
	int result[5][5] = { 0 };
	return NULL;
}

int win_game(const game_body game) {
	if (game.board[2][4] == 2) {
		return 1;
	}
	else if (game.board[2][0] == -2)
	{
		return -1;
	}
	int our_main = 0, enemy_main = 0;
	int i;
	for (i = 0; i < 25; i++) {
		switch (game.board[i/5][i%5])
		{
		case 2:
			our_main++;
		case -2:
			enemy_main++;
		default:
			break;
		}
		/*if (game.board[i / 5][i % 5] == 2) our_main = 1;
		else if (game.board[i / 5][i % 5] == -2)
		{
			enemy_main = 1;
		}*/
	}
	if (!our_main) return -1;
	else if (!enemy_main) return 1;
	return 0;
}

void player_move(struct Game* game) {
	char command[3][20];
	while (1) {
		//command initial
		int i, j;
		for (i = 0; i < 60; i++) {
			command[i / 20][i % 20] = '\0';
		}
		gets(command[0]);
		gets(command[1]);
		gets(command[2]);
		//check illegal, prasing command
		if (command[0][0] != 'U'|| command[1][0] != 'S'|| command[2][0] != 'E') {
			printf("Input again!\n");
			continue;
		}
		int selected_card = -1;
		//printf(command[0] + 4);
		if (name_to_index(command[0] + 4) == game->our_card[0]) selected_card = 0;
		else if (name_to_index(command[0] + 4) == game->our_card[1]) selected_card = 1;
		else {
			printf("Input again!\n");
			continue;
		}
		int start_end[2][2] = { 0 };//[n][0] for x, [n][1] for y, [0] for initial, [1] for final
		if (command[1][7] - '0' < 1 || command[1][7] - '0' > 5 || command[1][9] - '0' < 1 || command[1][9] - '0' > 5) {
			printf("Input again!\n");
			continue;
		}
		start_end[0][0] = command[1][7] - '0'-1;
		start_end[0][1] = command[1][9] - '0'-1;
		if (command[2][5] - '0' < 1 || command[2][5] - '0' > 5 || command[2][7] - '0' < 1 || command[2][7] - '0' > 5) {
			printf("Input again!\n");
			continue;
		}
		start_end[1][0] = command[2][5] - '0'-1;
		start_end[1][1] = command[2][7] - '0'-1;
		//end of parsing & checking

		//game_body* temp = game;
		int able[2][40][2][2];//[0] for first card, [1] for second card
		for (i = 0; i < 40; i++) {
			for (j = 0; j < 4; j++) {
				able[0][i][j / 2][j % 2] = -1;
				able[1][i][j / 2][j % 2] = -1;
			}
		}
		int card;
		for (card = 0; card < 2; card++) {
			all_move(*game, able[card], game->our_card[card]);
		}
		int possible=0;
		for (i = 0; i < 40; i++) {
			//if end of possible
			if (able[selected_card][i][0][0] == -1) break;
			int all_possible = 0;
			for (j = 0; j < 4; j++) {//check start & end
				if (able[selected_card][i][j / 2][j % 2] == start_end[j / 2][j % 2]) all_possible++;
			}
			if (all_possible == 4) {
				possible = 1;
				break;
			}
		}
		if (possible) {
			int role = game->board[start_end[0][0]][start_end[0][1]];
			game->board[start_end[0][0]][start_end[0][1]] = 0;
			game->board[start_end[1][0]][start_end[1][1]] = role;
			return;
		}
		else {
			printf("Can't move! Input again!\n");
			continue;
		}
	}
}

int value_of_game(const game_body game) {
	int win = 0;
	if (game.current_player == 1) {
		if (game.board[2][4] == 2) {
		win = 1;
		}
		else if (game.board[2][0] == -2)
		{
		win = -1;
		}
	}
	else {
		if (game.board[2][4] == -2) {
		win = -1;
		}
		else if (game.board[2][0] == 2)
		{
		win = 1;
		}
	}
	/*if (game.board[2][4] == 2) {
		win = 1;
	}
	else if (game.board[2][0] == -2)
	{
		win = -1;
	}*/
	switch (win) {
	case 1:
		return 100000;
	case -1:
		return -100000;
	default:
		break;
	}
	int our_main = 0, enemy_main = 0;
	int our_man = 0, enemy_man = 0;
	int i, j;
	//int current_player = game.current_player;
	int *p ;
	for (p = game.board; p < (&game.board)[1]; p++) {
		switch (*p)
		{
		case 2:
			our_main++;
			break;
		case -2:
			enemy_main++;
			break;
		case 1:
			our_man++;
			break;
		case -1:
			enemy_man++;
			break;
		default:
			break;
		}
	}
	if (!our_main) return -100000;
	else if (!enemy_main) return 100000;
	else if (our_man == enemy_man && (our_man + enemy_man) < 8) 
		return 100 * (8 - (our_man + enemy_man));
	else return 1000 * (our_man - enemy_man);
}

/*
01 function alphabeta(node, depth, alpha, beta, maximizingPlayer) is
02     if depth = 0 or node is a terminal node then
03         return the heuristic value of node
04     if maximizingPlayer then
05         v := -INF
06         for each child of node do
07             v := max(v, alphabeta(child, depth - 1, alpha, beta, FALSE))
08             alpha := max(alpha, v)
09             if beta <= alpha then
10                 break (* beta cut-off *)
11         return v
12     else
13         v := +INF
14         for each child of node do
15             v := min(v, alphabeta(child, depth - 1, alpha, beta, TRUE))
16             beta := min(beta, v)
17             if beta <= alpha then
18                 break (* alpha cut-off *)
19         return v
alphabeta(origin, depth, -INF, +INF, TRUE)
*/

int alpha_beta(game_body game, int depth, int alpha, int beta, int maximum_player) {
	total_node++;
	
	if (depth <= 0) {
		if (clock() / CLOCKS_PER_SEC > 3) {
			out_of_time_node++;
			return 0;
		}
		int value = value_of_game(game);
		return value;
		//if (value == 0) return value + rand() % 5 - 2;
		//return (value > 0) ? value + rand() % 10 : value - rand() % 10; /*- rand() % 3*/
	}
	
	int v = value_of_game(game);
	if (v == 100000) return v /*+ rand() % 5*/ + depth * 10 + rand() % 2;
	else if (v == -100000) return v /*- rand() % 5*/ - depth * 10 - rand() % 2;
	
	//int v = value_of_game(game);
	//if (v == 100 || v == -100) return v;
	//int v = value_of_game(game);
	//if (v >= 50 || v <= 50) 
	//	return v;
	//else if (v <= -50) return v;
	//out of time break
	
	int i=0, j=0, card = 0;

	change_player(&game);
	if (maximum_player) {
		//if (value_of_game(game) == 100) return 100;
		int value = NEGINF;
		int able[2][40][2][2];//[0] for first card, [1] for second card
		memcpy(able, neg_one, 2 * 40 * 2 * 2 * sizeof(able[0][0][0][0]));
		/*int *p = able;
		for (p = able; p < (&able)[1]; p++) {
			*p = -1;
		}
		for (i = 0; i < 40; i++) {
			for (j = 0; j < 4; j++) {
				able[0][i][j / 2][j % 2] = -1;
				able[1][i][j / 2][j % 2] = -1;
			}
		}*/
		for (card = 0; card < 2; card++) {
			all_move(game, able[card], game.our_card[card]);
		}
		/*//bug
		print_board_min(game);
		char name[20];
		printf("Use card %s\n", index_to_name(game.our_card[0], name));
		for (i = 0; i < 40; i++) {
			if (able[0][i][0][0] == -1) break;
			printf("Start (%d,%d), End (%d,%d)\n", able[0][i][0][0], able[0][i][0][1], able[0][i][1][0], able[0][i][1][1]);
		}
		printf("Use card %s\n", index_to_name(game.our_card[1], name));
		for (i = 0; i < 40; i++) {
			if (able[1][i][0][0] == -1) break;
			printf("Start (%d,%d), End (%d,%d)\n", able[1][i][0][0], able[1][i][0][1], able[1][i][1][0], able[1][i][1][1]);
		}
		*///bug
		for (card = 0; card < 2; card++) {
			int breaking = 0;
			for (i = 0; i < 40 ; i++) {
				if (able[card][i][0][0] == -1) break;
				game_body next_node = game;
				int temp;
				next_node.board[able[card][i][1][0]][able[card][i][1][1]] =
					game.board[able[card][i][0][0]][able[card][i][0][1]];
				next_node.board[able[card][i][0][0]][able[card][i][0][1]] = 0;
				//temp = game.wild_card;
				next_node.wild_card = game.our_card[card];
				next_node.our_card[card] = game.wild_card;
				//int v = value_of_game(next_node);
				//if (v >= 50 || v <= -50) 
				//	return v;
				//print_board(next_node);
				temp = alpha_beta(next_node, depth - 1, alpha, beta, 0);
				//v := max(v, alphabeta(child, depth - 1, alpha, beta, FALSE))
				//alpha := max(alpha, v)
				value = (temp > value) ? temp : value;
				alpha = (value > alpha) ? value : alpha;
				
				if (beta <= alpha) {
					breaking = 1;
					break;
				}
			}
			if (breaking) break;
		}
		//if (value == NEGINF) return NEGINF;
		return value;
	}
	else {
		//if (value_of_game(game) == -100) return -100;
		int value = INF;
		int able[2][40][2][2];//[0] for first card, [1] for second card
		memcpy(able, neg_one, 2 * 40 * 2 * 2 * sizeof(able[0][0][0][0]));
		/*int *p = able;
		for (p = able; p < (&able)[1]; p++) {
			*p = -1;
		}
		for (i = 0; i < 40; i++) {
			for (j = 0; j < 4; j++) {
				able[0][i][j / 2][j % 2] = -1;
				able[1][i][j / 2][j % 2] = -1;
			}
		}*/
		for (card = 0; card < 2; card++) {
			//printf("%x\n", able[card]);
			all_move(game, able[card], game.our_card[card]);
		}
		for (card = 0; card < 2; card++) {
			int breaking = 0;
			for (i = 0; i < 40; i++) {
				if (able[card][i][0][0] == -1) { 
					 
					break; 
				}
				game_body next_node = game;
				int temp;
				//temp = next_node.board[able[card][i][0][0]][able[card][i][0][1]];
				next_node.board[able[card][i][1][0]][able[card][i][1][1]] =
					game.board[able[card][i][0][0]][able[card][i][0][1]];
				next_node.board[able[card][i][0][0]][able[card][i][0][1]] = 0;
				next_node.wild_card = game.our_card[card];
				next_node.our_card[card] = game.wild_card;
				//int v = value_of_game(next_node);
				//if (v >= 50 || v <= -50) 
				//	return v;
				//print_board(next_node);
				temp = alpha_beta(next_node, depth - 1, alpha, beta, 1);
				value = (temp < value) ? temp : value;
				beta = (value < beta) ? value : beta;
				if (beta <= alpha) {
					breaking = 1;
					break;
				}
			}
			if (breaking) break;
		}
		if (DEPTH == depth) 
			printf("Node value : %d\n", value);
		//if (value == INF) return NEGINF;
		return value;
	}
}

void computer_move(struct Game* game) {
	
}

void UI();
void AI(int argc, char* args[]);
void test();

int main(int argc, char *argv[]) {
	//test();
	//total initial
	srand(time(NULL));
	int *p = neg_one;
	for (p = neg_one; p < (&neg_one)[1]; p++) {
		*p = -1;
	}
	if (argc == 0) {
		UI();
	}
	else {
		AI(argc, argv);
		printf("Total node : %d\n", total_node);
		printf("Total ignored node : %d\n", out_of_time_node);
		printf("Total time : %f\n", (float)clock() / CLOCKS_PER_SEC);
		//system("PAUSE");
		return 0;
	}
	//test();
	//UI();
	//argc = 4;
	return 0;
}

void UI() {
	printf(
		"Onitama start!\n"
		"Select mode : 1 for PvP, 2 for PvC, 3 for CvC\n");
	int mode=0;//1 for PvP, 2 for PvC, 3 for CvC
	scanf("%d", &mode);
	getchar();
	struct Game main_game;
	initial(&main_game,NULL);
	char input[20] = { 0 };
	if (mode == 1) {//PvP
		while (1) {
			print_board(main_game);
			int i;
			for (i = 0; i < 20; i++) {//input renew
				input[i] = 0;
			}
			player_move(&main_game);//first player
			change_player(&main_game);//change board
			int win = win_game(main_game);
			if (win) {
				printf("Player %d win!\n", win);
				return;
			}
		}
	}
	else if (mode == 2) {//PvC

	}
}

void AI(int argc, char* args[]) {
	game_body main_game;
	initial(&main_game, args[1]);
	print_board(main_game);
	
	//int move[2][2] = { -1,-1,-1,-1 };
	int value = NEGINF;
	int alpha = NEGINF;
	int beta = INF;
	int able[2][40][2][2];//[0] for first card, [1] for second card
	int i;
	int same_able[40][2];
	int total_same[2] = { 0,0 };//[0] for total number, [1] for score
	int total_player = 0, depth_reduce = 0;
	for (i = 0; i < 25; i++) {
		if (main_game.board[i / 5][i % 5] != 0) total_player++;
	}
	if (total_player <= 10 && total_player >= 6) {
		depth_reduce = 1;
	}
	/*else if (total_player <= 4) {
		depth_reduce = -1;
	}*/
	for (i = 0; i < 40; i++) {
		int j;
		for (j = 0; j < 4; j++) {
			able[0][i][j / 2][j % 2] = -1;
			able[1][i][j / 2][j % 2] = -1;
		}
	}
	int card;
	for (card = 0; card < 2; card++) {
		all_move(main_game, able[card], main_game.our_card[card]);
	}
	int best_index[2] = { 0 };//[0] for card, [1] for index
	int first = 0;
	//game_body enemy = main_game;
	//change_player(&enemy);
	for (card = 0; card < 2; card++) {
		int breaking = 0;
		for (i = 0; i < 40; i++) {
			if (able[card][i][0][0] == -1) break;
			
			game_body next_node = main_game;
			int temp = 0;
			//temp = main_game.board[able[card][i][0][0]][able[card][i][0][1]];
			next_node.board[able[card][i][1][0]][able[card][i][1][1]] =
				main_game.board[able[card][i][0][0]][able[card][i][0][1]];
			next_node.board[able[card][i][0][0]][able[card][i][0][1]] = 0;
			next_node.our_card[card] = main_game.wild_card;
			next_node.wild_card = main_game.our_card[card];
			print_board_min(next_node);

			temp = alpha_beta(next_node, DEPTH - depth_reduce, alpha, beta, 0);
			printf("temp : %d , value : %d\n", temp, value);
			/*if (i == 0) {
				if (temp > value && temp >= 0) {
					value = temp;
					best_index[0] = card;
					best_index[1] = i;
					printf("Card index : %d , best index move : (%d,%d), (%d,%d)\n", card, able[best_index[0]][best_index[1]][0][0], able[best_index[0]][best_index[1]][0][1], able[best_index[0]][best_index[1]][1][0], able[best_index[0]][best_index[1]][1][1]);
				}
				break;
			}*/
			if (temp > value) {
				total_same[0] = 0;
				total_same[1] = 0;
				int k;
				for (k = 0; k < 40; k++) {
					same_able[k][0] = -1;
					same_able[k][1] = -1;
				}
				//first = i;
				value = temp;
				alpha = (alpha > value) ? alpha : value;
				best_index[0] = card;
				best_index[1] = i;
				printf("Card index : %d , best index move : (%d,%d), (%d,%d)\n", card, able[best_index[0]][best_index[1]][0][0], able[best_index[0]][best_index[1]][0][1], able[best_index[0]][best_index[1]][1][0], able[best_index[0]][best_index[1]][1][1]);
				printf("best card index : %d\n\n", i);
				same_able[total_same[0]][0] = card;
				same_able[total_same[0]][1] = i;
				total_same[0]++;
				total_same[1] = temp;
			}
			else if (temp == value) {
				same_able[total_same[0]][0] = card;
				same_able[total_same[0]][1] = i;
				total_same[0]++;
			}
			/*int win = win_game(next_node);
			if (win > temp) {
				temp = win;
				best_index[0] = card;
				best_index[1] = i;
			}*/
			

		}
		if (breaking) break;
		//alpha_beta(main_game, DEPTH, NEGINF, INF, main_game.current_player);
		//system("PAUSE");
	}
	FILE *output_file = NULL;
	output_file = fopen(args[3], "w");
	char name[20] = { 0 };
	srand(time(NULL));
	if (total_same[0] > 1) {
		int select = rand() % total_same[0];
		best_index[0] = same_able[select][0];
		best_index[1] = same_able[select][1];
	}
	index_to_name(main_game.our_card[best_index[0]], name);
	printf("Best card index:%d, Best place index : %d\n", main_game.our_card[best_index[0]],best_index[1]);
	printf("Use %s\n", name);
	printf("Start (%d,%d)\n", able[best_index[0]][best_index[1]][0][0] + 1, able[best_index[0]][best_index[1]][0][1] + 1);
	printf("End (%d,%d)\n", able[best_index[0]][best_index[1]][1][0] + 1, able[best_index[0]][best_index[1]][1][1] + 1);

	//output file
	fprintf(output_file,"Use %s\n", name);
	fprintf(output_file, "Start (%d,%d)\n", able[best_index[0]][best_index[1]][0][0] + 1, able[best_index[0]][best_index[1]][0][1] + 1);
	fprintf(output_file, "End (%d,%d)\n", able[best_index[0]][best_index[1]][1][0] + 1, able[best_index[0]][best_index[1]][1][1] + 1);

}

void test() {
	int i;
	int c[10] = { 1,2,3,4,5,6,7,8,9,0 };
	int a[5];
	memcpy(a, c, sizeof(*a) * 5);
	for (i = 0; i < 5; i++) {
		printf("%d\n", a[i]);
	}
	game_body game;
	initial(&game, NULL);
	game.our_card[0] = 15;
	print_board(game);
	int able[2][40][2][2] = { 0 };
	
	for (i = 0; i < 40; i++) {
		int j;
		for (j = 0; j < 4; j++) {
			able[0][i][j / 2][j % 2] = -1;
			able[1][i][j / 2][j % 2] = -1;
		}
	}
	all_move(game, able[0], game.our_card[0]);
	all_move(game, able[1], game.our_card[1]);
	char name[40];
	printf("Use card %s\n", index_to_name(game.our_card[0], name));
	for (i = 0; i < 40; i++) {
		if (able[0][i][0][0] == -1) break;
		printf("Start (%d,%d), End (%d,%d)\n", able[0][i][0][0], able[0][i][0][1], able[0][i][1][0], able[0][i][1][1]);
	}
	printf("Use card %s\n", index_to_name(game.our_card[1], name));
	for (i = 0; i < 40; i++) {
		if (able[1][i][0][0] == -1) break;
		printf("Start (%d,%d), End (%d,%d)\n", able[1][i][0][0], able[1][i][0][1], able[1][i][1][0], able[1][i][1][1]);
	}
	printf("\n");
	change_player(&game);
	print_board(game);
	for (i = 0; i < 40; i++) {
		int j;
		for (j = 0; j < 4; j++) {
			able[0][i][j / 2][j % 2] = -1;
			able[1][i][j / 2][j % 2] = -1;
		}
	}
	all_move(game, able[0], game.our_card[0]);
	all_move(game, able[1], game.our_card[1]);
	//char name[40];
	printf("Use card %s\n", index_to_name(game.our_card[0], name));
	for (i = 0; i < 40; i++) {
		if (able[0][i][0][0] == -1) break;
		printf("Start (%d,%d), End (%d,%d)\n", able[0][i][0][0], able[0][i][0][1], able[0][i][1][0], able[0][i][1][1]);
	}
	printf("Use card %s\n", index_to_name(game.our_card[1], name));
	for (i = 0; i < 40; i++) {
		if (able[1][i][0][0] == -1) break;
		printf("Start (%d,%d), End (%d,%d)\n", able[1][i][0][0], able[1][i][0][1], able[1][i][1][0], able[1][i][1][1]);
	}
	printf("\n");
	game.board[2][0] = 0;
	printf("Win : %d\n", win_game(game));
	system("PAUSE");
}