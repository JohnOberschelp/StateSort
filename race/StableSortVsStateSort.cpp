#include <algorithm>
#include <chrono>
#include <random>

#define STATESORT_NAME     StateSort
#define STATESORT_ELEMENT  int
#include "StateSort.h"

int main( )
{
    std::mt19937 MersenneTwister( time(NULL) );

    int NumElements = 1'000'000;
    int NumTests    = 100;

    int* Array = new int[NumElements];

    typedef std::chrono::steady_clock Clock;
    #define DURATION std::chrono::duration_cast<std::chrono::duration<float>>

    Clock::duration StableSortMinDuration = std::chrono::hours(1);
    Clock::duration StateSortMinDuration  = std::chrono::hours(1);

    printf( "\nSorting %d Elements %d Times\n\n", NumElements, NumTests );
    printf( "         Minimum       Minimum\n" );
    printf( "         Seconds       Seconds      Time\n" );
    printf( "std::stable_sort     StateSort      Saved\n\n" );

    for ( int Test = 1; Test <= NumTests; Test++ )
    {

        //  Randomize the array.
        for ( int e = 0; e < NumElements; e++ ) Array[e] = MersenneTwister() % NumElements;

        //  Try for a new minimum time for one or the other sort.
        if ( MersenneTwister() & 1 )
        {
            Clock::time_point Start = Clock::now();
            std::stable_sort( Array, Array+NumElements  );
            Clock::duration D = Clock::now() - Start;
            StableSortMinDuration = min( StableSortMinDuration, D );
        }
        else
        {
            Clock::time_point Start = Clock::now();
            StateSort( Array, NumElements, 0 );
            Clock::duration D = Clock::now() - Start;
            StateSortMinDuration = min( StateSortMinDuration, D );
        }

    }

    float StateSortSeconds  = ( DURATION( StateSortMinDuration  ) ).count();
    float StableSortSeconds = ( DURATION( StableSortMinDuration ) ).count();
    double Saved = ( StableSortSeconds - StateSortSeconds ) / StableSortSeconds;

    printf( "  %14.5f%14.5f%10.2f%%\n", StableSortSeconds, StateSortSeconds, Saved * 100 );

    delete[] Array;

    return 0;
}
