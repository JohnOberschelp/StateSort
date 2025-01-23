
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
#include <random>

//////////////////////////////////////////////////////////////////////
//
//  This instrumented element is used to test sort routines for stability.
//
//  The value OriginalPosition is set to where an element was in an array
//  pre-sort, and is examined post-sort.
//

class Element
{

  public:

    int OriginalPosition;
    int Value;

    Element& operator=( const Element& E )
    {
        Value            = E.Value;
        OriginalPosition = E.OriginalPosition;
        return *this;
    }

    bool operator<  ( const Element &E ) const { return Value <  E.Value; }
    bool operator<= ( const Element &E ) const { return Value <= E.Value; }
    bool operator== ( const Element &E ) const { return Value == E.Value; }
    bool operator>= ( const Element &E ) const { return Value >= E.Value; }
    bool operator>  ( const Element &E ) const { return Value >  E.Value; }
    bool operator!= ( const Element &E ) const { return Value != E.Value; }

};

//--------------------------------------------------------------------

#define SORT_ELEMENT Element
#include "../../StateSort/StateSort.h"
#include "../../StateSort/OtherSorts.h"

//////////////////////////////////////////////////////////////////////

struct
{
    bool operator()(Element a, Element b) const
    {
        return a.Value < b.Value;
    }
} LessThanFunction;

//////////////////////////////////////////////////////////////////////

int main( )
{
    std::mt19937 MersenneTwister( 0 );

    int MaxNumElements = 500;

    Element* GoldSortArray = ( Element* ) malloc( MaxNumElements * sizeof( Element ) );
    Element* TestSortArray = ( Element* ) malloc( MaxNumElements * sizeof( Element ) );
    assert( GoldSortArray );
    assert( TestSortArray );
    const char* Name = 0;


    for ( int WhichSort = 0; WhichSort < 8; WhichSort++ )
    {
        bool Sorted = true;
        bool Stable = true;

        for ( int NumElements = 0; NumElements <= MaxNumElements; NumElements++ )
        {

            //  Prepare the elements to be sorted, remembering their original order.
            for ( int i = 0; i < NumElements; i++ )
            {
                Element E;
                E.Value = MersenneTwister() % NumElements;
                E.OriginalPosition = i;
                GoldSortArray[i] = TestSortArray[i] = E;
            }


            //  Do our gold standard stable sort.
            std::stable_sort( GoldSortArray, GoldSortArray + NumElements, LessThanFunction );


            //  Do our test sort.
            switch ( WhichSort )
            {
              case 0: Name = "BubbleSort"       ; BubbleSort       ( TestSortArray, NumElements ); break;
              case 1: Name = "SelectionSort"    ; SelectionSort    ( TestSortArray, NumElements ); break;
              case 2: Name = "ShellSort"        ; ShellSort        ( TestSortArray, NumElements ); break;
              case 3: Name = "HeapSort"         ; HeapSort         ( TestSortArray, NumElements ); break;
              case 4: Name = "LomutosQuickSort" ; LomutosQuickSort ( TestSortArray, 0, NumElements-1 ); break;
              case 5: Name = "HoaresQuickSort"  ; HoaresQuickSort  ( TestSortArray, 0, NumElements-1 ); break;
              case 6: Name = "HybridMergeSort"  ; HybridMergeSort  ( TestSortArray, NumElements, 0  ); break;
              case 7: Name = "StateSort"        ; StateSort        ( TestSortArray, NumElements, 0  ); break;
            }

            for ( int i = 0; i < NumElements; i++ )
                if ( TestSortArray[i].Value != GoldSortArray[i].Value )
                    Sorted = false;

            //  Check that the sort results are identical.
            if ( memcmp( TestSortArray, GoldSortArray, NumElements * sizeof( Element ) ) )
                Stable = false;
        }

        printf( "%-20s %-12s %-12s\n", Name, 
                Sorted ? "Sorted" : "Didn't Sort",
                Stable ? "Stable" : "Not Stable" );

    }

    free( GoldSortArray );
    free( TestSortArray );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

