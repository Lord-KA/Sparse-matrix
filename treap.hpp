#ifndef TREAP_HPP
#define TREAP_HPP

#include <cstdint>
#include <cstddef>
#include <random>
#include <vector>
#include <iostream>

std::mt19937 rnd(179);
//==================================
// Object pool

template<typename Data>
class ObjPool{
public:
    ObjPool(size_t capacity = 1) : capacity(capacity) 
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
    
    Data *get(size_t id)
    {
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
        // Node() : next(nullptr);
        ~Node() {};
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

template<typename Key, typename Data>
class Treap
{
public:
    Treap()
    {
        root = nullptr;
    }
    ~Treap() 
    {
        delete root;
    }
    
    void insert(Key x, Data val)
    {
        Data *v = find(x);
        if (v)
        {
            *v = val;
            return;
        }
        
        auto [tl, tr] = split(root, x);
        Node *tm = new Node(x, val);
        root = merge(merge(tl, tm), tr);
    }
    
    Data pop(Key x)
    {
        auto [tl, tr] = split(root, x);
        if (!tl)
            return Data();
        Node *ptl = nullptr;
        while (tl->right)
        {
            ptl = tl;
            tl = tl->right;
        }
        if (ptl){
            ptl->right = tl->left;
        } else ptl = tl->left;
        tl->left = nullptr;
        Data result = tl->val;
        delete tl;
        root = merge(ptl, tr);
        return result;
    }
    
    Data *find(Key x)
    {
        auto [tl, tr] = split(root, x);
        if (tl == nullptr)
            return nullptr;
        Node *v = max_vert(tl);
        root = merge(tl, tr);
        if (v->x == x)
            return &v->val;
        return nullptr;
    }
    
    void print(std::ostream &out)
    {
        print(out, root);
    }
    
private:
    
    struct Node
    {
        Key x;
        size_t prior;
        Data val;
        Node *left, *right;
        
        Node(Key x, Data val) : x(x), prior(rnd()), val(val), left(nullptr), right(nullptr) {}
        
        ~Node()
        {
            /*
            if (left) delete left;
            if (right) delete right;
            */
        }
    };
    void print(std::ostream &out, Node *v)
    {
        if (!v) return;
        print(out, v->left);
        
        out << '(' << v->x << ", " << v->val << ") ";
        
        print(out, v->right);
    } 
    Node *merge(Node *tl, Node *tr)
    {
        if (!tl)
            return tr;
        if (!tr)
            return tl;
        if (tl->prior < tr->prior)
        {
            tl->right = merge(tl->right, tr);
            return tl;
        }
        else
        {
            tr->left = merge(tl, tr->left);
            return tr;
        }
    }

    std::pair<Node*, Node*> split(Node *t, Key k)
    {
        if (!t)
            return {nullptr, nullptr};
        if (t->x <= k)
        {
            auto [tl, tr] = split(t->right, k);
            t->right = tl;
            return {t, tr};
        }
        else
        {
            auto [tl, tr] = split(t->left, k);
            t->left = tr;
            return {tl, t};
        }
    }
    
    Node *max_vert(Node *v)
    {
        if (v == nullptr)
            return nullptr;
        while (v->right)
            v = v->right;
        return v;
    }

    Node *root;
    ObjPool<Node> pool;
};


#endif

void test_treap()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.insert(1, 1);
    t.insert(2, 2);
    t.insert(2, 4);

    t.print(std::cerr);
    std::cerr << "\n";
    t.pop(2);
    t.print(std::cerr);
}

void test_pool()
{
    const int n = 6;
    ObjPool<int> pool(n);
    pool.print(std::cerr);
    
    for (int i = 0; i < n + 1; ++i)
    {
        std::cerr << pool.alloc() << "\n";
        pool.print(std::cerr);
    }
    
    //pool.alloc();
    //pool.print(std::cerr);
    
    
}



int main()
{
    // test_treap();
    test_pool();
    
    return 0;
}
