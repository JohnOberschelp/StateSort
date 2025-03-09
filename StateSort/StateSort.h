
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

//  StateSort.h

//////////////////////////////////////////////////////////////////////

static inline void insertionSort( SORT_ELEMENT* V, int NumElements )
{
    int i, j;
    SORT_ELEMENT v;
    for ( i = 1; i < NumElements; i++ )
    {
        v = V[i];
        for ( j = i-1; j >= 0 && V[j] > v; j-- ) V[j + 1] = V[j];
        V[j + 1] = v;
    }
}

//////////////////////////////////////////////////////////////////////

static int stateMerge( SORT_ELEMENT *To, SORT_ELEMENT *Fm, SORT_ELEMENT *EndFm, int ElementsPerRun )
{

        SORT_ELEMENT* OriginalTo = To;

        SORT_ELEMENT *A, a, *LastA;
        SORT_ELEMENT *B, b, *LastB;
        SORT_ELEMENT *C, c, *LastC;
        SORT_ELEMENT *D, d, *LastD;


        //  Set up A, B, C, D, a, b, c, d, LastA, LastB, LastC, and LastD
        //  for the merge, based on the number of runs. Whew!
        A = Fm;
        int NumRuns = ( EndFm - A + ElementsPerRun - 1) / ElementsPerRun;
        switch ( NumRuns )
        {
            case 1: LastA = EndFm-1;
                    a = *A;
                    goto Runs1;

            case 2: B = A + ElementsPerRun;
                    LastA = B-1;
                    LastB = EndFm-1;
                    a = *A;
                    b = *B;
                    goto Runs2;

            case 3: B = A + ElementsPerRun;
                    C = B + ElementsPerRun;
                    LastA = B-1;
                    LastB = C-1;
                    LastC = EndFm-1;
                    a = *A;
                    b = *B;
                    c = *C;
                    goto Runs3;

            default:B = A + ElementsPerRun;
                    C = B + ElementsPerRun;
                    D = C + ElementsPerRun;
                    LastA = B-1;
                    LastB = C-1;
                    LastC = D-1;
                    LastD = D + ElementsPerRun;
                    if ( EndFm < LastD ) LastD = EndFm;
                    LastD--;
                    a = *A;
                    b = *B;
                    c = *C;
                    d = *D;
                    break;
        }


        //  Handle four runs here.
        //  In what order are the first elements of A, B, C, and D?

        if (a <= b)
        {
            if (b <= c)
            {
                if (b <= d) { if (c <= d) goto ABCD; goto ABDC; }
                if (a <= d) goto ADBC; goto DABC;
            }

            if (a <= c)
            {
                if (c <= d) { if (b <= d) goto ACBD; goto ACDB; }
                if (a <= d) goto ADCB; goto DACB;
            }

            if (a <= d) { if (b <= d) goto CABD; goto CADB; }
            if (c <= d) goto CDAB; goto DCAB;
        }

        if (a <= c)
        {
            if (a <= d) { if (c <= d) goto BACD; goto BADC; }
            if (b <= d) goto BDAC; goto DBAC;
        }

        if (b <= c)
        {
            if (c <= d) { if (a <= d) goto BCAD; goto BCDA; }
            if (b <= d) goto BDCA; goto DBCA;
        }

        if (b <= d) { if (a <= d) goto CBAD; goto CBDA; }
        if (c <= d) goto CDBA; goto DCBA;


        //  Below are 24 labels, one per state for every
        //  possible ordering of four runs.

ABCD:   do { *To++ = a; if (A == LastA) goto BCD; a = *(++A); } while (a <= b);
        if (a <= c) goto BACD; if (a <= d) goto BCAD; goto BCDA;

ABDC:   do { *To++ = a; if (A == LastA) goto BDC; a = *(++A); } while (a <= b);
        if (a <= d) goto BADC; if (a <= c) goto BDAC; goto BDCA;

ACBD:   do { *To++ = a; if (A == LastA) goto CBD; a = *(++A); } while (a <= c);
        if (a <= b) goto CABD; if (a <= d) goto CBAD; goto CBDA;

ACDB:   do { *To++ = a; if (A == LastA) goto CDB; a = *(++A); } while (a <= c);
        if (a <= d) goto CADB; if (a <= b) goto CDAB; goto CDBA;

ADBC:   do { *To++ = a; if (A == LastA) goto DBC; a = *(++A); } while (a <= d);
        if (a <= b) goto DABC; if (a <= c) goto DBAC; goto DBCA;

ADCB:   do { *To++ = a; if (A == LastA) goto DCB; a = *(++A); } while (a <= d);
        if (a <= c) goto DACB; if (a <= b) goto DCAB; goto DCBA;

BACD:   do { *To++ = b; if (B == LastB) goto ACD; b = *(++B); } while (b < a);
        if (b <= c) goto ABCD; if (b <= d) goto ACBD; goto ACDB;

BADC:   do { *To++ = b; if (B == LastB) goto ADC; b = *(++B); } while (b < a);
        if (b <= d) goto ABDC; if (b <= c) goto ADBC; goto ADCB;

BCAD:   do { *To++ = b; if (B == LastB) goto CAD; b = *(++B); } while (b <= c);
        if (b < a) goto CBAD; if (b <= d) goto CABD; goto CADB;

BCDA:   do { *To++ = b; if (B == LastB) goto CDA; b = *(++B); } while (b <= c);
        if (b <= d) goto CBDA; if (b < a) goto CDBA; goto CDAB;

BDAC:   do { *To++ = b; if (B == LastB) goto DAC; b = *(++B); } while (b <= d);
        if (b < a) goto DBAC; if (b <= c) goto DABC; goto DACB;

BDCA:   do { *To++ = b; if (B == LastB) goto DCA; b = *(++B); } while (b <= d);
        if (b <= c) goto DBCA; if (b < a) goto DCBA; goto DCAB;

CABD:   do { *To++ = c; if (C == LastC) goto ABD; c = *(++C); } while (c < a);
        if (c < b) goto ACBD; if (c <= d) goto ABCD; goto ABDC;

CADB:   do { *To++ = c; if (C == LastC) goto ADB; c = *(++C); } while (c < a);
        if (c <= d) goto ACDB; if (c < b) goto ADCB; goto ADBC;

CBAD:   do { *To++ = c; if (C == LastC) goto BAD; c = *(++C); } while (c < b);
        if (c < a) goto BCAD; if (c <= d) goto BACD; goto BADC;

CBDA:   do { *To++ = c; if (C == LastC) goto BDA; c = *(++C); } while (c < b);
        if (c <= d) goto BCDA; if (c < a) goto BDCA; goto BDAC;

CDAB:   do { *To++ = c; if (C == LastC) goto DAB; c = *(++C); } while (c <= d);
        if (c < a) goto DCAB; if (c < b) goto DACB; goto DABC;

CDBA:   do { *To++ = c; if (C == LastC) goto DBA; c = *(++C); } while (c <= d);
        if (c < b) goto DCBA; if (c < a) goto DBCA; goto DBAC;

DABC:   do { *To++ = d; if (D == LastD) goto ABC; d = *(++D); } while (d < a);
        if (d < b) goto ADBC; if (d < c) goto ABDC; goto ABCD;

DACB:   do { *To++ = d; if (D == LastD) goto ACB; d = *(++D); } while (d < a);
        if (d < c) goto ADCB; if (d < b) goto ACDB; goto ACBD;

DBAC:   do { *To++ = d; if (D == LastD) goto BAC; d = *(++D); } while (d < b);
        if (d < a) goto BDAC; if (d < c) goto BADC; goto BACD;

DBCA:   do { *To++ = d; if (D == LastD) goto BCA; d = *(++D); } while (d < b);
        if (d < c) goto BDCA; if (d < a) goto BCDA; goto BCAD;

DCAB:   do { *To++ = d; if (D == LastD) goto CAB; d = *(++D); } while (d < c);
        if (d < a) goto CDAB; if (d < b) goto CADB; goto CABD;

DCBA:   do { *To++ = d; if (D == LastD) goto CBA; d = *(++D); } while (d < c);
        if (d < b) goto CDBA; if (d < a) goto CBDA; goto CBAD;


Runs3:  //  Handle three runs here.
        //  In what order are the first elements of A, B, and C?

        if (a <= b)
        {
            if (b <= c) goto ABC;
            if (a <= c) goto ACB;
            goto CAB;
        }
        if (a <= c) goto BAC;
        if (b <= c) goto BCA;
        goto CBA;


        //  Below are 24 labels, one per state for every possible ordering,
        //  once 1 run is exhausted, and we only have 3 left.

ABC:    do { *To++ = a; if (A == LastA) goto BC; a = *(++A); } while (a <= b);
        if (a <= c) goto BAC; goto BCA;

ABD:    do { *To++ = a; if (A == LastA) goto BD; a = *(++A); } while (a <= b);
        if (a <= d) goto BAD; goto BDA;

ACB:    do { *To++ = a; if (A == LastA) goto CB; a = *(++A); } while (a <= c);
        if (a <= b) goto CAB; goto CBA;

ACD:    do { *To++ = a; if (A == LastA) goto CD; a = *(++A); } while (a <= c);
        if (a <= d) goto CAD; goto CDA;

ADB:    do { *To++ = a; if (A == LastA) goto DB; a = *(++A); } while (a <= d);
        if (a <= b) goto DAB; goto DBA;

ADC:    do { *To++ = a; if (A == LastA) goto DC; a = *(++A); } while (a <= d);
        if (a <= c) goto DAC; goto DCA;

BAC:    do { *To++ = b; if (B == LastB) goto AC; b = *(++B); } while (b < a);
        if (b <= c) goto ABC; goto ACB;

BAD:    do { *To++ = b; if (B == LastB) goto AD; b = *(++B); } while (b < a);
        if (b <= d) goto ABD; goto ADB;

BCA:    do { *To++ = b; if (B == LastB) goto CA; b = *(++B); } while (b <= c);
        if (b < a) goto CBA; goto CAB;

BCD:    do { *To++ = b; if (B == LastB) goto CD; b = *(++B); } while (b <= c);
        if (b <= d) goto CBD; goto CDB;

BDA:    do { *To++ = b; if (B == LastB) goto DA; b = *(++B); } while (b <= d);
        if (b < a) goto DBA; goto DAB;

BDC:    do { *To++ = b; if (B == LastB) goto DC; b = *(++B); } while (b <= d);
        if (b <= c) goto DBC; goto DCB;

CAB:    do { *To++ = c; if (C == LastC) goto AB; c = *(++C); } while (c < a);
        if (c < b) goto ACB; goto ABC;

CAD:    do { *To++ = c; if (C == LastC) goto AD; c = *(++C); } while (c < a);
        if (c <= d) goto ACD; goto ADC;

CBA:    do { *To++ = c; if (C == LastC) goto BA; c = *(++C); } while (c < b);
        if (c < a) goto BCA; goto BAC;

CBD:    do { *To++ = c; if (C == LastC) goto BD; c = *(++C); } while (c < b);
        if (c <= d) goto BCD; goto BDC;

CDA:    do { *To++ = c; if (C == LastC) goto DA; c = *(++C); } while (c <= d);
        if (c < a) goto DCA; goto DAC;

CDB:    do { *To++ = c; if (C == LastC) goto DB; c = *(++C); } while (c <= d);
        if (c < b) goto DCB; goto DBC;

DAB:    do { *To++ = d; if (D == LastD) goto AB; d = *(++D); } while (d < a);
        if (d < b) goto ADB; goto ABD;

DAC:    do { *To++ = d; if (D == LastD) goto AC; d = *(++D); } while (d < a);
        if (d < c) goto ADC; goto ACD;

DBA:    do { *To++ = d; if (D == LastD) goto BA; d = *(++D); } while (d < b);
        if (d < a) goto BDA; goto BAD;

DBC:    do { *To++ = d; if (D == LastD) goto BC; d = *(++D); } while (d < b);
        if (d < c) goto BDC; goto BCD;

DCA:    do { *To++ = d; if (D == LastD) goto CA; d = *(++D); } while (d < c);
        if (d < a) goto CDA; goto CAD;

DCB:    do { *To++ = d; if (D == LastD) goto CB; d = *(++D); } while (d < c);
        if (d < b) goto CDB; goto CBD;


Runs2:  //  Handle two runs here.
        //  In what order are the first elements of A and B?

        if (a <= b) goto AB;
        goto BA;


        //  Below are 12 labels, one per state for every possible ordering,
        //  once 2 runs are exhausted, and we only have 2 left.

AB:     do { *To++ = a; if (A == LastA) goto JustB; a = *(++A); } while (a <= b);
        goto BA;

AC:     do { *To++ = a; if (A == LastA) goto JustC; a = *(++A); } while (a <= c);
        goto CA;

AD:     do { *To++ = a; if (A == LastA) goto JustD; a = *(++A); } while (a <= d);
        goto DA;

BA:     do { *To++ = b; if (B == LastB) goto JustA; b = *(++B); } while (b < a);
        goto AB;

BC:     do { *To++ = b; if (B == LastB) goto JustC; b = *(++B); } while (b <= c);
        goto CB;

BD:     do { *To++ = b; if (B == LastB) goto JustD; b = *(++B); } while (b <= d);
        goto DB;

CA:     do { *To++ = c; if (C == LastC) goto JustA; c = *(++C); } while (c < a);
        goto AC;

CB:     do { *To++ = c; if (C == LastC) goto JustB; c = *(++C); } while (c < b);
        goto BC;

CD:     do { *To++ = c; if (C == LastC) goto JustD; c = *(++C); } while (c <= d);
        goto DC;

DA:     do { *To++ = d; if (D == LastD) goto JustA; d = *(++D); } while (d < a);
        goto AD;

DB:     do { *To++ = d; if (D == LastD) goto JustB; d = *(++D); } while (d < b);
        goto BD;

DC:     do { *To++ = d; if (D == LastD) goto JustC; d = *(++D); } while (d < c);
        goto CD;


Runs1:  //  Handle a single run here.
        //  Fall through to JustA.


        //  Below are 4 labels, one per state once 3 runs are exhausted,
        //  and we only have 1 left.

JustA:  *To++ = a; if (A == LastA) goto DoneWithMerge; a = *(++A); goto JustA;
JustB:  *To++ = b; if (B == LastB) goto DoneWithMerge; b = *(++B); goto JustB;
JustC:  *To++ = c; if (C == LastC) goto DoneWithMerge; c = *(++C); goto JustC;
JustD:  *To++ = d; if (D == LastD) goto DoneWithMerge; d = *(++D); goto JustD;


DoneWithMerge:

        return To - OriginalTo;  //  Return the number of elements merged.
}

