// header file for matrix template class
// NOTE:  all matrices handled here must be SQUARE 
//        (i.e., # rows = # columns)
// in addition, all DIAGONAL ELEMENTS MUST BE NONZERO
// written by Mike Dinolfo 12/98
// version 1.0

#ifndef __mjdmatrix_h
#define __mjdmatrix_h
#include <iostream>

using namespace std;

// generic object (class) definition of matrix:
template <class D> class matrix{
  // NOTE: maxsize determines available memory storage, but
  // actualsize determines the actual size of the stored matrix in use
  // at a particular time.
  int maxsize;  // max number of rows (same as max number of columns)
  int actualsize;  // actual size (rows, or columns) of the stored matrix
  D* data;      // where the data contents of the matrix are stored
  void allocate()   {
    delete[] data;
    data = new D [maxsize*maxsize];
    };
    public:
  matrix() {};                  // private ctor's
  matrix(int newmaxsize) {matrix(newmaxsize,newmaxsize);};

  matrix(int newmaxsize, int newactualsize)  { // the only public ctor
    if (newmaxsize <= 0) newmaxsize = 5;
    maxsize = newmaxsize; 
    if ((newactualsize <= newmaxsize)&&(newactualsize>0))
      actualsize = newactualsize;
    else 
      actualsize = newmaxsize;
    // since allocate() will first call delete[] on data:
    data = 0;
    allocate();
    };
  ~matrix() { delete[] data; };
  void comparetoidentity()  {
    int worstdiagonal = 0;
    D maxunitydeviation = 0.0;
    D currentunitydeviation;
    for ( int i = 0; i < actualsize; i++ )  {
      currentunitydeviation = data[i*maxsize+i] - 1.;
      if ( currentunitydeviation < 0.0) currentunitydeviation *= -1.;
      if ( currentunitydeviation > maxunitydeviation )  {
           maxunitydeviation = currentunitydeviation;
           worstdiagonal = i;
           }
      }
    int worstoffdiagonalrow = 0;
    int worstoffdiagonalcolumn = 0;
    D maxzerodeviation = 0.0;
    D currentzerodeviation ;
    for ( int i = 0; i < actualsize; i++ )  {
      for ( int j = 0; j < actualsize; j++ )  {
        if ( i == j ) continue;  // we look only at non-diagonal terms
        currentzerodeviation = data[i*maxsize+j];
        if ( currentzerodeviation < 0.0) currentzerodeviation *= -1.0;
        if ( currentzerodeviation > maxzerodeviation )  {
          maxzerodeviation = currentzerodeviation;
          worstoffdiagonalrow = i;
          worstoffdiagonalcolumn = j;
          }

        }
      }
    cout << "Worst diagonal value deviation from unity: " 
       << maxunitydeviation << " at row/column " << worstdiagonal << endl;
    cout << "Worst off-diagonal value deviation from zero: " 
       << maxzerodeviation << " at row = " << worstoffdiagonalrow 
       << ", column = " << worstoffdiagonalcolumn << endl;
  }
  void printMatrix(){
      cout << "[";
      for (int i=0; i< actualsize; ++i){
          for (int j=0; j< actualsize; ++j){                
                cout << data[ i * maxsize + j ] << " ";
          }
          cout << "; ";
      }
      cout << "]\n";
  }  
  void settoproduct(matrix& left, matrix& right)  {
    actualsize = left.getactualsize();
    if ( maxsize < left.getactualsize() )   {
      maxsize = left.getactualsize();
      allocate();
      }
    for ( int i = 0; i < actualsize; i++ )
      for ( int j = 0; j < actualsize; j++ )  {
        D sum = 0.0;
        D leftvalue, rightvalue;
        bool success;
        for (int c = 0; c < actualsize; c++)  {
            left.getvalue(i,c,leftvalue,success);
            right.getvalue(c,j,rightvalue,success);
            sum += leftvalue * rightvalue;
            }
        setvalue(i,j,sum);
        }
    }
  void copymatrix(matrix&  source)  {
    actualsize = source.getactualsize();
    if ( maxsize < source.getactualsize() )  {
      maxsize = source.getactualsize();
      allocate();
      }
    for ( int i = 0; i < actualsize; i++ )
      for ( int j = 0; j < actualsize; j++ )  {
        D value;
        bool success;
        source.getvalue(i,j,value,success);
        data[i*maxsize+j] = value;
        }
    };
  void setactualsize(int newactualsize) {
    if ( newactualsize > maxsize )
      {
        maxsize = newactualsize ; // * 2;  // wastes memory but saves
                                      // time otherwise required for
                                      // operation new[]
        allocate();
      }
    if (newactualsize >= 0) actualsize = newactualsize;
    };
  int getactualsize() { return actualsize; };
  void getvalue(int row, int column, D& returnvalue, bool& success)   {
    if ( (row>=maxsize) || (column>=maxsize) 
      || (row<0) || (column<0) )
      {  success = false;
         return;    }
    returnvalue = data[ row * maxsize + column ];
    success = true;
    };
    
  D getValue(int pos){
      if ((pos>=maxsize*maxsize) || (pos<maxsize))
        return -1;
      return data[pos];
  }
  bool setvalue(int row, int column, D newvalue)  {
    if ( (row >= maxsize) || (column >= maxsize) 
        || (row<0) || (column<0) ) return false;
    data[ row * maxsize + column ] = newvalue;
    return true;
    };
    
    void invert() {
    /* This function calculates the inverse of a square matrix
     *
     * matrix_inverse(double *Min, double *Mout, int actualsize)
     *
     * Min : Pointer to Input square Double Matrix
     * Mout : Pointer to Output (empty) memory space with size of Min
     * actualsize : The number of rows/columns
     *
     * Notes:
     *  - the matrix must be invertible
     *  - there's no pivoting of rows or columns, hence,
     *        accuracy might not be adequate for your needs.
     *
     * Code is rewritten from c++ template code Mike Dinolfo
     */
    /* Loop variables */
    int i, j, k;
    /* Sum variables */
    double sum,x;
    
    /*  Copy the input matrix to output matrix */
    //for(i=0; i<actualsize*actualsize; i++) { Mout[i]=Min[i]; }
    
    /* Add small value to diagonal if diagonal is zero */
    for(i=0; i<actualsize; i++)
    { 
        j=i*actualsize+i;
        if((data[j]<1e-12)&&(data[j]>-1e-12)){ data[j]=1e-12; }
    }
    
    /* Matrix size must be larger than one */
    if (actualsize <= 1) return;
    
    for (i=1; i < actualsize; i++) {
        data[i] /= data[0]; /* normalize row 0 */
    }
    
    for (i=1; i < actualsize; i++)  {
        for (j=i; j < actualsize; j++)  { /* do a column of L */
            sum = 0.0;
            for (k = 0; k < i; k++) {
                sum += data[j*actualsize+k] * data[k*actualsize+i];
            }
            data[j*actualsize+i] -= sum;
        }
        if (i == actualsize-1) continue;
        for (j=i+1; j < actualsize; j++)  {  /* do a row of U */
            sum = 0.0;
            for (k = 0; k < i; k++) {
                sum += data[i*actualsize+k]*data[k*actualsize+j];
            }
            data[i*actualsize+j] = (data[i*actualsize+j]-sum) / data[i*actualsize+i];
        }
    }
    for ( i = 0; i < actualsize; i++ )  /* invert L */ {
        for ( j = i; j < actualsize; j++ )  {
            x = 1.0;
            if ( i != j ) {
                x = 0.0;
                for ( k = i; k < j; k++ ) {
                    x -= data[j*actualsize+k]*data[k*actualsize+i];
                }
            }
            data[j*actualsize+i] = x / data[j*actualsize+j];
        }
    }
    for ( i = 0; i < actualsize; i++ ) /* invert U */ {
        for ( j = i; j < actualsize; j++ )  {
            if ( i == j ) continue;
            sum = 0.0;
            for ( k = i; k < j; k++ ) {
                sum += data[k*actualsize+j]*( (i==k) ? 1.0 : data[i*actualsize+k] );
            }
            data[i*actualsize+j] = -sum;
        }
    }
    for ( i = 0; i < actualsize; i++ ) /* final inversion */ {
        for ( j = 0; j < actualsize; j++ )  {
            sum = 0.0;
            for ( k = ((i>j)?i:j); k < actualsize; k++ ) {
                sum += ((j==k)?1.0:data[j*actualsize+k])*data[k*actualsize+i];
            }
            data[j*actualsize+i] = sum;
        }
    }
}

    double determinant(){
        D **a = NULL ;
        a = (D **) malloc(actualsize* sizeof(D *)) ;

        for (int i = 0 ; i < actualsize ; i++)
            a[i] = (D *) malloc(actualsize* sizeof(D)) ;

        D value;
        bool success;
        for (int i = 0 ; i < actualsize ; i++){
            for (int j = 0; j < actualsize; j++){
                getvalue(i,j,value,success);
                a[i][j] = value;
            }
        }
        
        double det = _determinant(a,actualsize);
        
        for (int i = 0 ; i < actualsize ; i++) free(a[i]);
        free(a);
        // compute determinant recursively
        return det;
    }

    double _determinant(D **a, int n)
    {
        int i,j,j1,j2 ;                    // general loop and matrix subscripts
        double det = 0 ;                   // init determinant
        D **m = NULL ;                // pointer to pointers to implement 2d
       
        // square array
        

        if (n < 1)    {   }                // error condition, should never get here

        else if (n == 1) {                 // should not get here
            det = a[0][0] ;
            }

        else if (n == 2)  {                // basic 2X2 sub-matrix determinate
                                        // definition. When n==2, this ends the
            det = a[0][0] * a[1][1] - a[1][0] * a[0][1] ;// the recursion series
            }


                                        // recursion continues, solve next sub-matrix
        else {                             // solve the next minor by building a
                                        // sub matrix
            det = 0 ;                      // initialize determinant of sub-matrix

                                            // for each column in sub-matrix
            for (j1 = 0 ; j1 < n ; j1++) {
                                            // get space for the pointer list
                m = (D **) malloc((n-1)* sizeof(D *)) ;

                for (i = 0 ; i < n-1 ; i++)
                    m[i] = (D *) malloc((n-1)* sizeof(D)) ;
                        //     i[0][1][2][3]  first malloc
                        //  m -> +  +  +  +   space for 4 pointers
                        //       |  |  |  |          j  second malloc
                        //       |  |  |  +-> _ _ _ [0] pointers to
                        //       |  |  +----> _ _ _ [1] and memory for
                        //       |  +-------> _ a _ [2] 4 doubles
                        //       +----------> _ _ _ [3]
                        //
                        //                   a[1][2]
                        // build sub-matrix with minor elements excluded
                for (i = 1 ; i < n ; i++) {
                    j2 = 0 ;               // start at first sum-matrix column position
                                        // loop to copy source matrix less one column
                    for (j = 0 ; j < n ; j++) {
                        if (j == j1) continue ; // don't copy the minor column element

                        m[i-1][j2] = a[i][j] ;  // copy source element into new sub-matrix
                                                // i-1 because new sub-matrix is one row
                                                // (and column) smaller with excluded minors
                        j2++ ;                  // move to next sub-matrix column position
                        }
                    }

                det += pow(-1.0,1.0 + j1 + 1.0) * a[0][j1] * _determinant(m,n-1) ;
                                                // sum x raised to y power
                                                // recursively get determinant of next
                                                // sub-matrix which is now one
                                                // row & column smaller

                for (i = 0 ; i < n-1 ; i++) {free(m[i]);}// free the storage allocated to
                                                // to this minor's set of pointers
                free(m) ;                       // free the storage for the original
                                               // pointer to pointer
            }
        }
        return(det) ;
    }
    
    

};

#endif

