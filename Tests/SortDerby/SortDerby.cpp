
#include <algorithm>
#include <chrono>
#include <stdio.h>

#define SORT_ELEMENT  int
#include "../../StateSort/StateSort.h"
#include "../../StateSort/OtherSorts.h"

#define DURATION std::chrono::duration_cast<std::chrono::duration<float>>
typedef std::chrono::steady_clock Clock;

////////////////////////////////////////////////////////////////////////

int main( )
{
    const int NumElements = 100'000;
    const int NumSorts = 10;

    const char* Name[NumSorts] = {
        "BubbleSort",
        "SelectionSort",
        "ShellSort",
        "HeapSort",
        "HoaresQuickSort",
        "LomutosQuickSort",
        "HybridMergeSort",
        "StateSort",
        "std::sort",
        "std::stable_sort",
////////"YOUR_SORT_HERE"  //  (optional)
    };

    SORT_ELEMENT* Array = new SORT_ELEMENT[NumElements];

    double Seconds[NumSorts];
    int Sort;

    printf( "\nTime Required For %d Sorts To Sort %d Elements\n\n", NumSorts, NumElements );
    printf( "                            Seconds         Ratio\n" );
    printf( "                            -------         -----\n" );
#if defined DEBUG || defined _DEBUG
    printf("(debug mode will skew the results)\n");
#endif

    int Seed = time(0);
    for ( Sort = 0; Sort < NumSorts; Sort++ )
    {

        //  Use the same array for each sort.
        srand( Seed );
        for ( int i = 0; i < NumElements; i++ )
            Array[i] = rand() % NumElements;

        Clock::time_point Start = Clock::now();
        while ( Start == Clock::now() ) {}
        Start = Clock::now();

        switch ( Sort )
        {
            case  0: BubbleSort       ( Array, NumElements ); break;
            case  1: SelectionSort    ( Array, NumElements ); break;
            case  2: ShellSort        ( Array, NumElements ); break;
            case  3: HeapSort         ( Array, NumElements ); break;
            case  4: HoaresQuickSort  ( Array, 0, NumElements-1 ); break;
            case  5: LomutosQuickSort ( Array, 0, NumElements-1 ); break;
            case  6: HybridMergeSort  ( Array, NumElements, 0 ); break;
            case  7: StateSort        ( Array, NumElements, 0 ); break;
            case  8: std::sort        ( Array, Array+NumElements ); break;
            case  9: std::stable_sort ( Array, Array+NumElements ); break;
//////////  case 10: YOUR_SORT_HERE   ( ... ); break;  //  (optional)
        }

        Clock::duration D = Clock::now() - Start;
        Seconds[ Sort ] = ( DURATION( D ) ).count();

        for ( int i = 0; i < NumElements-1; i++ )
            if (Array[i] > Array[i+1] )
                { printf( "Sort %d failed\n", Sort ); exit( __LINE__ ); }

    }

    double LeastSeconds = Seconds[0];
    for ( Sort = 1; Sort < NumSorts; Sort++ )
        if ( Seconds[Sort] < LeastSeconds ) LeastSeconds = Seconds[Sort];

    for ( Sort = 0; Sort < NumSorts; Sort++ )
    {
        double Ratio = Seconds[Sort] / LeastSeconds;
        printf( "%-16s %18.5f", Name[Sort], Seconds[Sort] );
        printf( "%14.5f %s\n", Ratio, Ratio == 1 ? "fastest" : "" );
    }

    delete[] Array;

    printf( "\n(Press a key)\n" );
    getchar();

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

