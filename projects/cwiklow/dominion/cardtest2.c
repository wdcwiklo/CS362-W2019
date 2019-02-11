/* -----------------------------------------------------------------------
 * Unit test for Adventurer card
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if Adventurer card works correctly
//p: player number
//post: gameState to be used 
//Returns 1 for success, 0 for failure
int checkAdventurer(int p, struct gameState* post)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int result = 1; //return value flag, 1 if correct, 0 if failed
	int num, i;

	//play Adventurer card
	num = playAdventurer(p, post);
	
	//Adventurer should draw from the deck until 2 treasure cards are found,
	//those treasure cards are added to the hand. The drawn cards which are
	//not treasure cards are discarded.

	//check that 2 cards added to hand
	num = pre.handCount[p]; //number of cards in pre hand
	
	if (num + 2 == post->handCount[p])
		printf("Correct number of cards added to hand\n");
	else
	{
		printf("Incorrect number of cards added to hand. Added %d cards\n", 
			post->handCount[p] - num);
		result = 0;
	}

	//check if newly added cards are treasure cards
	for (i = pre.handCount[p]; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == copper || post->hand[p][i] == silver ||
		    post->hand[p][i] == gold)
			printf("Correct: a treasure card was added\n");
		else
		{
			printf("Failure: a non-treasure card was added\n");
			result = 0;
		} 
	}


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
	printf("Beginning Card Test 2: Adventurer\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assume that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	//put an Adventurer as first card in hand
	G.hand[0][0] = adventurer;

/*
int i;
for (i = 0; i < G.deckCount[0]; i++)
	printf("Deck #%d: %d\n", i+1, G.deck[0][i]);
for (i = 0; i < G.handCount[0]; i++)
	printf("Hand #%d: %d\n", i+1, G.hand[0][i]);
*/

	printf("Test 1:\n");
	testCount++;
	//play Adventurer card
	r = checkAdventurer(0, &G);
	testPassed += r;
/*
for (i = 0; i < G.deckCount[0]; i++)
	printf("Deck #%d: %d\n", i+1, G.deck[0][i]);
for (i = 0; i < G.handCount[0]; i++)
	printf("Hand #%d: %d\n", i+1, G.hand[0][i]);*/

	//stack deck with treasure cards
	for (i = 0; i < 5; i++)
		G.deck[0][i] = gold;
	G.deckCount[0] = 5;

	printf("Test 2:\n");
	testCount++;
	//play Adventurer card
	r = checkAdventurer(0, &G);
	testPassed += r;

	//remove all deck cards
	G.deckCount[0] = 0;
	
	printf("Test 3:\n");
	testCount++;
	//play Adventurer card
	r = checkAdventurer(0, &G);
	testPassed += r;

	printf("\nCard Test 1 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
