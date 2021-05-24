#include "ulint.hpp"

std::ostream& operator<<(std::ostream& os, __int128 x){ //DEBUG
    if(x<0) return os << "-" << -x;
    if(x<10) return  os << (char)(x+'0');
    return os << x/10 << (char)(x%10+'0');
}

static inline __int128 mulhi64(uint64_t a, uint64_t b) {
     return a * (unsigned __int128)b;
}

template<typename T>
bool max_out_of_two(const std::vector<T> &one, const std::vector<T> &two) {
    if (one.size() > two.size())
        return 0;
    if (one.size() < two.size())
        return 1;
    for (size_t i = one.size() - 1; i != ULLONG_MAX; --i){
        if (one[i] > two[i])
            return 0;
        else if (two[i] > one[i])
            return 1;
    }
    return 1;
}


ulint ulint::operator+(const ulint &other) const {
    ulint result;
    size_t end;
    if (data.size() > other.data.size()){
        result = (*this);
        end = other.data.size();
    }
    else {
        result = other;
        end = data.size();
    }
    result.bitFill = bitFill^other.bitFill;

    bool overflow = false;
    for (size_t i = 0; i < end; ++i) {
        if (__builtin_uaddll_overflow(result.data[i], other.data[i], &result.data[i]) && overflow) {
            result.data[i] += 1;
        }
        else if (overflow)
            overflow = __builtin_uaddll_overflow(result.data[i], 1, &result.data[i]);
    }
    bool flag = false;
    while (end < result.data.size() && result.data[end] == ULLONG_MAX) {
        result.data[end] = 0;
        ++end;
        flag = true;
    }
    if (!flag)
        return result;
  
    if (end == result.data.size())
        result.data.push_back(1);
    else
        result.data[end] += 1;
  
    return result;
}

ulint ulint::operator-(const ulint &other) const {
    return (*this) + -other;
}

ulint ulint::operator*(const ulint &other) const{
    if (other.data.size() < 10 || data.size() < 10){
        ulint result;
        result.data.resize(data.size() + other.data.size());
        result.bitFill = bitFill ^ other.bitFill;
        for (auto &elem : result.data)
            elem = result.bitFill;
        
        for (size_t i = 0; i < data.size(); ++i){
            unsigned long long overflow = 0;
            for (size_t j = 0; j < other.data.size() || overflow != 0; ++j){
                __int128 overflowAndResult = 0;
                if (j < other.data.size())
                    overflowAndResult = mulhi64(data[i], other.data[j]);
                overflow = __builtin_uaddll_overflow(result.data[i + j], overflow, &result.data[i + j]);
                overflow += __builtin_uaddll_overflow(result.data[i + j], static_cast<uint64_t>(overflowAndResult), &result.data[i + j]); //TODO fix this crasy shit
                std::cout << std::bitset<64>(static_cast<uint64_t>(overflowAndResult)) << '\n';        //DEBUG
                std::cout << std::bitset<64>(static_cast<uint64_t>(overflowAndResult >> 64)) << '\n';
                std::cout << std::bitset<128>(overflowAndResult) << '\n';
                overflow += static_cast<uint64_t>(overflowAndResult >> 64);
            }


        }
        for (size_t i = result.data.size() - 1; i > 0 && result.data[i] == 0; --i)
            result.data.pop_back();
        return result;
    }
    assert(false);                                  //TODO write karatsuba
}


bool ulint::operator<(const ulint &other) const {
    if (bitFill && !other.bitFill)
        return true;
    if (!bitFill && other.bitFill)
        return false;
    bool m = max_out_of_two(data, other.data);
    if (bitFill)
        return !m;
    return m;
}

bool ulint::operator>(const ulint &other) const {
    if (bitFill && !other.bitFill)
        return false;
    if (!bitFill && other.bitFill)
        return true;
    bool m = max_out_of_two(data, other.data);
    if (bitFill)
        return m;
    return !m;
}

