// CardGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define MAX(a,b) ((a)<(b)?(b):(a))

// Enum definitions
enum PlayerType { SINGLE_PLAYER, DOUBLE_PLAYER };
enum CardTypes {
	CLUBS, DIAMONDS, HEARTS, SPADES
};
enum Cards {
	TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

// Type definitions
typedef struct Player {
	char *name = (char *)malloc(50);
	int score =0;
	int numberOfWins;
	int numberOfLosses;
	int currentSelectedCard[2];
};
typedef struct Game {
	PlayerType playerType;
	int numberOfRounds;
	int cardDeck[4][13];
};


// Varibale declarations
int playerChoice = -1, numberOfRounds;

// Method signatures
void mainMenu();
void selectPlayerMode(Game *);
void selectNumberOfRounds(Game *);
void populateCardDeck(Game *);
void shuffleCardDeck(Game *);
char * getCardChoice(Player *);
void addPlayerName(Player *, int);
void printCardDeck(Game *, bool);
char * getCardName(int, int, bool);
char * getGameResult(Player *, Player *);
void evaluateCardSelection(Game *, Player *, Player *);
void printStats(Game *, Player *, Player *);


// Method definitions
int main(int argc, char **argv)
{
	mainMenu();
	return 0;
}

void mainMenu()
{
	Game gameObject;
	Player player_1;
	Player player_2;

	selectPlayerMode(&gameObject);
	addPlayerName(&player_1, 1);
	if (gameObject.playerType == DOUBLE_PLAYER) {
		addPlayerName(&player_2, 2);
	}
	else
	{
		strcpy_s(player_2.name, 50, "Computer");
	}
	selectNumberOfRounds(&gameObject);
	//do {
	//
	//} while (gameObject.numberOfRounds > 0);
	getCardChoice(&player_1);
	getCardChoice(&player_2);
	evaluateCardSelection(&gameObject, &player_1, &player_2);
	printStats(&gameObject, &player_1, &player_2);
}

void selectPlayerMode(Game *gameObject)
{
	char input;
	bool shouldContinue = true;

	do {
		puts("\n\t*** Select player combination ***\n");
		puts("1. Player Vs Computer?");
		puts("2. Player Vs Player?");
		printf("\nINPUT >> ");

		scanf_s(" %c", &input);

		switch (input) {
		case '1':
			gameObject->playerType = SINGLE_PLAYER;
			shouldContinue = false;
			break;
		case '2':
			gameObject->playerType = DOUBLE_PLAYER;
			shouldContinue = false;
			break;
		default:
			puts("\n\t--- Invalid player type selection! ---");
			break;
		}
	} while (shouldContinue);
}

void selectNumberOfRounds(Game *gameObject)
{
	int input;
	bool shouldContinue = true;

	do {
		puts("\n\t*** Enter the number of rounds ***\n");
		printf("INPUT >> ");

		if (scanf_s(" %d", &input)) {
			gameObject->numberOfRounds = input;
			shouldContinue = false;
		}
		else {
			puts("\n\t--- Please enter a valid number! ---");
			while ((input = getchar()) != '\n' && input != EOF) {
				;//clear the buffer
			}
			input = -1;
		}
	} while (shouldContinue || input < 0 || input > 52);
}

void populateCardDeck(Game *gameObject)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			gameObject->cardDeck[i][j] = j;
		}
	}
}

void shuffleCardDeck(Game *gameObject)
{
	for (int k = 0; k < 4; k++)
	{
		size_t i;
		for (i = 0; i < 13; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (13 - i) + 1);
			int t = gameObject->cardDeck[k][j];
			gameObject->cardDeck[k][j] = gameObject->cardDeck[k][i];
			gameObject->cardDeck[k][i] = t;
		}
	}
}

char * getCardChoice(Player *player)
{
	char input[5];
	bool shouldContinue = true;
	int column, row;

	do {
		puts("\n\t*** Enter a row and a column in 2 digits (##) ***\n");
		printf("INPUT >> ");
		while ((*input = getchar()) != '\n' && *input != EOF) {
			;//clear the buffer
		}
		fgets(input, 5, stdin);
		input[strcspn(input, "\n")] = 0;
		row = input[0] - '0';
		column = input[1] - '0';
		if (column > 0 && column <= 13 && row > 0 && row <= 4) {
			player->currentSelectedCard[0] = row;
			player->currentSelectedCard[1] = column;
			shouldContinue = false;
		}
		else {
			puts("\n\t--- Please enter a valid number! ---");
		}

	} while (shouldContinue);
	return input;
}

void addPlayerName(Player *player, int index)
{
	char input[50];
	bool shouldContinue = true;

	do {
		printf("\n\t*** Add a name for Player: %d ***\n", index);
		printf("\nINPUT >> ");
		while ((*input = getchar()) != '\n' && *input != EOF) {
			;//clear the buffer
		}
		fgets(input, 50, stdin);
		input[strcspn(input, "\n")] = 0;
		strcpy_s(player->name, 50, input);
		shouldContinue = false;
	} while (shouldContinue);
}

