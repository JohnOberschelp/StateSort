#include <algorithm>
#include <chrono>
#include <float.h>
#include <stdio.h>

#define SORT_ELEMENT  int
#include "StateSort.h"

#define DURATION std::chrono::duration_cast<std::chrono::duration<float>>
typedef std::chrono::steady_clock Clock;
uint32_t minstd_rand() { static uint64_t x = 1; return (x = (x * 48271) % 2147483647); }

int main( )
{
    int NumElements = 1'000'000;
    int NumTests    = 100;

    int* Array = new int[NumElements];

    double S[2] = { DBL_MAX, DBL_MAX };  //  best time per algorithm in seconds

    printf( "Minimum Seconds Sorting %d Elements Over %d Tests\n\n", NumElements, NumTests );

    for ( int Test = 1; Test <= NumTests; Test++ )
    {
        //  Randomize the array.
        for ( int e = 0; e < NumElements; e++ ) Array[e] = minstd_rand() % NumElements;

        //  Try for a new minimum time for one or the other sort.
        int WhichSort = minstd_rand() & 1;
        Clock::time_point Start = Clock::now();

        if ( WhichSort == 0 ) std::stable_sort( Array, Array+NumElements );
        else                  StateSort( Array, NumElements, 0 );

        Clock::duration D = Clock::now() - Start;
        float Seconds = ( DURATION( D ) ).count();
        if ( Seconds < S[ WhichSort ] ) S[ WhichSort ] = Seconds;
    }

    printf( "%18s   %18s\n\n", "std::stable_sort", "StateSort" );
    printf( "%18.5f   %18.5f\n\n", S[0], S[1] );
    if ( S[0] == S[1] ) printf( "%18s   %18s\n", "Tie!", "Tie!" );
    if ( S[0] <  S[1] ) printf( "%*s%8.2f%% faster!\n",  7, "", 100*(S[1]-S[0])/S[0] );
    if ( S[0] >  S[1] ) printf( "%*s%8.2f%% faster!\n", 28, "", 100*(S[0]-S[1])/S[1] );

    delete[] Array;

    return 0;
}
