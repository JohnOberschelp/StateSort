
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

#if !defined( STATESORT_ELEMENT ) 
#error STATESORT_ELEMENT must be defined before including StateSort.h
#endif

//////////////////////////////////////////////////////////////////////

inline void insertionSort( STATESORT_ELEMENT* V, int NumElements )
{
    int i, j;
    STATESORT_ELEMENT v;
    for ( i = 1; i < NumElements; i++ )
    {
        v = V[i];
        for ( j = i-1; j >= 0 && V[j] > v; j-- )
        {
            V[j + 1] = V[j];
        }
        V[j + 1] = v;
    }
}

//////////////////////////////////////////////////////////////////////

inline void moveARun( STATESORT_ELEMENT* S, STATESORT_ELEMENT* A, STATESORT_ELEMENT* B )
{
    while ( A < B ) *S++ = *A++;
}

//////////////////////////////////////////////////////////////////////

static void merge2Runs( STATESORT_ELEMENT* S, STATESORT_ELEMENT* A, STATESORT_ELEMENT* B, STATESORT_ELEMENT* C )
{
    STATESORT_ELEMENT* LastA = B-1;
    STATESORT_ELEMENT* LastB = C-1;

    STATESORT_ELEMENT a = *A;
    STATESORT_ELEMENT b = *B;


    if (a > b) goto BA;


AB:     *S++ = a; if (A == LastA) goto JustB; a = *(++A); if (a <= b) goto AB; goto BA;
BA:     *S++ = b; if (B == LastB) goto JustA; b = *(++B); if (b <  a) goto BA; goto AB;


JustA:  *S++ = a; if (A == LastA) return; a = *(++A); goto JustA;
JustB:  *S++ = b; if (B == LastB) return; b = *(++B); goto JustB;

}

//////////////////////////////////////////////////////////////////////

static void merge3Runs( STATESORT_ELEMENT* S, STATESORT_ELEMENT* A, STATESORT_ELEMENT* B, STATESORT_ELEMENT* C, STATESORT_ELEMENT* D )
{
    STATESORT_ELEMENT* LastA = B-1;
    STATESORT_ELEMENT* LastB = C-1;
    STATESORT_ELEMENT* LastC = D-1;

    STATESORT_ELEMENT a = *A;
    STATESORT_ELEMENT b = *B;
    STATESORT_ELEMENT c = *C;


    if (a <= b)
    {
        if (b <= c) goto ABC;
        if (a <= c) goto ACB;
        goto CAB;
    }
    if (a <= c) goto BAC;
    if (b <= c) goto BCA;
    goto CBA;


ABC:    *S++ = a; if (A == LastA) goto BC; a = *(++A); if (a <= b) goto ABC; if (a <= c) goto BAC; goto BCA; 
ACB:    *S++ = a; if (A == LastA) goto CB; a = *(++A); if (a <= c) goto ACB; if (a <= b) goto CAB; goto CBA; 
BAC:    *S++ = b; if (B == LastB) goto AC; b = *(++B); if (b <  a) goto BAC; if (b <= c) goto ABC; goto ACB; 
BCA:    *S++ = b; if (B == LastB) goto CA; b = *(++B); if (b <= c) goto BCA; if (b <  a) goto CBA; goto CAB; 
CAB:    *S++ = c; if (C == LastC) goto AB; c = *(++C); if (c <  a) goto CAB; if (c <  b) goto ACB; goto ABC; 
CBA:    *S++ = c; if (C == LastC) goto BA; c = *(++C); if (c <  b) goto CBA; if (c <  a) goto BCA; goto BAC; 


AB:     *S++ = a; if (A == LastA) goto JustB; a = *(++A); if (a <= b) goto AB; goto BA;
AC:     *S++ = a; if (A == LastA) goto JustC; a = *(++A); if (a <= c) goto AC; goto CA;

BA:     *S++ = b; if (B == LastB) goto JustA; b = *(++B); if (b <  a) goto BA; goto AB;
BC:     *S++ = b; if (B == LastB) goto JustC; b = *(++B); if (b <= c) goto BC; goto CB;

CA:     *S++ = c; if (C == LastC) goto JustA; c = *(++C); if (c <  a) goto CA; goto AC;
CB:     *S++ = c; if (C == LastC) goto JustB; c = *(++C); if (c <  b) goto CB; goto BC;


JustA:  *S++ = a; if (A == LastA) return; a = *(++A); goto JustA;
JustB:  *S++ = b; if (B == LastB) return; b = *(++B); goto JustB;
JustC:  *S++ = c; if (C == LastC) return; c = *(++C); goto JustC;

}

//////////////////////////////////////////////////////////////////////

static void merge4Runs( STATESORT_ELEMENT* S, STATESORT_ELEMENT* A, STATESORT_ELEMENT* B, STATESORT_ELEMENT* C, STATESORT_ELEMENT* D, STATESORT_ELEMENT* E )
{
    STATESORT_ELEMENT* LastA = B-1;
    STATESORT_ELEMENT* LastB = C-1;
    STATESORT_ELEMENT* LastC = D-1;
    STATESORT_ELEMENT* LastD = E-1;

    STATESORT_ELEMENT a = *A;
    STATESORT_ELEMENT b = *B;
    STATESORT_ELEMENT c = *C;
    STATESORT_ELEMENT d = *D;


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


ABCD:	*S++ = a; if (A == LastA) goto BCD; a = *(++A); if (a <= c) { if (a <= b) goto ABCD; goto BACD; } if (a <= d) goto BCAD; goto BCDA;
ABDC:	*S++ = a; if (A == LastA) goto BDC; a = *(++A); if (a <= d) { if (a <= b) goto ABDC; goto BADC; } if (a <= c) goto BDAC; goto BDCA;
ACBD:	*S++ = a; if (A == LastA) goto CBD; a = *(++A); if (a <= b) { if (a <= c) goto ACBD; goto CABD; } if (a <= d) goto CBAD; goto CBDA;
ACDB:	*S++ = a; if (A == LastA) goto CDB; a = *(++A); if (a <= d) { if (a <= c) goto ACDB; goto CADB; } if (a <= b) goto CDAB; goto CDBA;
ADBC:	*S++ = a; if (A == LastA) goto DBC; a = *(++A); if (a <= b) { if (a <= d) goto ADBC; goto DABC; } if (a <= c) goto DBAC; goto DBCA;
ADCB:	*S++ = a; if (A == LastA) goto DCB; a = *(++A); if (a <= c) { if (a <= d) goto ADCB; goto DACB; } if (a <= b) goto DCAB; goto DCBA;

BACD:   *S++ = b; if (B == LastB) goto ACD; b = *(++B); if (b <= c) { if (b <  a) goto BACD; goto ABCD; } if (b <= d) goto ACBD; goto ACDB; 
BADC:   *S++ = b; if (B == LastB) goto ADC; b = *(++B); if (b <= d) { if (b <  a) goto BADC; goto ABDC; } if (b <= c) goto ADBC; goto ADCB; 
BCAD:   *S++ = b; if (B == LastB) goto CAD; b = *(++B); if (b <  a) { if (b <= c) goto BCAD; goto CBAD; } if (b <= d) goto CABD; goto CADB; 
BCDA:   *S++ = b; if (B == LastB) goto CDA; b = *(++B); if (b <= d) { if (b <= c) goto BCDA; goto CBDA; } if (b <  a) goto CDBA; goto CDAB; 
BDAC:   *S++ = b; if (B == LastB) goto DAC; b = *(++B); if (b <  a) { if (b <= d) goto BDAC; goto DBAC; } if (b <= c) goto DABC; goto DACB; 
BDCA:   *S++ = b; if (B == LastB) goto DCA; b = *(++B); if (b <= c) { if (b <= d) goto BDCA; goto DBCA; } if (b <  a) goto DCBA; goto DCAB; 

CABD:   *S++ = c; if (C == LastC) goto ABD; c = *(++C); if (c <  b) { if (c <  a) goto CABD; goto ACBD; } if (c <= d) goto ABCD; goto ABDC; 
CADB:   *S++ = c; if (C == LastC) goto ADB; c = *(++C); if (c <= d) { if (c <  a) goto CADB; goto ACDB; } if (c <  b) goto ADCB; goto ADBC; 
CBAD:   *S++ = c; if (C == LastC) goto BAD; c = *(++C); if (c <  a) { if (c <  b) goto CBAD; goto BCAD; } if (c <= d) goto BACD; goto BADC; 
CBDA:   *S++ = c; if (C == LastC) goto BDA; c = *(++C); if (c <= d) { if (c <  b) goto CBDA; goto BCDA; } if (c <  a) goto BDCA; goto BDAC; 
CDAB:   *S++ = c; if (C == LastC) goto DAB; c = *(++C); if (c <  a) { if (c <= d) goto CDAB; goto DCAB; } if (c <  b) goto DACB; goto DABC; 
CDBA:   *S++ = c; if (C == LastC) goto DBA; c = *(++C); if (c <  b) { if (c <= d) goto CDBA; goto DCBA; } if (c <  a) goto DBCA; goto DBAC; 

DABC:   *S++ = d; if (D == LastD) goto ABC; d = *(++D); if (d <  b) { if (d <  a) goto DABC; goto ADBC; } if (d <  c) goto ABDC; goto ABCD; 
DACB:   *S++ = d; if (D == LastD) goto ACB; d = *(++D); if (d <  c) { if (d <  a) goto DACB; goto ADCB; } if (d <  b) goto ACDB; goto ACBD; 
DBAC:   *S++ = d; if (D == LastD) goto BAC; d = *(++D); if (d <  a) { if (d <  b) goto DBAC; goto BDAC; } if (d <  c) goto BADC; goto BACD; 
DBCA:   *S++ = d; if (D == LastD) goto BCA; d = *(++D); if (d <  c) { if (d <  b) goto DBCA; goto BDCA; } if (d <  a) goto BCDA; goto BCAD; 
DCAB:   *S++ = d; if (D == LastD) goto CAB; d = *(++D); if (d <  a) { if (d <  c) goto DCAB; goto CDAB; } if (d <  b) goto CADB; goto CABD; 
DCBA:   *S++ = d; if (D == LastD) goto CBA; d = *(++D); if (d <  b) { if (d <  c) goto DCBA; goto CDBA; } if (d <  a) goto CBDA; goto CBAD; 


ABC:    *S++ = a; if (A == LastA) goto BC; a = *(++A); if (a <= b) goto ABC; if (a <= c) goto BAC; goto BCA; 
ACB:    *S++ = a; if (A == LastA) goto CB; a = *(++A); if (a <= c) goto ACB; if (a <= b) goto CAB; goto CBA; 
BAC:    *S++ = b; if (B == LastB) goto AC; b = *(++B); if (b <  a) goto BAC; if (b <= c) goto ABC; goto ACB; 
BCA:    *S++ = b; if (B == LastB) goto CA; b = *(++B); if (b <= c) goto BCA; if (b <  a) goto CBA; goto CAB; 
CAB:    *S++ = c; if (C == LastC) goto AB; c = *(++C); if (c <  a) goto CAB; if (c <  b) goto ACB; goto ABC; 
CBA:    *S++ = c; if (C == LastC) goto BA; c = *(++C); if (c <  b) goto CBA; if (c <  a) goto BCA; goto BAC; 

ABD:    *S++ = a; if (A == LastA) goto BD; a = *(++A); if (a <= b) goto ABD; if (a <= d) goto BAD; goto BDA; 
ADB:    *S++ = a; if (A == LastA) goto DB; a = *(++A); if (a <= d) goto ADB; if (a <= b) goto DAB; goto DBA; 
BAD:    *S++ = b; if (B == LastB) goto AD; b = *(++B); if (b <  a) goto BAD; if (b <= d) goto ABD; goto ADB; 
BDA:    *S++ = b; if (B == LastB) goto DA; b = *(++B); if (b <= d) goto BDA; if (b <  a) goto DBA; goto DAB; 
DAB:    *S++ = d; if (D == LastD) goto AB; d = *(++D); if (d <  a) goto DAB; if (d <  b) goto ADB; goto ABD; 
DBA:    *S++ = d; if (D == LastD) goto BA; d = *(++D); if (d <  b) goto DBA; if (d <  a) goto BDA; goto BAD; 

ACD:    *S++ = a; if (A == LastA) goto CD; a = *(++A); if (a <= c) goto ACD; if (a <= d) goto CAD; goto CDA; 
ADC:    *S++ = a; if (A == LastA) goto DC; a = *(++A); if (a <= d) goto ADC; if (a <= c) goto DAC; goto DCA; 
CAD:    *S++ = c; if (C == LastC) goto AD; c = *(++C); if (c <  a) goto CAD; if (c <= d) goto ACD; goto ADC; 
CDA:    *S++ = c; if (C == LastC) goto DA; c = *(++C); if (c <= d) goto CDA; if (c <  a) goto DCA; goto DAC; 
DAC:    *S++ = d; if (D == LastD) goto AC; d = *(++D); if (d <  a) goto DAC; if (d <  c) goto ADC; goto ACD; 
DCA:    *S++ = d; if (D == LastD) goto CA; d = *(++D); if (d <  c) goto DCA; if (d <  a) goto CDA; goto CAD; 

BCD:    *S++ = b; if (B == LastB) goto CD; b = *(++B); if (b <= c) goto BCD; if (b <= d) goto CBD; goto CDB; 
BDC:    *S++ = b; if (B == LastB) goto DC; b = *(++B); if (b <= d) goto BDC; if (b <= c) goto DBC; goto DCB; 
CBD:    *S++ = c; if (C == LastC) goto BD; c = *(++C); if (c <  b) goto CBD; if (c <= d) goto BCD; goto BDC; 
CDB:    *S++ = c; if (C == LastC) goto DB; c = *(++C); if (c <= d) goto CDB; if (c <  b) goto DCB; goto DBC; 
DBC:    *S++ = d; if (D == LastD) goto BC; d = *(++D); if (d <  b) goto DBC; if (d <  c) goto BDC; goto BCD; 
DCB:    *S++ = d; if (D == LastD) goto CB; d = *(++D); if (d <  c) goto DCB; if (d <  b) goto CDB; goto CBD; 


AB:     *S++ = a; if (A == LastA) goto JustB; a = *(++A); if (a <= b) goto AB; goto BA;
AC:     *S++ = a; if (A == LastA) goto JustC; a = *(++A); if (a <= c) goto AC; goto CA;
AD:     *S++ = a; if (A == LastA) goto JustD; a = *(++A); if (a <= d) goto AD; goto DA;

BA:     *S++ = b; if (B == LastB) goto JustA; b = *(++B); if (b <  a) goto BA; goto AB;
BC:     *S++ = b; if (B == LastB) goto JustC; b = *(++B); if (b <= c) goto BC; goto CB;
BD:     *S++ = b; if (B == LastB) goto JustD; b = *(++B); if (b <= d) goto BD; goto DB;

CA:     *S++ = c; if (C == LastC) goto JustA; c = *(++C); if (c <  a) goto CA; goto AC;
CB:     *S++ = c; if (C == LastC) goto JustB; c = *(++C); if (c <  b) goto CB; goto BC;
CD:     *S++ = c; if (C == LastC) goto JustD; c = *(++C); if (c <= d) goto CD; goto DC;

DA:     *S++ = d; if (D == LastD) goto JustA; d = *(++D); if (d <  a) goto DA; goto AD;
DB:     *S++ = d; if (D == LastD) goto JustB; d = *(++D); if (d <  b) goto DB; goto BD;
DC:     *S++ = d; if (D == LastD) goto JustC; d = *(++D); if (d <  c) goto DC; goto CD;


JustA:  *S++ = a; if (A == LastA) return; a = *(++A); goto JustA;
JustB:  *S++ = b; if (B == LastB) return; b = *(++B); goto JustB;
JustC:  *S++ = c; if (C == LastC) return; c = *(++C); goto JustC;
JustD:  *S++ = d; if (D == LastD) return; d = *(++D); goto JustD;

}

