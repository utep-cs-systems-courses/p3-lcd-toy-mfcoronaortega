#include <msp430.h>
#include <libTimer.h>
#include <string.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "led.h"
#include "gameLogic.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

char redraw_screen = 1;
u_int backgroundColor = COLOR_WHITE;
char letter_bank1[9] = {'A', 'B', 'C', 'D', 'E', 'F' ,'G', 'H', 'I'};
char letter_bank2[9] = {'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'};
char letter_bank3[8] = {'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int current_letter = 0;
u_char select_col = 10;
u_char select_row = 120;
u_char select_col2 = 10;
u_char select_col3 =10;

int results = 0;
int tries = 6; //6 tries to guess (1)head, (2)body, (3/4)left/right arms, (5/6)left/right legs
int total_letter_count = 9;//letters to guess

void letter_selection (void){
  const u_char char_width = 12;
  const u_char char_height = 12;
  u_int current = COLOR_WHITE;
  u_int color_letter = COLOR_BLACK;
  u_int used = COLOR_GRAY;

  if (switch1_state == down) {
    if(current_letter < 9){
      current= COLOR_GREEN;
      drawChar5x7(select_col, select_row, letter_bank1[current_letter], color_letter, current);
      current_letter++;
      select_col += char_width;
    }
    if(current_letter >= 9 && current_letter < 18){
      current = COLOR_GREEN;
      drawChar5x7(select_col2, select_row + char_height, letter_bank2[current_letter-9], color_letter, current);
      current_letter++;
      select_col2 += char_width;
    }
    if(current_letter >= 18 && current_letter < 26){
      current = COLOR_GREEN;
      drawChar5x7(select_col3, select_row + char_height + char_height, letter_bank3[current_letter-18], color_letter, current);
      current_letter++;
      select_col3 += char_width;
    }
  }
  
  if(switch2_state == down){

  }
}

void drawGameLayout(void)
{
  const u_char welcome_row = 10;
  const u_char welcome_col = 10;
  const u_int title_color = COLOR_BLACK;

  drawString5x7(welcome_col, welcome_row, "WELCOME TO HANGMAN", title_color, COLOR_YELLOW);
  
  const u_char start_row = 120;
  static u_char start_col = 10;
  const u_char char_width = 12;
  const u_char char_height = 12;
  const u_int grid_color = COLOR_BLACK;
  
  int x = 0;
  for(x = 0; x < 9; x++){
    drawChar5x7(start_col, start_row, letter_bank1[x] , grid_color, backgroundColor);
    start_col += char_width;
  }

  start_col = 10;
  
  for(x = 0; x < 9; x++){
    drawChar5x7(start_col, start_row + char_height, letter_bank2[x], grid_color, backgroundColor);
    start_col += char_width;
  }

  start_col = 10;
  
  for(x = 0; x < 8; x++){
    drawChar5x7(start_col, start_row + char_height + char_height, letter_bank3[x], grid_color, backgroundColor);
    start_col += char_width;
  }

  static u_char pole_row = 90;//side to side
  static u_char pole_col = 30;//up, down
  const u_int pole_color = COLOR_BLACK;
  
  int y = 0;
  
  for(y = 0; y <= 65; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_col++;
  }

  pole_row = 90;
  pole_col = 30;
  
  for(y = 0; y <= 40; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_row--;
  }

  pole_col = 30;
  for(y = 0; y <= 15; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_col++;
  }
}

void update_text(char * my_word)
{
  u_char text_row = 105;
  u_char text_col = 10;
  const u_char char_width = 12;
  static u_char blue = 31, green = 16, red = 31;
  u_int on_color  =                (green << 5) | red;
  u_int off_color = (blue << 11)                | red;
  int size = strlen(my_word);

  char toASCII = current_letter + 64;

  //drawString5x7(text_col, text_row, my_word, on_color, backgroundColor);
  
  int x = 0;
  for(x = 0; x < size; x++){
    drawString5x7(text_col, text_row, "_ ", on_color, backgroundColor);
    text_col += char_width;
  }

  text_col = 10;
 
  if (switch2_state == down) { //Selects the current letter when we use the second switch
    drawChar5x7(text_col, text_row, toASCII, on_color, backgroundColor);
    //results = isFound(toASCII, my_word);
    //if(results == 0){
    //tries--;
    //}
    //if(results >= 1){
    //total_letter_count -= results;
    //}
    //if(isLoss(tries)){
    //drawString5x7(text_col, text_row, "YOU LOST!", on_color, backgroundColor);
    //}
    //if(isWin(total_letter_count)){
    //drawString5x7(text_col, text_row, "YOU WIN!", on_color, backgroundColor);
    //}
  }
}

void
drawHorizontalLine(u_int x_start, u_int x_end, u_int y, u_int colorBGR)
{
  u_int length = x_end - x_start;
  // set the draw area from the start of the line to the end
  // height is 1 since its a line
  lcd_setArea(x_start, y, x_end, y);
  for (u_int i = 0; i < length; i++) {
    lcd_writeColor(colorBGR);
  } 
}

void drawLines(u_int x_coord, u_int y_coord, u_int x_point, u_int y_point, u_int color)
{
  // bottom
  drawHorizontalLine(x_coord - x_point, x_coord + x_point, y_coord + y_point, color);
  // top
  drawHorizontalLine(x_coord - x_point, x_coord + x_point, y_coord - y_point, color);
  // and the middle two
  drawHorizontalLine(x_coord - y_point, x_coord + y_point, y_coord + x_point, color);
  drawHorizontalLine(x_coord - y_point, x_coord + y_point, y_coord - x_point, color);
					  
}


void draw_head(void)
{
  int x_coord = 50;
  int y_coord = 55;
  int r = 10;
  // first point will be the very top;
  int x_point = 0;
  int y_point = r;
  int decision = 3 - (2 * r);

  u_int color = COLOR_BLUE;

  drawLines(x_coord, y_coord, x_point, y_point, color);

  while (y_point >= x_point) {
    // move x over one
    x_point++;

    // check decision point
    if (decision > 0) {
      // move y down one
      y_point--;
      decision = decision + 4 * (x_point - y_point) + 10;
    } else {
      decision = decision + 4 * x_point + 6;
    }
    drawLines(x_coord, y_coord, x_point, y_point, color);
  }
}

void draw_body(void)
{
  static u_char pole_row = 50;//side to side
  static u_char pole_col = 65;//up, down
  const u_int pole_color = COLOR_BLUE;
  
  int y = 0;
  
  for(y = 0; y <= 20; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_col++;
  }
}

void draw_left_arm(void)
{
  static u_char pole_row = 50;//side to side
  static u_char pole_col = 75;//up, down
  const u_int pole_color = COLOR_BLUE;
  
  int y = 0;
  
  for(y = 0; y <= 15; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_row--;
    pole_col--;
  }

}

void draw_right_arm(void)
{
  
  static u_char pole_row = 50;//side to side
  static u_char pole_col = 75;//up, down
  const u_int pole_color = COLOR_BLUE;
  
  int y = 0;
  
  for(y = 0; y <= 15; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_row++;
    pole_col--;
  }
}
void draw_left_leg(void)
{
  static u_char pole_row = 50;//side to side
  static u_char pole_col = 85;//up, down
  const u_int pole_color = COLOR_BLUE;
  
  int y = 0;
  
  for(y = 0; y <= 15; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_row--;
    pole_col++;
  }

}

void draw_right_leg(void)
{   
  static u_char pole_row = 50;//side to side
  static u_char pole_col = 85;//up, down
  const u_int pole_color = COLOR_BLUE;
  
  int y = 0;
  
  for(y = 0; y <= 15; y++){
    drawPixel(pole_row, pole_col, pole_color);
    pole_row++;
    pole_col++;
  }

}



void main(void)
{ 
  configureClocks();
  led_init();
  switch_p2_init();
  lcd_init();
  char *my_word = selected_word(0);

  //enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(backgroundColor);
  drawGameLayout();
  draw_head();
  draw_body();
  draw_left_arm();
  draw_right_arm();
  draw_left_leg();
  draw_right_leg();
  
  while (1) { /* forever */
    if (redraw_screen) {
      redraw_screen = 0;
      update_text(my_word);
      letter_selection();
    }
    green_on = 0;   	/* led off */
    led_changed = 1;
    led_update();
    or_sr(0x10);	/**< CPU OFF */
    
    green_on = 1;	/* led on */
    led_changed = 1;
    led_update();
  }
}
