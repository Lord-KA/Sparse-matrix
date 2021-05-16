#ifndef MATRIX_h
#define MATRIX_h

#include <iostream>
#include <cassert>
#include <random>
#include <utility>

const long int INF = 1e2;

template<typename T> //DEBUG
class SPMatrix;

template<typename T>
class Matrix
{
    friend SPMatrix<T>;
    private:

        size_t rows;
        size_t cols;
        T determinant;
        bool determinantIsNaN;

        T *matrix;


    public:
        // Matrix() = default;
        Matrix( size_t rows = 0, size_t cols = 0);
        Matrix( const Matrix & other);
        Matrix( Matrix&& other);

        ~Matrix();

        void WriteMatrix() const;
        void ReadMatrix();

        void FillMagickSE(); // DEBUG
        void FillMatrix();   // DEBUG
        void FillMatrixOp(); // DEBUG

        void FillMatrixRandom(T (*CustomRandom)());   // DEBUG
        void FillMatrixRandom();                      // DEBUG

        T CalcDeterminant();
        Matrix GaussianMethod() const;
        void swapRows(size_t i, size_t j);
        T MinorsMethod() const;
        Matrix Minor(size_t i, size_t j) const;

        Matrix Transposition() const;

        Matrix operator+( const Matrix & other ) const;
        Matrix operator-( const Matrix & other ) const;
        Matrix operator-() const;
        Matrix operator*( const Matrix & other ) const;
        Matrix operator*( const T &n) const;
        Matrix& operator=( const Matrix & other );
        Matrix& operator=( Matrix&& other);
        
        T& operator() (const size_t i, const size_t j) const; 
        T& operator() (const size_t i, const size_t j); 

    friend std::ostream& operator<<(std::ostream &out, const Matrix &a)
    {    
        for(size_t i=0; i < a.rows; ++i){
            for(size_t j=0; j < a.cols; ++j)
                out << a(i, j) << ' ';
            out << '\n';
        }
        out << "\n";
        return out;
    }

    friend Matrix operator*( const T &n, const Matrix &M)
    {
        Matrix<T> result = Matrix<T>(M.rows, M.cols);
        for(size_t i=0; i < M.rows * M.cols; ++i)
           result.matrix[i] = M.matrix[i] * n;
        return result;
    }

};


template<typename T>
T& Matrix<T>::operator() (const size_t i, const size_t j) const
{
    return matrix[i * cols + j];
}

template<typename T>
T& Matrix<T>::operator() (const size_t i, const size_t j)
{
    return matrix[i * cols + j];
}


template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & other) const
{
    assert(rows == other.rows && cols == other.cols);
    
    Matrix result = Matrix(other);
    for(size_t i=0; i < rows * cols; ++i)
        result.matrix[i] += matrix[i];

    return result; 
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> & other) const
{
    assert(rows == other.rows && cols == other.cols);
    
    return *this + -1 * other;
}

template<typename T>
Matrix<T> Matrix<T>::operator-() const
{
    return -1 * (*this);
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> & other) const
{
    assert(cols == other.rows);


    Matrix result(rows, other.cols);
    for(size_t i=0; i < rows; ++i)
        for(size_t j=0; j < other.cols; ++j){
            T sum = 0;
            for(size_t r=0; r < cols; ++r)
                sum += (*this)(i, r) * other(r, j); 
            result(i, j) = sum;
        }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T &n) const
{
    Matrix result = Matrix(rows, cols);
    for(size_t i=0; i < rows * cols; ++i)
        result.matrix[i] = matrix[i] * n;
    return result;
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other)
{
    if(this == &other)
        return *this;
    delete[] matrix;
    matrix = std::exchange(other.matrix, nullptr);
    rows = std::exchange(other.rows, 0);
    cols = std::exchange(other.cols, 0);

    determinantIsNaN = other.determinantIsNaN;
    determinant = other.determinant;

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other)
{
    if (this == &other)
        return *this;

    rows = other.rows;
    cols = other.cols;

    determinant = other.determinant;
    determinantIsNaN = other.determinantIsNaN;

    T *temp = matrix;

    try{
        matrix = new T[rows * cols];
        for(size_t i=0; i < rows*cols; ++i)
            matrix[i] = other.matrix[i];
        delete[] temp;
    }
    catch(...)
    {
        matrix = temp;
     
        assert(false);
    }

    return *this;
}


template<typename T>
Matrix<T>::~Matrix()
{
    delete[] matrix;

    matrix = nullptr;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other)
{
    rows = other.rows;
    cols = other.cols;
    determinant = other.determinant;
    determinantIsNaN = other.determinantIsNaN;

    try{
        matrix = new T[rows * cols];

        for(size_t i = 0; i < rows * cols; ++i)
                matrix[i] = other.matrix[i];
    }
    catch(...)
    {
        assert(false);
    }
}

template<typename T>
Matrix<T>::Matrix( Matrix&& other)
{
    matrix = std::exchange(other.matrix, nullptr);
    rows = std::exchange(other.rows, 0);
    cols = std::exchange(other.cols, 0);
    determinantIsNaN = std::exchange(other.determinantIsNaN, true);
    determinant = std::exchange(other.determinant, 0);
}

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c) : rows(r), cols(c), determinantIsNaN(true), determinant(0)
{
    //rows = r;
    //cols = c;
    //determinantIsNaN = true;

    try{
        matrix = new T[rows * cols];
        for(size_t i=0; i < rows * cols; ++i)
            matrix[i] = 0;
    }
    catch(...)
    {
        assert(false);
    }
}


