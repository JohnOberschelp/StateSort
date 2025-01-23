
//        BSD 3-Clause License
//
//        Copyright (c) 2024, John Oberschelp
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions are met:
//
//        1. Redistributions of source code must retain the above copyright notice, this
//           list of conditions and the following disclaimer.
//
//        2. Redistributions in binary form must reproduce the above copyright notice,
//           this list of conditions and the following disclaimer in the documentation
//           and/or other materials provided with the distribution.
//
//        3. Neither the name of the copyright holder nor the names of its
//           contributors may be used to endorse or promote products derived from
//           this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//        AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//        IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//        DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//        FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//        DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//        SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//        CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//        OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <random>

#define SORT_ELEMENT int
#include "../../StateSort/StateSort.h"
#include "../../StateSort/OtherSorts.h"

#pragma warning( disable:  4996 )  //  unsafe str tok

//////////////////////////////////////////////////////////////////////

#define NUM_SORTS 6

const char* SortNames[] = {
    "HeapSort",
    "ShellSort",
    "MergeSort",
    "std::sort", 
    "std::stable_sort",
    "StateSort", 0 };

typedef enum {
    HEAP_SORT       = 0,
    SHELL_SORT      = 1,
    MERGE_SORT      = 2,
    STD_SORT        = 3,
    STD_STABLE_SORT = 4,
    STATE_SORT      = 5,
} SORT_TYPE ; 


const char* SortColors[] = {
    "purple",
    "blue",
    "orange",
    "green", 0 };

//////////////////////////////////////////////////////////////////////

#define NUM_DATA_ARRANGEMENTS 4

const char* DataArrangementNames[] = {
    "Random",
    "Ascending",
    "Descending",
    "A_80_R_20", 0 };  //  First 80% Ascending, last 20% random.  How about Nearly Sorted, Few Unique?

typedef enum {
    RANDOM            = 0,
    ASCENDING         = 1,
    DESCENDING        = 2,
    LAST_20PCT_RANDOM = 3,
} DATA_ARRANGEMENT_TYPE ; 

//////////////////////////////////////////////////////////////////////

int NumElementsForStep[102];

double usMinimums[NUM_SORTS][NUM_DATA_ARRANGEMENTS][102];

//////////////////////////////////////////////////////////////////////

LARGE_INTEGER TicksPerSec, Start, Stop; 

//////////////////////////////////////////////////////////////////////

static double TimeASort( SORT_TYPE SortType, SORT_ELEMENT* Vin, int NumElements, double ApproxNumSecondsToTest )
{
    std::mt19937 MersenneTwister( 0 );

    double NumMicrosecondsForPreTest      = ApproxNumSecondsToTest * ( 0.1 * 1'000'000 );
    double NumMicrosecondsForTimedTesting = ApproxNumSecondsToTest * ( 0.9 * 1'000'000 );

    SORT_ELEMENT* V = new SORT_ELEMENT[NumElements];

    QueryPerformanceCounter( &Start );

    int n;
    double MicrosecondsForPreSort;
    for ( n = 1;; n++ )
    {
        for ( int e = 0; e < NumElements; e++ ) V[e] = Vin[e];
        switch ( SortType )
        {
          case HEAP_SORT       :  HeapSort         ( V, NumElements    ); break;
          case SHELL_SORT      :  ShellSort        ( V, NumElements    ); break;
          case MERGE_SORT      :  HybridMergeSort  ( V, NumElements, 0 ); break;
          case STD_SORT        :  std::sort        ( V, V+NumElements  ); break;
          case STD_STABLE_SORT :  std::stable_sort ( V, V+NumElements  ); break;
          case STATE_SORT      :  StateSort        ( V, NumElements, 0 ); break;
          default              :  DebugBreak( );
        }

        QueryPerformanceCounter( &Stop );
        MicrosecondsForPreSort = ( double )( Stop.QuadPart - Start.QuadPart ) * 1'000'000 / TicksPerSec.QuadPart;
        if ( MicrosecondsForPreSort >= NumMicrosecondsForPreTest ) break;
    }

    delete[] V;


    //  Based on that, about how many times should we sort?
    double RoughMicrosecondsPerSort = MicrosecondsForPreSort / n;
    int NumTimesToSort = ( int ) ( NumMicrosecondsForTimedTesting / RoughMicrosecondsPerSort );
    if ( NumTimesToSort < 1 ) NumTimesToSort = 1;


    //  Set up the same data for this many sorts.
    int TotalNumElementsForAllSorts = NumElements * NumTimesToSort;
    SORT_ELEMENT* VV = new SORT_ELEMENT[TotalNumElementsForAllSorts];
    for ( int e = 0; e < NumElements; e++ )
    {
        for ( V = VV; V < VV + TotalNumElementsForAllSorts; V += NumElements ) V[e] = Vin[e];
    }


    //  Time all these sorts; we want very little besides the sorting in here.

    QueryPerformanceCounter( &Start );

    for ( V = VV; V < VV+TotalNumElementsForAllSorts; V += NumElements )
    {
        switch ( SortType )
        {
          case HEAP_SORT       :  HeapSort         ( V, NumElements    ); break;
          case SHELL_SORT      :  ShellSort        ( V, NumElements    ); break;
          case MERGE_SORT      :  HybridMergeSort  ( V, NumElements, 0 ); break;
          case STD_SORT        :  std::sort        ( V, V+NumElements  ); break;
          case STD_STABLE_SORT :  std::stable_sort ( V, V+NumElements  ); break;
          case STATE_SORT      :  StateSort        ( V, NumElements, 0 ); break;
          default              :  DebugBreak( );
        }
    }

    QueryPerformanceCounter( &Stop );
    double MicrosecondsForAllSorts = ( double )( Stop.QuadPart - Start.QuadPart ) * 1'000'000.0 / TicksPerSec.QuadPart;

    double MicrosecondsPerSort = MicrosecondsForAllSorts / NumTimesToSort;

    delete[] VV;

    return MicrosecondsPerSort;
}

