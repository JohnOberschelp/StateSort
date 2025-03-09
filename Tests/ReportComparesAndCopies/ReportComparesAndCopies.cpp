
//        BSD 3-Clause License
//
//        Copyright (c) 2024-2025, John Oberschelp
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

#include <random>

//////////////////////////////////////////////////////////////////////
//
//  This instrumented element is used to test sort routines
//  for the number of compares and copies they perform.
//
//  To count the number of compares and copies that a sort performs,
//  pre-sort, the global variables ElementCompares and ElementCopies are
//  initialized to zero. During element copies, ElementCopies is incremented.
//  During compares, ElementCompares is incremented.
//  Post-sort, these values are then examined post-sort.
//

    int ElementCompares;
    int ElementCopies;

class Element
{

  public:

    int Value;

    Element(                     ) : Value( 0      ) { }
    Element( int          Int    ) : Value( Int    ) { }
    Element( unsigned int UInt   ) : Value( UInt   ) { }

    Element& operator=( const Element& E )
    {
        ElementCopies++;
        Value = E.Value;
        return *this;
    }

    bool operator<  ( const Element &E ) const { ElementCompares++; return ( int )( Value ) <  ( int )( E.Value ); }
    bool operator<= ( const Element &E ) const { ElementCompares++; return ( int )( Value ) <= ( int )( E.Value ); }
    bool operator== ( const Element &E ) const { ElementCompares++; return ( int )( Value ) == ( int )( E.Value ); }
    bool operator>= ( const Element &E ) const { ElementCompares++; return ( int )( Value ) >= ( int )( E.Value ); }
    bool operator>  ( const Element &E ) const { ElementCompares++; return ( int )( Value ) >  ( int )( E.Value ); }
    bool operator!= ( const Element &E ) const { ElementCompares++; return ( int )( Value ) != ( int )( E.Value ); }

};

//--------------------------------------------------------------------

#define SORT_ELEMENT Element
#include "../../StateSort/StateSort.h"
#include "../../StateSort/OtherSorts.h"

//////////////////////////////////////////////////////////////////////

char* IntWithSeperators( char* Buffer, int V )
{
    int Negative = V < 0;
    V = abs( V );
    char* C = Buffer+30;
    *C-- = 0;
    for ( ;; )
    {
        *C-- = ( V % 10 ) + '0';
        V /= 10;
        if ( ! V ) break;
        if ( ( ( C-Buffer ) % 4 ) == 2 ) *C-- = ',';
    }
    if ( Negative ) *C-- = '-';
    return C+1;
}

//////////////////////////////////////////////////////////////////////

void PrintRow( const char* Name )
{
    char ComparesBuffer [32];
    char CopiesBuffer   [32];
    char* ComparesString = IntWithSeperators( ComparesBuffer , ElementCompares );
    char* CopiesString   = IntWithSeperators( CopiesBuffer   , ElementCopies   );
    printf( "%-20s %15s %15s\n", Name, ComparesString, CopiesString );
}

//////////////////////////////////////////////////////////////////////

int main( )
{

    std::mt19937 MersenneTwister( 0 );
    int NumElements = 10'000;
    SORT_ELEMENT* V = new SORT_ELEMENT[NumElements];

    char NumElementsBuffer[32];
    char* NumElementsString = IntWithSeperators( NumElementsBuffer, NumElements );
    printf( "\nNumber of Compares and Number of Copies Used by Various Sorts\n\n" );
    printf( "Sorting %s Elements, Using std::mt19937 Seeded to 0\n\n", NumElementsString );
    printf( "                            Compares          Copies\n" );
    printf( "                            --------          ------\n" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    BubbleSort( V, NumElements );
    PrintRow( "BubbleSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    SelectionSort( V, NumElements );
    PrintRow( "SelectionSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    ShellSort( V, NumElements );
    PrintRow( "ShellSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    HeapSort( V, NumElements );
    PrintRow( "HeapSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    HoaresQuickSort( V, 0, NumElements-1 );
    PrintRow( "HoaresQuickSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    LomutosQuickSort( V, 0, NumElements-1 );
    PrintRow( "LomutosQuickSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    HybridMergeSort( V, NumElements, 0  );
    PrintRow( "HybridMergeSort" );


    MersenneTwister.seed(0);
    for ( int e = 0; e < NumElements; e++ ) V[e] = MersenneTwister() % NumElements;
    ElementCompares = ElementCopies = 0;
    StateSort( V, NumElements, 0  );
    PrintRow( "StateSort" );


    delete[] V;

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

