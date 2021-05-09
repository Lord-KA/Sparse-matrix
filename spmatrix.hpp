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
        SPMatrix() = default;                              //TODO think if this is fine
        SPMatrix( size_t rows, size_t cols ) : rows(rows), cols(cols) {};
        SPMatrix( const SPMatrix &other ) : rows(other.rows), cols(other.cols), matrix(other.matrix) {}
        SPMatrix( SPMatrix &&other );                      //TODO
        
        ~SPMatrix();

        SPMatrix operator+( const SPMatrix &other ) const; //TODO optimise it
        SPMatrix operator-( const SPMatrix &other ) const;
        SPMatrix operator-() const { return -1 * (*this); }
        SPMatrix operator+() const { return (*this); }

        SPMatrix operator*( const SPMatrix &other ) const; //TODO
        SPMatrix operator*( const T &n ) const;

        SPMatrix& operator=( const SPMatrix & other );     //TODO
        SPMatrix& operator=( SPMatrix&& other );           //TODO

        SPMatrix operator+=( const SPMatrix &other );
        SPMatrix operator-=( const SPMatrix &other );
        SPMatrix operator*=( const SPMatrix &other );      //TODO
        SPMatrix operator*=( const T &n );   

        T& operator() ( const size_t i, const size_t j ) const;
        T& operator() ( const size_t i, const size_t j );
        
        void insert(size_t x, size_t y, T value);
        void pop(size_t x, size_t y);

        //TODO think if get/set len/rows/cols funcs are needed

    friend std::ostream& operator<<( std::ostream &out, const SPMatrix &a){
                                                           //TODO add outp
    }

    friend SPMatrix operator*( const T &n, const SPMatrix &M){
        SPMatrix result(M);
        result *= n;
        return result;
    }



};


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
    //TODO write it
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
