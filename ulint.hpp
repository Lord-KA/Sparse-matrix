#ifndef UNINT_HPP
#define UNINT_HPP

#include <string>
#include <iostream>
#include <bitset>

class ulint{
    private:
        std::string num;


    public:
        ulint();
        ulint(const ulint &other);
        ulint(const ulint &&other);
        ulint(const std::string &other);
        ~ulint() = default; //TODO find out if destructor is really needed


        ulint operator+( const ulint &other ) const; //TODO think about creating template operators for all integer base types; same thing about constructors
        ulint operator-( const ulint &other ) const;
        ulint operator*( const ulint &other ) const;
        ulint operator/( const ulint &other ) const;
        ulint& operator+=( const ulint &other );
        ulint& operator-=( const ulint &other );
        ulint& operator*=( const ulint &other );
        ulint& operator/=( const ulint &other );
        ulint& operator=( const ulint &other );
        
        operator std::string() const;


        friend std::ostream& operator<<(std::ostream &out, const ulint &n);

        friend std::istream& operator>>(const std::istream &in, ulint &n);
};

#endif
