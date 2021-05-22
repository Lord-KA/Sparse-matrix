#include "ulint.hpp"

static inline __int128 mulhi64(uint64_t a, uint64_t b) {
     return a * (unsigned __int128)b;
}

bool max_out_of_two(const std::deque<unsigned long long> &one, const std::deque<unsigned long long> &two) {
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
    if (!neg && other.neg)
        return (*this) - -other;
    if (neg && !other.neg)
        return other - -(*this);

    ulint result;
    result.neg = neg;
    size_t end;
    if (data.size() > other.data.size()){
        result = (*this);
        end = other.data.size();
    }
    else {
        result = other;
        end = data.size();
    }

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
        result.data.push_front(1);
    else
        result.data[end] += 1;

    return result;
}

ulint ulint::operator-(const ulint &other) const {
    if (neg && !other.neg)
        return -(-(*this) + other);
    if (!neg && other.neg)
        return -(*this + -other);

    ulint result;
    size_t end;
    const ulint* subtrahend;
    if ((*this) > other){
        result = (*this);
        result.neg = false;
        end = other.data.size();
        subtrahend = &other;
    }
    else {
        result = other;
        result.neg = true;
        end = data.size();
        subtrahend = this;
    }
    bool overflow = false;
    for (size_t i = 0; i < end; ++i) {
        if (__builtin_usubll_overflow(result.data[i], subtrahend->data[i], &result.data[i]) && overflow){
            result.data[i] -= 1;
        }
        else if (overflow)
            overflow = __builtin_usubll_overflow(result.data[i], 1, &result.data[i]);
    }
    while (end < result.data.size() && result.data[end] == 0) {
        result.data[end] = ULLONG_MAX;
        ++end;
    }
    if (end == result.data.size())
        return result;

    if (result.data[end] == 1)
        result.data.pop_back();
    else
        result.data[end] -= 1;

    return result;
}

ulint ulint::operator*(const ulint &other) const{
    if (other.data.size() < 10 || data.size() < 10){
        


    }
}


bool ulint::operator<(const ulint &other) const {
    if (neg && !other.neg)
        return true;
    if (!neg && other.neg)
        return false;
    bool m = max_out_of_two(data, other.data);
    if (neg)
        return !m;
    return m;
}

bool ulint::operator>(const ulint &other) const {
    if (neg && !other.neg)
        return false;
    if (!neg && other.neg)
        return true;
    bool m = max_out_of_two(data, other.data);
    if (neg)
        return m;
    return !m;
}

