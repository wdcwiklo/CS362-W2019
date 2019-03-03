/* -----------------------------------------------------------------------
 * Randomized Unit test for Adventurer card
 *
 * Tests Adventurer card by randomiing input parameters and running a high
 * number of tests to ensure each branch of the playAdventurer function is 
 * covered. Input to consider:
 * 	player number:
 * 		The gameState can only have a small range of valid player
 * 	 	numbers. I decided to randomly generate a player number 
 * 	 	within the valid range for each test. 
 * 	gameState:
 * 		gameState has many components with many possible values. 
 * 		To ensure Adventurer does not affect unintended components of
 * 		gameState, the gameState struct will be 'randomized' with
 * 		random bytes. Relevant components will be set to a random
 * 		value within their respective appropriate range, and the 
 * 		gameState will be passed to the test function. A copy of
 * 		the gameState will be made and later compared against the
 * 		original gameState.
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

const int LOUD = 0; //to toggle if print statements are displayed during tests

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
	{
		if (LOUD)
			printf("Correct number of cards added to hand\n");
	}
	else
	{
		if (LOUD)
			printf("Incorrect number of cards added to hand. Added %d cards\n", 
				post->handCount[p] - num);
		result = 0;
	}

	//check if newly added cards are treasure cards
	for (i = pre.handCount[p]; i < post->handCount[p]; i++)
	{
		if (post->hand[p][i] == copper || post->hand[p][i] == silver ||
		    post->hand[p][i] == gold)
		{
			if (LOUD)
				printf("Correct: a treasure card was added\n");
		}
		else
		{
			if (LOUD)
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
	int numPlayer = MAX_PLAYERS;
	int i, n, p, r, num, numTreasure;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
			remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("---------------------------------------------\n");
	printf("Beginning Randomized Card Test: Adventurer\n");
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

		//printf("testCount: %d\n", testCount);

		//set random number of cards in deck, discard pile, and hand
		//deck must be at least 2, so that the treasure cards can fit into deck
       	        G.deckCount[p] = floor(Random() * (MAX_DECK - 2)) + 2;
     		G.discardCount[p] = 0; //floor(Random() * MAX_DECK);
        	G.handCount[p] = floor(Random() * MAX_DECK);
		
		//add treasure cards
		//add at a minimun of 2 cards, because otherwise Adventurer will
		//loop indefinitely as it searches for treasure cards which are
		//not present
		numTreasure = floor(Random() * (G.deckCount[p] - 2)) + 2;

		/*
		printf("numTreasure = %d\n", numTreasure);
		printf("deckCount: %d\n", G.deckCount[p]);
		printf("discardCount: %d\n", G.discardCount[p]);
		printf("handCount: %d\n\n", G.handCount[p]);
		*/

		//add treasure cards to deck
		while (numTreasure > 0)
		{
			//decide what type of treasure to add
			num = floor(Random() * 3);

			//decide what position in deck
			i = floor(Random() * G.deckCount[p]);

			//add treasure to hand, if position not treasure already
			if (G.deck[p][i] != copper && G.deck[p][i] != silver && 
			    G.deck[p][i] != gold)
			{
				//valid position found, add treasure
				switch (num)
				{
				case 0:
					G.deck[p][i] = copper;
					break;
				case 1:
					G.deck[p][i] = silver;
					break;				
				case 2:
					G.deck[p][i] = gold;
					break;		
				}

				numTreasure--;
				//if (LOUD) printf("Added treasure type: %d to deck position %d\n", num, i);
			}

		}

		//won't allow 0 cards in hand
		if (G.handCount[p] == 0) G.handCount[p] = 1;

		//place Adventurer randomly into hand
		num = floor(Random() * G.handCount[p]);	
		G.hand[p][num] = adventurer;

		//set playedCardCount so cards can be discarded
		G.playedCardCount = 0;

               	//test Adenturer card
                r = checkAdventurer(p, &G);

		//store result of test
       	        testPassed += r;
	}


	printf("\nAdventurer Card Test Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