template<typename T>
Matrix<T> Matrix<T>::Transposition() const
{
    Matrix result = Matrix(cols, rows);
    for(size_t i=0; i < rows; ++i)
        for(size_t j=0; j < cols; ++j)
            result(j, i) = (*this)(i, j); 
    return result;
}


template<typename T>
T Matrix<T>::CalcDeterminant()
{
    if (!determinantIsNaN)
        return determinant;
    
    if (rows != cols) 
        return MinorsMethod();
    

    determinant = 1;
    determinantIsNaN = false;
    Matrix Triangular = GaussianMethod();
    if (Triangular.matrix == nullptr){
        determinant = MinorsMethod();
        return determinant;
    }
    for(size_t i=0; i < rows; ++i)
        determinant *= Triangular(i, i);

    return determinant;
}

template<typename T>
Matrix<T> Matrix<T>::GaussianMethod() const
{
    Matrix result = Matrix(*this);
    int determinant_ratio = 1;
    for(size_t k=0; k < rows; ++k)
    {
        if (result(k, k) == T(0)){
            size_t i = k + 1;
            while (i < cols && result(k, i) == T(0))
                ++i;
            if (i != cols){
                result.swapRows(k, i);
                determinant_ratio *= -1;
            }
            else
                return Matrix<T>();
        }
        
        for (size_t i=k+1; i < rows; ++i)
        {
            T ratio = result(i, k) / result(k, k);
            for (size_t j = 0; j < cols; ++j){
                result(i, j) -= result(k, j) * ratio;
            }
        }
    }
    return result * determinant_ratio;
}

template<typename T>
void Matrix<T>::swapRows(size_t row_1, size_t row_2)
{
    for(size_t i=0; i<cols; ++i){
        T temp = (*this)(row_1, i);
        (*this)(row_1, i) = (*this)(row_2, i);
        (*this)(row_2, i) = temp;
    }
}

template<typename T>
T Matrix<T>::MinorsMethod() const
{
    T result = 0;
    if (rows==2 && cols==2) 
        return (*this)(0,0) * (*this)(1,1) - (*this)(0, 1) * (*this)(1, 0);

    for(size_t i=0; i < rows; ++i){
        result += (*this)(0, i) * T(i%2==0?1:-1) * Minor(0, i).MinorsMethod();
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::Minor(size_t i, size_t j) const
{
    Matrix result(rows-1, cols-1);
    bool flag_row = false; // flag for skipping r == i

    for(size_t r=0; r<rows-1; ++r){
        bool flag_col = false; // flag for skipping c = j every r

        for(size_t c=0; c<cols-1; ++c){
            if (r == i) flag_row = true;
            if (c == j) flag_col = true;
        
            result(r, c) = (*this)(r + flag_row, c + flag_col);
        }
    }
    return result;
}


template<typename T>
void Matrix<T>::FillMagickSE() // Funcs to fill matrix in a weird way for testing purposes
{
    size_t cnt = 0;
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j){
            if (cols <= j + i){
                ++cnt;
                (*this)(i, j) = cnt; 
            }
    }
}

template<typename T>
void Matrix<T>::FillMatrix()
{
    size_t cnt = 0;
    for(size_t i = 0; i < rows; ++i)
        for(size_t j = 0; j < cols; ++j)
        {
            (*this)(i, j) = cnt; 
            ++cnt;
        }
}

template<typename T>
void Matrix<T>::FillMatrixOp()
{
    size_t cnt = rows * cols;
    for(size_t i = 0; i < rows; ++i)
        for(size_t j = 0; j < cols; ++j)
        {
            (*this)(i, j) = cnt; 
            --cnt;
        }
}


template<typename T>
T Random()
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    std::uniform_int_distribution<T> uniform_dist(-INF, INF);
    return uniform_dist(e1);
}

template<>
double Random()
{
    static std::random_device r;
    static std::default_random_engine e1(r());
    static std::uniform_real_distribution<double> uniform_dist(-INF, INF);
    return uniform_dist(e1);
}

template<typename T>
void Matrix<T>::FillMatrixRandom(T (*CustomRandom)())
{
    for(size_t i = 0; i < rows; ++i)
        for(size_t j = 0; j < cols; ++j){
            (*this)(i, j) = (*CustomRandom)(); 
        }
}

template<typename T>
void Matrix<T>::FillMatrixRandom()
{
   for(size_t i = 0; i < rows; ++i)
        for(size_t j = 0; j < cols; ++j){
            (*this)(i, j) = Random<T>(); 
        }
}


template<typename T>
void Matrix<T>::WriteMatrix() const
{
    std::cout << rows << ' ' << cols << std::endl;

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j)
            std::cout << (*this)(i, j) << ' '; 
        std::cout << std::endl;
    }
}


template<typename T>
void Matrix<T>::ReadMatrix()
{
    size_t r, c;
    std::cin >> r >> c;
    if (r != rows || c != cols)
    {
        T temp = matrix;
        try{
            matrix = new T[rows * cols];
            rows = r;
            cols = c;
        }
        catch(...)
        {
            matrix = temp;
            assert(false);
        }
    }

    for(size_t i=0; i<rows; ++i)
        for(size_t j=0; j<cols; ++j)
            std::cin >> (*this)(i, j);
}


#endif
