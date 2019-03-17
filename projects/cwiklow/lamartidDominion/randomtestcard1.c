/* -----------------------------------------------------------------------
 * Randomized Unit test for Smithy card
 *
 * Tests Smithy card by randomiing input parameters and running a high
 * number of tests to ensure each branch of the playSmithy function is 
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
 *		The Smithy card will be in a specific position in the
 *		player's hand. Eligible positions are 0 to player handCount - 1.
 *		This position will be randomly decided. To assist with 
 *		testing the discard functionality of Smithy, only one
 *		Smithy card will be present in the hand during a test.
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

const int LOUD = 0; //to toggle if print statements are displayed during tests

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
	int* bonus; //needed for teammates refactored code
	smithy_play(p, post, handPos, bonus);
	
	//Smithy should draw 3 cards from the player's deck, and then discard the 
	//Smithy from the hand to the discard pile

	//check that 3 cards added to hand, to the extent possible
	num = pre.deckCount[p] + pre.discardCount[p]; //number of cards available to draw

	if (num > 3) //if more than 3 cards in deck, than the max 3 cards will be drawn
		num = 3;
	//else 2, 1, or 0 cards will be drawn

	//check correct number of cards drawn
	if (pre.deckCount[p] >= 3)
	{
		if (pre.deckCount[p] - num == post->deckCount[p])
		{
			if (LOUD) {printf("Correct number of cards drawn\n");}
		}
		else
		{
			if (LOUD) {printf("Incorrect number of cards drawn\n");}
			result = 0;
		}
	}
	else 
	{
		if (pre.deckCount[p] + pre.discardCount[p] - num == post->deckCount[p] + post->discardCount[p])
		{
			if (LOUD) {printf("Correct number of cards drawn\n");}
		}
		else
		{
			if (LOUD) {printf("Incorrect number of cards drawn\n");}
			result = 0;
		}
	}

	//check that cards drawn are actually in hand
	if (pre.handCount[p] + num - 1 == post->handCount[p]) //-1 because Smithy supposed to be discarded
	{
		if (LOUD) {printf("Correct number of cards in hand\n");}
	}
	else
	{
		if (LOUD) {printf("Incorrect number of cards in hand\n");}
		result = 0;
	}	

	//check that Smithy removed from hand after drawing cards
	//only 1 Smithy in hand, so fails if any Smithy is found
	for (i = 0; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == smithy)
		{
			if (LOUD) {printf("Discard failed: Smithy still in hand\n");}
			result = 0;
		}
	}

	//perform actions on control gameState to compare entire state
	for (i = 0; i < 3; i++)
		drawCard(p, &pre);
	discardCard(handPos, p, &pre, 0);

	//check against entire gameState
	if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
	{
		//no other attributes of gameState modified
		if (LOUD) {printf("gameState is correct\n");}
	}
	else
	{
		//some other attribute modified
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
	printf("Beginning Randomized Card Test: Smithy\n");
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
         	G.handCount[p] = floor(Random() * MAX_DECK);
      		G.discardCount[p] = floor(Random() * MAX_DECK);
		
		//won't allow 0 cards in hand
		if (G.handCount[p] == 0) G.handCount[p] = 1;
		
		//set playedCardCount so cards can be discarded
		G.playedCardCount = 0;
	
		//place Smithy randomly into hand
		num = floor(Random() * G.handCount[p]);	
		G.hand[p][num] = smithy;

               	//test Smithy card
                r = checkSmithy(p, &G, num);

		//store result of test
       	        testPassed += r;
	}

	printf("\nSmithy Card Test Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
