/* -----------------------------------------------------------------------
 * Unit test for Village card
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if Village card works correctly
//p: player number
//post: gameState to be used 
//handPos: position of card in deck
//Returns 1 for success, 0 for failure
int checkVillage(int p, struct gameState* post, int handPos)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int result = 1; //return value flag, 1 if correct, 0 if failed
	int num, i;

	//play Village card
	//variables for refactored code
	int choice1, choice2, choice3, bonus;
	num = cardEffect(village, choice1, choice2, choice3, post, handPos, &bonus);
	
	//Village should draw one card, provide 2 additional actions, and then be discarded

	//check that 1 card drawn from deck
	num = pre.deckCount[p]; //number of cards in pre hand
	
	if (num - 1 == post->deckCount[p])
		printf("Correct number of cards drawn\n");
	else
	{
		printf("Incorrect number of cards drawn. Drew %d cards\n", 
			num - post->deckCount[p]);
		result = 0;
	}

	//check if 2 additional actions have been added
	num = pre.numActions;

	if (num + 2 == post->numActions)
		printf("Correct number of actions added\n");
	else
	{	
		printf("Incorrect number of actions added. Added %d actions\n", 
			post->numActions - num);
		result = 0;
	}

	//check that Village was discarded
	int flag = 1;
	for (i = 0; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == village)
		{
			printf("Discard failed: Village still in hand\n");
			result = 0;
			flag = 0;
		}
	}

	if (flag)
		printf("Village discarded\n");

	return result;
}

int main() {
	int testCount = 0;
	int testPassed = 0;
	int seed = 1000;
	int numPlayer = 2;
	int r;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
			remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("---------------------------------------------\n");
	printf("Beginning Card Test 3: Village\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assume that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	//put a Village as first card in hand
	G.hand[0][0] = village;

	printf("Test 1:\n");
	testCount++;
	//play Village card
	r = checkVillage(0, &G, 0);
	testPassed += r;
	
	//put a Village as last card in hand
	G.hand[0][G.handCount[0]-1] = village;

	printf("Test 2:\n");
	testCount++;
	//play Village card
	r = checkVillage(0, &G, G.handCount[0]-1);
	testPassed += r;
	
	printf("\nCard Test 3 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
