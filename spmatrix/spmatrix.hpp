#ifndef SPMATRIX_HPP
#define SPMATRIX_HPP

#include <iostream>
#include <vector>
#include <cassert>

#include "treap.hpp"

#include "Matrix.hpp" //DEBUG

template<typename T>
class SPMatrix{                                            //TODO fix cleanUp; 
    private:
 
        size_t rows;
        size_t cols;
        T nullVal = T();

        Treap<std::pair<size_t, size_t>, T> matrix;

        void removeByValue(const T &val);


    public:
        SPMatrix( size_t rows = 0, size_t cols = 0 ) : rows(rows), cols(cols) {};
        SPMatrix( const SPMatrix &other ) : rows(other.rows), cols(other.cols), matrix(other.matrix) { this->cleanUp(); }
        SPMatrix( SPMatrix &&other ) : rows  (std::exchange( other.rows, 0 )), \
                                       cols  (std::exchange( other.cols, 0 )) \
                                       { matrix = std::move(other.matrix); this->cleanUp(); }

        SPMatrix( const Matrix<T> &other ) : rows(other.rows), cols(other.cols) { //DEBUG
            for (size_t r = 0; r < rows; ++r)
                for(size_t c = 0; c < cols; ++c)
                    (*this)(r, c) = other(r, c);

            this->cleanUp();
        } 

        ~SPMatrix() {}

        SPMatrix operator+( const SPMatrix &other ) const; //TODO optimise it
        SPMatrix operator-( const SPMatrix &other ) const;
        SPMatrix operator-() const { return -1 * (*this); }
        SPMatrix operator+() const { return (*this); }

        SPMatrix operator*( const SPMatrix &other ) const; 
        SPMatrix operator*( const T &n ) const;

        SPMatrix& operator=( const SPMatrix & other );
        SPMatrix& operator=( SPMatrix&& other );

        SPMatrix operator+=( const SPMatrix &other );
        SPMatrix operator-=( const SPMatrix &other );
        SPMatrix operator*=( const SPMatrix &other );      
        SPMatrix operator*=( const T &n );   

        bool operator==( const SPMatrix &other ) const;
        bool operator!=( const SPMatrix &other ) const { return !((*this) == other); }

        void cleanUp() {return; removeByValue(nullVal); }  //TODO fix "Treap error in merge at 526" (I assume problem is in erase);

        SPMatrix Transpose() const;

        const T& operator() ( const size_t i, const size_t j ) const;
        T& operator() ( const size_t i, const size_t j );
        

    friend std::ostream& operator<<( std::ostream &out, const SPMatrix &M){
        for(size_t i=0; i < M.rows; ++i){
            for(size_t j=0; j < M.cols; ++j)
                out << M(i, j) << ' ';
            out << '\n';
        }
        out << "\n";
        return out;
    }

    friend SPMatrix operator*( const T &n, const SPMatrix &M){
        SPMatrix result(M);
        result *= n;
        return result;
    }



};

template<typename T>
SPMatrix<T> SPMatrix<T>::Transpose() const {
    SPMatrix result(cols, rows);
    for (auto elem : matrix) {
        size_t x = elem.first.first, y = elem.first.second;
        result(y, x) = elem.second;
    }
    return result;
}

template<typename T>
void SPMatrix<T>::removeByValue(const T &val) {
    std::vector<std::pair<size_t, size_t>> toBeRemoved;
    for (auto elem : matrix)
        if (elem.second == val)
            toBeRemoved.push_back(elem.first);

    for (auto elem : toBeRemoved)
        matrix.erase(elem);
}

template<typename T>
SPMatrix<T>& SPMatrix<T>::operator=(const SPMatrix<T> &other) {
    rows = other.rows;
    cols = other.cols;
    matrix = other.matrix;
    this->cleanUp();
    return (*this);
}

template<typename T>
SPMatrix<T>& SPMatrix<T>::operator=(SPMatrix<T> &&other) {
    rows = std::exchange(other.rows, 0);
    cols = std::exchange(other.cols, 0);
    matrix = std::move(other.matrix);
    this->cleanUp();
    return (*this);
}

template<typename T>
const T& SPMatrix<T>::operator() (const size_t i, const size_t j) const{
    assert(i >= 0 && i < rows && j >= 0 && j < cols);
    T* result = matrix.find(std::make_pair(i, j));
    if (result)
        return *result;
    return nullVal;
}

template<typename T>
T& SPMatrix<T>::operator() (const size_t i, const size_t j) {
    assert(i >= 0 && i < rows && j >= 0 && j < cols);
    T* node = matrix.find(std::make_pair(i, j));
    if (node)
        return *node;
    node = matrix.insert({i, j});

    return *node;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator+(const SPMatrix<T> &other) const {
    assert(rows == other.rows && cols == other.cols);
    SPMatrix result(other);
    for (auto elem : matrix)
        result(elem.first.first, elem.first.second) += elem.second;

    return result;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator-(const SPMatrix<T> &other) const {
    return (*this) + -other;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator*(const SPMatrix<T> &other) const {
    assert(cols == other.rows);

    SPMatrix<T> result(rows, other.cols);
    for (const auto pair : matrix) {                                      
        T elem = pair.second;
        size_t x = pair.first.first, y = pair.first.second;
        for (size_t r = 0; r < other.cols; ++r){
            result(x, r) += elem * other(y, r);
        }
    }
    return result;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator*(const T &n) const {
    SPMatrix result(*this);
    for (auto elem : result.matrix)
        elem.second *= n;
    return result;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator+=(const SPMatrix<T> &other) {
    assert(rows == other.rows && cols == other.cols);
    for (auto elem : other.matrix)
        (*this)(elem.first.first, elem.first.second) += elem.second;
    this->cleanUp();
    return (*this);
}


template<typename T>
SPMatrix<T> SPMatrix<T>::operator-=(const SPMatrix<T> &other) {
    (*this) += -other;
    return (*this);
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator*=(const T &n) {
    for (auto elem : matrix)
        elem.second *= n;
    this->cleanUp();
    return *this;
}

template<typename T>
SPMatrix<T> SPMatrix<T>::operator*=(const SPMatrix<T> &other) { 
    (*this) = (*this) * other;
    this->cleanUp();
    return (*this);
}

template<typename T>
bool SPMatrix<T>::operator==(const SPMatrix &other) const {
    if (rows != other.rows || cols != other.cols)
        return false;
    T *val;
    for (auto elem : matrix) 
        if (elem.second != nullVal && ((other(elem.first.first, elem.first.second) != elem.second)))
            return false;

    for (auto elem : other.matrix) 
        if (elem.second != nullVal && (((*this)(elem.first.first, elem.first.second) != elem.second)))
            return false;
    return true;
}

#endif
