#ifndef UNINT_HPP
#define UNINT_HPP

#include <string>
#include <deque>
#include <iostream>
#include <bitset>
#include <climits>
#include <algorithm>

class ulint{
    private:
        bool neg;
        std::deque<unsigned long long> data;


    public:
        ulint() = default;
        ulint(const ulint &other) : neg(other.neg), data(other.data) {}
        ulint(const int n) : neg(n < 0) { data.push_back(std::abs(n)); }
        ulint(const uint64_t n, bool is_usigned) : neg(0) { data.push_back(n); } //TODO think of a better fix to "ulint(int n) is ambiguous"
        ulint(ulint &&other) : neg(other.neg) { data = std::move(other.data); }
        explicit ulint(const std::string &other);
        ~ulint() = default; 
        

        ulint operator-() const { ulint result(*this); result.neg = !neg; return result; }
        ulint operator+() const { return (*this); }

        ulint operator+( const ulint &other ) const; 
        ulint operator-( const ulint &other ) const;
        ulint operator*( const ulint &other ) const; //TODO
        ulint operator/( const ulint &other ) const; //TODO
        ulint operator%( const ulint &other ) const; //TODO

        ulint& operator+=( const ulint &other ) { (*this) = (*this) + other; return (*this); }
        ulint& operator-=( const ulint &other ) { (*this) = (*this) - other; return (*this); }
        ulint& operator*=( const ulint &other ) { (*this) = (*this) * other; return (*this); }
        ulint& operator/=( const ulint &other ) { (*this) = (*this) / other; return (*this); } 
        ulint& operator%=( const ulint &other ) { (*this) = (*this) % other; return (*this); }
        ulint& operator= ( const ulint &other ) { neg = other.neg; data = other.data; return (*this); }
    

        uint64_t operator%( const int &n ) const { return data[0] % n; };

        bool operator==( const ulint &other ) const { return (neg == other.neg && data == other.data); }
        bool operator< ( const ulint &other ) const;
        bool operator> ( const ulint &other ) const;

        operator bool() const { return (data.size() == 1 && data[0] == 0); }

        void dump(std::ostream &out) {
            if (neg)
                out << "- ";
            for (auto elem : data)
                out << elem << ' ';
            out << '\n';
        }
        
        explicit operator std::string() const {
            ulint other(*this);
            std::string result = "";
            do {
                result.push_back(other % 10);
                other /= 10;
            } while (other);
            if (neg)
                result.push_back('-');
            std::reverse(result.begin(), result.end());
            return result;
        }


        friend std::ostream& operator<<(std::ostream &out, const ulint &n) {
            out << static_cast<std::string>(n);
            return out;
        }

        friend std::istream& operator>>(std::istream &in, ulint &n) {
            char c;
            in.get(c);
            while (!in.eof() && c != '\n') {
                n *= 10;
                n += c - '0';
            }
            return in;
        }
};


#endif
