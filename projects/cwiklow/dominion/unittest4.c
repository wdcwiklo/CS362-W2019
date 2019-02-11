/* -----------------------------------------------------------------------
 * Unit test for updateCoins function
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//checks if updateCoins functions correctly
//p: player number
//post: gameState to be used 
//bonus: money bonus to add
//Returns 1 for success, 0 for failure
int checkUpdateCoins(int p, struct gameState* post, int bonus)
{
	struct gameState pre; //to be used as baseline for later comparisons
	memcpy (&pre, post, sizeof(struct gameState));
	pre.coins = 0;
//	printf("pre coins: %d\n", pre.coins);
//	printf("post coins: %d\n", post->coins);
	
	updateCoins(p, post, bonus);
	
//	printf("pre coins: %d\n", pre.coins);
//	printf("post coins: %d\n", post->coins);

	int i;
	for (i = 0; i < pre.handCount[p]; i++)
	{
		if (pre.hand[p][i] == copper)
			pre.coins += 1;
		else if (pre.hand[p][i] == silver)
			pre.coins += 2;
		else if (pre.hand[p][i] == gold)
			pre.coins += 3;
	}
	pre.coins += bonus;

	//updateCoins should only check for treasure cards in the player's hand,
	//there should be no changes in the game state
	if (post->coins == pre.coins)
	{
		printf("Found %d coins as expected\n", post->coins);		
		
		if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
		{
			printf("SUCCESS: game state unchanged\n");
			return 1;
		}
		else
		{
			printf("FAILURE: game state unchanged\n");
			return 0;
		}
	}
	else
	{
		printf("Found %d coins out of %d expected\n", post->coins, pre.coins);
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
	printf("Beginning Unit Test 4: updateCoins Function\n");
	printf("---------------------------------------------\n");
	
	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assuming that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	//remove all cards/coins from player hand
	for (i = 0; i < G.handCount[0]; i++)
		G.hand[0][i] = -1;
	G.handCount[0] = 0;
	G.coins = 0;

	printf("Test 1:\n");
	testCount++; //test 1
	//should have 0 coins
	r = checkUpdateCoins(0, &G, 0);
	testPassed += r;

	//test if bonus works
	printf("Test 2:\n");
	testCount++; //test 2
	//should return 5 per the bonus
	r = checkUpdateCoins(0, &G, 5);
	testPassed += r;

	//add asortment of treasure cards
	G.hand[0][0] = gold;
	G.hand[0][1] = silver;
	G.hand[0][2] = copper;
	G.handCount[0] = 3;
	
	printf("Test 3:\n");
	testCount++; //test 3
	//should return 6
	r = checkUpdateCoins(0, &G, 0);
	testPassed += r;

	printf("\nUnit Test 4 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