//////////////////////////////////////////////////////////////////////

static void ProduceTestData( SORT_ELEMENT* V, int NumElements, DATA_ARRANGEMENT_TYPE DataArrangement, int Seed )
{
    std::mt19937 MersenneTwister( Seed );

    for ( int i = 0; i < NumElements; i++ )
    {
        switch ( DataArrangement )
        {
          case RANDOM:
            V[i] = MersenneTwister( ) % NumElements;
            break;
          case ASCENDING:
            V[i] = i;
            break;
          case DESCENDING:
            V[i] = NumElements - 1 - i;
            break;
          case LAST_20PCT_RANDOM:
            if ( i < NumElements * 80 / 100 ) V[i] = i;
            else                              V[i] = MersenneTwister( ) % NumElements;
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
//
//  Run this as a release build, and use "Start Without Debugging".
//  Also, turn off screen saver first?

int main( )
{
    QueryPerformanceFrequency( &TicksPerSec );

    ////  0.01 and 1 will take about 30 seconds.
    //double ApproxNumSecondsPerTest = 0.01;
    //int    NumTimesToRepeatTest    = 1;

    ////  0.01 and 10 will take about 6 minutes.
    //double ApproxNumSecondsPerTest = 0.01;
    //int    NumTimesToRepeatTest    = 10;

    //  0.05 and 50 will take about 150 minutes.
    double ApproxNumSecondsPerTest = 0.05;
    int    NumTimesToRepeatTest    = 50;

    char FileName[256];
    SYSTEMTIME LocalTime;
    GetLocalTime( &LocalTime );
    sprintf( FileName, "Results_%4d-%02d-%02d_%02d-%02d.txt",
            LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay,
            LocalTime.wHour, LocalTime.wMinute );

    SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_TIME_CRITICAL );


    //  Create our results table for minimums, and initialize as maximums.
    for ( int s = 0; s < NUM_SORTS; s++ )
    for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
    for ( int Step = 17; Step <= 100; Step++ )
        usMinimums[s][a][Step] = DBL_MAX;


    //  Repeat, keeping the minimum.
    LARGE_INTEGER StartOfRepeat, StopOfRepeat;
    for ( int r = 0; r < NumTimesToRepeatTest; r++ )
    {

        printf( "Time %d of %d", r+1, NumTimesToRepeatTest );

        if ( ! r )
        {
            QueryPerformanceCounter( &StartOfRepeat );
        }
        else
        {
            QueryPerformanceCounter( &StopOfRepeat );
            double MicrosecondsSoFar = ( double )( StopOfRepeat.QuadPart - StartOfRepeat.QuadPart ) * 1'000'000.0 / TicksPerSec.QuadPart;
            double MicrosecondsLeft = MicrosecondsSoFar / r * ( NumTimesToRepeatTest - r );
            printf( "                                                     "
                    " About %.2f minutes left.", MicrosecondsLeft / 1'000'000.0 / 60.0 );
        }
        printf( "\n" );


        for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
        {
            printf( "                 Arrangement %d of %d\n", a+1, NUM_DATA_ARRANGEMENTS );
            DATA_ARRANGEMENT_TYPE DataArrangement = ( DATA_ARRANGEMENT_TYPE ) a;


            for ( int s = 0; s < NUM_SORTS; s++ )
            {
                printf( "                                        Sort %d of %d\n", s+1, NUM_SORTS );
                SORT_TYPE SortType = ( SORT_TYPE ) s;

                for ( int Step = 17; Step <= 100; Step++ )
                {

                    double exp = 6.0 * Step / 100;
                    double floatNumElements = pow( 10.0, exp );
                    int NumElements = ( int ) floatNumElements;

                    SORT_ELEMENT* Vin = new SORT_ELEMENT[NumElements];

                    ProduceTestData( Vin, NumElements, DataArrangement, 0 );

                    double us = TimeASort( SortType, Vin, NumElements, ApproxNumSecondsPerTest );
                    if ( us < usMinimums[s][a][Step] ) usMinimums[s][a][Step] = us;

                    delete[] Vin;

                }  //  Step


            }  //  SortType


        }  //  ELEMENT_ARRANGEMENT


    }  //  NumTimesToRepeatTest


    SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_NORMAL );


    printf( "\nSaving.\n" );


    FILE* report;
    int err = fopen_s( &report, FileName, "wb" );
    if ( err )
    {
        DebugBreak( );
        exit( __LINE__ );
    }

    //  Print the column headers which are NumElements.
    fprintf( report, "\t" );
    for ( int Step = 17; Step <= 100; Step++ )
    {
        double exp = 6.0 * Step / 100;
        double floatNumElements = pow( 10.0, exp );
        int NumElements = ( int ) floatNumElements;
        fprintf( report, "\t%d", NumElements );
    }
    fprintf( report, "\n\n" );


    for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
    {
        for ( int s = 0; s < NUM_SORTS; s++ )
        {
            fprintf( report, "%s\t%s", SortNames[s], DataArrangementNames[a] );

            for ( int Step = 17; Step <= 100; Step++ )
            {
                double exp = 6.0 * Step / 100;
                double floatNumElements = pow( 10.0, exp );
                int NumElements = ( int ) floatNumElements;

                double us = usMinimums[s][a][Step];
                fprintf( report, "\t%f", us );
            }

            fprintf( report, "\n" );
        }
    }


    fclose( report );
    printf( "\nDone.\n" );
    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

