#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state,
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3,
	       struct gameState *state, int handPos, int *bonus);

// Individual card-calling functions
void smithy_play(
	int currentPlayer, struct gameState *state, int handPos, int *bonus);

void adventurer_play(
	int currentPlayer, int drawntreasure, int z, int temphand[],
	struct gameState *state, int handPos, int *bonus);

void council_room_play(int currentPlayer, struct gameState *state, int handPos);

int mine_play(
	int currentPlayer, struct gameState *state, int handPos,
	int choice1, int choice2);

void steward_play(
	int currentPlayer, struct gameState *state, int handPos,
	int choice1, int choice2, int choice3);

#endif