//////////////////////////////////////////////////////////////////////

int StateSort( SORT_ELEMENT* V, int NumElements, void* LoanerBuffer )
{
    if ( NumElements < 2 ) return 0;

    SORT_ELEMENT* Buffer =
            LoanerBuffer ? ( SORT_ELEMENT* ) LoanerBuffer
                         : ( SORT_ELEMENT* ) malloc( NumElements * sizeof( SORT_ELEMENT ) );

    if ( ! Buffer ) return __LINE__;


    //  Make initial sorted runs in place, using InsertionSort.
    int ElementsPerRun = 30;
    int HowManyRuns     = NumElements / ElementsPerRun;
    int HowManyLeftOver = NumElements % ElementsPerRun;
    SORT_ELEMENT* PastWholeRuns = V + NumElements - HowManyLeftOver;
    for ( SORT_ELEMENT* A = V; A < PastWholeRuns; A += ElementsPerRun )
    {
        insertionSort( A, ElementsPerRun );
    }
    if ( HowManyLeftOver > 1 )
    {
        insertionSort( PastWholeRuns, HowManyLeftOver );
    }


    //  Make as many merging passes as needed,
    //  making the runs four times larger each pass.

    int DataIsInTheBuffer;
    for (;;)
    {

        //  Make a merge pass from V to the buffer.
        int NumMerged = 0;
        for ( ;; )
        {
            if ( NumMerged >= NumElements ) { DataIsInTheBuffer = 1; break; }

            NumMerged += stateMerge( Buffer+NumMerged, V+NumMerged, V+NumElements, ElementsPerRun );
        }
        //  Finished the pass.
        ElementsPerRun *= 4;
        if ( ElementsPerRun >= NumElements ) break;


        //  Make a merge pass from the buffer to V.
        NumMerged = 0;
        for ( ;; )
        {
            if ( NumMerged >= NumElements ) { DataIsInTheBuffer = 0; break; }

            NumMerged += stateMerge( V+NumMerged, Buffer+NumMerged, Buffer+NumElements, ElementsPerRun );
        }
        //  Finished the pass.
        ElementsPerRun *= 4;
        if ( ElementsPerRun >= NumElements ) break;

    }

    //  Finished all passes.

    //  Did we end up with our sorted data in the buffer?
    if ( DataIsInTheBuffer )
    {
        for ( int i = 0; i < NumElements; i++ ) V[i] = Buffer[i];
    }

    if ( ! LoanerBuffer ) free( Buffer );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

