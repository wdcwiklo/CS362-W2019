CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittestresults: dominion.o unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c
	echo -e "Results for unittest:\n" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 unittest1.c -g dominion.o rngs.o $(CFLAGS)
	./unittest1 >> unittestresults.out
	
	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 unittest2.c -g dominion.o rngs.o $(CFLAGS)
	./unittest2 >> unittestresults.out
	
	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 unittest3.c -g dominion.o rngs.o $(CFLAGS)
	./unittest3 >> unittestresults.out
	
	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 unittest4.c -g dominion.o rngs.o $(CFLAGS)
	./unittest4 >> unittestresults.out
	
	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 cardtest1.c -g dominion.o rngs.o $(CFLAGS)
	./cardtest1 >> unittestresults.out
	
	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 cardtest2.c -g dominion.o rngs.o $(CFLAGS)
	./cardtest2 >> unittestresults.out
	
	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 cardtest3.c -g dominion.o rngs.o $(CFLAGS)
	./cardtest3 >> unittestresults.out
	
	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 cardtest4.c -g dominion.o rngs.o $(CFLAGS)
	./cardtest4 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	
	cat dominion.c.gcov >> unittestresults.out

randomtestadventurer: dominion.o randomtestadventurer.c
	echo -e "Results for randomized tests:\n" > randomtestadventurer.out
	echo "randomtestadventurer.c:" >> randomtestadventurer.out
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o rngs.o $(CFLAGS)
	./randomtestadventurer >> randomtestadventurer.out
	gcov -b -f dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

randomtestcard1: dominion.o randomtestcard1.c	
	echo -e "Results for randomized tests:\n" > randomtestcard1.out
	echo "randomtestcard1.c:" >> randomtestcard1.out
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o rngs.o $(CFLAGS)
	./randomtestcard1 >> randomtestcard1.out
	gcov -b -f dominion.c >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out

randomtestcard2: dominion.o randomtestcard2.c
	echo -e "Results for randomized tests:\n" > randomtestcard2.out
	echo "randomtestcard2.c:" >> randomtestcard2.out
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o rngs.o $(CFLAGS)
	./randomtestcard2 >> randomtestcard2.out
	gcov -b -f dominion.c >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out

all: playdom player

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
