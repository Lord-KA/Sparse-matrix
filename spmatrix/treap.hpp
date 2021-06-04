#ifndef TREAP_HPP
#define TREAP_HPP

#include <cstdint>
#include <cstddef>
#include <random>
#include <vector>
#include <iostream>
#include <cassert>
#include <set>

std::mt19937 rnd(179);

template<class T, class U = T>
T exchange(T& obj, U&& new_value)
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

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
        // ~Node() {};
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
// Treap

#ifndef NDEBUG
#define TREAP_CHECK(v) { \
    if (!graph_check(v)) \
    { \
        std::cerr << "Treap error in " << __func__ << " at " << __LINE__ << "\n"; \
        std::cerr.flush(); \
        std::exit(0); \
    } }
#else
#define TREAP_CHECK(v) {}
#endif

template<typename Key, typename Data>
class Treap 
{
private:
     struct Node
    {
        Key x;
        size_t prior;
        Data val = Data();
        size_t parent;
        size_t left, right;
        size_t size;
        
        Node()                : prior(rnd()), parent(-1), left(-1), right(-1), size(1) {}
        Node(Key x, Data val) : x(x), prior(rnd()), val(val), parent(-1), left(-1), right(-1), size(1) {}
        
        ~Node() {};
    };

    size_t root_id;
    ObjPool<Node> pool;

public:
    struct Iterator 
    {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Node;
        
        Iterator( size_t id=-1, const Treap* this_=nullptr ) : id(id), this_(this_), pos(0) {}     // WARNING: don't use for non-begin iterator, pos will invalidate
        Iterator( const Iterator &other ) = default;
        
        bool operator==( const Iterator other ) const { return id == other.id; }
        bool operator!=( const Iterator other ) const { return id != other.id; }

        bool operator< ( const Iterator other ) const { return pos <  other.pos; }
        bool operator> ( const Iterator other ) const { return pos >  other.pos; }
        bool operator<=( const Iterator other ) const { return pos <= other.pos; }
        bool operator>=( const Iterator other ) const { return pos >= other.pos; }

        void setPos(size_t pos_) { pos = pos_; }
        void setId (size_t id_ ) { id  = id_;  }

        std::pair<Key, Data&> operator*() { Node* v = this_->pool.get(id); \
                                            return {v->x, v->val}; }

        const std::pair<const Key, const Data&> operator*() const { Node* v = this_->pool.get(id); \
                                            return std::make_pair(v->x, v->val); } 

 
        Iterator operator++() 
        {
            assert(id != -1);
            ++pos;
            Node* v = this_->pool.get(id);
            if (v->right != -1)
            {
                id = v->right;   
                while ((v = this_->pool.get(id))->left != -1)
                    id = v->left;
                return (*this);
            }
            while ((v = this_->pool.get(id))->parent != -1)
            {
                if (this_->pool.get(v->parent)->left == id)
                {
                    id = v->parent;
                    return (*this);
                }
                id = v->parent;
            }
            id = -1;
            return (*this);
        }

        Iterator operator++(int) 
        {
            assert(id != -1);
            ++pos;
            Node* v = this_->pool.get(id);
            Iterator result(*this);
            if (v->right != -1)
            {
                id = v->right;
                while ((v = this_->pool.get(id))->left != -1)
                    id = v->left;
                return result;
            }
            while ((v = this_->pool.get(id))->parent != -1)
            {
                if (this_->pool.get(v->parent)->left == id)
                {
                    id = v->parent;
                    return result;
                }
                id = v->parent;
            }
            id = -1;
            return result;
        }

        Iterator operator--()
        {
            assert(id != -1);
            --pos;
            Node* v = this_->pool.get(id);
            if (v->left != -1)
            {
                id = v->left;
                while ((v = this_->pool.get(id))->right != -1)
                    id = v->right;
                return (*this);
            }
            while ((v = this_->pool.get(id))->parent != -1)
            {
                if (this_->pool.get(v->parent)->right == id)
                {
                    id = v->parent;
                    return (*this);
                }
                id = v->parent;
            }
            id = -1;
            return (*this);
        }

