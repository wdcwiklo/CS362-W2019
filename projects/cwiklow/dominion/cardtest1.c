/* -----------------------------------------------------------------------
 * Unit test for Smithy card
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if Smith card works correctly
//p: player number
//post: gameState to be used 
//handPos: position of Smithy card in player hand
//Returns 1 for success, 0 for failure
int checkSmithy(int p, struct gameState* post, int handPos)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int result = 1; //return value flag, 1 if correct, 0 if failed
	int num, i;
	//play Smithy card
	num = playSmithy(p, post, handPos);
	
	//Smithy should draw 3 cards from the player's deck, and then discard the 
	//Smithy from the hand to the discard pile

	//check that 3 cards added to hand, to the extent possible
	num = pre.deckCount[p]; //number of cards avialable to draw
	
	if (num >= 3) //should draw 3 cards from deck
	{
		if (pre.deckCount[p] - 3 == post->deckCount[p])
			printf("Correct number of cards drawn\n");
		else
		{
			printf("Incorrect number of cards drawn\n");
			result = 0;
		}		
	}
	else if (num == 2) //should draw 2 cards from deck
	{
		if (pre.deckCount[p] - 2 == post->deckCount[p])
			printf("Correct number of cards drawn\n");
		else
		{
			printf("Incorrect number of cards drawn\n");
			result = 0;
		}	
	}
	else if (num == 1) //should draw 1 card from deck
	{
		if (pre.deckCount[p] + 1 == post->deckCount[p])
			printf("Correct number of cards drawn\n");
		else
		{
			printf("Incorrect number of cards drawn\n");
			result = 0;
		}	
	}
	else if (num <= 0) //no cards available to draw
	{
		if (pre.deckCount[p] == post->deckCount[p])
			printf("Correct number of cards drawn\n");
		else
		{
			printf("Incorrect number of cards drawn\n");
			result = 0;
		}	
	}

	//check that Smithy removed from hand after drawing cards
	for (i = 0; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == smithy)
		{
			printf("Discard failed: Smithy still in hand\n");
			result = 0;
		}
	}

/*SOMETHING TO CHECK AGAINST THE REST OF THE GAME STATE? BUT WHERE TO LIMIT?
 * E.G. WHAT SHOULDN'T THE CARD DO/AFFECT? DEVISE BETTER MEMORY CHECK
 */


	return result;
}

int main() {
	int testCount = 0;
	int testPassed = 0;
	int seed = 1000;
	int numPlayer = 2;
	int r, i;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
			remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("---------------------------------------------\n");
	printf("Beginning Card Test 1: Smithy\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assume that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	//put a Smithy as first card in hand
	G.hand[0][0] = smithy;

	printf("Test 1:\n");
	testCount++;
	//play Smithy card
	r = checkSmithy(0, &G, 0);
	testPassed += r;

	//put a Smithy as first card in hand
	G.hand[0][0] = smithy;
	//remove all cards from deck
	for (i = 0; i < G.deckCount[0]; i++)
		G.deck[0][i] = -1;
	G.deckCount[0] = 0;

	printf("Test 2:\n");
	testCount++;
	//play Smithy card
	r = checkSmithy(0, &G, 0);
	testPassed += r;

	//put a Smithy as first card in hand
	G.hand[0][0] = smithy;
	//add 2 cards to deck
	G.deck[0][0] = gold;
	G.deck[0][1] = silver;
	G.deckCount[0] = 2;

	printf("Test 3:\n");
	testCount++;
	//play Smithy card
	r = checkSmithy(0, &G, 0);
	testPassed += r;


	printf("\nCard Test 1 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
