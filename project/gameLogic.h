#ifndef _GAMELOGIC_
#define _GAMELOGIC_

//This class in in charge of game logic, it manages the word bank, letters picked and wins and losses

char *draw_blanks(int length); 

int length_word(char * my_word);

char *selected_word(int selection);

int isFound(char letter, char * my_word);

int isWin(int total_count);

int isLoss(int tries);

int gameManager(char guess, char * myWord);

#endif