        Iterator operator--(int)
        {
            assert(id != -1);
            --pos;
            Node* v = this_->pool.get(id);
            Iterator result(*this);
            if (v->left != -1)
            {
                id = v->left;
                while ((v = this_->pool.get(id))->right != -1)
                    id = v->right;
                return result;
            }
            while ((v = this_->pool.get(id))->parent != -1)
            {
                if (this_->pool.get(v->parent)->right == id)
                {
                    id = v->parent;
                    return result;
                }
                id = v->parent;
            }
            id = -1;
            return result;
        }
        
        Iterator operator+(long long n) const {
            return this_->kth_elem(pos + n);
        }

        Iterator operator-(long long n) const {     
            return this_->kth_elem(pos - n);
        }

        Iterator& operator+=(long long n) {
            *this = *this + n;
            return *this;
        }

        Iterator& operator-=(long long n) {
            *this = *this - n;
            return *this;
        }


        long long operator-(const Iterator &other) const {
            return pos - other.pos;
        }

        private:
            size_t pos;
            size_t id;
            const Treap* this_;
    };

    Iterator kth_elem(size_t k) const {
        Iterator result(0, this);
        result.setPos(k);
        size_t id = root_id;
        assert(id != -1);
        Node *v;
        while (id != -1){
            v = pool.get(id);
            size_t i = getSize(v->left);
            if (i == k){
                result.setId(id);
                return result;
            }
            if (k < i)
                id = v->left;
            else {
                k -= i + 1;
                id = v->right;
            }   
        }
        return end();
    }



    Iterator begin() const { return Iterator(min_vert(root_id), this); } 
    Iterator end()   const { return Iterator(-1, this); }
    
   

    //======================================
    // TREAP interface functions

    Treap() : root_id(-1) {}
    Treap(const Treap &other) : root_id(other.root_id), pool(other.pool) {}
    Treap(Treap &&other);
    ~Treap() = default;


    Treap& operator=( const Treap &other );
    Treap& operator=( Treap &&other );

    bool  operator==( const Treap &other ) const;
    bool  operator!=( const Treap &other ) const { return !((*this) == other); }

    Data& operator[](size_t n) { return (*(begin() + n)).second; }
    const Data& operator[](size_t n) const { return *(begin() + n); }

    size_t size() const { if (root_id == -1) return 0; return pool.get(root_id)->size; }

    void   insert( Key x, Data val );
    Data*  insert( Key x );
    
    void   erase ( Key x ) { if (root_id != -1)  root_id = erase(root_id, x); }
    size_t erase ( size_t id, Key x );
        
    Data* find( Key x ) const;
        
    #ifndef NDEBUG
    void print      ( std::ostream &out ) const { print(out, root_id); out << '\n'; }
    void print_graph( std::ostream &out ) const
    {
        static size_t dumpn = 0;
        out << "digraph tree" << dumpn++ <<  "{\n"
               "    node [shape=record];\n";
        if (root_id != -1)
            print_graph(out, root_id);
        out << "};\n";
    }
    
    bool graph_check() const
    {
        if (root_id != -1 && pool.get(root_id)->parent != -1)
           return false;
        return graph_check(root_id);
    }

    bool graph_check(size_t id) const
    {
        std::set<size_t> S;
        return graph_check(id, S);
    }
    #endif  
    
private:

    bool graph_check( size_t id, std::set<size_t> &S ) const;
    void print_graph( std::ostream &out, size_t id ) const;
    void print( std::ostream &out, size_t id ) const;

    size_t                    merge( size_t tl_id, size_t tr_id );
    std::pair<size_t, size_t> split( size_t t_id, Key k );
   
    void update( size_t id );
    void insert( Node &node);                       //TODO write it to emplement faster 0 nodes removal

