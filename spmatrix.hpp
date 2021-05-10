#ifndef SPMATRIX_HPP
#define SPMATRIX_HPP

#include <iostream>
#include <vector>
#include <cassert>

#include "treap.hpp"

template<typename T>
class SPMatrix{
    private:

        // size_t len;
        size_t rows;
        size_t cols;

        Treap<std::pair<size_t, size_t>, T> matrix;


    public:
        SPMatrix( size_t rows = 0, size_t cols = 0 ) : rows(rows), cols(cols) {};
        SPMatrix( const SPMatrix &other ) : rows(other.rows), cols(other.cols), matrix(other.matrix) {}
        SPMatrix( SPMatrix &&other ) : rows  (std::exchange( other.rows, 0 )), \
                                       cols  (std::exchange( other.cols, 0 ),  \
                                       matrix(std::move(     other.matrix  ))) {}                      
        
        ~SPMatrix();

        SPMatrix operator+( const SPMatrix &other ) const; //TODO optimise it
        SPMatrix operator-( const SPMatrix &other ) const;
        SPMatrix operator-() const { return -1 * (*this); }
        SPMatrix operator+() const { return (*this); }

        SPMatrix operator*( const SPMatrix &other ) const; //TODO fix error
        SPMatrix operator*( const T &n ) const;

        SPMatrix& operator=( const SPMatrix & other );     
        SPMatrix& operator=( SPMatrix&& other );           

        SPMatrix operator+=( const SPMatrix &other );
        SPMatrix operator-=( const SPMatrix &other );
        SPMatrix operator*=( const SPMatrix &other );      //TODO
        SPMatrix operator*=( const T &n );   

        T& operator() ( const size_t i, const size_t j ) const;
        T& operator() ( const size_t i, const size_t j );
        
        void insert( size_t x, size_t y, T value );
        void pop( size_t x, size_t y );

        //TODO think if get/set len/rows/cols funcs are needed

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
SPMatrix<T>& SPMatrix<T>::operator=(const SPMatrix<T> &other) {
    rows = other.rows;
    cols = other.cols;
    matrix = other.matrix;
    return (*this);
}

template<typename T>
SPMatrix<T>& SPMatrix<T>::operator=(SPMatrix<T> &&other) {
    rows = std::exchange(other.rows, 0);
    cols = std::exchange(other.cols, 0);
    matrix = std::move(other.matrix);
    return (*this);
}

template<typename T>
T& SPMatrix<T>::operator() (const size_t i, const size_t j) const{
    return *matrix.find(std::make_pair(i, j));
}

template<typename T>
T& SPMatrix<T>::operator() (const size_t i, const size_t j) {
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

    SPMatrix result(rows, other.cols);
    for (auto pair : matrix) {                                      //TODO fix error of "passing as ‘this’ argument discards const)"
        T elem = pair.second;
        size_t x = pair.first.first, y = pair.first.second;
        for (size_t r = 0; r < other.rows; ++r)
            result(x, r) += elem * other(r, y);
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
    return *this;
}

#endif
