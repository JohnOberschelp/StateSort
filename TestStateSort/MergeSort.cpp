
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

#include <stdlib.h>

#include "StateSort.h"  //  for a copy for insertionSort

//////////////////////////////////////////////////////////////////////

static void merge( Element* V, int Fm, int Middle, int To, Element* Buffer )
{
    //  Put the elements in the buffer before merging them back.
    for ( int i = Fm; i < To; i++ ) Buffer[i] = V[i];

    //  Merge elements by taking the least from the left or the right half.
    Element* L = Buffer+Fm;
    Element* R = Buffer+Middle;
    V += Fm;
    while ( L < Buffer+Middle && R < Buffer+To ) {
        if ( *L <= *R )  *V++ = *L++;
        else             *V++ = *R++;
    }

    // Copy any remaining elements.
    while ( L < Buffer+Middle ) *V++ = *L++;
    while ( R < Buffer+To     ) *V++ = *R++;
}

//////////////////////////////////////////////////////////////////////

static void recursiveMergeSort( Element* V, int Fm, int To, Element* Buffer )
{

    int NumElements = To - Fm;
    if ( NumElements <= 16 )
    {
        insertionSort( V+Fm, NumElements );
        return;
    }

    int Middle = Fm + NumElements / 2;

    // Sort first and second halves.
    recursiveMergeSort( V, Fm, Middle     , Buffer );
    recursiveMergeSort( V,     Middle, To , Buffer );

    merge( V, Fm, Middle, To, Buffer );

}

//////////////////////////////////////////////////////////////////////

int MergeSort( Element* V, int NumElements, void* LoanerBuffer )
{
    Element* Buffer =
            LoanerBuffer ? ( Element* ) LoanerBuffer
                         : ( Element* ) malloc( NumElements * sizeof( Element ) );
    if ( ! Buffer ) return __LINE__;

    recursiveMergeSort( V, 0, NumElements, Buffer );

    if ( ! LoanerBuffer ) free( Buffer );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

