#include "utils.h"

#define NO_KILL 0
#define KILL 1

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{

	// TODO
	int size, len = 0;
	char *afisare;

	//Save size of matrix
	size = *((int*)input);
	afisare = calloc(MAX_LEN_STR_OUT, sizeof(*afisare));

	//Create new form of matrix and saving each element in a buffer
	for(int i = 1; i <= size; i++){

		for(int j = size; j > 0; j--){

			len += sprintf(afisare + len, "%d%d ", j, i);

		}

		len += sprintf(afisare + len, "\n");

	}

	sprintf(afisare + len - 1, '\0');

	//Return the rotated matrix
	return (void*)afisare;

}

// Task 2
void *DecodeString(void *input)
{
	// TODO
	int sum = 0, *buffer, val;
	int len = strlen((char*)input);
	char *string, *token;

	buffer = calloc(MAX_LEN_STR_OUT, sizeof(*buffer));

	//Save string needed to be decoded
	string = (char*)input;
	token = strtok(string, "_");
	while(token != NULL){

		//Take each integer element of the string and increase sum
		//with integers value
		val = atoi(token);
		sum += val;
		token = strtok(NULL, "_");

	}

	//Save sum's value in a buffer
	sprintf(buffer, "%d", sum);

	//Return sum of integers elements from string
	return (void*)buffer;
}

//Task 3
void *InvertArray(void *input)
{
	// TODO
	int point = 0, len;
	int *array;
	char *buffer;

	buffer = calloc(MAX_LEN_STR_OUT, sizeof(*buffer));

	//Save initial array
	array = (int*)input;

	//First element of array has length of array
	len = array[0];

	//Create array without value of length on first element
	for(int i = 0; i < len; i++){

		array[i] = array[i + 1];

	}

	//Case of even length
	if(len % 2 == 0){

		for(int i = 0; i < len; i = i + 2){

			//Switch elements and save in buffer element by element
			point += sprintf(buffer + point, "%d %d ", array[i + 1], array[i]);

		}

	//Case of odd length
	} else{

		for(int i = len - 1; i >= 0; i--){

			//Invert array and save in buffer element by element
			point += sprintf(buffer + point, "%d ", array[i]);

		}

	}
	
	//Return final state of array
	return (void*)buffer;
}

//Task 4
Player *allocPlayer()
{
	// TODO
	Player *_player;
	_player = calloc(1, sizeof(*_player));
	_player->name = calloc(MAX_LEN_STR_ATR, sizeof(*_player->name));
	_player->color = calloc(MAX_LEN_STR_ATR, sizeof(*_player->color));
	_player->hat = calloc(MAX_LEN_STR_ATR, sizeof(*_player->hat));
	(*_player).alive = 1;

	return _player;
}

//Task 4
Game *allocGame()
{
	// TODO
	Game *_game;
	_game = calloc(1, sizeof(*_game));
	_game->name = calloc(MAX_LEN_STR_ATR, sizeof(*_game->name));

	return _game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	// TODO
	Player *_player = allocPlayer();
	char *string;
	string = calloc(MAX_LEN_STR_ATR, sizeof(*string));

	fscanf(inputFile, "%s\n", _player->name);
	fscanf(inputFile, "%s\n", _player->color);
	fscanf(inputFile, "%s\n", _player->hat);
	fscanf(inputFile, "%d", &(_player->numberOfLocations));
	_player->locations = calloc(_player->numberOfLocations, sizeof(Location));
	fgetc(inputFile);

	//Read only integers that represent locations
	//of player, without parenthesis or commas
	for(int i = 0; i < _player->numberOfLocations; i++){

		while(fgetc(inputFile) != '(');
		fscanf(inputFile, "%d", &(_player->locations[i].x));
		fgetc(inputFile);
		fscanf(inputFile, "%d", &(_player->locations[i].y));


	}
	//Read "\n" to continue reading from input file with scanf
	fgetc(inputFile);

	//Read role as a string and convert to enum
	fscanf(inputFile, "%s\n", string);
	if(strcmp(string, "Rotator") == 0){

		_player->playerRole = Rotator;

	} else if(strcmp(string, "Decoder") == 0){

		_player->playerRole = Decoder;

	} else if(strcmp(string, "Invertor") == 0){

		_player->playerRole = Invertor;

	} else{

		_player->playerRole = Impostor;

	}

	//Save ability of player
	_player->ability = Abilities[_player->playerRole];
	free(string);

	return _player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	// TODO
	Game *_game = allocGame();

	fscanf(inputFile, "%s\n", _game -> name);
	fscanf(inputFile, "%d\n", &(_game -> killRange));
	fscanf(inputFile, "%d\n", &(_game -> numberOfCrewmates));
	_game->crewmates = calloc(_game -> numberOfCrewmates, sizeof(Player*));
	for(int i = 0; i < _game -> numberOfCrewmates; i++){

		_game->crewmates[i] = ReadPlayer(inputFile);

	}

	_game->impostor = ReadPlayer(inputFile);


	return _game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	// TODO
	char *string = fromEnumtoString(player->playerRole);

	fprintf(outputFile, "Player %s ", player->name);
	fprintf(outputFile, "with color %s, ", player->color);
	fprintf(outputFile, "hat %s and ", player->hat);
	fprintf(outputFile, "role %s ", string);
	free(string);
	fprintf(outputFile, "has entered the game.\n");
	fprintf(outputFile, "Player's locations:");
	for(int i = 0; i < player->numberOfLocations; i++){

		fprintf(outputFile, " (%d,%d)", 
		player->locations[i].x, player->locations[i].y);

	}

	fprintf(outputFile, "\n");

	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	// TODO
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "	Game options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	fprintf(outputFile, "	Crewmates:\n");
	for(int i = 0; i < game -> numberOfCrewmates; i++){

		WritePlayer(game->crewmates[i], outputFile);

	}
	fprintf(outputFile, "\n	Impostor:\n");
	fprintf(outputFile, "Player %s ", game->impostor->name);
	fprintf(outputFile, "with color %s, ", game->impostor->color);
	fprintf(outputFile, "hat %s and ", game->impostor->hat);
	fprintf(outputFile, "role Impostor has entered the game.\n");
	fprintf(outputFile, "Player's locations:");
	for(int i = 0; i < game -> impostor->numberOfLocations; i++){

		fprintf(outputFile, " (%d,%d)",
		 game->impostor->locations[i].x, game->impostor->locations[i].y);

	}

	fprintf(outputFile, "\n");

	return;
}

