#ifndef SPMATRIX_HPP
#define SPMATRIX_HPP

#include <iostream>
#include <vector>
#include <cassert>

#include "treap.hpp"

template<typename T>
class SPMatrix{
    private:

        size_t len;
        size_t rows;
        size_t cols;

        Treap<std::pair<size_t, size_t>, T> matrix;


    public:
        SPMatrix() = default;
        SPMatrix( size_t rows, size_t cols );
        SPMatrix( const SPMatrix &other );
        SPMatrix( SPMatrix &&other );
        
        ~SPMatrix();

        SPMatrix operator+( const SPMatrix &other) const;
        SPMatrix operator-( const SPMatrix &other) const;
        SPMatrix operator-() const;

        inline SPMatrix operator+() const {return SPMatrix(*this);};

        SPMatrix operator*( const SPMatrix &other ) const;
        SPMatrix operator*( const long long int &n ) const;

        SPMatrix& operator=( const SPMatrix & other );
        SPMatrix& operator=( SPMatrix&& other);

        SPMatrix operator+=( const SPMatrix &other ) const;
        SPMatrix operator-=( const SPMatrix &other ) const;
        SPMatrix operator*=( const SPMatrix &other ) const;
        SPMatrix operator*=( const T &n ) const;

        T& operator() ( const size_t i, const size_t j ) const;
        T& operator() ( const size_t i, const size_t j );

        //TODO think if get/set len/rows/cols funcs are needed

    friend std::ostream& operator<<( std::ostream &out, const SPMatrix &a){
        //TODO add outp
    }

    friend SPMatrix operator*( const T &n, const SPMatrix &M){
        //TODO add multiplication
    }



};

#endif
