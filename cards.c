

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAIN_FILE "cards_list"
#define NUM_CARDS 52
#define LEN_STRING 50

void strremove (char*, const char*);
void read_main_file (char [][LEN_STRING]);
void shuffle(char [][LEN_STRING]);
void generation_file_names(char [][LEN_STRING], const int);
void card_to_gamers(char [][LEN_STRING], char [][LEN_STRING], const int, const int);
void rest_cards(char [][LEN_STRING], const int, const int);
void get_file_list(char *argv[], char file_list[][LEN_STRING], const int num_gamers);
void print_to_file(FILE* file, char* , const int, const int);
void return_cards(char file_list[][LEN_STRING], const int num_gamers);


int main(int argc, char *argv[]) {

	int argv2;
	sscanf(argv[2], "%d", &argv2);


	// //variant 1 - peremewat' karti
	if (argv2 == 1) {

		// zapominaem file s kartami v strokovii massiv cards[][]
		char cards[NUM_CARDS][LEN_STRING];
		read_main_file(cards);

		//perezapisivaem massiv obratno v fail v sly4ainom poryadke
		shuffle(cards);
	}


	// razda4a kart igrokam
	if (argv2 == 2) {

		// zapominaem file s kartami v strokovii massiv cards[][]
		char cards[NUM_CARDS][LEN_STRING];
		read_main_file(cards);

		//izvlekaem koli4estvo igrokov i koli4estvo razdavaemih kart
		int argv3, argv4;
		sscanf(argv[3], "%d", &argv3);
		sscanf(argv[4], "%d", &argv4);

		int num_gamers = argv3;
		int num_cards_for_gamer = argv4;

		// generiruem imena failov igrokov 
		char file_list[num_gamers][LEN_STRING];
		generation_file_names(file_list, num_gamers);

		// zapisivaem karti v faili igrokov
		card_to_gamers(cards, file_list, num_gamers, num_cards_for_gamer);

		// perezapisivaem v fail nerozdannie karti 
		rest_cards(cards, num_gamers, num_cards_for_gamer);
	}


 	//zabiraem karti y igrokov
	if (argv2 == 3) {

		int argv3;
		sscanf(argv[3], "%d",&argv3);
		int num_gamers = argv3;

	//sozdaem massiv nazvanii failov igrokov
		char file_list[num_gamers][LEN_STRING];
		get_file_list(argv, file_list, num_gamers);

	//dopisivaem karti igrokov v fail s kolodoi kart
		return_cards(file_list, num_gamers);
	}

	return EXIT_SUCCESS;
}


void read_main_file (char cards[][LEN_STRING]) {
	
	FILE* file;
	int i = 0;
	file = fopen (MAIN_FILE, "r");

	// y kazhdogo elementa v konce stroki prisutstvyet "\n" krome poslednego
	while (!feof(file)) {
		fgets(cards[i++], LEN_STRING, file);
	}

	fclose(file);

	// dobavlyaem "\n" poslednemy elementy
	strcat(cards[NUM_CARDS - 1], "\n");
}


void shuffle(char cards[][LEN_STRING]) {

	// v sly4ainom vibiraem element iz massiva i zapisivaem ego v fail
	// nomera yzhe zapisannih elementov sohranyaem v massiv
	srand(time(0));
	FILE * file;
	file = fopen(MAIN_FILE, "w");

	int rand_card = 0, num_card_array[NUM_CARDS];
	int j = 0, i = 0, got_it = 0;

	for(i = 0; i < NUM_CARDS; ) {
		rand_card = rand() % NUM_CARDS;

		//proverka na sovpadenie slu4ainogo elementa i yzhe zapisannih elementov
		got_it = 0;
		for(j = 0; j < i; j++) {
			if (num_card_array[j] == rand_card) {
				got_it = 1;
				break;
			}
		}

		if (got_it == 0) {
			print_to_file(file, cards[rand_card], i, NUM_CARDS);
			num_card_array[i++] = rand_card;
		}
	}
	fclose(file);
}


void print_to_file(FILE* file, char* card, const int i, const int max_card){

	//sledim 4tobi v faile ne bilo pystih strok. 
	//pri zapisi poslednego elementa ne perehodim na novyiy stroky 
	if(i < max_card - 1)
		fprintf(file, "%s", card);
	else {
		strremove(card, "\n");
		fprintf(file, "%s", card);
		strcat(card, "\n");
	}
}
	

void generation_file_names(char file_list[][LEN_STRING], const int num_gamers) {

	// obrazuem imena failov kak gamer_1, game2, gamer_3 i t.d.
	char file_name_base[LEN_STRING] = "gamer_";
	char string_number[LEN_STRING];

	int i;
	for (i = 0; i < num_gamers; i++) {

		//polu4aem imena failov kak gamer_1, gamer_2, gamer_3, ....
		strcpy(file_list[i], file_name_base);
		sprintf(string_number, "%d", i + 1);
		strcat(file_list[i], string_number);
	}
}


void card_to_gamers(char cards[][LEN_STRING], char file_list[][LEN_STRING], const int num_gamers, const int num_cards_for_gamer) {

	FILE* file;
	int i, j, num_card = 0;

	for (i = 0; i < num_gamers; i++) {

		file = fopen(file_list[i], "w");
		for(j = 0; j < num_cards_for_gamer; j++) {

			//zapisivaem karti iz na4ala kolodi s shagom
			num_card = i + j * num_gamers;
			print_to_file(file, cards[num_card], j, num_cards_for_gamer);
		}
		fclose(file);
	}
}


void rest_cards(char cards[][LEN_STRING], const int num_gamers, const int num_cards_for_gamer) {

	//perezapisivaem ostavshiesya karti v ishodnii fail
	FILE* file;
	int i;
	file = fopen(MAIN_FILE, "w");
	for(i = num_gamers * num_cards_for_gamer; i < NUM_CARDS; i++)
		print_to_file(file, cards[i], i, NUM_CARDS);
	fclose(file);
}


void return_cards(char file_list[][LEN_STRING], int num_gamers){
	FILE * main_file, * file;
	main_file = fopen(MAIN_FILE, "a");

	//poo4erednoe otkritie, zapis' i ydalenie failov igrokov
	int i;
	char temp_str[LEN_STRING];

	for (i = 0; i < num_gamers; i++) {
		file = fopen(file_list[i], "r");
		fprintf(main_file, "\n");

		while(!feof(file)) {
			fgets(temp_str, LEN_STRING, file);
			fprintf(main_file, "%s", temp_str);
		}
		fclose(file);
		remove(file_list[i]);
	}
	fclose(main_file);
}


void get_file_list(char *argv[], char file_list[][LEN_STRING], int num_gamers) {
	int i = 0;
	for(i = 0; i < num_gamers; i++) {
		strcpy(file_list[i], argv[i + 4]);
	}
}


//funkciya ydaleniya stroki (del) iz drugoi stroki (str)
void strremove(char* str, const char* del){
	char* temp;
	temp = strstr(str, del);
	strcpy(temp, temp + strlen(del));
}