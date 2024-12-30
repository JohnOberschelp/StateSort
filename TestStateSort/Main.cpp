
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
#include <assert.h>
#include <random>

#include "StateSort.h"

#pragma warning( disable:  4996 )  //  unsafe str tok

//////////////////////////////////////////////////////////////////////

#define NUM_SORTS 4

const char* SortNames[] = {
    "MergeSort",
    "std::sort", 
    "std::stable_sort",
    "StateSort", 0 };

typedef enum {
  MERGE_SORT      = 0,
  STD_SORT        = 1,
  STD_STABLE_SORT = 2,
  STATE_SORT      = 3,
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

int MergeSort( Element* Sort, int NumElements, void* LoanerBuffer );

//////////////////////////////////////////////////////////////////////

static double timeASort( SORT_TYPE SortType, Element* Vin, int NumElements, double NumSecondsForTotalTest )
{
    std::mt19937 MersenneTwister( 0 );  //  seed here?

    double NumMicrosecondsForPreTest      = NumSecondsForTotalTest * ( 0.1 * 1'000'000 );
    double NumMicrosecondsForTimedTesting = NumSecondsForTotalTest * ( 0.9 * 1'000'000 );


    //  Compute about how long a tenth of our sorting would take.
    Element* V = new Element[NumElements];

    QueryPerformanceCounter( &Start );

    int n;
    double MicrosecondsForPreSort;
    for ( n = 1;; n++ )
    {
        for ( int e = 0; e < NumElements; e++ ) V[e] = Vin[e];
        switch ( SortType )
        {
          case MERGE_SORT      :  MergeSort        ( V, NumElements, 0 ); break;
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
    Element* VV = new Element[TotalNumElementsForAllSorts];
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
          case MERGE_SORT      :  MergeSort        ( V, NumElements, 0 ); break;
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

static void produceTestData( Element* V, int NumElements, DATA_ARRANGEMENT_TYPE DataArrangement, int Seed )
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
          default: assert( 0 );
        }
    }
}

//////////////////////////////////////////////////////////////////////


int TestSortForOrder( )
{
    std::mt19937 MersenneTwister( 0 );

    Element* V = ( Element* ) malloc( 1'000'000 * sizeof( Element ) );
    Element* W = ( Element* ) malloc( 1'000'000 * sizeof( Element ) );
    assert( V );
    assert( W );

    for ( int NumElements = 1'000'000; NumElements > 60-1; NumElements = ( NumElements < 10'000 ) ? NumElements-1 : NumElements * 9999LL / 10000 )
    {

        //  Prepare the elements with random.
        for ( int i = 0; i < NumElements; i++ )
        {
            //V[i] = W[i] = NumElements - 1 - i;
            V[i] = W[i] = MersenneTwister() % NumElements;
        }


        printf( "\n\n%10d:\n", NumElements );


        StateSort        ( V,     NumElements, 0 );
        //MergeSort        ( V,     NumElements, 0 );

        std::stable_sort ( W, W + NumElements    );


        int Difference = memcmp( V, W, NumElements * sizeof( Element ) );
 

        if ( Difference )
        {
            printf( "\nOops. Sort failed.\n" );
            assert( ! Difference );
            return __LINE__;
        }

    }

    return 0;
}

//////////////////////////////////////////////////////////////////////

int TestSortForOrderAndStability( )
{
    std::mt19937 MersenneTwister( 0 );

    Element* V = ( Element* ) malloc( 1'000'000 * sizeof( Element ) );
    Element* W = ( Element* ) malloc( 1'000'000 * sizeof( Element ) );
    assert( V );
    assert( W );

    for ( int NumElements = 1'000'000; NumElements > 60-1; NumElements = ( NumElements < 10'000 ) ? NumElements-1 : NumElements * 9999LL / 10000 )
    {

        //  Prepare the elements with the fractional part holding its original ordering.
        for ( int i = 0; i < NumElements; i++ )
        {
            //V[i] = W[i] = NumElements - 1 - i;
            V[i] = W[i] = ( MersenneTwister() % NumElements ) + i / 1'000'000;
        }


printf( "\n\n%10d:\n", NumElements );


        StateSort        ( V,     NumElements, 0 );

        std::stable_sort ( W, W + NumElements    );

        int Difference = memcmp( V, W, NumElements * sizeof( Element ) );
 
        if ( Difference )
        {
            printf( "\nOops. Sort failed.\n" );
            assert( ! Difference );
            return __LINE__;
        }

//printf( "\nSort was successful.\n" );


    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//
//  Run this as a release build, and use "Start Without Debugging".
//  Also, turn off screen saver first?

int ComputeAndWriteRaceResults( )
{
    double NumSeconds = 0.01;
    int    NumRepeats = 100;

    char FileName[256];
    SYSTEMTIME LocalTime;
    GetLocalTime( &LocalTime );
    sprintf( FileName, "..\\RaceResults_%4d-%02d-%02d_%02d-%02d.txt", LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute );

    SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_TIME_CRITICAL );


    //  Create our results table of minimums, and initialize crazy high.
    for ( int s = 0; s < NUM_SORTS; s++ )
    for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
    for ( int Step = 17; Step <= 100; Step++ )
        usMinimums[s][a][Step] = 9e12;


    //  Repeat keeping the minimum.
    LARGE_INTEGER StartOfRepeat, StopOfRepeat;
    for ( int r = 0; r < NumRepeats; r++ )
    {

        printf( "Time %d of %d", r, NumRepeats );

        if ( ! r )
        {
            QueryPerformanceCounter( &StartOfRepeat );
        }
        else
        {
            QueryPerformanceCounter( &StopOfRepeat );
            double MicrosecondsSoFar = ( double )( StopOfRepeat.QuadPart - StartOfRepeat.QuadPart ) * 1'000'000.0 / TicksPerSec.QuadPart;
            double MicrosecondsLeft = MicrosecondsSoFar / r * ( NumRepeats - r );
            printf( "                                                     "
                    " about %f minutes left", MicrosecondsLeft / 1'000'000.0 / 60.0 );
        }
        printf( "\n" );


        for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
        {
            printf( "                 Arrangement %d of %d\n", a, NUM_DATA_ARRANGEMENTS );
            DATA_ARRANGEMENT_TYPE DataArrangement = ( DATA_ARRANGEMENT_TYPE ) a;


            for ( int s = 0; s < NUM_SORTS; s++ )
            {
                printf( "                                        Sort %d of %d\n", s, NUM_SORTS );
                SORT_TYPE SortType = ( SORT_TYPE ) s;

                for ( int Step = 17; Step <= 100; Step++ )
                {

                    double exp = 6.0 * Step / 100;
                    double floatNumElements = pow( 10.0, exp );
                    int NumElements = ( int ) floatNumElements;

                    Element* Vin = new Element[NumElements];

                    produceTestData( Vin, NumElements, DataArrangement, 0 );

                    double us = timeASort( SortType, Vin, NumElements, NumSeconds );
                    if ( us < usMinimums[s][a][Step] ) usMinimums[s][a][Step] = us;

                    delete[] Vin;

                }  //  Step


            }  //  SortType


        }  //  ELEMENT_ARRANGEMENT


    }  //  NumRepeats


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

int ReadAllTimes( const char* FileName )
{
    const int MAX_LINE = 30'000;
    FILE *File;
    char Line[MAX_LINE]; 
    char *Token;


    int Err = fopen_s( &File, FileName, "rb" );
    assert ( ! Err );
    assert ( File );


    // Process the header line.
    bool got = fgets( Line, MAX_LINE, File );
    assert( got );
    Token = strtok( Line, "\t" );
    for ( int Step = 17; Step <= 100; Step++ )
    {
        int NumElements = atoi( Token );
        assert( NumElements > 0 && NumElements < 1'000'000'000 );
        NumElementsForStep[Step] = NumElements;
        Token = strtok( 0, "\t" ); 
    }


    // Process the  blank line.
    got = fgets( Line, MAX_LINE, File );
    assert( got );
    assert( strlen( Line ) < 4 );


    //  Process Sort + DataArrangement lines.
    for ( int a = 0; a < NUM_DATA_ARRANGEMENTS; a++ )
    {
        for ( int s = 0; s < NUM_SORTS; s++ )
        {
            got = fgets( Line, MAX_LINE, File );
            assert( got );

            Token = strtok( Line, "\t" ); 
            assert( strcmp( Token, SortNames[s] ) == 0 );

            Token = strtok( 0, "\t" ); 
            assert( strcmp( Token, DataArrangementNames[a] ) == 0 );

            for ( int Step = 17; Step <= 100; Step++ )
            {
                Token = strtok( 0, "\t" ); 
                assert( Token );

                double us = atof( Token );
                assert( us > 0.0 && us < 500'000.0 );

                usMinimums[s][a][Step] = us;
            }
        }
    }


    fclose( File ); 
    return 0;
}

//////////////////////////////////////////////////////////////////////

int GraphLog( const char* DataFileName )
{
    //  Define the limits of the graph.
    int    MaxSortSize     = 1'000'000;
    double MaxMicroseconds =   100'000;


    // Define some SVG graph constants.
    int Width    =    800;
    int Height   =    800;
    int Margin   =     50;
    int Top      = Margin + 10 + 20 * NUM_SORTS;
    int Bottom   = Height - Margin;
    int Left     = Margin;
    int Right    = Width - Margin;


    //  Open the SVG file.
    char SvgFileName[256];
    strcpy( SvgFileName, DataFileName );
    strcat( SvgFileName, ".log.svg" );
    FILE *svg;
    int error = fopen_s( &svg, SvgFileName, "w" );
    if ( error ) { perror( "Error opening file" ); return __LINE__; }


    //  Write the SVG header.
    fprintf( svg, "<?xml version=\"1.0\" standalone=\"no\"?>\n" );
    fprintf( svg, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", Width, Height );


    //  Calculate scales.
    double ScaleX = ( Right - Left ) / log10( MaxSortSize );
    double ScaleY = ( Bottom - Top ) / log10( MaxMicroseconds );


    //  Box the graph.
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Top    , Right , Top    );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Bottom , Right , Bottom );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Top    , Left  , Bottom );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Right , Top    , Right , Bottom );


     //  Add the axis labels.
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"16\" text-anchor=\"middle\">Integers</text>\n", Width / 2, Bottom + Margin * 2 / 3 );
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"16\" text-anchor=\"start\" transform=\"rotate(-90 %d %d)\">Microseconds</text>\n", Margin / 2, Bottom   , Margin / 2, Bottom   );


    //  Add the x-axis reference lines.
    for ( double i = 0; i < log10( MaxSortSize ); i++ )
    {
        double x = Left + i * ScaleX;
        double X = pow( 10,i );

        fprintf( svg, "<line x1=\"%.2f\" y1=\"%d\" x2=\"%.2f\" y2=\"%d\" stroke=\"black\" stroke-width=\"0.25\" />\n", x, Top, x, Bottom + 5 );
        fprintf( svg, "<text x=\"%.2f\" y=\"%d\" font-size=\"12\" text-anchor=\"middle\">%d</text>\n", x, Bottom + 15, ( int ) X );

        for ( int tenths = 1; tenths <= 9; tenths++ )
        {
            double X2 = X*tenths;
            double j = log10( X2 );
            double xxx = Left + j * ScaleX;

            fprintf( svg, "<line x1=\"%.2f\" y1=\"%d\" x2=\"%.2f\" y2=\"%d\" stroke=\"black\" stroke-width=\"0.125\" />\n", xxx, Top, xxx, Bottom );
        }
    }


    //  Add the y-axis reference lines.
    for ( double j = log10( MaxMicroseconds ) / 5; j <= log10( MaxMicroseconds ); j += log10( MaxMicroseconds ) / 5 )
    {
        double y = Bottom - j * ScaleY;
        double Y = pow( 10,j );

        fprintf( svg, "<line x1=\"%d\" y1=\"%.2f\" x2=\"%d\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"0.25\" />\n", Left - 5, y, Right, y );
        fprintf( svg, "<text x=\"%d\" y=\"%.2f\" font-size=\"12\" text-anchor=\"end\">%d</text>\n", Left - 10, y, ( int ) Y );

        for ( int tenths = 1; tenths <= 9; tenths++ )
        {
            double Y2 = Y*tenths/10.0;
            double jj = log10( Y2 );
            double yyy = Bottom - jj * ScaleY;

            fprintf( svg, "<line x1=\"%d\" y1=\"%.2f\" x2=\"%d\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"0.125\" />\n", Left, yyy, Right, yyy );
        }
    }


    //  Add the title.
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin*2, "Log Comparison of Sort Routines" );


    //  Add the legend.
    for ( int s = 0; s < NUM_SORTS; s++ )
    {
        int x = Right - 200;
        int y = Margin + 10 + 20 * s;
        fprintf( svg, "<circle cx=\"%d\" cy=\"%d\" r=\"6\" fill=\"%s\" fill-opacity=\"1.0\" />\n", x, y-3, SortColors[s] );
        fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"14\" text-anchor=\"start\">%s</text>\n", x + 12, y, SortNames[s] );
    }


    int Result = ReadAllTimes( DataFileName );
    assert( ! Result );


    for ( int Sort = 0; Sort < NUM_SORTS; Sort++ )
    {

        //  For each step along the X-axis...
        double xOld = 0;
        double yOld = 0;
        for ( int Step = 17; Step <= 100; Step++ )
        {
            int DataArrangementToDo = RANDOM;

            int SortSize = NumElementsForStep[Step];

            double Microseconds = usMinimums[Sort][DataArrangementToDo][Step];


            //  Draw the data point.

            const char* color = SortColors[Sort];
            if ( SortSize     <=  1              ) { color = "red"; SortSize     = 1; }
            if ( Microseconds <=  1              ) { color = "red"; Microseconds = 1; }
            if ( SortSize     >  MaxSortSize     ) { color = "red"; SortSize     = MaxSortSize; }
            if ( Microseconds >  MaxMicroseconds ) { color = "red"; Microseconds = MaxMicroseconds; }

            double x = Left   + log10( SortSize     ) * ScaleX;
            double y = Bottom - log10( Microseconds ) * ScaleY;
            fprintf( svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"3\" fill=\"%s\" fill-opacity=\"0.5\" />\n", x, y, color );

            if ( xOld && strcmp( color, "red" ) )
            {
                fprintf( svg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n", xOld, yOld, x, y, color );
            }

            xOld = x;
            yOld = y;

        }  //  Step

    }  //  Sort


    // Close the SVG file.
    fprintf( svg, "</svg>\n" );
    fclose( svg );

    return 0;
}

//////////////////////////////////////////////////////////////////////

int Graph( const char* DataFileName )
{
    //  Define the limits of the graph.
    int    MaxSortSize     = 1'000'000;
    double MaxMicroseconds =   100'000;


    // Define some SVG graph constants.
    int Width    =    800;
    int Height   =    800;
    int Margin   =     50;
    int Top      = Margin + 10 + 20 * NUM_SORTS;
    int Bottom   = Height - Margin;
    int Left     = Margin;
    int Right    = Width - Margin;


    //  Open the SVG file.
    char SvgFileName[256];
    strcpy( SvgFileName, DataFileName );
    strcat( SvgFileName, ".svg" );
    FILE *svg;
    int error = fopen_s( &svg, SvgFileName, "w" );
    if ( error ) { perror( "Error opening file" ); return __LINE__; }


    //  Write the SVG header.
    fprintf( svg, "<?xml version=\"1.0\" standalone=\"no\"?>\n" );
    fprintf( svg, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", Width, Height );


    ////  Calculate scales.
    double ScaleX = ( double )( Right - Left ) / MaxSortSize;
    double ScaleY = ( double )( Bottom - Top ) / MaxMicroseconds;


    //  Box the graph.
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Top    , Right , Top    );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Bottom , Right , Bottom );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Left  , Top    , Left  , Bottom );
    fprintf( svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", Right , Top    , Right , Bottom );


     //  Add the axis labels.
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"16\" text-anchor=\"middle\">Integers</text>\n", Width / 2, Bottom + Margin * 2 / 3 );
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"16\" text-anchor=\"start\" transform=\"rotate(-90 %d %d)\">Microseconds</text>\n", Margin / 2, Bottom   , Margin / 2, Bottom   );


    //  Add the x-axis reference lines.
    for ( double i = MaxSortSize/10; i < MaxSortSize ; i += MaxSortSize/10 )
    {
        double x = Left + i * ScaleX;
        double X = i;

        fprintf( svg, "<line x1=\"%.2f\" y1=\"%d\" x2=\"%.2f\" y2=\"%d\" stroke=\"black\" stroke-width=\"0.25\" />\n", x, Top, x, Bottom + 5 );
        fprintf( svg, "<text x=\"%.2f\" y=\"%d\" font-size=\"12\" text-anchor=\"middle\">%d</text>\n", x, Bottom + 15, ( int ) X );
    }


    //  Add the y-axis reference lines.
    for ( double j = MaxMicroseconds/10; j <= MaxMicroseconds; j += MaxMicroseconds/10 )
    {
        double y = Bottom - j * ScaleY;
        double Y = j;

        fprintf( svg, "<line x1=\"%d\" y1=\"%.2f\" x2=\"%d\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"0.25\" />\n", Left - 5, y, Right, y );
        fprintf( svg, "<text x=\"%d\" y=\"%.2f\" font-size=\"12\" text-anchor=\"end\">%d</text>\n", Left - 10, y, ( int ) Y );

        for ( int tenths = 1; tenths <= 9; tenths++ )
        {
            double Y2 = Y*tenths/10.0;
            double jj = log10( Y2 );
            double yyy = Bottom - jj * ScaleY;

            fprintf( svg, "<line x1=\"%d\" y1=\"%.2f\" x2=\"%d\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"0.125\" />\n", Left, yyy, Right, yyy );
        }
    }


    //  Add the title.
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin*2, "Comparison of Sort Routines" );


    //  Add the legend.
    for ( int s = 0; s < NUM_SORTS; s++ )
    {
        int x = Right - 200;
        int y = Margin + 10 + 20 * s;
        fprintf( svg, "<circle cx=\"%d\" cy=\"%d\" r=\"6\" fill=\"%s\" fill-opacity=\"1.0\" />\n", x, y-3, SortColors[s] );
        fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"14\" text-anchor=\"start\">%s</text>\n", x + 12, y, SortNames[s] );
    }


    int Result = ReadAllTimes( DataFileName );
    assert( ! Result );


    for ( int Sort = 0; Sort < NUM_SORTS; Sort++ )
    {

        //  For each step along the X-axis...
        double xOld = 0;
        double yOld = 0;
        for ( int Step = 17; Step <= 100; Step++ )
        {
            int DataArrangementToDo = RANDOM;

            int SortSize = NumElementsForStep[Step];

            double Microseconds = usMinimums[Sort][DataArrangementToDo][Step];


            //  Draw the data point.

            const char* color = SortColors[Sort];
            if ( SortSize     <=  1              ) { color = "red"; SortSize     = 1; }
            if ( Microseconds <=  1              ) { color = "red"; Microseconds = 1; }
            if ( SortSize     >  MaxSortSize     ) { color = "red"; SortSize     = MaxSortSize; }
            if ( Microseconds >  MaxMicroseconds ) { color = "red"; Microseconds = MaxMicroseconds; }

            double x = Left   + SortSize * ScaleX;
            double y = Bottom - Microseconds * ScaleY;
            fprintf( svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"3\" fill=\"%s\" fill-opacity=\"0.5\" />\n", x, y, color );

            if ( xOld && strcmp( color, "red" ) )
            {
                fprintf( svg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n", xOld, yOld, x, y, color );
            }

            xOld = x;
            yOld = y;

        }  //  Step

    }  //  Sort


    // Close the SVG file.
    fprintf( svg, "</svg>\n" );
    fclose( svg );

    return 0;
}

//////////////////////////////////////////////////////////////////////

int main( )
{
    QueryPerformanceFrequency( &TicksPerSec );

    int Result = __LINE__;

    Result = TestSortForOrder( );
    //Result = TestSortForOrderAndStability();
    //Result = ComputeAndWriteRaceResults( );
    //Result = GraphLog ( "..\\RaceResults_2024-12-29_12-38.txt" );
    //Result = Graph    ( "..\\RaceResults_2024-12-29_12-38.txt" );

    return Result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