//Task 7
void *KillPlayer(void *input)
{
	// TODO
	Game *_game = (Game*)input;
	int distance, len = 0, flag = NO_KILL, location;
	int loc_min = MAX_LEN_STR_ATR, dis_min = _game->killRange, poz_kill;
	char *buffer, *crew_name;

	buffer = calloc(MAX_LEN_STR_OUT, sizeof(*buffer));
	crew_name = calloc(MAX_LEN_STR_ATR, sizeof(*crew_name));

	//Analyze each crewmate
	for(int i = 0; i < _game->numberOfCrewmates; i++){

		if(_game->crewmates[i]->alive){

			//Calculate Manhattan distance
			location = _game->crewmates[i]->indexOfLocation;
			distance = abs(_game->crewmates[i]->locations[location].x
			 - _game->impostor->locations[_game->impostor->indexOfLocation].x);
		
			distance += abs(_game->crewmates[i]->locations[location].y
			 - _game->impostor->locations[_game->impostor->indexOfLocation].y);

				//Verify if crewmate is closer to impostor than the others
				if(distance <= dis_min){

					//If yes, save index from array, distance to impostor,
					//crewmate's name and set crewmate as killed
					poz_kill = i;
					dis_min = distance;
					strcpy(crew_name, _game->crewmates[i]->name);
					flag = KILL;

				}

		}

	}

	if(flag == KILL){

		len += sprintf(buffer + len, "Impostor %s has just killed",
												_game->impostor->name);
		sprintf(buffer + len, " crewmate %s from distance %d.",
												crew_name, dis_min);
		_game->crewmates[poz_kill]->alive = 0;

	} else{

		sprintf(buffer, "Impostor %s couldn't kill anybody.",
											_game->impostor->name);

	}

	free(crew_name);

	return (void*)buffer;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	// TODO
	int i;
	char *power;

	for(i = 0; i < game->numberOfCrewmates; i++){

		if(game->crewmates[i]->alive == 0){

			fprintf(outputFile, "Crewmate %s is dead.\n",
									game->crewmates[i]->name);

		} else {

			//Change crewmate's location after each cycle
			if(game->crewmates[i]->indexOfLocation == game->crewmates[i]->numberOfLocations - 1){

			game->crewmates[i]->indexOfLocation = 0;

			} else{

			game->crewmates[i]->indexOfLocation++;

			}

			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", 
													game->crewmates[i]->name,
			game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].x,
			game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].y);
			fprintf(outputFile, "Crewmate %s's output:\n",
								game->crewmates[i]->name);

			//Buffer needed to output ability of crewmate
			power = (char*)game->crewmates[i]->ability(inputMatrix[i]);
			fprintf(outputFile, "%s\n", power);
			free(power);

		}

	}

	//Change impostor's location after each cycle
	if(game->impostor->indexOfLocation == game->impostor->numberOfLocations - 1){

		game->impostor->indexOfLocation = 0;

	} else{

		game->impostor->indexOfLocation++;

	}

	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n",
											game->impostor->name,
	game->impostor->locations[game->impostor->indexOfLocation].x,
	game->impostor->locations[game->impostor->indexOfLocation].y);
	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);

	//Buffer needed to output ability of impostor
	power = (char*)game->impostor->ability(inputMatrix[i]);
	fprintf(outputFile, "%s\n", power);
	free(power);

	return;
}

// Task 9
void FreePlayer(Player *player)
{
	// TODO
	free(player->locations);
	free(player->hat);
	free(player->color);
	free(player->name);
	free(player);

	return;
}

// Task 9
void FreeGame(Game *game)
{
	// TODO
	int i;

	FreePlayer(game->impostor);
	for(i = 0; i < game->numberOfCrewmates; i++){

		FreePlayer(game->crewmates[i]);

	}
	free(game->crewmates);
	free(game->name);
	free(game);	
	return;
}