#ifndef UNINT_HPP
#define UNINT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <climits>
#include <algorithm>
#include <cassert>

class ulint {
    private:
        unsigned long long bitFill;
        std::vector<unsigned long long> data;


    public:
        ulint() = default;
        ulint(const ulint &other) : bitFill(other.bitFill), data(other.data) {}
        ulint(const int64_t n) : bitFill(static_cast<uint64_t>(n < 0) * ULLONG_MAX) { data.push_back(n); }
        ulint(const uint64_t n, bool is_usigned) : bitFill(0) { data.push_back(n); } //TODO think of a better fix to "ulint(int n) is ambiguous"
        ulint(ulint &&other) : bitFill(other.bitFill) { data = std::move(other.data); }
        explicit ulint(const std::string &other);
        ~ulint() = default; 
        

        ulint operator-() const { 
            ulint result(*this);
            for (auto &elem : result.data)
                elem = ~elem;
            size_t i = 0;
            size_t end = result.data.size();
            while (i < end && __builtin_uaddll_overflow(result.data[i], 1, &result.data[i]))
                ++i;
            if (i == end)
                result.data.push_back(1);
            result.bitFill = ~bitFill;
            return result;
        }
        ulint operator+() const { return (*this); }

        ulint operator+( const ulint &other ) const; 
        ulint operator-( const ulint &other ) const;
        ulint operator*( const ulint &other ) const; //TODO
        ulint operator/( const ulint &other ) const; //TODO
        ulint operator%( const ulint &other ) const; //TODO

        ulint& operator+=( const ulint &other ) { (*this) = (*this) + other; return (*this); }
        ulint& operator-=( const ulint &other ) { (*this) = (*this) - other; return (*this); }
        ulint& operator*=( const ulint &other );
        ulint& operator/=( const ulint &other ) { (*this) = (*this) / other; return (*this); } 
        ulint& operator%=( const ulint &other ) { (*this) = (*this) % other; return (*this); }
        ulint& operator= ( const ulint &other ) { bitFill = other.bitFill; data = other.data; return (*this); }
    

        uint64_t operator%( const int &n ) const { return data[0] % n; };

        bool operator==( const ulint &other ) const { return (bitFill == other.bitFill && data == other.data); }
        bool operator< ( const ulint &other ) const;
        bool operator> ( const ulint &other ) const;

        operator bool() const { return (data.size() == 1 && data[0] == 0); }

        void dump(std::ostream &out) {
            if (bitFill)
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
            if (bitFill)
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
            if (c == '-'){
                n.bitFill = ULLONG_MAX;
                in.get(c);
            }
            while (!in.eof() && c != '\n') {
                n *= 10;
                n += c - '0';
            }
            return in;
        }
};


#endif
