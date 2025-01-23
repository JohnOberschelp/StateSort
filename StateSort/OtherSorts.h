
//////////////////////////////////////////////////////////////////////

//  BubbleSort

//--------------------------------------------------------------------

void BubbleSort( SORT_ELEMENT* V, int NumElements )
{
    bool Swapped;
    for ( int Range = NumElements-1; Range > 1; Range-- )
    {
        Swapped = false;

        for ( int i = 0; i < Range; i++ )
        {
            if ( V[i] > V[i+1] )
            {
                SORT_ELEMENT Swap = V[i]; V[i] = V[i+1]; V[i+1] = Swap;
                Swapped = true;
            }
        }

        if ( ! Swapped ) break;
    }
}

//////////////////////////////////////////////////////////////////////

//  SelectionSort

//--------------------------------------------------------------------

void SelectionSort( SORT_ELEMENT* V, int NumElements )
{
    for ( int i = 0; i < NumElements-1; i++ )
    {
        int Min = i;
        
        for ( int j = i+1; j < NumElements; j++ )
        {
            if ( V[j] < V[Min] ) Min = j;
        }
        
        SORT_ELEMENT Swap = V[i]; V[i] = V[Min]; V[Min] = Swap;
    }
}

//////////////////////////////////////////////////////////////////////

//  ShellSort

//--------------------------------------------------------------------

void ShellSort( SORT_ELEMENT* V, int NumElements )
{
////for ( int Gap = NumElements / 2; Gap; Gap /= 2 )
    for ( int Gap = NumElements / 2; Gap; Gap = Gap == 2 ? 1 : ( int)( Gap / 2.2 ) )
    {
        for( int i = Gap; i < NumElements; i++ )
        {
            SORT_ELEMENT Keep = V[i];
            int j = i;
            while ( j >= Gap && Keep < V[j - Gap] )
            {
                V[j] = V[j - Gap];   
                j -= Gap;            
            }
            V[j] = Keep;
        }
    }
}

//////////////////////////////////////////////////////////////////////

//  HeapSort

//--------------------------------------------------------------------

void heapify( SORT_ELEMENT* V, int NumElements, int i )
{
    int largest = i;

    int l = 2*i+1;
    int r = 2*i+2;

    if ( l < NumElements && V[l] > V[largest] ) largest = l;

    if ( r < NumElements && V[r] > V[largest] ) largest = r;

    if ( largest != i )
    {
        SORT_ELEMENT Swap = V[i]; V[i] = V[largest]; V[largest] = Swap;

        heapify( V, NumElements, largest );
    }
}

//--------------------------------------------------------------------

void HeapSort( SORT_ELEMENT* V, int NumElements )
{

    for ( int i = NumElements/2-1; i >= 0; i-- )
    {
        heapify( V, NumElements, i );
    }

    for (int i = NumElements-1; i > 0; i-- )
    {
        SORT_ELEMENT Swap = V[0]; V[0] = V[i]; V[i] = Swap;
        heapify( V, i, 0 );
    }
}

//////////////////////////////////////////////////////////////////////

//  Lomuto’s QuickSort

//--------------------------------------------------------------------
//
//int LomutosPartition( SORT_ELEMENT* V, int Beg, int End )
//{
//    SORT_ELEMENT Pivot = V[End];
//    int i = Beg-1;
//
//    for ( int j = Beg; j < End; j++ )
//    {
//        if ( V[j] <= Pivot )
//        {
//            i++;
//            SORT_ELEMENT Swap = V[i]; V[i] = V[j]; V[j] = Swap;
//        }
//    }
//    SORT_ELEMENT Swap = V[i+1]; V[i+1] = V[End]; V[End] = Swap;
//    return i+1;
//}
//
//--------------------------------------------------------------------

int LomutosPartition( SORT_ELEMENT* V, int Beg, int End )
{
    SORT_ELEMENT Pivot = V[End];
    int i = Beg;

    for ( int j = Beg; j < End; j++ )
    {
        if ( V[j] <= Pivot )
        {
            SORT_ELEMENT Swap = V[i]; V[i] = V[j]; V[j] = Swap;
            i++;
        }
    }
    SORT_ELEMENT Swap = V[i]; V[i] = V[End]; V[End] = Swap;
    return i;
}

//--------------------------------------------------------------------

