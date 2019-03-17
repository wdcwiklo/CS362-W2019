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
	//variables for refactored code
	int temphand[MAX_HAND];
	int drawntreasure = 0;
	int z = 0;
	int handPos = 0;
	int* bonus;

	adventurer_play(p, drawntreasure, z, temphand, post, handPos, bonus);
	
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

	//check shuffle
	//count treasure cards in hand
	int numT = 0; //number of treasure cards found in deck
	for (int i = 0; i < pre.deckCount[p]; i++)
	{
		if (pre.deck[p][i] == copper || pre.deck[p][i] == silver ||
			pre.deck[p][i] == gold)
		{
			numT++;
		}
	}

	if (numT >= 2) //shouldn't need to shuffle and add more cards to deck
	{
		if (pre.discardCount[p] > 0) 
		{
			if (post->discardCount[p] == 0) //means shuffle happened
			{
			printf("Discard pile was shuffled back into deck when not necessary\n");
			result = 0;
			}
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
	G.handCount[0] = 1;

	//stack deck with treasure cards
	for (i = 0; i < 4; i++)
		G.deck[0][i] = gold;
	G.deckCount[0] = 4;
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

	//put an Adventurer as first card in hand
	G.hand[0][0] = adventurer;
	G.handCount[0] = 1;

	//stack discard pile with treasure cards
	for (i = 0; i < 4; i++)
	{	G.discard[0][i] = gold;}
	G.discardCount[0] = 4;

	//empty deck
	G.deckCount[0] = 0;

	printf("Test 2:\n");
	testCount++;
	//play Adventurer card
	r = checkAdventurer(0, &G);
	testPassed += r;

	//stack deck with treasure cards
	for (i = 0; i < 4; i++)
		G.deck[0][i] = gold;
	G.deckCount[0] = 4;

	//put an Adventurer as first card in hand
	G.hand[0][0] = adventurer;
	G.handCount[0] = 1;

	//stack discard with non-treasure cards
	for (i = 0; i < 4; i++)
		G.discard[0][i] = village;
	G.discardCount[0] = 4;


	printf("Test 3:\n");
	testCount++;
	//play Adventurer card
	r = checkAdventurer(0, &G);
	testPassed += r;

	printf("\nCard Test 1 Complete: Passed %d out of %d tests\n\n", testPassed, testCount);

	return 0;
}