//////////////////////////////////////////////////////////////////////

static void merge5RunsN( STATESORT_ELEMENT* S, STATESORT_ELEMENT* A, STATESORT_ELEMENT* B, STATESORT_ELEMENT* C, STATESORT_ELEMENT* D, STATESORT_ELEMENT* E, STATESORT_ELEMENT* F, int N )
{
    int RunLength = ( int ) ( B - A );

    while ( N-- )
    {
        STATESORT_ELEMENT* LastA = B-1;
        STATESORT_ELEMENT* LastB = C-1;
        STATESORT_ELEMENT* LastC = D-1;
        STATESORT_ELEMENT* LastD = E-1;
        STATESORT_ELEMENT* LastE = F-1;

        STATESORT_ELEMENT a = *A;
        STATESORT_ELEMENT b = *B;
        STATESORT_ELEMENT c = *C;
        STATESORT_ELEMENT d = *D;
        STATESORT_ELEMENT e = *E;


        if (a <= b)
        {
            if (b <= c)
            {
                if (b <= d) { if (c <= d) goto eABCD; goto eABDC; }
                if (a <= d) goto eADBC; goto eDABC;
            }
            if (a <= c)
            {
                if (c <= d) { if (b <= d) goto eACBD; goto eACDB; }
                if (a <= d) goto eADCB; goto eDACB;
            }
            if (a <= d) { if (b <= d) goto eCABD; goto eCADB; }
            if (c <= d) goto eCDAB; goto eDCAB;
        }
        if (a <= c)
        {
            if (a <= d) { if (c <= d) goto eBACD; goto eBADC; }
            if (b <= d) goto eBDAC; goto eDBAC;
        }
        if (b <= c)
        {
            if (c <= d) { if (a <= d) goto eBCAD; goto eBCDA; }
            if (b <= d) goto eBDCA; goto eDBCA;
        }
        if (b <= d) { if (a <= d) goto eCBAD; goto eCBDA; }
        if (c <= d) goto eCDBA; goto eDCBA;



        //  Below are 24 labels, one per state for every ordering of
        //  runs A through D, when we do not yet know where E fits in.

eABCD:  if (e < b) { if (e < a) goto EABCD; goto AEBCD; }
        if (e < d) { if (e < c) goto ABECD; goto ABCED; } goto ABCDE;

eABDC:  if (e < b) { if (e < a) goto EABDC; goto AEBDC; }
        if (e < c) { if (e < d) goto ABEDC; goto ABDEC; } goto ABDCE;

eACBD:  if (e < c) { if (e < a) goto EACBD; goto AECBD; }
        if (e < d) { if (e < b) goto ACEBD; goto ACBED; } goto ACBDE;

eACDB:  if (e < c) { if (e < a) goto EACDB; goto AECDB; }
        if (e < b) { if (e < d) goto ACEDB; goto ACDEB; } goto ACDBE;

eADBC:  if (e < d) { if (e < a) goto EADBC; goto AEDBC; }
        if (e < c) { if (e < b) goto ADEBC; goto ADBEC; } goto ADBCE;

eADCB:  if (e < d) { if (e < a) goto EADCB; goto AEDCB; }
        if (e < b) { if (e < c) goto ADECB; goto ADCEB; } goto ADCBE;

eBACD:  if (e < a) { if (e < b) goto EBACD; goto BEACD; }
        if (e < d) { if (e < c) goto BAECD; goto BACED; } goto BACDE;

eBADC:  if (e < a) { if (e < b) goto EBADC; goto BEADC; }
        if (e < c) { if (e < d) goto BAEDC; goto BADEC; } goto BADCE;

eBCAD:  if (e < c) { if (e < b) goto EBCAD; goto BECAD; }
        if (e < d) { if (e < a) goto BCEAD; goto BCAED; } goto BCADE;

eBCDA:  if (e < c) { if (e < b) goto EBCDA; goto BECDA; }
        if (e < a) { if (e < d) goto BCEDA; goto BCDEA; } goto BCDAE;

eBDAC:  if (e < d) { if (e < b) goto EBDAC; goto BEDAC; }
        if (e < c) { if (e < a) goto BDEAC; goto BDAEC; } goto BDACE;

eBDCA:  if (e < d) { if (e < b) goto EBDCA; goto BEDCA; }
        if (e < a) { if (e < c) goto BDECA; goto BDCEA; } goto BDCAE;

eCABD:  if (e < a) { if (e < c) goto ECABD; goto CEABD; }
        if (e < d) { if (e < b) goto CAEBD; goto CABED; } goto CABDE;

eCADB:  if (e < a) { if (e < c) goto ECADB; goto CEADB; }
        if (e < b) { if (e < d) goto CAEDB; goto CADEB; } goto CADBE;

eCBAD:  if (e < b) { if (e < c) goto ECBAD; goto CEBAD; }
        if (e < d) { if (e < a) goto CBEAD; goto CBAED; } goto CBADE;

eCBDA:  if (e < b) { if (e < c) goto ECBDA; goto CEBDA; }
        if (e < a) { if (e < d) goto CBEDA; goto CBDEA; } goto CBDAE;

eCDAB:  if (e < d) { if (e < c) goto ECDAB; goto CEDAB; }
        if (e < b) { if (e < a) goto CDEAB; goto CDAEB; } goto CDABE;

eCDBA:  if (e < d) { if (e < c) goto ECDBA; goto CEDBA; }
        if (e < a) { if (e < b) goto CDEBA; goto CDBEA; } goto CDBAE;

eDABC:  if (e < a) { if (e < d) goto EDABC; goto DEABC; }
        if (e < c) { if (e < b) goto DAEBC; goto DABEC; } goto DABCE;

eDACB:  if (e < a) { if (e < d) goto EDACB; goto DEACB; }
        if (e < b) { if (e < c) goto DAECB; goto DACEB; } goto DACBE;

eDBAC:  if (e < b) { if (e < d) goto EDBAC; goto DEBAC; }
        if (e < c) { if (e < a) goto DBEAC; goto DBAEC; } goto DBACE;

eDBCA:  if (e < b) { if (e < d) goto EDBCA; goto DEBCA; }
        if (e < a) { if (e < c) goto DBECA; goto DBCEA; } goto DBCAE;

eDCAB:  if (e < c) { if (e < d) goto EDCAB; goto DECAB; }
        if (e < b) { if (e < a) goto DCEAB; goto DCAEB; } goto DCABE;

eDCBA:  if (e < c) { if (e < d) goto EDCBA; goto DECBA; }
        if (e < a) { if (e < b) goto DCEBA; goto DCBEA; } goto DCBAE;



        //  Below are 120 labels, one per state for every ordering of
        //  our 5 runs, A through E.

ABCDE:  do { *S++ = a; if (A == LastA) goto BCDE; a = *(++A); } while (a <= b);
        if (a <= d) { if (a <= c) goto BACDE; goto BCADE; }
        if (a <= e) goto BCDAE; goto BCDEA;

ABCED:  do { *S++ = a; if (A == LastA) goto BCED; a = *(++A); } while (a <= b);
        if (a <= e) { if (a <= c) goto BACED; goto BCAED; }
        if (a <= d) goto BCEAD; goto BCEDA;

ABDCE:  do { *S++ = a; if (A == LastA) goto BDCE; a = *(++A); } while (a <= b);
        if (a <= c) { if (a <= d) goto BADCE; goto BDACE; }
        if (a <= e) goto BDCAE; goto BDCEA;

ABDEC:  do { *S++ = a; if (A == LastA) goto BDEC; a = *(++A); } while (a <= b);
        if (a <= e) { if (a <= d) goto BADEC; goto BDAEC; }
        if (a <= c) goto BDEAC; goto BDECA;

ABECD:  do { *S++ = a; if (A == LastA) goto BECD; a = *(++A); } while (a <= b);
        if (a <= c) { if (a <= e) goto BAECD; goto BEACD; }
        if (a <= d) goto BECAD; goto BECDA;

ABEDC:  do { *S++ = a; if (A == LastA) goto BEDC; a = *(++A); } while (a <= b);
        if (a <= d) { if (a <= e) goto BAEDC; goto BEADC; }
        if (a <= c) goto BEDAC; goto BEDCA;

ACBDE:  do { *S++ = a; if (A == LastA) goto CBDE; a = *(++A); } while (a <= c);
        if (a <= d) { if (a <= b) goto CABDE; goto CBADE; }
        if (a <= e) goto CBDAE; goto CBDEA;

ACBED:  do { *S++ = a; if (A == LastA) goto CBED; a = *(++A); } while (a <= c);
        if (a <= e) { if (a <= b) goto CABED; goto CBAED; }
        if (a <= d) goto CBEAD; goto CBEDA;

ACDBE:  do { *S++ = a; if (A == LastA) goto CDBE; a = *(++A); } while (a <= c);
        if (a <= b) { if (a <= d) goto CADBE; goto CDABE; }
        if (a <= e) goto CDBAE; goto CDBEA;

ACDEB:  do { *S++ = a; if (A == LastA) goto CDEB; a = *(++A); } while (a <= c);
        if (a <= e) { if (a <= d) goto CADEB; goto CDAEB; }
        if (a <= b) goto CDEAB; goto CDEBA;

ACEBD:  do { *S++ = a; if (A == LastA) goto CEBD; a = *(++A); } while (a <= c);
        if (a <= b) { if (a <= e) goto CAEBD; goto CEABD; }
        if (a <= d) goto CEBAD; goto CEBDA;

ACEDB:  do { *S++ = a; if (A == LastA) goto CEDB; a = *(++A); } while (a <= c);
        if (a <= d) { if (a <= e) goto CAEDB; goto CEADB; }
        if (a <= b) goto CEDAB; goto CEDBA;

ADBCE:  do { *S++ = a; if (A == LastA) goto DBCE; a = *(++A); } while (a <= d);
        if (a <= c) { if (a <= b) goto DABCE; goto DBACE; }
        if (a <= e) goto DBCAE; goto DBCEA;

ADBEC: do { *S++ = a; if (A == LastA) goto DBEC; a = *(++A); } while (a <= d);
        if (a <= e) { if (a <= b) goto DABEC; goto DBAEC; }
        if (a <= c) goto DBEAC; goto DBECA;

ADCBE:  do { *S++ = a; if (A == LastA) goto DCBE; a = *(++A); } while (a <= d);
        if (a <= b) { if (a <= c) goto DACBE; goto DCABE; }
        if (a <= e) goto DCBAE; goto DCBEA;

ADCEB:  do { *S++ = a; if (A == LastA) goto DCEB; a = *(++A); } while (a <= d);
        if (a <= e) { if (a <= c) goto DACEB; goto DCAEB; }
        if (a <= b) goto DCEAB; goto DCEBA;

ADEBC:  do { *S++ = a; if (A == LastA) goto DEBC; a = *(++A); } while (a <= d);
        if (a <= b) { if (a <= e) goto DAEBC; goto DEABC; }
        if (a <= c) goto DEBAC; goto DEBCA;

ADECB:  do { *S++ = a; if (A == LastA) goto DECB; a = *(++A); } while (a <= d);
        if (a <= c) { if (a <= e) goto DAECB; goto DEACB; }
        if (a <= b) goto DECAB; goto DECBA;

AEBCD:  do { *S++ = a; if (A == LastA) goto EBCD; a = *(++A); } while (a <= e);
        if (a <= c) { if (a <= b) goto EABCD; goto EBACD; }
        if (a <= d) goto EBCAD; goto EBCDA;

AEBDC:  do { *S++ = a; if (A == LastA) goto EBDC; a = *(++A); } while (a <= e);
        if (a <= d) { if (a <= b) goto EABDC; goto EBADC; }
        if (a <= c) goto EBDAC; goto EBDCA;

AECBD:  do { *S++ = a; if (A == LastA) goto ECBD; a = *(++A); } while (a <= e);
        if (a <= b) { if (a <= c) goto EACBD; goto ECABD; }
        if (a <= d) goto ECBAD; goto ECBDA;

AECDB:  do { *S++ = a; if (A == LastA) goto ECDB; a = *(++A); } while (a <= e);
        if (a <= d) { if (a <= c) goto EACDB; goto ECADB; }
        if (a <= b) goto ECDAB; goto ECDBA;

AEDBC:  do { *S++ = a; if (A == LastA) goto EDBC; a = *(++A); } while (a <= e);
        if (a <= b) { if (a <= d) goto EADBC; goto EDABC; }
        if (a <= c) goto EDBAC; goto EDBCA;

AEDCB:  do { *S++ = a; if (A == LastA) goto EDCB; a = *(++A); } while (a <= e);
        if (a <= c) { if (a <= d) goto EADCB; goto EDACB; }
        if (a <= b) goto EDCAB; goto EDCBA;

BACDE:  do { *S++ = b; if (B == LastB) goto ACDE; b = *(++B); } while (b < a);
        if (b <= d) { if (b <= c) goto ABCDE; goto ACBDE; }
        if (b <= e) goto ACDBE; goto ACDEB;

BACED:  do { *S++ = b; if (B == LastB) goto ACED; b = *(++B); } while (b < a);
        if (b <= e) { if (b <= c) goto ABCED; goto ACBED; }
        if (b <= d) goto ACEBD; goto ACEDB;

BADCE:  do { *S++ = b; if (B == LastB) goto ADCE; b = *(++B); } while (b < a);
        if (b <= c) { if (b <= d) goto ABDCE; goto ADBCE; }
        if (b <= e) goto ADCBE; goto ADCEB;

BADEC:  do { *S++ = b; if (B == LastB) goto ADEC; b = *(++B); } while (b < a);
        if (b <= e) { if (b <= d) goto ABDEC; goto ADBEC; }
        if (b <= c) goto ADEBC; goto ADECB;

BAECD:  do { *S++ = b; if (B == LastB) goto AECD; b = *(++B); } while (b < a);
        if (b <= c) { if (b <= e) goto ABECD; goto AEBCD; }
        if (b <= d) goto AECBD; goto AECDB;

BAEDC:  do { *S++ = b; if (B == LastB) goto AEDC; b = *(++B); } while (b < a);
        if (b <= d) { if (b <= e) goto ABEDC; goto AEBDC; }
        if (b <= c) goto AEDBC; goto AEDCB;

BCADE:  do { *S++ = b; if (B == LastB) goto CADE; b = *(++B); } while (b <= c);
        if (b <= d) { if (b < a) goto CBADE; goto CABDE; }
        if (b <= e) goto CADBE; goto CADEB;

BCAED:  do { *S++ = b; if (B == LastB) goto CAED; b = *(++B); } while (b <= c);
        if (b <= e) { if (b < a) goto CBAED; goto CABED; }
        if (b <= d) goto CAEBD; goto CAEDB;

BCDAE:  do { *S++ = b; if (B == LastB) goto CDAE; b = *(++B); } while (b <= c);
        if (b < a) { if (b <= d) goto CBDAE; goto CDBAE; }
        if (b <= e) goto CDABE; goto CDAEB;

BCDEA:  do { *S++ = b; if (B == LastB) goto CDEA; b = *(++B); } while (b <= c);
        if (b <= e) { if (b <= d) goto CBDEA; goto CDBEA; }
        if (b < a) goto CDEBA; goto CDEAB;

BCEAD:  do { *S++ = b; if (B == LastB) goto CEAD; b = *(++B); } while (b <= c);
        if (b < a) { if (b <= e) goto CBEAD; goto CEBAD; }
        if (b <= d) goto CEABD; goto CEADB;

BCEDA:  do { *S++ = b; if (B == LastB) goto CEDA; b = *(++B); } while (b <= c);
        if (b <= d) { if (b <= e) goto CBEDA; goto CEBDA; }
        if (b < a) goto CEDBA; goto CEDAB;

BDACE:  do { *S++ = b; if (B == LastB) goto DACE; b = *(++B); } while (b <= d);
        if (b <= c) { if (b < a) goto DBACE; goto DABCE; }
        if (b <= e) goto DACBE; goto DACEB;

BDAEC:  do { *S++ = b; if (B == LastB) goto DAEC; b = *(++B); } while (b <= d);
        if (b <= e) { if (b < a) goto DBAEC; goto DABEC; }
        if (b <= c) goto DAEBC; goto DAECB;

BDCAE:  do { *S++ = b; if (B == LastB) goto DCAE; b = *(++B); } while (b <= d);
        if (b < a) { if (b <= c) goto DBCAE; goto DCBAE; }
        if (b <= e) goto DCABE; goto DCAEB;

BDCEA:  do { *S++ = b; if (B == LastB) goto DCEA; b = *(++B); } while (b <= d);
        if (b <= e) { if (b <= c) goto DBCEA; goto DCBEA; }
        if (b < a) goto DCEBA; goto DCEAB;

BDEAC:  do { *S++ = b; if (B == LastB) goto DEAC; b = *(++B); } while (b <= d);
        if (b < a) { if (b <= e) goto DBEAC; goto DEBAC; }
        if (b <= c) goto DEABC; goto DEACB;

BDECA:  do { *S++ = b; if (B == LastB) goto DECA; b = *(++B); } while (b <= d);
        if (b <= c) { if (b <= e) goto DBECA; goto DEBCA; }
        if (b < a) goto DECBA; goto DECAB;

BEACD:  do { *S++ = b; if (B == LastB) goto EACD; b = *(++B); } while (b <= e);
        if (b <= c) { if (b < a) goto EBACD; goto EABCD; }
        if (b <= d) goto EACBD; goto EACDB;

BEADC:  do { *S++ = b; if (B == LastB) goto EADC; b = *(++B); } while (b <= e);
        if (b <= d) { if (b < a) goto EBADC; goto EABDC; }
        if (b <= c) goto EADBC; goto EADCB;

BECAD:  do { *S++ = b; if (B == LastB) goto ECAD; b = *(++B); } while (b <= e);
        if (b < a) { if (b <= c) goto EBCAD; goto ECBAD; }
        if (b <= d) goto ECABD; goto ECADB;

BECDA:  do { *S++ = b; if (B == LastB) goto ECDA; b = *(++B); } while (b <= e);
        if (b <= d) { if (b <= c) goto EBCDA; goto ECBDA; }
        if (b < a) goto ECDBA; goto ECDAB;

BEDAC:  do { *S++ = b; if (B == LastB) goto EDAC; b = *(++B); } while (b <= e);
        if (b < a) { if (b <= d) goto EBDAC; goto EDBAC; }
        if (b <= c) goto EDABC; goto EDACB;

BEDCA:  do { *S++ = b; if (B == LastB) goto EDCA; b = *(++B); } while (b <= e);
        if (b <= c) { if (b <= d) goto EBDCA; goto EDBCA; }
        if (b < a) goto EDCBA; goto EDCAB;

CABDE:  do { *S++ = c; if (C == LastC) goto ABDE; c = *(++C); } while (c < a);
        if (c <= d) { if (c < b) goto ACBDE; goto ABCDE; }
        if (c <= e) goto ABDCE; goto ABDEC;

CABED:  do { *S++ = c; if (C == LastC) goto ABED; c = *(++C); } while (c < a);
        if (c <= e) { if (c < b) goto ACBED; goto ABCED; }
        if (c <= d) goto ABECD; goto ABEDC;

CADBE:  do { *S++ = c; if (C == LastC) goto ADBE; c = *(++C); } while (c < a);
        if (c < b) { if (c <= d) goto ACDBE; goto ADCBE; }
        if (c <= e) goto ADBCE; goto ADBEC;

CADEB:  do { *S++ = c; if (C == LastC) goto ADEB; c = *(++C); } while (c < a);
        if (c <= e) { if (c <= d) goto ACDEB; goto ADCEB; }
        if (c < b) goto ADECB; goto ADEBC;

CAEBD:  do { *S++ = c; if (C == LastC) goto AEBD; c = *(++C); } while (c < a);
        if (c < b) { if (c <= e) goto ACEBD; goto AECBD; }
        if (c <= d) goto AEBCD; goto AEBDC;

CAEDB:  do { *S++ = c; if (C == LastC) goto AEDB; c = *(++C); } while (c < a);
        if (c <= d) { if (c <= e) goto ACEDB; goto AECDB; }
        if (c < b) goto AEDCB; goto AEDBC;

CBADE:  do { *S++ = c; if (C == LastC) goto BADE; c = *(++C); } while (c < b);
        if (c <= d) { if (c < a) goto BCADE; goto BACDE; }
        if (c <= e) goto BADCE; goto BADEC;

CBAED:  do { *S++ = c; if (C == LastC) goto BAED; c = *(++C); } while (c < b);
        if (c <= e) { if (c < a) goto BCAED; goto BACED; }
        if (c <= d) goto BAECD; goto BAEDC;

CBDAE:  do { *S++ = c; if (C == LastC) goto BDAE; c = *(++C); } while (c < b);
        if (c < a) { if (c <= d) goto BCDAE; goto BDCAE; }
        if (c <= e) goto BDACE; goto BDAEC;

CBDEA:  do { *S++ = c; if (C == LastC) goto BDEA; c = *(++C); } while (c < b);
        if (c <= e) { if (c <= d) goto BCDEA; goto BDCEA; }
        if (c < a) goto BDECA; goto BDEAC;

CBEAD:  do { *S++ = c; if (C == LastC) goto BEAD; c = *(++C); } while (c < b);
        if (c < a) { if (c <= e) goto BCEAD; goto BECAD; }
        if (c <= d) goto BEACD; goto BEADC;

CBEDA:  do { *S++ = c; if (C == LastC) goto BEDA; c = *(++C); } while (c < b);
        if (c <= d) { if (c <= e) goto BCEDA; goto BECDA; }
        if (c < a) goto BEDCA; goto BEDAC;

CDABE:  do { *S++ = c; if (C == LastC) goto DABE; c = *(++C); } while (c <= d);
        if (c < b) { if (c < a) goto DCABE; goto DACBE; }
        if (c <= e) goto DABCE; goto DABEC;

CDAEB:  do { *S++ = c; if (C == LastC) goto DAEB; c = *(++C); } while (c <= d);
        if (c <= e) { if (c < a) goto DCAEB; goto DACEB; }
        if (c < b) goto DAECB; goto DAEBC;

CDBAE:  do { *S++ = c; if (C == LastC) goto DBAE; c = *(++C); } while (c <= d);
        if (c < a) { if (c < b) goto DCBAE; goto DBCAE; }
        if (c <= e) goto DBACE; goto DBAEC;

CDBEA:  do { *S++ = c; if (C == LastC) goto DBEA; c = *(++C); } while (c <= d);
        if (c <= e) { if (c < b) goto DCBEA; goto DBCEA; }
        if (c < a) goto DBECA; goto DBEAC;

CDEAB:  do { *S++ = c; if (C == LastC) goto DEAB; c = *(++C); } while (c <= d);
        if (c < a) { if (c <= e) goto DCEAB; goto DECAB; }
        if (c < b) goto DEACB; goto DEABC;

CDEBA:  do { *S++ = c; if (C == LastC) goto DEBA; c = *(++C); } while (c <= d);
        if (c < b) { if (c <= e) goto DCEBA; goto DECBA; }
        if (c < a) goto DEBCA; goto DEBAC;

CEABD:  do { *S++ = c; if (C == LastC) goto EABD; c = *(++C); } while (c <= e);
        if (c < b) { if (c < a) goto ECABD; goto EACBD; }
        if (c <= d) goto EABCD; goto EABDC;

CEADB:  do { *S++ = c; if (C == LastC) goto EADB; c = *(++C); } while (c <= e);
        if (c <= d) { if (c < a) goto ECADB; goto EACDB; }
        if (c < b) goto EADCB; goto EADBC;

CEBAD:  do { *S++ = c; if (C == LastC) goto EBAD; c = *(++C); } while (c <= e);
        if (c < a) { if (c < b) goto ECBAD; goto EBCAD; }
        if (c <= d) goto EBACD; goto EBADC;

CEBDA:  do { *S++ = c; if (C == LastC) goto EBDA; c = *(++C); } while (c <= e);
        if (c <= d) { if (c < b) goto ECBDA; goto EBCDA; }
        if (c < a) goto EBDCA; goto EBDAC;

CEDAB:  do { *S++ = c; if (C == LastC) goto EDAB; c = *(++C); } while (c <= e);
        if (c < a) { if (c <= d) goto ECDAB; goto EDCAB; }
        if (c < b) goto EDACB; goto EDABC;

CEDBA:  do { *S++ = c; if (C == LastC) goto EDBA; c = *(++C); } while (c <= e);
        if (c < b) { if (c <= d) goto ECDBA; goto EDCBA; }
        if (c < a) goto EDBCA; goto EDBAC;

DABCE:  do { *S++ = d; if (D == LastD) goto ABCE; d = *(++D); } while (d < a);
        if (d < c) { if (d < b) goto ADBCE; goto ABDCE; }
        if (d <= e) goto ABCDE; goto ABCED;

DABEC:  do { *S++ = d; if (D == LastD) goto ABEC; d = *(++D); } while (d < a);
        if (d <= e) { if (d < b) goto ADBEC; goto ABDEC; }
        if (d < c) goto ABEDC; goto ABECD;

DACBE:  do { *S++ = d; if (D == LastD) goto ACBE; d = *(++D); } while (d < a);
        if (d < b) { if (d < c) goto ADCBE; goto ACDBE; }
        if (d <= e) goto ACBDE; goto ACBED;

DACEB:  do { *S++ = d; if (D == LastD) goto ACEB; d = *(++D); } while (d < a);
        if (d <= e) { if (d < c) goto ADCEB; goto ACDEB; }
        if (d < b) goto ACEDB; goto ACEBD;

DAEBC:  do { *S++ = d; if (D == LastD) goto AEBC; d = *(++D); } while (d < a);
        if (d < b) { if (d <= e) goto ADEBC; goto AEDBC; }
        if (d < c) goto AEBDC; goto AEBCD;

DAECB:  do { *S++ = d; if (D == LastD) goto AECB; d = *(++D); } while (d < a);
        if (d < c) { if (d <= e) goto ADECB; goto AEDCB; }
        if (d < b) goto AECDB; goto AECBD;

DBACE:  do { *S++ = d; if (D == LastD) goto BACE; d = *(++D); } while (d < b);
        if (d < c) { if (d < a) goto BDACE; goto BADCE; }
        if (d <= e) goto BACDE; goto BACED;

DBAEC:  do { *S++ = d; if (D == LastD) goto BAEC; d = *(++D); } while (d < b);
        if (d <= e) { if (d < a) goto BDAEC; goto BADEC; }
        if (d < c) goto BAEDC; goto BAECD;

DBCAE:  do { *S++ = d; if (D == LastD) goto BCAE; d = *(++D); } while (d < b);
        if (d < a) { if (d < c) goto BDCAE; goto BCDAE; }
        if (d <= e) goto BCADE; goto BCAED;

DBCEA:  do { *S++ = d; if (D == LastD) goto BCEA; d = *(++D); } while (d < b);
        if (d <= e) { if (d < c) goto BDCEA; goto BCDEA; }
        if (d < a) goto BCEDA; goto BCEAD;

DBEAC:  do { *S++ = d; if (D == LastD) goto BEAC; d = *(++D); } while (d < b);
        if (d < a) { if (d <= e) goto BDEAC; goto BEDAC; }
        if (d < c) goto BEADC; goto BEACD;

DBECA:  do { *S++ = d; if (D == LastD) goto BECA; d = *(++D); } while (d < b);
        if (d < c) { if (d <= e) goto BDECA; goto BEDCA; }
        if (d < a) goto BECDA; goto BECAD;

DCABE:  do { *S++ = d; if (D == LastD) goto CABE; d = *(++D); } while (d < c);
        if (d < b) { if (d < a) goto CDABE; goto CADBE; }
        if (d <= e) goto CABDE; goto CABED;

DCAEB:  do { *S++ = d; if (D == LastD) goto CAEB; d = *(++D); } while (d < c);
        if (d <= e) { if (d < a) goto CDAEB; goto CADEB; }
        if (d < b) goto CAEDB; goto CAEBD;

DCBAE:  do { *S++ = d; if (D == LastD) goto CBAE; d = *(++D); } while (d < c);
        if (d < a) { if (d < b) goto CDBAE; goto CBDAE; }
        if (d <= e) goto CBADE; goto CBAED;

DCBEA:  do { *S++ = d; if (D == LastD) goto CBEA; d = *(++D); } while (d < c);
        if (d <= e) { if (d < b) goto CDBEA; goto CBDEA; }
        if (d < a) goto CBEDA; goto CBEAD;

DCEAB:  do { *S++ = d; if (D == LastD) goto CEAB; d = *(++D); } while (d < c);
        if (d < a) { if (d <= e) goto CDEAB; goto CEDAB; }
        if (d < b) goto CEADB; goto CEABD;

DCEBA:  do { *S++ = d; if (D == LastD) goto CEBA; d = *(++D); } while (d < c);
        if (d < b) { if (d <= e) goto CDEBA; goto CEDBA; }
        if (d < a) goto CEBDA; goto CEBAD;

DEABC:  do { *S++ = d; if (D == LastD) goto EABC; d = *(++D); } while (d <= e);
        if (d < b) { if (d < a) goto EDABC; goto EADBC; }
        if (d < c) goto EABDC; goto EABCD;

DEACB:  do { *S++ = d; if (D == LastD) goto EACB; d = *(++D); } while (d <= e);
        if (d < c) { if (d < a) goto EDACB; goto EADCB; }
        if (d < b) goto EACDB; goto EACBD;

DEBAC:  do { *S++ = d; if (D == LastD) goto EBAC; d = *(++D); } while (d <= e);
        if (d < a) { if (d < b) goto EDBAC; goto EBDAC; }
        if (d < c) goto EBADC; goto EBACD;

DEBCA:  do { *S++ = d; if (D == LastD) goto EBCA; d = *(++D); } while (d <= e);
        if (d < c) { if (d < b) goto EDBCA; goto EBDCA; }
        if (d < a) goto EBCDA; goto EBCAD;

DECAB:  do { *S++ = d; if (D == LastD) goto ECAB; d = *(++D); } while (d <= e);
        if (d < a) { if (d < c) goto EDCAB; goto ECDAB; }
        if (d < b) goto ECADB; goto ECABD;

DECBA:  do { *S++ = d; if (D == LastD) goto ECBA; d = *(++D); } while (d <= e);
        if (d < b) { if (d < c) goto EDCBA; goto ECDBA; }
        if (d < a) goto ECBDA; goto ECBAD;

EABCD:  do { *S++ = e; if (E == LastE) goto ABCD; e = *(++E); } while (e < a);
        if (e < c) { if (e < b) goto AEBCD; goto ABECD; }
        if (e < d) goto ABCED; goto ABCDE;

EABDC:  do { *S++ = e; if (E == LastE) goto ABDC; e = *(++E); } while (e < a);
        if (e < d) { if (e < b) goto AEBDC; goto ABEDC; }
        if (e < c) goto ABDEC; goto ABDCE;

EACBD:  do { *S++ = e; if (E == LastE) goto ACBD; e = *(++E); } while (e < a);
        if (e < b) { if (e < c) goto AECBD; goto ACEBD; }
        if (e < d) goto ACBED; goto ACBDE;

EACDB:  do { *S++ = e; if (E == LastE) goto ACDB; e = *(++E); } while (e < a);
        if (e < d) { if (e < c) goto AECDB; goto ACEDB; }
        if (e < b) goto ACDEB; goto ACDBE;

EADBC:  do { *S++ = e; if (E == LastE) goto ADBC; e = *(++E); } while (e < a);
        if (e < b) { if (e < d) goto AEDBC; goto ADEBC; }
        if (e < c) goto ADBEC; goto ADBCE;

EADCB:  do { *S++ = e; if (E == LastE) goto ADCB; e = *(++E); } while (e < a);
        if (e < c) { if (e < d) goto AEDCB; goto ADECB; }
        if (e < b) goto ADCEB; goto ADCBE;

EBACD:  do { *S++ = e; if (E == LastE) goto BACD; e = *(++E); } while (e < b);
        if (e < c) { if (e < a) goto BEACD; goto BAECD; }
        if (e < d) goto BACED; goto BACDE;

EBADC:  do { *S++ = e; if (E == LastE) goto BADC; e = *(++E); } while (e < b);
        if (e < d) { if (e < a) goto BEADC; goto BAEDC; }
        if (e < c) goto BADEC; goto BADCE;

EBCAD:  do { *S++ = e; if (E == LastE) goto BCAD; e = *(++E); } while (e < b);
        if (e < a) { if (e < c) goto BECAD; goto BCEAD; }
        if (e < d) goto BCAED; goto BCADE;

EBCDA:  do { *S++ = e; if (E == LastE) goto BCDA; e = *(++E); } while (e < b);
        if (e < d) { if (e < c) goto BECDA; goto BCEDA; }
        if (e < a) goto BCDEA; goto BCDAE;

EBDAC:  do { *S++ = e; if (E == LastE) goto BDAC; e = *(++E); } while (e < b);
        if (e < a) { if (e < d) goto BEDAC; goto BDEAC; }
        if (e < c) goto BDAEC; goto BDACE;

EBDCA:  do { *S++ = e; if (E == LastE) goto BDCA; e = *(++E); } while (e < b);
        if (e < c) { if (e < d) goto BEDCA; goto BDECA; }
        if (e < a) goto BDCEA; goto BDCAE;

ECABD:  do { *S++ = e; if (E == LastE) goto CABD; e = *(++E); } while (e < c);
        if (e < b) { if (e < a) goto CEABD; goto CAEBD; }
        if (e < d) goto CABED; goto CABDE;

ECADB:  do { *S++ = e; if (E == LastE) goto CADB; e = *(++E); } while (e < c);
        if (e < d) { if (e < a) goto CEADB; goto CAEDB; }
        if (e < b) goto CADEB; goto CADBE;

ECBAD:  do { *S++ = e; if (E == LastE) goto CBAD; e = *(++E); } while (e < c);
        if (e < a) { if (e < b) goto CEBAD; goto CBEAD; }
        if (e < d) goto CBAED; goto CBADE;

ECBDA:  do { *S++ = e; if (E == LastE) goto CBDA; e = *(++E); } while (e < c);
        if (e < d) { if (e < b) goto CEBDA; goto CBEDA; }
        if (e < a) goto CBDEA; goto CBDAE;

ECDAB:  do { *S++ = e; if (E == LastE) goto CDAB; e = *(++E); } while (e < c);
        if (e < a) { if (e < d) goto CEDAB; goto CDEAB; }
        if (e < b) goto CDAEB; goto CDABE;

ECDBA:  do { *S++ = e; if (E == LastE) goto CDBA; e = *(++E); } while (e < c);
        if (e < b) { if (e < d) goto CEDBA; goto CDEBA; }
        if (e < a) goto CDBEA; goto CDBAE;

EDABC:  do { *S++ = e; if (E == LastE) goto DABC; e = *(++E); } while (e < d);
        if (e < b) { if (e < a) goto DEABC; goto DAEBC; }
        if (e < c) goto DABEC; goto DABCE;

EDACB:  do { *S++ = e; if (E == LastE) goto DACB; e = *(++E); } while (e < d);
        if (e < c) { if (e < a) goto DEACB; goto DAECB; }
        if (e < b) goto DACEB; goto DACBE;

EDBAC:  do { *S++ = e; if (E == LastE) goto DBAC; e = *(++E); } while (e < d);
        if (e < a) { if (e < b) goto DEBAC; goto DBEAC; }
        if (e < c) goto DBAEC; goto DBACE;

EDBCA:  do { *S++ = e; if (E == LastE) goto DBCA; e = *(++E); } while (e < d);
        if (e < c) { if (e < b) goto DEBCA; goto DBECA; }
        if (e < a) goto DBCEA; goto DBCAE;

EDCAB:  do { *S++ = e; if (E == LastE) goto DCAB; e = *(++E); } while (e < d);
        if (e < a) { if (e < c) goto DECAB; goto DCEAB; }
        if (e < b) goto DCAEB; goto DCABE;

EDCBA:  do { *S++ = e; if (E == LastE) goto DCBA; e = *(++E); } while (e < d);
        if (e < b) { if (e < c) goto DECBA; goto DCEBA; }
        if (e < a) goto DCBEA; goto DCBAE;



        //  Below are 120 labels, one per state for every possible ordering,
        //  once one run is exhausted, and we only have four left.

ABCD:   do { *S++ = a; if (A == LastA) goto BCD; a = *(++A); } while (a <= b);
        if (a <= c) goto BACD; if (a <= d) goto BCAD; goto BCDA;

ABCE:   do { *S++ = a; if (A == LastA) goto BCE; a = *(++A); } while (a <= b);
        if (a <= c) goto BACE; if (a <= e) goto BCAE; goto BCEA;

ABDC:   do { *S++ = a; if (A == LastA) goto BDC; a = *(++A); } while (a <= b);
        if (a <= d) goto BADC; if (a <= c) goto BDAC; goto BDCA;

ABDE:   do { *S++ = a; if (A == LastA) goto BDE; a = *(++A); } while (a <= b);
        if (a <= d) goto BADE; if (a <= e) goto BDAE; goto BDEA;

ABEC:   do { *S++ = a; if (A == LastA) goto BEC; a = *(++A); } while (a <= b);
        if (a <= e) goto BAEC; if (a <= c) goto BEAC; goto BECA;

ABED:   do { *S++ = a; if (A == LastA) goto BED; a = *(++A); } while (a <= b);
        if (a <= e) goto BAED; if (a <= d) goto BEAD; goto BEDA;

ACBD:   do { *S++ = a; if (A == LastA) goto CBD; a = *(++A); } while (a <= c);
        if (a <= b) goto CABD; if (a <= d) goto CBAD; goto CBDA;

ACBE:   do { *S++ = a; if (A == LastA) goto CBE; a = *(++A); } while (a <= c);
        if (a <= b) goto CABE; if (a <= e) goto CBAE; goto CBEA;

ACDB:   do { *S++ = a; if (A == LastA) goto CDB; a = *(++A); } while (a <= c);
        if (a <= d) goto CADB; if (a <= b) goto CDAB; goto CDBA;

ACDE:   do { *S++ = a; if (A == LastA) goto CDE; a = *(++A); } while (a <= c);
        if (a <= d) goto CADE; if (a <= e) goto CDAE; goto CDEA;

ACEB:   do { *S++ = a; if (A == LastA) goto CEB; a = *(++A); } while (a <= c);
        if (a <= e) goto CAEB; if (a <= b) goto CEAB; goto CEBA;

ACED:   do { *S++ = a; if (A == LastA) goto CED; a = *(++A); } while (a <= c);
        if (a <= e) goto CAED; if (a <= d) goto CEAD; goto CEDA;

ADBC:   do { *S++ = a; if (A == LastA) goto DBC; a = *(++A); } while (a <= d);
        if (a <= b) goto DABC; if (a <= c) goto DBAC; goto DBCA;

ADBE:   do { *S++ = a; if (A == LastA) goto DBE; a = *(++A); } while (a <= d);
        if (a <= b) goto DABE; if (a <= e) goto DBAE; goto DBEA;

ADCB:   do { *S++ = a; if (A == LastA) goto DCB; a = *(++A); } while (a <= d);
        if (a <= c) goto DACB; if (a <= b) goto DCAB; goto DCBA;

ADCE:   do { *S++ = a; if (A == LastA) goto DCE; a = *(++A); } while (a <= d);
        if (a <= c) goto DACE; if (a <= e) goto DCAE; goto DCEA;

ADEB:   do { *S++ = a; if (A == LastA) goto DEB; a = *(++A); } while (a <= d);
        if (a <= e) goto DAEB; if (a <= b) goto DEAB; goto DEBA;

ADEC:   do { *S++ = a; if (A == LastA) goto DEC; a = *(++A); } while (a <= d);
        if (a <= e) goto DAEC; if (a <= c) goto DEAC; goto DECA;

AEBC:   do { *S++ = a; if (A == LastA) goto EBC; a = *(++A); } while (a <= e);
        if (a <= b) goto EABC; if (a <= c) goto EBAC; goto EBCA;

AEBD:   do { *S++ = a; if (A == LastA) goto EBD; a = *(++A); } while (a <= e);
        if (a <= b) goto EABD; if (a <= d) goto EBAD; goto EBDA;

AECB:   do { *S++ = a; if (A == LastA) goto ECB; a = *(++A); } while (a <= e);
        if (a <= c) goto EACB; if (a <= b) goto ECAB; goto ECBA;

AECD:   do { *S++ = a; if (A == LastA) goto ECD; a = *(++A); } while (a <= e);
        if (a <= c) goto EACD; if (a <= d) goto ECAD; goto ECDA;

AEDB:   do { *S++ = a; if (A == LastA) goto EDB; a = *(++A); } while (a <= e);
        if (a <= d) goto EADB; if (a <= b) goto EDAB; goto EDBA;

AEDC:   do { *S++ = a; if (A == LastA) goto EDC; a = *(++A); } while (a <= e);
        if (a <= d) goto EADC; if (a <= c) goto EDAC; goto EDCA;

BACD:   do { *S++ = b; if (B == LastB) goto ACD; b = *(++B); } while (b < a);
        if (b <= c) goto ABCD; if (b <= d) goto ACBD; goto ACDB;

BACE:   do { *S++ = b; if (B == LastB) goto ACE; b = *(++B); } while (b < a);
        if (b <= c) goto ABCE; if (b <= e) goto ACBE; goto ACEB;

BADC:   do { *S++ = b; if (B == LastB) goto ADC; b = *(++B); } while (b < a);
        if (b <= d) goto ABDC; if (b <= c) goto ADBC; goto ADCB;

BADE:   do { *S++ = b; if (B == LastB) goto ADE; b = *(++B); } while (b < a);
        if (b <= d) goto ABDE; if (b <= e) goto ADBE; goto ADEB;

BAEC:   do { *S++ = b; if (B == LastB) goto AEC; b = *(++B); } while (b < a);
        if (b <= e) goto ABEC; if (b <= c) goto AEBC; goto AECB;

BAED:   do { *S++ = b; if (B == LastB) goto AED; b = *(++B); } while (b < a);
        if (b <= e) goto ABED; if (b <= d) goto AEBD; goto AEDB;

BCAD:   do { *S++ = b; if (B == LastB) goto CAD; b = *(++B); } while (b <= c);
        if (b < a) goto CBAD; if (b <= d) goto CABD; goto CADB;

BCAE:   do { *S++ = b; if (B == LastB) goto CAE; b = *(++B); } while (b <= c);
        if (b < a) goto CBAE; if (b <= e) goto CABE; goto CAEB;

BCDA:   do { *S++ = b; if (B == LastB) goto CDA; b = *(++B); } while (b <= c);
        if (b <= d) goto CBDA; if (b < a) goto CDBA; goto CDAB;

BCDE:   do { *S++ = b; if (B == LastB) goto CDE; b = *(++B); } while (b <= c);
        if (b <= d) goto CBDE; if (b <= e) goto CDBE; goto CDEB;

BCEA:   do { *S++ = b; if (B == LastB) goto CEA; b = *(++B); } while (b <= c);
        if (b <= e) goto CBEA; if (b < a) goto CEBA; goto CEAB;

BCED:   do { *S++ = b; if (B == LastB) goto CED; b = *(++B); } while (b <= c);
        if (b <= e) goto CBED; if (b <= d) goto CEBD; goto CEDB;

BDAC:   do { *S++ = b; if (B == LastB) goto DAC; b = *(++B); } while (b <= d);
        if (b < a) goto DBAC; if (b <= c) goto DABC; goto DACB;

BDAE:   do { *S++ = b; if (B == LastB) goto DAE; b = *(++B); } while (b <= d);
        if (b < a) goto DBAE; if (b <= e) goto DABE; goto DAEB;

BDCA:   do { *S++ = b; if (B == LastB) goto DCA; b = *(++B); } while (b <= d);
        if (b <= c) goto DBCA; if (b < a) goto DCBA; goto DCAB;

BDCE:   do { *S++ = b; if (B == LastB) goto DCE; b = *(++B); } while (b <= d);
        if (b <= c) goto DBCE; if (b <= e) goto DCBE; goto DCEB;

BDEA:   do { *S++ = b; if (B == LastB) goto DEA; b = *(++B); } while (b <= d);
        if (b <= e) goto DBEA; if (b < a) goto DEBA; goto DEAB;

BDEC:   do { *S++ = b; if (B == LastB) goto DEC; b = *(++B); } while (b <= d);
        if (b <= e) goto DBEC; if (b <= c) goto DEBC; goto DECB;

BEAC:   do { *S++ = b; if (B == LastB) goto EAC; b = *(++B); } while (b <= e);
        if (b < a) goto EBAC; if (b <= c) goto EABC; goto EACB;

BEAD:   do { *S++ = b; if (B == LastB) goto EAD; b = *(++B); } while (b <= e);
        if (b < a) goto EBAD; if (b <= d) goto EABD; goto EADB;

BECA:   do { *S++ = b; if (B == LastB) goto ECA; b = *(++B); } while (b <= e);
        if (b <= c) goto EBCA; if (b < a) goto ECBA; goto ECAB;

BECD:   do { *S++ = b; if (B == LastB) goto ECD; b = *(++B); } while (b <= e);
        if (b <= c) goto EBCD; if (b <= d) goto ECBD; goto ECDB;

BEDA:   do { *S++ = b; if (B == LastB) goto EDA; b = *(++B); } while (b <= e);
        if (b <= d) goto EBDA; if (b < a) goto EDBA; goto EDAB;

BEDC:   do { *S++ = b; if (B == LastB) goto EDC; b = *(++B); } while (b <= e);
        if (b <= d) goto EBDC; if (b <= c) goto EDBC; goto EDCB;

CABD:   do { *S++ = c; if (C == LastC) goto ABD; c = *(++C); } while (c < a);
        if (c < b) goto ACBD; if (c <= d) goto ABCD; goto ABDC;

CABE:   do { *S++ = c; if (C == LastC) goto ABE; c = *(++C); } while (c < a);
        if (c < b) goto ACBE; if (c <= e) goto ABCE; goto ABEC;

CADB:   do { *S++ = c; if (C == LastC) goto ADB; c = *(++C); } while (c < a);
        if (c <= d) goto ACDB; if (c < b) goto ADCB; goto ADBC;

CADE:   do { *S++ = c; if (C == LastC) goto ADE; c = *(++C); } while (c < a);
        if (c <= d) goto ACDE; if (c <= e) goto ADCE; goto ADEC;

CAEB:   do { *S++ = c; if (C == LastC) goto AEB; c = *(++C); } while (c < a);
        if (c <= e) goto ACEB; if (c < b) goto AECB; goto AEBC;

CAED:   do { *S++ = c; if (C == LastC) goto AED; c = *(++C); } while (c < a);
        if (c <= e) goto ACED; if (c <= d) goto AECD; goto AEDC;

CBAD:   do { *S++ = c; if (C == LastC) goto BAD; c = *(++C); } while (c < b);
        if (c < a) goto BCAD; if (c <= d) goto BACD; goto BADC;

CBAE:   do { *S++ = c; if (C == LastC) goto BAE; c = *(++C); } while (c < b);
        if (c < a) goto BCAE; if (c <= e) goto BACE; goto BAEC;

CBDA:   do { *S++ = c; if (C == LastC) goto BDA; c = *(++C); } while (c < b);
        if (c <= d) goto BCDA; if (c < a) goto BDCA; goto BDAC;

CBDE:   do { *S++ = c; if (C == LastC) goto BDE; c = *(++C); } while (c < b);
        if (c <= d) goto BCDE; if (c <= e) goto BDCE; goto BDEC;

CBEA:   do { *S++ = c; if (C == LastC) goto BEA; c = *(++C); } while (c < b);
        if (c <= e) goto BCEA; if (c < a) goto BECA; goto BEAC;

CBED:   do { *S++ = c; if (C == LastC) goto BED; c = *(++C); } while (c < b);
        if (c <= e) goto BCED; if (c <= d) goto BECD; goto BEDC;

CDAB:   do { *S++ = c; if (C == LastC) goto DAB; c = *(++C); } while (c <= d);
        if (c < a) goto DCAB; if (c < b) goto DACB; goto DABC;

CDAE:   do { *S++ = c; if (C == LastC) goto DAE; c = *(++C); } while (c <= d);
        if (c < a) goto DCAE; if (c <= e) goto DACE; goto DAEC;

CDBA:   do { *S++ = c; if (C == LastC) goto DBA; c = *(++C); } while (c <= d);
        if (c < b) goto DCBA; if (c < a) goto DBCA; goto DBAC;

CDBE:   do { *S++ = c; if (C == LastC) goto DBE; c = *(++C); } while (c <= d);
        if (c < b) goto DCBE; if (c <= e) goto DBCE; goto DBEC;

CDEA:   do { *S++ = c; if (C == LastC) goto DEA; c = *(++C); } while (c <= d);
        if (c <= e) goto DCEA; if (c < a) goto DECA; goto DEAC;

CDEB:   do { *S++ = c; if (C == LastC) goto DEB; c = *(++C); } while (c <= d);
        if (c <= e) goto DCEB; if (c < b) goto DECB; goto DEBC;

CEAB:   do { *S++ = c; if (C == LastC) goto EAB; c = *(++C); } while (c <= e);
        if (c < a) goto ECAB; if (c < b) goto EACB; goto EABC;

CEAD:   do { *S++ = c; if (C == LastC) goto EAD; c = *(++C); } while (c <= e);
        if (c < a) goto ECAD; if (c <= d) goto EACD; goto EADC;

CEBA:   do { *S++ = c; if (C == LastC) goto EBA; c = *(++C); } while (c <= e);
        if (c < b) goto ECBA; if (c < a) goto EBCA; goto EBAC;

CEBD:   do { *S++ = c; if (C == LastC) goto EBD; c = *(++C); } while (c <= e);
        if (c < b) goto ECBD; if (c <= d) goto EBCD; goto EBDC;

CEDA:   do { *S++ = c; if (C == LastC) goto EDA; c = *(++C); } while (c <= e);
        if (c <= d) goto ECDA; if (c < a) goto EDCA; goto EDAC;

CEDB:   do { *S++ = c; if (C == LastC) goto EDB; c = *(++C); } while (c <= e);
        if (c <= d) goto ECDB; if (c < b) goto EDCB; goto EDBC;

DABC:   do { *S++ = d; if (D == LastD) goto ABC; d = *(++D); } while (d < a);
        if (d < b) goto ADBC; if (d < c) goto ABDC; goto ABCD;

DABE:   do { *S++ = d; if (D == LastD) goto ABE; d = *(++D); } while (d < a);
        if (d < b) goto ADBE; if (d <= e) goto ABDE; goto ABED;

DACB:   do { *S++ = d; if (D == LastD) goto ACB; d = *(++D); } while (d < a);
        if (d < c) goto ADCB; if (d < b) goto ACDB; goto ACBD;

DACE:   do { *S++ = d; if (D == LastD) goto ACE; d = *(++D); } while (d < a);
        if (d < c) goto ADCE; if (d <= e) goto ACDE; goto ACED;

DAEB:   do { *S++ = d; if (D == LastD) goto AEB; d = *(++D); } while (d < a);
        if (d <= e) goto ADEB; if (d < b) goto AEDB; goto AEBD;

DAEC:   do { *S++ = d; if (D == LastD) goto AEC; d = *(++D); } while (d < a);
        if (d <= e) goto ADEC; if (d < c) goto AEDC; goto AECD;

DBAC:   do { *S++ = d; if (D == LastD) goto BAC; d = *(++D); } while (d < b);
        if (d < a) goto BDAC; if (d < c) goto BADC; goto BACD;

DBAE:   do { *S++ = d; if (D == LastD) goto BAE; d = *(++D); } while (d < b);
        if (d < a) goto BDAE; if (d <= e) goto BADE; goto BAED;

DBCA:   do { *S++ = d; if (D == LastD) goto BCA; d = *(++D); } while (d < b);
        if (d < c) goto BDCA; if (d < a) goto BCDA; goto BCAD;

DBCE:   do { *S++ = d; if (D == LastD) goto BCE; d = *(++D); } while (d < b);
        if (d < c) goto BDCE; if (d <= e) goto BCDE; goto BCED;

DBEA:   do { *S++ = d; if (D == LastD) goto BEA; d = *(++D); } while (d < b);
        if (d <= e) goto BDEA; if (d < a) goto BEDA; goto BEAD;

DBEC:   do { *S++ = d; if (D == LastD) goto BEC; d = *(++D); } while (d < b);
        if (d <= e) goto BDEC; if (d < c) goto BEDC; goto BECD;

DCAB:   do { *S++ = d; if (D == LastD) goto CAB; d = *(++D); } while (d < c);
        if (d < a) goto CDAB; if (d < b) goto CADB; goto CABD;

DCAE:   do { *S++ = d; if (D == LastD) goto CAE; d = *(++D); } while (d < c);
        if (d < a) goto CDAE; if (d <= e) goto CADE; goto CAED;

DCBA:   do { *S++ = d; if (D == LastD) goto CBA; d = *(++D); } while (d < c);
        if (d < b) goto CDBA; if (d < a) goto CBDA; goto CBAD;

DCBE:   do { *S++ = d; if (D == LastD) goto CBE; d = *(++D); } while (d < c);
        if (d < b) goto CDBE; if (d <= e) goto CBDE; goto CBED;

DCEA:   do { *S++ = d; if (D == LastD) goto CEA; d = *(++D); } while (d < c);
        if (d <= e) goto CDEA; if (d < a) goto CEDA; goto CEAD;

DCEB:   do { *S++ = d; if (D == LastD) goto CEB; d = *(++D); } while (d < c);
        if (d <= e) goto CDEB; if (d < b) goto CEDB; goto CEBD;

DEAB:   do { *S++ = d; if (D == LastD) goto EAB; d = *(++D); } while (d <= e);
        if (d < a) goto EDAB; if (d < b) goto EADB; goto EABD;

DEAC:   do { *S++ = d; if (D == LastD) goto EAC; d = *(++D); } while (d <= e);
        if (d < a) goto EDAC; if (d < c) goto EADC; goto EACD;

DEBA:   do { *S++ = d; if (D == LastD) goto EBA; d = *(++D); } while (d <= e);
        if (d < b) goto EDBA; if (d < a) goto EBDA; goto EBAD;

DEBC:   do { *S++ = d; if (D == LastD) goto EBC; d = *(++D); } while (d <= e);
        if (d < b) goto EDBC; if (d < c) goto EBDC; goto EBCD;

DECA:   do { *S++ = d; if (D == LastD) goto ECA; d = *(++D); } while (d <= e);
        if (d < c) goto EDCA; if (d < a) goto ECDA; goto ECAD;

DECB:   do { *S++ = d; if (D == LastD) goto ECB; d = *(++D); } while (d <= e);
        if (d < c) goto EDCB; if (d < b) goto ECDB; goto ECBD;

EABC:   do { *S++ = e; if (E == LastE) goto ABC; e = *(++E); } while (e < a);
        if (e < b) goto AEBC; if (e < c) goto ABEC; goto ABCE;

EABD:   do { *S++ = e; if (E == LastE) goto ABD; e = *(++E); } while (e < a);
        if (e < b) goto AEBD; if (e < d) goto ABED; goto ABDE;

EACB:   do { *S++ = e; if (E == LastE) goto ACB; e = *(++E); } while (e < a);
        if (e < c) goto AECB; if (e < b) goto ACEB; goto ACBE;

EACD:   do { *S++ = e; if (E == LastE) goto ACD; e = *(++E); } while (e < a);
        if (e < c) goto AECD; if (e < d) goto ACED; goto ACDE;

EADB:   do { *S++ = e; if (E == LastE) goto ADB; e = *(++E); } while (e < a);
        if (e < d) goto AEDB; if (e < b) goto ADEB; goto ADBE;

EADC:   do { *S++ = e; if (E == LastE) goto ADC; e = *(++E); } while (e < a);
        if (e < d) goto AEDC; if (e < c) goto ADEC; goto ADCE;

EBAC:   do { *S++ = e; if (E == LastE) goto BAC; e = *(++E); } while (e < b);
        if (e < a) goto BEAC; if (e < c) goto BAEC; goto BACE;

EBAD:   do { *S++ = e; if (E == LastE) goto BAD; e = *(++E); } while (e < b);
        if (e < a) goto BEAD; if (e < d) goto BAED; goto BADE;

EBCA:   do { *S++ = e; if (E == LastE) goto BCA; e = *(++E); } while (e < b);
        if (e < c) goto BECA; if (e < a) goto BCEA; goto BCAE;

EBCD:   do { *S++ = e; if (E == LastE) goto BCD; e = *(++E); } while (e < b);
        if (e < c) goto BECD; if (e < d) goto BCED; goto BCDE;

EBDA:   do { *S++ = e; if (E == LastE) goto BDA; e = *(++E); } while (e < b);
        if (e < d) goto BEDA; if (e < a) goto BDEA; goto BDAE;

EBDC:   do { *S++ = e; if (E == LastE) goto BDC; e = *(++E); } while (e < b);
        if (e < d) goto BEDC; if (e < c) goto BDEC; goto BDCE;

ECAB:   do { *S++ = e; if (E == LastE) goto CAB; e = *(++E); } while (e < c);
        if (e < a) goto CEAB; if (e < b) goto CAEB; goto CABE;

ECAD:   do { *S++ = e; if (E == LastE) goto CAD; e = *(++E); } while (e < c);
        if (e < a) goto CEAD; if (e < d) goto CAED; goto CADE;

ECBA:   do { *S++ = e; if (E == LastE) goto CBA; e = *(++E); } while (e < c);
        if (e < b) goto CEBA; if (e < a) goto CBEA; goto CBAE;

ECBD:   do { *S++ = e; if (E == LastE) goto CBD; e = *(++E); } while (e < c);
        if (e < b) goto CEBD; if (e < d) goto CBED; goto CBDE;

ECDA:   do { *S++ = e; if (E == LastE) goto CDA; e = *(++E); } while (e < c);
        if (e < d) goto CEDA; if (e < a) goto CDEA; goto CDAE;

ECDB:   do { *S++ = e; if (E == LastE) goto CDB; e = *(++E); } while (e < c);
        if (e < d) goto CEDB; if (e < b) goto CDEB; goto CDBE;

EDAB:   do { *S++ = e; if (E == LastE) goto DAB; e = *(++E); } while (e < d);
        if (e < a) goto DEAB; if (e < b) goto DAEB; goto DABE;

EDAC:   do { *S++ = e; if (E == LastE) goto DAC; e = *(++E); } while (e < d);
        if (e < a) goto DEAC; if (e < c) goto DAEC; goto DACE;

EDBA:   do { *S++ = e; if (E == LastE) goto DBA; e = *(++E); } while (e < d);
        if (e < b) goto DEBA; if (e < a) goto DBEA; goto DBAE;

EDBC:   do { *S++ = e; if (E == LastE) goto DBC; e = *(++E); } while (e < d);
        if (e < b) goto DEBC; if (e < c) goto DBEC; goto DBCE;

EDCA:   do { *S++ = e; if (E == LastE) goto DCA; e = *(++E); } while (e < d);
        if (e < c) goto DECA; if (e < a) goto DCEA; goto DCAE;

EDCB:   do { *S++ = e; if (E == LastE) goto DCB; e = *(++E); } while (e < d);
        if (e < c) goto DECB; if (e < b) goto DCEB; goto DCBE;



        //  Below are 60 labels, one per state for every possible ordering,
        //  once two runs are exhausted, and we only have three left.

ABC:    do { *S++ = a; if (A == LastA) goto BC; a = *(++A); } while (a <= b);
        if (a <= c) goto BAC; goto BCA;

ABD:    do { *S++ = a; if (A == LastA) goto BD; a = *(++A); } while (a <= b);
        if (a <= d) goto BAD; goto BDA;

ABE:    do { *S++ = a; if (A == LastA) goto BE; a = *(++A); } while (a <= b);
        if (a <= e) goto BAE; goto BEA;

ACB:    do { *S++ = a; if (A == LastA) goto CB; a = *(++A); } while (a <= c);
        if (a <= b) goto CAB; goto CBA;

ACD:    do { *S++ = a; if (A == LastA) goto CD; a = *(++A); } while (a <= c);
        if (a <= d) goto CAD; goto CDA;

ACE:    do { *S++ = a; if (A == LastA) goto CE; a = *(++A); } while (a <= c);
        if (a <= e) goto CAE; goto CEA;

ADB:    do { *S++ = a; if (A == LastA) goto DB; a = *(++A); } while (a <= d);
        if (a <= b) goto DAB; goto DBA;

ADC:    do { *S++ = a; if (A == LastA) goto DC; a = *(++A); } while (a <= d);
        if (a <= c) goto DAC; goto DCA;

ADE:    do { *S++ = a; if (A == LastA) goto DE; a = *(++A); } while (a <= d);
        if (a <= e) goto DAE; goto DEA;

AEB:    do { *S++ = a; if (A == LastA) goto EB; a = *(++A); } while (a <= e);
        if (a <= b) goto EAB; goto EBA;

AEC:    do { *S++ = a; if (A == LastA) goto EC; a = *(++A); } while (a <= e);
        if (a <= c) goto EAC; goto ECA;

AED:    do { *S++ = a; if (A == LastA) goto ED; a = *(++A); } while (a <= e);
        if (a <= d) goto EAD; goto EDA;

BAC:    do { *S++ = b; if (B == LastB) goto AC; b = *(++B); } while (b < a);
        if (b <= c) goto ABC; goto ACB;

BAD:    do { *S++ = b; if (B == LastB) goto AD; b = *(++B); } while (b < a);
        if (b <= d) goto ABD; goto ADB;

BAE:    do { *S++ = b; if (B == LastB) goto AE; b = *(++B); } while (b < a);
        if (b <= e) goto ABE; goto AEB;

BCA:    do { *S++ = b; if (B == LastB) goto CA; b = *(++B); } while (b <= c);
        if (b < a) goto CBA; goto CAB;

BCD:    do { *S++ = b; if (B == LastB) goto CD; b = *(++B); } while (b <= c);
        if (b <= d) goto CBD; goto CDB;

BCE:    do { *S++ = b; if (B == LastB) goto CE; b = *(++B); } while (b <= c);
        if (b <= e) goto CBE; goto CEB;

BDA:    do { *S++ = b; if (B == LastB) goto DA; b = *(++B); } while (b <= d);
        if (b < a) goto DBA; goto DAB;

BDC:    do { *S++ = b; if (B == LastB) goto DC; b = *(++B); } while (b <= d);
        if (b <= c) goto DBC; goto DCB;

BDE:    do { *S++ = b; if (B == LastB) goto DE; b = *(++B); } while (b <= d);
        if (b <= e) goto DBE; goto DEB;

BEA:    do { *S++ = b; if (B == LastB) goto EA; b = *(++B); } while (b <= e);
        if (b < a) goto EBA; goto EAB;

BEC:    do { *S++ = b; if (B == LastB) goto EC; b = *(++B); } while (b <= e);
        if (b <= c) goto EBC; goto ECB;

BED:    do { *S++ = b; if (B == LastB) goto ED; b = *(++B); } while (b <= e);
        if (b <= d) goto EBD; goto EDB;

CAB:    do { *S++ = c; if (C == LastC) goto AB; c = *(++C); } while (c < a);
        if (c < b) goto ACB; goto ABC;

CAD:    do { *S++ = c; if (C == LastC) goto AD; c = *(++C); } while (c < a);
        if (c <= d) goto ACD; goto ADC;

CAE:    do { *S++ = c; if (C == LastC) goto AE; c = *(++C); } while (c < a);
        if (c <= e) goto ACE; goto AEC;

CBA:    do { *S++ = c; if (C == LastC) goto BA; c = *(++C); } while (c < b);
        if (c < a) goto BCA; goto BAC;

CBD:    do { *S++ = c; if (C == LastC) goto BD; c = *(++C); } while (c < b);
        if (c <= d) goto BCD; goto BDC;

CBE:    do { *S++ = c; if (C == LastC) goto BE; c = *(++C); } while (c < b);
        if (c <= e) goto BCE; goto BEC;

CDA:    do { *S++ = c; if (C == LastC) goto DA; c = *(++C); } while (c <= d);
        if (c < a) goto DCA; goto DAC;

CDB:    do { *S++ = c; if (C == LastC) goto DB; c = *(++C); } while (c <= d);
        if (c < b) goto DCB; goto DBC;

CDE:    do { *S++ = c; if (C == LastC) goto DE; c = *(++C); } while (c <= d);
        if (c <= e) goto DCE; goto DEC;

CEA:    do { *S++ = c; if (C == LastC) goto EA; c = *(++C); } while (c <= e);
        if (c < a) goto ECA; goto EAC;

CEB:    do { *S++ = c; if (C == LastC) goto EB; c = *(++C); } while (c <= e);
        if (c < b) goto ECB; goto EBC;

CED:    do { *S++ = c; if (C == LastC) goto ED; c = *(++C); } while (c <= e);
        if (c <= d) goto ECD; goto EDC;

DAB:    do { *S++ = d; if (D == LastD) goto AB; d = *(++D); } while (d < a);
        if (d < b) goto ADB; goto ABD;

DAC:    do { *S++ = d; if (D == LastD) goto AC; d = *(++D); } while (d < a);
        if (d < c) goto ADC; goto ACD;

DAE:    do { *S++ = d; if (D == LastD) goto AE; d = *(++D); } while (d < a);
        if (d <= e) goto ADE; goto AED;

DBA:    do { *S++ = d; if (D == LastD) goto BA; d = *(++D); } while (d < b);
        if (d < a) goto BDA; goto BAD;

DBC:    do { *S++ = d; if (D == LastD) goto BC; d = *(++D); } while (d < b);
        if (d < c) goto BDC; goto BCD;

DBE:    do { *S++ = d; if (D == LastD) goto BE; d = *(++D); } while (d < b);
        if (d <= e) goto BDE; goto BED;

DCA:    do { *S++ = d; if (D == LastD) goto CA; d = *(++D); } while (d < c);
        if (d < a) goto CDA; goto CAD;

DCB:    do { *S++ = d; if (D == LastD) goto CB; d = *(++D); } while (d < c);
        if (d < b) goto CDB; goto CBD;

DCE:    do { *S++ = d; if (D == LastD) goto CE; d = *(++D); } while (d < c);
        if (d <= e) goto CDE; goto CED;

DEA:    do { *S++ = d; if (D == LastD) goto EA; d = *(++D); } while (d <= e);
        if (d < a) goto EDA; goto EAD;

DEB:    do { *S++ = d; if (D == LastD) goto EB; d = *(++D); } while (d <= e);
        if (d < b) goto EDB; goto EBD;

DEC:    do { *S++ = d; if (D == LastD) goto EC; d = *(++D); } while (d <= e);
        if (d < c) goto EDC; goto ECD;

EAB:    do { *S++ = e; if (E == LastE) goto AB; e = *(++E); } while (e < a);
        if (e < b) goto AEB; goto ABE;

EAC:    do { *S++ = e; if (E == LastE) goto AC; e = *(++E); } while (e < a);
        if (e < c) goto AEC; goto ACE;

EAD:    do { *S++ = e; if (E == LastE) goto AD; e = *(++E); } while (e < a);
        if (e < d) goto AED; goto ADE;

EBA:    do { *S++ = e; if (E == LastE) goto BA; e = *(++E); } while (e < b);
        if (e < a) goto BEA; goto BAE;

EBC:    do { *S++ = e; if (E == LastE) goto BC; e = *(++E); } while (e < b);
        if (e < c) goto BEC; goto BCE;

EBD:    do { *S++ = e; if (E == LastE) goto BD; e = *(++E); } while (e < b);
        if (e < d) goto BED; goto BDE;

ECA:    do { *S++ = e; if (E == LastE) goto CA; e = *(++E); } while (e < c);
        if (e < a) goto CEA; goto CAE;

ECB:    do { *S++ = e; if (E == LastE) goto CB; e = *(++E); } while (e < c);
        if (e < b) goto CEB; goto CBE;

ECD:    do { *S++ = e; if (E == LastE) goto CD; e = *(++E); } while (e < c);
        if (e < d) goto CED; goto CDE;

EDA:    do { *S++ = e; if (E == LastE) goto DA; e = *(++E); } while (e < d);
        if (e < a) goto DEA; goto DAE;

EDB:    do { *S++ = e; if (E == LastE) goto DB; e = *(++E); } while (e < d);
        if (e < b) goto DEB; goto DBE;

EDC:    do { *S++ = e; if (E == LastE) goto DC; e = *(++E); } while (e < d);
        if (e < c) goto DEC; goto DCE;



        //  Below are 20 labels, one per state for every possible ordering,
        //  once three runs are exhausted, and we only have two left.

AB:     do { *S++ = a; if (A == LastA) goto JustB; a = *(++A); } while (a <= b);
        goto BA;

AC:     do { *S++ = a; if (A == LastA) goto JustC; a = *(++A); } while (a <= c);
        goto CA;

AD:     do { *S++ = a; if (A == LastA) goto JustD; a = *(++A); } while (a <= d);
        goto DA;

AE:     do { *S++ = a; if (A == LastA) goto JustE; a = *(++A); } while (a <= e);
        goto EA;

BA:     do { *S++ = b; if (B == LastB) goto JustA; b = *(++B); } while (b < a);
        goto AB;

BC:     do { *S++ = b; if (B == LastB) goto JustC; b = *(++B); } while (b <= c);
        goto CB;

BD:     do { *S++ = b; if (B == LastB) goto JustD; b = *(++B); } while (b <= d);
        goto DB;

BE:     do { *S++ = b; if (B == LastB) goto JustE; b = *(++B); } while (b <= e);
        goto EB;

CA:     do { *S++ = c; if (C == LastC) goto JustA; c = *(++C); } while (c < a);
        goto AC;

CB:     do { *S++ = c; if (C == LastC) goto JustB; c = *(++C); } while (c < b);
        goto BC;

CD:     do { *S++ = c; if (C == LastC) goto JustD; c = *(++C); } while (c <= d);
        goto DC;

CE:     do { *S++ = c; if (C == LastC) goto JustE; c = *(++C); } while (c <= e);
        goto EC;

DA:     do { *S++ = d; if (D == LastD) goto JustA; d = *(++D); } while (d < a);
        goto AD;

DB:     do { *S++ = d; if (D == LastD) goto JustB; d = *(++D); } while (d < b);
        goto BD;

DC:     do { *S++ = d; if (D == LastD) goto JustC; d = *(++D); } while (d < c);
        goto CD;

DE:     do { *S++ = d; if (D == LastD) goto JustE; d = *(++D); } while (d <= e);
        goto ED;

EA:     do { *S++ = e; if (E == LastE) goto JustA; e = *(++E); } while (e < a);
        goto AE;

EB:     do { *S++ = e; if (E == LastE) goto JustB; e = *(++E); } while (e < b);
        goto BE;

EC:     do { *S++ = e; if (E == LastE) goto JustC; e = *(++E); } while (e < c);
        goto CE;

ED:     do { *S++ = e; if (E == LastE) goto JustD; e = *(++E); } while (e < d);
        goto DE;



        //  Below are 5 labels, one per state once four runs are exhausted,
        //  and we only have one left.

JustA:  *S++ = a; if (A == LastA) goto MergeComplete; a = *(++A); goto JustA;
JustB:  *S++ = b; if (B == LastB) goto MergeComplete; b = *(++B); goto JustB;
JustC:  *S++ = c; if (C == LastC) goto MergeComplete; c = *(++C); goto JustC;
JustD:  *S++ = d; if (D == LastD) goto MergeComplete; d = *(++D); goto JustD;
JustE:  *S++ = e; if (E == LastE) goto MergeComplete; e = *(++E); goto JustE;


MergeComplete:

        A = LastE + 1;
        B = A + RunLength;
        C = B + RunLength;
        D = C + RunLength;
        E = D + RunLength;
        F = E + RunLength;
    }
}

