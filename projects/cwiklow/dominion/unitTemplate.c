/* -----------------------------------------------------------------------
 * Template to set up unit tests for dominion
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"

//simple function to check if passed values are equal
int assertTrue(int a, int b)
{
	if (a == b)
		return 1;
	else
		return 0;
}

int main() {
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int maxBonus = 10;
	int p, r, handCount;
	int bonus;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int maxHandCount = 5;
	
	struct gameState G;
	struct gameState G2;
		
	// Initialize G.
	memcpy (&G2, &G, sizeof(struct gameState));


    
    return 0;
}
