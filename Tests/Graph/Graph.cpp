
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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  Shell_SORT       = 1,
  MERGE_SORT      = 2,
  STD_SORT        = 3,
  STD_STABLE_SORT = 4,
  STATE_SORT      = 5,
} SORT_TYPE ; 


const char* SortColors[] = {
    "black",
    "gray",
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

int ReadAllTimes( const char* FileName )
{
    const int MAX_LINE = 10'000;
    FILE *File;
    char Line[MAX_LINE]; 
    char *Token;

    int Err = fopen_s( &File, FileName, "rb" );
    assert( ! Err );
    assert( File );


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

void DataFileNameToGraphFileName( const char* DataFileName, const char* Type, char* GraphFileName )
{
    const char* NameBeg = strstr( DataFileName, "Results_" );
    strcpy( GraphFileName, NameBeg );
    char* NameEnd = strstr( GraphFileName, ".txt" );
    NameEnd[1] = 0;
    strcat( GraphFileName, Type );
    strcat( GraphFileName, ".svg" );
}

//////////////////////////////////////////////////////////////////////

int GraphLogarithmic( const char* DataFileName )
{
    //  Define the limits of the graph.
    int    MaxSortSize     = 1'000'000;
    double MaxMicroseconds =   100'000;


    // Define some SVG graph constants.
    int Width    =    800;
    int Height   =    800;
    int Margin   =     50;
    int Top      = Margin/2 + 20 * NUM_SORTS;
    int Bottom   = Height - Margin;
    int Left     = Margin;
    int Right    = Width - Margin;


    //  Open the SVG file.
    char SvgFileName[256];
    DataFileNameToGraphFileName( DataFileName, "Logarithmic", SvgFileName );


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
    fprintf( svg, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\" />" );
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
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin     , "Logarithmic Comparison" );
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin + 35, "of Sort Routines" );


    //  Add the legend.
    for ( int s = 0; s < NUM_SORTS; s++ )
    {
        int x = Right - 200;
        int y = Margin/2 + 20 * s;
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

int GraphLinear( const char* DataFileName )
{
    //  Define the limits of the graph.
    int    MaxSortSize     = 1'000'000;
    double MaxMicroseconds =   100'000;


    // Define some SVG graph constants.
    int Width    =    800;
    int Height   =    800;
    int Margin   =     50;
    int Top      = Margin/2 + 20 * NUM_SORTS;
    int Bottom   = Height - Margin;
    int Left     = Margin;
    int Right    = Width - Margin;


    //  Open the SVG file.
    char SvgFileName[256];
    DataFileNameToGraphFileName( DataFileName, "Linear", SvgFileName );


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
    fprintf( svg, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\" />" );
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
        if ( j > MaxMicroseconds/10 )
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
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin     , "Linear Comparison" );
    fprintf( svg, "<text x=\"%d\" y=\"%d\" font-size=\"28\" text-anchor=\"start\">%s</text>\n", Margin*2, Margin + 35, "of Sort Routines" );


    //  Add the legend.
    for ( int s = 0; s < NUM_SORTS; s++ )
    {
        int x = Right - 200;
        int y = Margin/2 + 20 * s;
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
    GraphLogarithmic ( "..\\TestSpeed\\Results_2025-01-22_13-46.txt" );
    GraphLinear      ( "..\\TestSpeed\\Results_2025-01-22_13-46.txt" );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