void printCardDeck(Game *gameObject, bool hiddenFlag)
{
	printf("\n\t    ");
	for (int i = 1; i <= 13; i++) {
		if (i < 10) {
			printf(" _%d  ", i);
		}
		else {
			printf("_%d  ", i);
		}
	}
	puts("\n");
	for (int i = 0; i < 4; i++)
	{
		printf("\t _%d ", i + 1);
		for (int j = 0; j < 13; j++)
		{
			if (gameObject->cardDeck[i][j] == -1) {
				printf("|  | ", gameObject->cardDeck[i][j]);
			}
			else if (hiddenFlag) {
				printf("|##| ", gameObject->cardDeck[i][j]);
			}
			else {
				printf("|%s| ", getCardName(i, gameObject->cardDeck[i][j], false));
			}

		}
		printf("\n");
	}
	printf("\n");
}

char * getCardName(int cardType, int card, bool lengthyNamesFlag)
{
	char *cardName = (char *)malloc(50);

	switch (card)
	{
	case TWO:
		strcpy_s(cardName, 50, "2");
		break;
	case THREE:
		strcpy_s(cardName, 50, "3");
		break;
	case FOUR:
		strcpy_s(cardName, 50, "4");
		break;
	case FIVE:
		strcpy_s(cardName, 50, "5");
		break;
	case SIX:
		strcpy_s(cardName, 50, "6");
		break;
	case SEVEN:
		strcpy_s(cardName, 50, "7");
		break;
	case EIGHT:
		strcpy_s(cardName, 50, "8");
		break;
	case NINE:
		strcpy_s(cardName, 50, "9");
		break;
	case TEN:
		strcpy_s(cardName, 50, "T");
		break;
	case JACK:
		strcpy_s(cardName, 50, "J");
		break;
	case QUEEN:
		strcpy_s(cardName, 50, "Q");
		break;
	case KING:
		strcpy_s(cardName, 50, "K");
		break;
	case ACE:
		strcpy_s(cardName, 50, "A");
		break;
	default:
		strcpy_s(cardName, 50, "--- Invalid card! ---");
		break;
	}

	switch (cardType)
	{
	case CLUBS:
		lengthyNamesFlag ? strcat_s(cardName, 50, " of Clubs") : strcat_s(cardName, 50, "C");
		break;
	case DIAMONDS:
		lengthyNamesFlag ? strcat_s(cardName, 50, " of Diamonds") : strcat_s(cardName, 50, "D");
		break;
	case HEARTS:
		lengthyNamesFlag ? strcat_s(cardName, 50, " of Hearts") : strcat_s(cardName, 50, "H");
		break;
	case SPADES:
		lengthyNamesFlag ? strcat_s(cardName, 50, " of Spades") : strcat_s(cardName, 50, "S");
		break;
	}

	return cardName;
}

char * getGameResult(Player *player_1, Player *player_2)
{
	char *result = (char *)malloc(50);
	if (player_1->score > player_2->score) {
		sprintf_s(result, 50, "%s Wins!", player_1->name);
	}
	else if (player_1->score < player_2->score) {
		sprintf_s(result, 50, "%s Wins!", player_2->name);
	}
	else {
		sprintf_s(result, 50, "Draw!");
	}

	return result;
}

void evaluateCardSelection(Game *gameObhect, Player *player_1, Player *player_2)
{
	char *result;
	printf("\n\tYou selected %s\n", getCardName(player_1->currentSelectedCard[0], player_1->currentSelectedCard[1], true));
	printf("\n\t%s selected %s\n", player_2->name, getCardName(player_2->currentSelectedCard[0], player_2->currentSelectedCard[1], true));
	if (player_1->currentSelectedCard[1] > player_2->currentSelectedCard[1]) {
		player_1->score++;
	}
	else if (player_1->currentSelectedCard[1] < player_2->currentSelectedCard[1]) {
		player_2->score++;
	}
	gameObhect->cardDeck[player_1->currentSelectedCard[0]][player_1->currentSelectedCard[1]] = -1;
	gameObhect->cardDeck[player_2->currentSelectedCard[0]][player_2->currentSelectedCard[1]] = -1;
	printf("\n\tNumber of rounds remaining: %d\n", gameObhect->numberOfRounds);
	gameObhect->numberOfRounds--;
}

void printStats(Game *gameObhect, Player *player_1, Player *player_2)
{
	puts("\n\t*** Game statistics ***\n");
	puts("\tPlayer\t\tScore\n");
	printf("\t%s\t%d\n", player_1->name, player_1->score);
	printf("\t%s\t%d\n", player_2->name, player_2->score);
	printf("\n\tResult: %s\n", getGameResult(player_1, player_2));
}
