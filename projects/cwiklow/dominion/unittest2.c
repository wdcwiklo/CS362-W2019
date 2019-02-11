/* -----------------------------------------------------------------------
 * Unit test for scoreFor function
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if scoreFor functions correctly
//p: player number
//post: gameState to be used 
//score: the expected score
//Returns 1 for success, 0 for failure
int checkScoreFor(int p, struct gameState* post, int score)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int num;
	num = scoreFor(p, post);
	
	//scoreFor should only tally a score for each subdeck, 
	//there should be no changes in the game state
	if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
	{
		if (num == score)
		{
			printf("SUCCESS: game state unchanged\n");
			printf("Found score %d out of expected %d\n", num, score);
			return 1;
		}
		else
		{
			printf("FAILURE: game state unchanged BUT\n");
			printf("Found score %d out of expected %d\n", num, score);
			return 0;
		}
	}
	else
	{
		printf("FAILURE: game state changed\n");
		printf("Found score %d out of expected %d\n", num, score);
		return 0;
	}
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
	printf("Beginning Unit Test 2: scoreFor Function\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assuming that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	printf("Test 1:\n");
	testCount++; //test 1
	//3 estates(1) = 3 points
	//get score
	r = checkScoreFor(0, &G, 3);

	testPassed += r;

	//add 1 duchy card to each subdeck
	G.hand[0][G.handCount[0]] = duchy;
	G.handCount[0]++;
	G.deck[0][G.deckCount[0]] = duchy;
	G.deckCount[0]++;
	G.discard[0][G.discardCount[0]] = duchy;
	G.discardCount[0]++;

	printf("Test 2:\n");
	testCount++; //test 2
	//3 estates (1) + 3 duchy (3) = 12 points
	//get score
	r = checkScoreFor(0, &G, 12);

	testPassed += r;

	//add 3 provinces to hand deck
	for (i = 0; i < 3; i++)
	{
		G.hand[0][G.handCount[0]] = province;
		G.handCount[0]++;
	}

	printf("Test 3:\n");
	testCount++; //test 3
	//3 estates (1) + 3 duchy (3) + 3 provinces (6) = 30 points
	//get score
	r = checkScoreFor(0, &G, 30);

	testPassed += r;

	//add 3 provinces to discard deck
	for (i = 0; i < 3; i++)
	{
		G.discard[0][G.discardCount[0]] = province;
		G.discardCount[0]++;
	}

	printf("Test 4:\n");
	testCount++; //test 4
	//3 estates (1) + 3 duchy (3) + 6 provinces (6) = 48 points
	//get score
	r = checkScoreFor(0, &G, 48);

	testPassed += r;

	//add 5 curses to deck
	for (i = 0; i < 5; i++)
	{
		G.deck[0][G.deckCount[0]] = curse;
		G.deckCount[0]++;
	}

	printf("Test 5:\n");
	testCount++; //test 5
	//3 estates (1) + 3 duchy (3) + 6 provinces (6) - 5 curses (-1) = 43 points
	//get score
	r = checkScoreFor(0, &G, 43);

	testPassed += r;

	//add 2 gardens to hand deck
	for (i = 0; i < 2; i++)
	{
		G.hand[0][G.handCount[0]] = gardens;
		G.handCount[0]++;
	}

	printf("Test 6:\n");
	testCount++; //test 6
	//3 estates (1) + 3 duchy (3) + 6 provinces (6) - 5 curses (-1) + 2 gardens (26/10) = 47 points
	//get score
	r = checkScoreFor(0, &G, 47);

	testPassed += r;

	printf("\nUnit Test 2 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