//////////////////////////////////////////////////////////////////////

inline void makeAMergePassThroughAllElements( STATESORT_ELEMENT* BegTo,  STATESORT_ELEMENT* BegFm, int ElementsPerRun, int NumElements )
{

    int& E = ElementsPerRun;  //  synonym
    STATESORT_ELEMENT* EndFm = BegFm + NumElements;
    STATESORT_ELEMENT* Fm = BegFm;
    STATESORT_ELEMENT* To = BegTo;
    int NumRuns  = NumElements / E;
    int NumFives = NumRuns / 5;

    if ( NumFives )
    {
        merge5RunsN( To, Fm, Fm+1*E, Fm+2*E, Fm+3*E, Fm+4*E, Fm+5*E, NumFives );

        int NumIntsInFives = NumFives * E * 5;
        Fm = BegFm + NumIntsInFives;
        To = BegTo + NumIntsInFives;
    }

    if ( Fm == EndFm ) return;

    int ElementsLeft = ( int ) ( EndFm - Fm );
    int WholeRunsLeft = ElementsLeft / E;
    int FractionalElementsLeft = ElementsLeft - WholeRunsLeft * E;

    //  The remainder will be the final run.

    int RunsLeft = WholeRunsLeft + ( FractionalElementsLeft ? 1 : 0 );
    switch ( RunsLeft )
    {
      case 1: moveARun     ( To, Fm,                               EndFm    ); break;
      case 2: merge2Runs  ( To, Fm, Fm+E,                         EndFm    ); break;
      case 3: merge3Runs  ( To, Fm, Fm+E, Fm+2*E,                 EndFm    ); break;
      case 4: merge4Runs  ( To, Fm, Fm+E, Fm+2*E, Fm+3*E,         EndFm    ); break;
      case 5: merge5RunsN ( To, Fm, Fm+E, Fm+2*E, Fm+3*E, Fm+4*E, EndFm, 1 ); break;
    }

}

