/*
 * Wesley Cwiklo
 * CS362 - Random Testing Quiz
 * 2/10/19
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	int num;

	//get a character from the ASCII table, from position 32 to 126
	//this range includes special characters, numbers, and letters 
	//(upper and lower case)
	num = (rand() % (126 - 32 + 1)) + 32;

	//return char value of the randomized number
	return (char)num;
}

char *inputString()
{
	int num, i;
	int len = 6;
	char text[len];
	char* letters = "reset\0";
	
	for (i = 0; i < len-1; i++)
	{
		num = rand() % 6;
		text[i] = letters[num];
	}
	text[len-1] = '\0';

	return text;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
