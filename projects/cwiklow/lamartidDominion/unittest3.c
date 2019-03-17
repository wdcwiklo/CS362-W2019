/* -----------------------------------------------------------------------
 * Unit test for isGameOver function
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if isGameOver functions correctly
//post: gameState to be used 
//game: 1 meaning "yes" game expected to be over, 0 meaning "no"
//Returns 1 for success, 0 for failure
int checkIsGameOver(struct gameState* post, int game)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));

	int num;
	num = isGameOver(post);
	
	//isGameOver should only check whether there are no provinces available or
	//if there are three empty supply piles, there should be no changes in
	//the game state
	if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
	{
		if (num == game)
		{
			if (game == 1)
			{
				printf("SUCCESS: game state unchanged\n");
				printf("Game is over as expected\n");
				return 1;
			}
			else
			{
				printf("SUCCESS: game state unchanged\n");
				printf("Game not over as expected\n");
				return 1;
			}
		}
		else
		{
			printf("FAILURE: game state unchanged BUT\n");
			printf("Game over incorrectly expected\n");
			return 0;
		}
	}
	else
	{
		printf("FAILURE: game state changed\n");
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
	printf("Beginning Unit Test 3: isGameOver Function\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assuming that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	printf("Test 1:\n");
	testCount++; //test 1
	//game initiated, game is not over
	//get game over status
	r = checkIsGameOver(&G, 0);

	testPassed += r;

	//empty the stack of province cards
	G.supplyCount[province] = 0;

	printf("Test 2:\n");
	testCount++; //test 2
	//province stack empty, game is over
	//get game over status
	r = checkIsGameOver(&G, 1);

	testPassed += r;

	//repopulate the stack of province cards
	G.supplyCount[province] = 1;

	//empty one supply stack
	G.supplyCount[0] = 0;

	printf("Test 3:\n");
	testCount++; //test 3
	//one supply stack empty, game is not over
	//get game over status
	r = checkIsGameOver(&G, 0);

	testPassed += r;

	//empty another supply stack
	G.supplyCount[1] = 0;

	printf("Test 4:\n");
	testCount++; //test 4
	//two supply stacks empty, game is not over
	//get game over status
	r = checkIsGameOver(&G, 0);

	testPassed += r;

	//empty another supply stack
	G.supplyCount[2] = 0;

	printf("Test 5:\n");
	testCount++; //test 5
	//three supply stacks empty, game is over
	//get game over status
	r = checkIsGameOver(&G, 1);

	testPassed += r;

	printf("\nUnit Test 3 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
