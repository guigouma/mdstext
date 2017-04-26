#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "suffixtree.h"

char *all_text;
int text_len;
SuffixTree* st;
void init(void)
{
  /* Read text file */
  FILE *fp = fopen ("text", "r");
  fseek (fp, 0, SEEK_END);
  text_len = ftell(fp);
  all_text = (char *) malloc(text_len + 1);
  fseek (fp, 0, SEEK_SET);
  fread (all_text, text_len, 1, fp);
  fclose (fp);
  all_text[text_len] = 0;

  /* Build suffix tree here... */
  st = new SuffixTree(all_text, text_len);
}
 
int get_character(void) 
{
  struct termios oldt, newt;
  int ch;
  tcgetattr (STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON|ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

/* 
   Return 1 if we are returning a prediction, 0 otherwise.
   If we return 1, we also need to set *start and *end to the indices
   in the text of the starting and ending characters of our prediction.
   s is the string we have typed so far.
*/

char * result_text;
int result_len;
int predict(char *s, int *start, int *end)
{
  /* Replace this function with your prediction logic based on the suffix tree... */
  /*if (strcmp(s, "Modern Data Structures") == 0) {
    *start = 7372318;
    *end = *start + 11;
    return 1;
  } */
  Node* result = st->find(s,st->root);
  if (result != nullptr ) {
        result_text = (char*)result->total.c_str();     
        *start = strlen(s)+ 1;
        *end = result->total.length() - 1; 
        return 1;
  }

      
  return 0;
}

void process_keystrokes(void)
{
  int ch = 0, start, end, i, j, last_len = 0;
  char s[1024] = "";

  /* Hide cursor */
  printf ("\e[?25l");
  while (ch != '\n') {
    for (i = 0; i < last_len + 1; i++) printf ("\b");
    printf ("\e[0;37;40m%s\e[0;33;40m", s);
    i = strlen(s);
    if (predict(s, &start, &end))
      while (start <= end) {
                printf ("%c", result_text[start++]);
                i++;
      }
    printf ("\e[0;37;40m");
    for (j=0; j<last_len-i; j++) printf (" ");
    for (j=0; j<last_len-i; j++) printf ("\b");
    last_len = i;
    fflush(stdout);
    ch = get_character();
    if ((ch == 8 || ch == 127) && s[0]) s[strlen(s)-1] = 0;
    else if (ch >= ' ') {
      s[strlen(s)+1] = 0;
      s[strlen(s)] = ch;
    } 
  }
  printf ("\e[0;37;40m\e[?25h\n");
}

int main(void)
{
  init();
  process_keystrokes();
  return 0;
}

