#ifndef UNINT_HPP
#define UNINT_HPP

#include <string>
#include <iostream>
#include <bitset>


class ulint{
    //private:
    public:
        std::string num;


    public:
        ulint() = default;
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
        
        operator std::string() const { 
            std::string result;
            for (auto elem : num){
                char buf = elem;
                result += (buf % 8 + '0');
                buf >>= 3;
                result += (buf % 8 + '0');
                buf >>= 3;
                result += (buf % 8 + '0');
                buf >>= 3;
                result += (buf % 8 + '0');
 
            }
            return result;
        }


        friend std::ostream& operator<<(std::ostream &out, const ulint &n) {
            for (auto elem : n.num){
                char buf = elem;
                while (buf){
                    out << (buf % 8 + '0');
                    buf /= 8;
                }
            }
            return out;
        }

        friend std::istream& operator>>(std::istream &in, ulint &n) {
            char c;
            char buf = 0;
            int cnt = 0;
            in.get(c);
            while (!in.eof() && c != '\n') {
                ++cnt;
                buf += c - '0';
                if (cnt == 4) {
                    n.num += buf;
                    cnt = 0;
                }
                buf *= 8;
                std::cout << static_cast<int>(buf) << '\n';
                in.get(c);
            }
            if (buf)
                n.num += buf;
            return in;
        }
};


#endif