    size_t getSize( size_t v_id ) const { if (v_id == -1) return 0; return pool.get(v_id)->size; }

    size_t min_vert( size_t v_id ) const;
    size_t max_vert( size_t v_id ) const;
};


template<typename Key, typename Data>
Treap<Key, Data>::Treap(Treap &&other)
{
    root_id = exchange(other.root_id, -1);
    pool = std::move(other.pool);
}


template<typename Key, typename Data>
Treap<Key, Data>& Treap<Key, Data>::operator=(const Treap<Key, Data> &other)
{
    root_id = other.root_id;
    pool = other.pool;
    return (*this);
}

template<typename Key, typename Data>
Treap<Key, Data>& Treap<Key, Data>::operator=(Treap<Key, Data> &&other)
{
    root_id = exchange(other.root_id, -1);
    pool = std::move(other.pool);
    return (*this);
}


template<typename Key, typename Data>
bool Treap<Key, Data>::operator==(const Treap<Key, Data> &other) const {
    if (root_id != other.root_id)
        return false;

    auto iter_this = this->begin(), iter_other = other.begin();
    while (iter_this != this->end() && iter_other != other.end()){
        if (*iter_this != *iter_other)
            return false;
        ++iter_this;
        ++iter_other;
    }
    if (iter_this != this->end() || iter_other != other.end())
        return false;
    return true;
}


template<typename Key, typename Data>
void Treap<Key, Data>::insert(Key x, Data val)
{
    Data* q = find(x);
    if (q)
    {
        *q = val;
        return;
    }
        
    auto [tl_id, tr_id] = split(root_id, x);
    size_t tm_id = pool.alloc();
    assert(tm_id != -1);
    Node *v = pool.get(tm_id);
    v->val = val;
    v->x = x;
    root_id = merge(merge(tl_id, tm_id), tr_id);
    TREAP_CHECK(root_id);
}

template<typename Key, typename Data>
Data* Treap<Key, Data>::insert(Key x)
{
    Data* q = find(x);
    if (q)
        return q;
        
    auto [tl_id, tr_id] = split(root_id, x);
    size_t tm_id = pool.alloc();
    assert(tm_id != -1);
    Node *v = pool.get(tm_id);
    v->x = x;
    root_id = merge(merge(tl_id, tm_id), tr_id);
    TREAP_CHECK(root_id);
    return &(v->val);
}


template<typename Key, typename Data>
size_t Treap<Key, Data>::erase(size_t id, Key x) //TODO find bug
{
    if (id == -1)
        return -1;
    Node *v = pool.get(id);
    if (v->x == x)
    {
        size_t tl_id = v->left;
        size_t tr_id = v->right;
      
        v->parent = -1;
        if (tr_id != -1)
            pool.get(tr_id)->parent = -1;
        if (tl_id != -1)
            pool.get(tl_id)->parent = -1;

        pool.free(id);
        return merge(tl_id, tr_id);
    }

    if (v->x < x)
        v->right = erase(v->right, x);
    else
        v->left = erase(v->left, x);

    update(id);
    TREAP_CHECK(root_id);
    return id;
}

template<typename Key, typename Data>
Data* Treap<Key, Data>::find(Key x) const
{
    size_t cur_id = root_id;
    Node *v;
    while (cur_id != -1 && ((v = pool.get(cur_id))->x != x))
    {
        if (v->x > x)
            cur_id = v->left;
        else
            cur_id = v->right;
    }
    if (cur_id != -1)
        return &v->val;
    return nullptr;
}

template<typename Key, typename Data>
bool Treap<Key, Data>::graph_check(size_t id, std::set<size_t> &S) const
{
    if (id == -1)
        return true;            
    if (S.find(id) != S.end())
        return false;
    S.insert(id);
    Node *v = pool.get(id);
    if (v->right != -1)
        if (!graph_check(v->right, S) || pool.get(v->right)->parent != id)
            return false;
    if (v->left != -1)
        if (!graph_check(v->left, S) || pool.get(v->left)->parent != id)
            return false;
    return true;
}

