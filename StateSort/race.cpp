
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <vector>
//#include <boost/range/algorithm.hpp>

#define SORT_ELEMENT  int
#include "StateSort.h"

#define DURATION std::chrono::duration_cast<std::chrono::duration<float>>
typedef std::chrono::steady_clock Clock;

int main( )
{
    const int NumElements = 1'000'000;

    SORT_ELEMENT* Array = new SORT_ELEMENT[NumElements];
    std::vector<int> Vector( NumElements );


    double TotalSeconds[2] = { 0, 0 };

    printf( "\nAverage Seconds To Sort %d Random Integers\n\n", NumElements );
    printf( "Press control-c to stop.\n\n" );
    printf( "Num Races      StateSort                           std::stableSort\n" );
////printf( "Num Races      StateSort                           std::sort\n" );
////printf( "Num Races      StateSort                           boost::range::stable_sort\n" );
////printf( "Num Races      StateSort                           boost::range::sort\n" );
    printf( "---------      -----------------------------       -----------------------------\n" );

    for ( int Race = 1;; Race++ )
    {

        for ( int WhichSort = 0; WhichSort < 2; WhichSort++ )
        {
            //  Randomize the data.
            for ( int i = 0; i < NumElements; i++ ) Array[i] = Vector[i] = rand() % NumElements;


            //  Delay a random amount, hoping to average out the effects of
            //  timer granularity and interrupt regularity.
            int Delay = rand() & 0xFFFF;
            for ( int i = 0; i < Delay; i++ ) if ( Race < 0 ) Race++;

            Clock::time_point Start = Clock::now();

            switch ( WhichSort )
            {
                case  0: StateSort        ( Array, NumElements, 0 ); break;
                case  1: std::stable_sort ( Array, Array+NumElements ); break;
////////////////case  1: std::sort        ( Array, Array+NumElements ); break;
////////////////case  1: boost::range::stable_sort( Vector ); break;
////////////////case  1: boost::range::sort( Vector ); break;
            }

            Clock::duration D = Clock::now() - Start;
            double Seconds = ( DURATION( D ) ).count();
            TotalSeconds[WhichSort] += Seconds;
        }

        //  Only print every 10th time, hoping to reduce the effect of
        //  interrupts due to printing.
        if ( Race % 10 == 0 )
        {
            double S0 = TotalSeconds[0] / Race;
            double S1 = TotalSeconds[1] / Race;
            if ( S0 < S1 )
            {
                printf( "%8d:  %6d%% faster at %7.4f seconds   %6d%% slower at %7.4f seconds\n",
                    Race, (int)((S1-S0)*100/S1), S0, (int)((S1-S0)*100/S0), S1 );
            }
            else
            {
                printf( "%8d:  %6d%% slower at %7.4f seconds   %6d%% faster at %7.4f seconds\n",
                    Race, (int)((S0-S1)*100/S1), S0, (int)((S0-S1)*100/S0), S1 );
            }
        }

    }

    return 0;
}
