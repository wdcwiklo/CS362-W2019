/* -----------------------------------------------------------------------
 * Unit test for fullDeckCount function
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if fullDeckCount functions correctly
//p: player number
//card: card type to be searched for
//post: gameState to be used 
//count: the expected number of cards to be found
//Returns 1 for success, 0 for failure
int checkDeckCount(int p, int card, struct gameState* post, int count)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int num;
	num = fullDeckCount(p, card, post);
	
	//fullDeckCount should only count a selected card in each subdeck, 
	//there should be no changes in the game state
	if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
	{
		if (num == count)
		{
			printf("SUCCESS: game state unchanged\n");
			printf("Counted %d out of expected %d cards\n", num, count);
			return 1;
		}
		else
		{
			printf("FAILURE: game state unchanged BUT\n");
			printf("Counted %d out of expected %d cards\n", num, count);
			return 0;
		}
	}
	else
	{
		printf("FAILURE: game state changed\n");
		printf("Counted %d out of expected %d cards\n", num, count);
		return 0;
	}
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
	printf("Beginning Unit Test 1: fullDeckCount Function\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assume that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	printf("Test 1:\n");
	testCount++;
	//game initiation gives player 10 cards (3 estates and 7 coppers)
	//get estate count
	r = checkDeckCount(0, estate, &G, 3);

	testPassed += r;

	printf("Test 2:\n");
	testCount++;
	//game initiation gives player 10 cards (3 estates and 7 coppers)
	//get copper count
	r = checkDeckCount(0, copper, &G, 7);

	testPassed += r;

	//add 1 stewart cards to each subdeck
	G.hand[0][G.handCount[0]] = steward;
	G.handCount[0]++;
	G.deck[0][G.deckCount[0]] = steward;
	G.deckCount[0]++;
	G.discard[0][G.discardCount[0]] = steward;
	G.discardCount[0]++;

	printf("Test 3:\n");
	testCount++;
	//get steward card count
	r = checkDeckCount(0, steward, &G, 3);
	
	testPassed += r;

	printf("\nUnit Test 1 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