template<typename Key, typename Data>
void Treap<Key, Data>::print_graph(std::ostream &out, size_t id) const
{
    assert(id != -1);
    Node *v = pool.get(id);
    out << "struct" << id << " [label=\"" << id << " | { key = " << v->x << " | data = " << v->val
    << " }\"];\n";
    if (v->left != -1)
    {
        out << "struct" << id << " -> " << "struct" << v->left << ";\n";
        print_graph(out, v->left);
    }
    if (v->right != -1)
    {
        out << "struct" << id << " -> " << "struct" << v->right << ";\n";
        print_graph(out, v->right);
    }
}


template<typename Key, typename Data>
size_t Treap<Key, Data>::merge(size_t tl_id, size_t tr_id)
{
    TREAP_CHECK(tl_id);
    TREAP_CHECK(tr_id);
    if (tl_id == -1)
        return tr_id;
    if (tr_id == -1)
        return tl_id;
    Node *tl = pool.get(tl_id);
    Node *tr = pool.get(tr_id);
    if (tl->prior < tr->prior)
    {
        tl->right = merge(tl->right, tr_id);
        update(tl_id);
        TREAP_CHECK(tl_id);
        return tl_id;
    }
    else
    {
        tr->left = merge(tl_id, tr->left);
        update(tr_id);
        TREAP_CHECK(tr_id);
        return tr_id;
    }
}

template<typename Key, typename Data>
std::pair<size_t, size_t> Treap<Key, Data>::split(size_t t_id, Key k)
{
    if (t_id == -1)
        return {-1, -1};
    Node *t = pool.get(t_id);
               
    if (t->x <= k)
    {
        auto [tl_id, tr_id] = split(t->right, k);
        t->right = tl_id;
        update(t_id);
        if (tr_id != -1)
            pool.get(tr_id)->parent = -1;
        t->parent = -1;
        TREAP_CHECK(tr_id);
        TREAP_CHECK(t_id);
        return {t_id, tr_id};
    }
    else
    {
        auto [tl_id, tr_id] = split(t->left, k);
        t->left = tr_id;
        update(t_id);
        if (tl_id != -1)
            pool.get(tl_id)->parent = -1;
        t->parent = -1;
        TREAP_CHECK(tl_id);
        TREAP_CHECK(t_id);
        return {tl_id, t_id};
    }
}
 
template<typename Key, typename Data>
void Treap<Key, Data>::update(size_t id)
{
    assert(id != -1);

    Node* v = pool.get(id);
    v->size = 1;
    if (v->left != -1)
    {
        Node* tl = pool.get(v->left);
        v->size += tl->size;
        tl->parent = id;
    }
    if (v->right != -1)
    {
        Node* tr = pool.get(v->right);
        v->size += tr->size;
        tr->parent = id;
    }
}

template<typename Key, typename Data>
size_t Treap<Key, Data>::min_vert(size_t v_id) const
{
    if (v_id == -1)
        return -1;
    Node *v;
    while ((v = pool.get(v_id))->left != -1) 
        v_id = v->left;
    return v_id;
}

template<typename Key, typename Data>
void Treap<Key, Data>::print(std::ostream &out, size_t id) const
{
    TREAP_CHECK(id);
    if (id == -1) return;
    Node *v = pool.get(id);
    print(out, v->left);
        
    out << '(' << v->x << ", " << v->val << ", " << v->size << ") ";
        
    print(out, v->right);
} 

template<typename Key, typename Data>
size_t Treap<Key, Data>::max_vert(size_t v_id) const
{
    if (v_id == -1)
        return -1;
    Node *v;
    while ((v = pool.get(v_id))->right != -1)
        v_id = v->right;
    return v_id;
}

#endif