//////////////////////////////////////////////////////////////////////

int StateSort( STATESORT_ELEMENT* V, int NumElements, void* LoanerBuffer )
{

    STATESORT_ELEMENT* Buffer =
            LoanerBuffer ? ( STATESORT_ELEMENT* ) LoanerBuffer
                         : ( STATESORT_ELEMENT* ) malloc( NumElements * sizeof( STATESORT_ELEMENT ) );

    if ( ! Buffer ) return __LINE__;


    //  Make sorted runs of 16 in place.
    int HowManyRunsOf16 = NumElements / 16;
    int HowManyLeftOver = NumElements % 16;
    {
        STATESORT_ELEMENT* PastA = V + 16 * HowManyRunsOf16;
        for ( STATESORT_ELEMENT* A = V; A < PastA; A += 16 )
        {
            insertionSort( A, 16 );
        }
    }

    //  Sort any left over.
    if ( HowManyLeftOver > 1 )
    {
        insertionSort( V + NumElements - HowManyLeftOver, HowManyLeftOver );
    }


    int ElementsPerRun = 16;

    STATESORT_ELEMENT* BegFm = V;
    STATESORT_ELEMENT* EndFm = V + NumElements;
    STATESORT_ELEMENT* BegTo = Buffer;
    STATESORT_ELEMENT* EndTo = Buffer + NumElements;

    //  Make multiple merging passes thru all the elements.
    for (;;)
    {
        makeAMergePassThroughAllElements( BegTo, BegFm, ElementsPerRun, NumElements );

        ElementsPerRun *= 5;
        if ( ElementsPerRun >= NumElements ) break;

        STATESORT_ELEMENT* swap = BegFm; BegFm = BegTo; BegTo = swap;
        STATESORT_ELEMENT* swAp = EndFm; EndFm = EndTo; EndTo = swAp;
    }


    if ( BegTo == Buffer )
    {
        moveARun( V, Buffer, EndTo );
    }


    if ( ! LoanerBuffer ) free( Buffer );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

