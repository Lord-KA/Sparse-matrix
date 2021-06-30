#ifndef SKIPLIST_HPP
#define SKIPLIST_HPP

#include <iostream>
#include <vector>
#include <cassert>

//==================================
// Object pool

template<typename Data>
class ObjPool{
public:
    
    ObjPool(const ObjPool &other)
    {
        capacity = other.capacity;
        last_free = other.last_free;
        data = new Node[capacity];
        std::copy(other.data, other.data + other.capacity, data);
    }

    ObjPool(ObjPool &&other)
    {
        capacity = exchange(other.capacity, 0);
        last_free = exchange(other.last_free, -1);
        data = exchange(other.data, nullptr);
    }

    ObjPool& operator=(const ObjPool &other)
    {
        capacity = other.capacity;
        last_free = other.last_free;
        if (data)
            delete[] data;
        data = new Node[capacity];
        std::copy(other.data, other.data + other.capacity, data);
        return (*this);
    }

    ObjPool& operator=(ObjPool &&other)
    {
        capacity = exchange(other.capacity, 0);
        last_free = exchange(other.last_free, -1);
        if (data)
            delete[] data;
        data = exchange(other.data, nullptr);
        return (*this);
    }
    

    ObjPool(size_t capacity=1) : capacity(capacity) 
    {
        data = new Node [capacity];
        for (size_t i=0; i<capacity - 1; ++i)
            data[i].next = i + 1;
        data[capacity - 1].next = -1;
        
        last_free = 0;
    }

    ~ObjPool()
    {
        delete [] data;
    }
    
    size_t alloc()
    {
        refit();
        size_t result = last_free;
        last_free = data[last_free].next;
        return result;
    }
    
    Data *get(size_t id) const
    {
        assert(id != -1);
        assert(id < capacity);
        return &data[id].val;
    }
 
    void free(size_t id) 
    {
        data[id].next = last_free;
        last_free = id;
    }
    
    void print(std::ostream& out)
    {
        for (size_t id = last_free; id != -1; id = data[id].next)
        {
            out << "(" << id << ") -> ";
        }
        out << '\n';                
    }
    
    
private:
    struct Node{
        size_t next;
        Data val;
    };
    
    Node *data;
    size_t capacity;
    size_t last_free;
    
    void refit()
    {
        if (last_free != -1)
            return;
        Node *nbuf = new Node[capacity * 2];
        std::copy(data, data + capacity, nbuf);
        assert(data);
        delete [] data;
        data = nbuf;
        capacity *= 2;
        for (size_t i = capacity / 2; i < capacity - 1; ++i)
            data[i].next = i + 1;
        data[capacity - 1].next = -1;
        last_free = capacity / 2;
    }
};



//==================================
// Skip list

template<typename Key, typename Data>
class SList
{
private:
    struct Node
    {
        Key key;
        Data data;
        size_t prev_id;
        std::vector<size_t> next_ids;
        Node( const Node& other ) = default;
        Node( const Key& key, const Data& data, size_t prev_id, size_t level );

        Node& operator=( const Node& other ) {
            key = other.key;
            data = other.data;
            prev_id = other.prev_id;
            next_ids = other.next_ids;
        }
        size_t prevOnLevel( const Key& key);
        size_t nextOnLevel( const Key& key);
    };

    size_t start_id;
    size_t maxLevel;
    double skipDivisor;
    ObjPool<Node> pool;

public:
    struct Iterator;
    Iterator begin() const;
    Iterator end()   const;

    SList();
    SList( size_t maxLevel, double skipDivisor );
    SList( const SList& other );
    SList( SList&& other );

    SList& operator=( const SList& other);
    SList& operator=( SList&& other);

    bool operator==( const SList& other ) const;
    bool operator!=( const SList& other ) const { return !((*this) == other); }

    Data& operator[](size_t n);
    const Data& operator[](size_t n) const;

    size_t size() const;

    void   insert( Key x, Data val );
    Data*  insert( Key x );

    void   erase ( Key x );
    size_t erase ( size_t id, Key x );

    Data* find( Key x ) const;


private:

    size_t newPrev( const Key& key );
    size_t getNode( const Key& key );
};


#endif
