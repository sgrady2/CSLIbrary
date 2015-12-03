//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb 24 03:30:03 PST 1999
// Last Modified: Wed Feb 24 03:30:03 PST 1999
// Filename:      /home/craig/sigNet/Matrix.h
// Syntax:        C++ 
// $Smake:        smake %b.cpp
//


#ifndef _MATRIX_H_INCLUDED
#define _MATRIX_H_INCLUDED

#include <iostream.h>

template<class type>
class Matrix {
   public: 
                    Matrix         (void);
                    Matrix         (const Matrix<type>& aMatrix);
                    Matrix         (int rowCount, int columnCount);
                    Matrix         (int columnCount);
                    Matrix         (type* data, int rowCount, int columnCount);
                   ~Matrix         ();

      Matrix<type>& add            (type scalar);
      Matrix<type>& add            (const Matrix<type>& aMatrix);
      type&         cell           (int row, int column);
      type          cell           (int row, int column) const;
      int           getSize        (void) const;
      int           getRowCount    (void) const;
      int           getColumnCount (void) const;
      Matrix<type>& multiply       (type scalar);
      type&         operator[]     (int index);
      Matrix<type>& operator=      (const Matrix<type>& aMatrix);
      Matrix<type>& operator+=     (const Matrix<type>& aMatrix);
      void          setSize        (int row, int column);
      void          transpose      (void);
      void          zero           (void);


   // static functions:
   static Matrix<type>& multiply 
      (Matrix<type>& output, const Matrix<type>& one, Matrix<type>& two);
   static Matrix<type>& multiply 
      (Matrix<type>& output, Matrix<type>& one, type aScalar);
   static Matrix<type>& add   
      (Matrix<type>& output, const Matrix<type>& one, const Matrix<type>& two);
   static void          transpose      
      (Matrix<type>& output, const Matrix<type>& one);

   protected:
      int transposeQ;         // whether or not the matrix is transposed
      int dim1;               // number of rows/columns in the matrix
      int dim2;               // number of rows/columns in the matrix
      type* storage;          // where the matrix of numbers is stored


   private:
      void      checkdim      (int row, int column) const;

};


// helping functions:

template<class type>
ostream& operator<<(ostream& out, const Matrix<type>& aMatrix);



// This following line is necessary because of template behavior:
#include "Matrix.cpp"


#endif // _MATRIX_H_INCLUDED

// md5sum:	59b32a962783202cb982246922f21b54  Matrix.h
