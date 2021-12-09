#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <string.h>

//This class in in charge of game logic, it manages the word bank, letters picked and wins and losses

char *draw_blanks(int length){
  int x = 0;
  char blanks[length];
  for(x = 0; x < length; x++){
    blanks[x] = "_ ";
  }
  return blanks;
} 

int length_word(char * my_word){
  return strlen(my_word);
}

char *selected_word(int selection)
{
  char * word_bank[5];
  word_bank[0] = "SCIENTIST";
  word_bank[1] = "UTEP";
  word_bank[2] = "COMPUTER";
  word_bank[3] = "CODING";
  word_bank[4] = "VIRTUAL";
  
  //srand(time(NULL));
  int word_pick = selection;

  return word_bank[word_pick];
}

int isFound(char letter, char * my_word){
  int size = strlen(my_word);
  int right = 0;

  int i = 0;
  for(i = 0; i < size; i++){
    if(my_word[i] == letter){
      printf("The letter %c is present!\n", letter);
      right++;
      printf("Was founs %d many times\n",right);
    }
  }
  printf("Ocurrances before returning: %d\n", right);
  return right;
}

int isWin(int total_count){
  if(total_count == 0){//no letters left to guess
    return 1;
  } else {
    return 0; //total letter count is not over (letters left to guess)
  }
}

int isLoss(int tries){
  if(tries == 0){
    return 1; //if we run out of trues we lost
  } else {
    return 0; //if we still have tries we have not lost
  }
}

int gameManager(char guess, char * myWord){
  int results = 0;
  int tries = 6; //6 tries to guess (1)head, (2)body, (3/4)left/right arms, (5/6)left/right legs
  int total_letter_count = strlen(myWord);//letters to guess
  
  do{
    results = isFound(guess, myWord);
    if(results == 0){
      tries--;
    }
    if(results >= 1){
      total_letter_count -= results;
    }
    if(isLoss(tries)){
      return 0;
    }
    if(isWin(total_letter_count)){
      return 1;
    }
  }while(!isLoss(tries) || !isWin(total_letter_count));//while we havent lost or won

}

//void main (void){
//char * my_word = selected_word();
//}
