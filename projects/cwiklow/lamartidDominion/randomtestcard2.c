/* -----------------------------------------------------------------------
 * Randomized Unit test for Village card
 *
 * Tests Village card by randomiing input parameters and running a high
 * number of tests to ensure each branch of the playVillage function is 
 * covered. Input to consider:
 * 	player number:
 * 		The gameState can only have a small range of valid player
 * 	 	numbers. I decided to randomly generate a player number 
 * 	 	within the valid range for each test. 
 * 	gameState:
 * 		gameState has many components with many possible values. 
 * 		To ensure Smithy does not affect unintended components of
 * 		gameState, the gameState struct will be 'randomized' with
 * 		random bytes. Relevant components will be set to a random
 * 		value within their respective appropriate range, and the 
 * 		gameState will be passed to the test function. A copy of
 * 		the gameState will be made and later compared against the
 * 		original gameState.
 *	hand position:
 *		The Village card will be in a specific position in the
 *		player's hand. Eligible positions are 0 to player handCount - 1.
 *		This position will be randomly decided. To assist with 
 *		testing the discard functionality of Village, only one
 *		Village card will be present in the hand during a test.
 * -----------------------------------------------------------------------
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

const int LOUD = 0; //to toggle if print statements are displayed during tests

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
	int choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	post->whoseTurn = p;
	pre.whoseTurn = p;
	num = cardEffect(village, choice1, choice2, choice3, post, handPos, &bonus);

	//Village should draw one card, provide 2 additional actions, and then be discarded

	//check that 1 card drawn from deck
	num = pre.deckCount[p]; //number of cards in pre hand
	
	if (num == 0)
	{
		if (pre.discardCount[p] - 1 == post->discardCount[p])
			if (LOUD) {printf("Correct number of cards drawn\n");}
		else
		{
			if (LOUD) {printf("Incorrect number of cards drawn. Drew %d cards\n", 
				pre.discardCount[p] - post->discardCount[p]);}
			result = 0;
		}
	}
	else
	{
		if (num - 1 == post->deckCount[p])
		{
			if (LOUD) {printf("Correct number of cards drawn\n");}
		}
		else
		{
			if (LOUD) {printf("Incorrect number of cards drawn. Drew %d cards\n", 
				num - post->deckCount[p]);}
			result = 0;
		}
	}

	//check if 2 additional actions have been added
	num = pre.numActions + 2;

	if (num == post->numActions)
	{
		if (LOUD) {printf("Correct number of actions added\n");}
	}
	else
	{	
		if (LOUD) {printf("Incorrect number of actions added. Added %d actions\n", 
			post->numActions - num);}
		result = 0;
	}

	//check that Village was discarded
	int flag = 1;
	for (i = 0; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == village)
		{
			if (LOUD) {printf("Discard failed: Village still in hand\n");}
			result = 0;
			flag = 0;
		}
	}

	if (flag)
	{
		if (LOUD) {printf("Village discarded\n");}
	}

	//perform actions on control gameState to compre full states
	drawCard(p, &pre);
	pre.numActions += 2;
	discardCard(handPos, p, &pre, 0);
		

	//check against entire gameState
	if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
	{
		//no other attributes of gameState modified
		if (LOUD) {printf("gameState is correct\n");}
	}
	else
	{
		//some part of state is modified
		if (LOUD) {printf("gameState is NOT correct\n");}
		result = 0;
	}

	return result;
}

int main() {
	int testCount = 0;
	int testPassed = 0;
	int seed = 1000;
	int numPlayer = MAX_PLAYERS;
	int i, n, p, r, num;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
			remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("---------------------------------------------\n");
	printf("Beginning Card Test: Village\n");
	printf("---------------------------------------------\n");

	r = initializeGame(numPlayer, k, seed, &G);

	if (r != 0) //assume that initializeGame works correctly
	{
		printf("Game not initialized, exiting test\n");
		return 0;
	}

	for (n = 0; n < 1000; n++) //loop for 1000 random gameStates
       	{
		//get random player number
		p = floor(Random() * MAX_PLAYERS);

		//randomize gameState
       	        for (i = 0; i < sizeof(struct gameState); i++)
       	        {
       	                ((char*)&G)[i] = floor(Random() * 256);
       	        }
	
		//increment test number for results
       	        testCount++;
       	        if (LOUD) {printf("Test %d:\n", testCount);}

		//set random number of cards in deck, discard pile, and hand
       	        G.deckCount[p] = floor(Random() * MAX_DECK);
       		G.discardCount[p] = floor(Random() * MAX_DECK);
        	G.handCount[p] = floor(Random() * MAX_DECK);
		
		//won't allow 0 cards in hand
		if (G.handCount[p] == 0) G.handCount[p] = 1;

		//set fixed number of actions, as every turn will always start with 1
		G.numActions = 1;
		
		//place Village randomly into hand
		num = floor(Random() * G.handCount[p]);	
		G.hand[p][num] = village;

		//set playedCardCount so cards can be discarded
		G.playedCardCount = 0;

               	//test Village card
                r = checkVillage(p, &G, num);

		//store result of test
       	        testPassed += r;
	}

	printf("\nVilage Card Test Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