void LomutosQuickSort( SORT_ELEMENT* V, int Beg, int End )
{
    if ( Beg < End )
    {
        int Pivot = LomutosPartition( V, Beg, End );

        LomutosQuickSort( V, Beg, Pivot-1      );
        LomutosQuickSort( V,      Pivot+1, End );
    }
}

//////////////////////////////////////////////////////////////////////

//  Hoare’s QuickSort

//--------------------------------------------------------------------

int HoaresPartition( SORT_ELEMENT* V, int Beg, int End )
{
    SORT_ELEMENT pivot = V[Beg];
    int i = Beg-1;
    int j = End+1;

    for ( ;; )
    {
        do { i++; } while ( V[i] < pivot );
        do { j--; } while ( V[j] > pivot );
        if ( i >= j ) return j;

        SORT_ELEMENT Swap = V[i]; V[i] = V[j]; V[j] = Swap;
    }
}

//--------------------------------------------------------------------

void HoaresQuickSort( SORT_ELEMENT* V, int Beg, int End )
{
    if ( Beg < End )
    {
        int iPivot = HoaresPartition( V, Beg, End );

        HoaresQuickSort( V, Beg, iPivot        );
        HoaresQuickSort( V,      iPivot+1, End );
    }
}

//////////////////////////////////////////////////////////////////////

//  HybridMergeSort

//--------------------------------------------------------------------

static inline int intLogBase2( int n )
{
    int r = 0;
//    if ( n >> 32 ) { r += 32; n >>= 32; }
    if ( n >> 16 ) { r += 16; n >>= 16; }
    if ( n >>  8 ) { r +=  8; n >>=  8; }
    if ( n >>  4 ) { r +=  4; n >>=  4; }
    if ( n >>  2 ) { r +=  2; n >>=  2; }
    if ( n >>  1 ) { r ++;              }
    return r;
}

//--------------------------------------------------------------------

static void iSort( SORT_ELEMENT* V, int NumElements )
{
    int i, j;
    SORT_ELEMENT v;
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

//--------------------------------------------------------------------

static inline void merge( SORT_ELEMENT* Fm, SORT_ELEMENT* To, int iBeg, int iMid, int iEnd )
{
    To += iBeg;
    int l = iBeg;
    int r = iMid;
    for (;;)
    {
        if ( Fm[l] <= Fm[r] )
        {
            *To++ = Fm[l++];
            if ( l == iMid ) { while(r < iEnd) *To++ = Fm[r++]; break; }
        }
        else
        {
            *To++ = Fm[r++];
            if ( r == iEnd ) { while(l < iMid) *To++ = Fm[l++]; break; }
        }
    }
}

//--------------------------------------------------------------------

static inline void hybridMergeSort( SORT_ELEMENT* Fm, SORT_ELEMENT* To, int NumElements )
{
    //  Pick a run length that will have us end all passes back in Fm.
    int RunLength = intLogBase2( NumElements-1 ) & 1 ? 16 : 32;

    //  Do insertion sorts at this run length through all elements.
    int i;
    for ( i = 0; i < NumElements - RunLength; i += RunLength )
        iSort( Fm+i, RunLength );
    iSort( Fm+i, NumElements - i );

    //  Make merge passes through all elements, doubling RunLength each time.
    for ( ; RunLength < NumElements; RunLength <<= 1 )
    {
        int iBeg, iMid, iEnd;
        for ( iBeg = 0; iBeg < NumElements; iBeg = iEnd )
        {
            iMid = iBeg + RunLength;
            if ( iMid >= NumElements )
            {
                while ( iBeg < NumElements ) { To[iBeg] = Fm[iBeg]; iBeg++; }
                break;
            }
            iEnd = iMid + RunLength;
            if ( iEnd > NumElements ) iEnd = NumElements;
            merge(Fm, To, iBeg, iMid, iEnd);
        }
        register SORT_ELEMENT* Swap = Fm; Fm = To; To = Swap;
    }
}

//--------------------------------------------------------------------

int HybridMergeSort( SORT_ELEMENT* V, int NumElements, void* LoanerBuffer )
{
    if ( NumElements < 2 ) return 0;

    SORT_ELEMENT* Buffer =
            LoanerBuffer ? ( SORT_ELEMENT* ) LoanerBuffer
                         : ( SORT_ELEMENT* ) malloc( NumElements * sizeof( SORT_ELEMENT ) );
    if ( ! Buffer ) return __LINE__;

    hybridMergeSort( V, Buffer, NumElements );

    if ( ! LoanerBuffer ) free( Buffer );

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

