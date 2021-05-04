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
    
    Data *get(size_t id) const
    {
        // std::cerr << (id == -1) << ' ' << id << '\n'; //DEBUG
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
        // std::cout << "last_free = " << last_free << '\n'; //DEBUG
    }
};

//==================================
// Treap

#define TREAP_CHECK(v) { \
    if (!graph_check(v)) \
    { \
        std::cerr << "Treap error in " << __func__ << " at " << __LINE__ << "\n"; \
        std::cerr.flush(); \
        std::exit(0); \
    } }

template<typename Key, typename Data>
class Treap
{
public:
    Treap()
    {
        root_id = -1;
    }
    ~Treap() {}
    
    void insert(Key x, Data val)
    {
        Data *q = find(x);
        if (q)
        {
            *q = val;
            return;
        }
        
        auto [tl_id, tr_id] = split(root_id, x);
        size_t tm_id = pool.alloc();
        // std::cout << "tm_id in insert=" << tm_id << '\n'; //DEBUG
        Node *v = pool.get(tm_id);
        v->val = val;
        v->x = x;
        root_id = merge(merge(tl_id, tm_id), tr_id);
        TREAP_CHECK(root_id);
    }
    
    /*
    Data pop(Key x)
    {
        auto [tl_id, tr_id] = split(root_id, x);
        if (tl_id == -1)
            return Data();
        size_t ptl_id = -1;
        Node *tl = nullptr;
        while ((tl = pool.get(tl_id))->right != -1)
        {
            ptl_id = tl_id;
            tl_id = tl->right;
        }
        if (ptl_id != -1){
            pool.get(ptl_id)->right = tl->left;
        } else ptl_id = tl->left;
        tl->left = -1;
        
        Data result = tl->val;
        pool.free(tl_id);
        root_id = merge(ptl_id, tr_id);
        TREAP_CHECK(root_id);
        return result;
    }
    */
    void erase(Key x)
    {
      if (root_id != -1)
      {
        root_id = erase(root_id, x);
      }
    }
    
    size_t erase(size_t id, Key x)
    {
      if (id == -1)
        return -1;
      Node *v = pool.get(id);
      if (v->x == x)
      {
        size_t tl_id = v->left;
        size_t tr_id = v->right;
        
        pool.free(id);
        return merge(tl_id, tr_id);
      }
      
      if (v->x < x)
      {
        v->right = erase(v->right, x);
        v->update(pool);
        return id;
      }
      else
      {
        v->left = erase(v->left, x);
        v->update(pool);
        return id;
      }
        
      
        
    }
    
    Data *find(Key x)
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
    
    void print(std::ostream &out)
    {
        print(out, root_id);
        out << '\n';
    }
    
    void print_graph(std::ostream &out)
    {
        static size_t dumpn = 0;
        out << "digraph tree" << dumpn++ <<  "{\n"
               "    node [shape=record];\n";
        if (root_id != -1)
            print_graph(out, root_id);
        out << "};\n";
    }
    
    bool graph_check()
    {
        return graph_check(root_id);
    }
    bool graph_check(size_t id)
    {
        std::set<size_t> S;
        return graph_check(id, S);
    }
    
private:
    bool graph_check(size_t id, std::set<size_t> &S)
    {
        if (id == -1)
            return true;            
        if (S.find(id) != S.end())
            return false;
        S.insert(id);
        Node *v = pool.get(id);
        if (v->right != -1)
            if (!graph_check(v->right, S))
                return false;
        if (v->left != -1)
            if (!graph_check(v->left, S))
                return false;
        return true;
    }

    void print_graph(std::ostream &out, size_t id)
    {
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
    
    struct Node
    {
        Key x;
        size_t prior;
        Data val;
        size_t left, right;
        size_t size;
        
        Node() : left(-1), right(-1), size(1), prior(rnd()){}
        Node(Key x, Data val) : x(x), prior(rnd()), val(val), left(-1), right(-1), size(1) {}
        
        void update(ObjPool<Node> &p)
        {
          size = 1;
          if (left != -1)
            size += p.get(left)->size;
          if (right != -1)
            size += p.get(right)->size;
        }
        ~Node()
        {
            /*
            if (left) delete left;
            if (right) delete right;
            */
        }
    };
    void print(std::ostream &out, size_t id)
    {
        TREAP_CHECK(id);
        if (id == -1) return;
        Node *v = pool.get(id);
        print(out, v->left);
        
        out << '(' << v->x << ", " << v->val << ", " << v->size << ") ";
        
        print(out, v->right);
    } 
    size_t merge(size_t tl_id, size_t tr_id)
    {
        TREAP_CHECK(tl_id);
        TREAP_CHECK(tr_id);
        if (tl_id == -1)
            return tr_id;
        if (tr_id == -1)
            return tl_id;
        // std::cout << "in merge tl_id, tr_id = " << tl_id << ' ' << tr_id << '\n'; //DEBUG
        Node *tl = pool.get(tl_id);
        Node *tr = pool.get(tr_id);
        if (tl->prior < tr->prior)
        {
            tl->right = merge(tl->right, tr_id);
            tl->update(pool);
            TREAP_CHECK(tl_id);
            return tl_id;
        }
        else
        {
            tr->left = merge(tl_id, tr->left);
            tl->update(pool);
            TREAP_CHECK(tr_id);
            return tr_id;
        }
    }

    std::pair<size_t, size_t> split(size_t t_id, Key k)
    {
        if (t_id == -1)
            return {-1, -1};
        Node *t = pool.get(t_id);
               
        if (t->x <= k)
        {
            auto [tl_id, tr_id] = split(t->right, k);
            t->right = tl_id;
            t->update(pool);
            TREAP_CHECK(tr_id);
            TREAP_CHECK(t_id);
            return {t_id, tr_id};
        }
        else
        {
            auto [tl_id, tr_id] = split(t->left, k);
            t->left = tr_id;
            t->update(pool);
            TREAP_CHECK(tl_id);
            TREAP_CHECK(t_id);
            return {tl_id, t_id};
        }
    }
    
    size_t max_vert(size_t v_id)
    {
        if (v_id == -1)
            return -1;
        Node *v;
        while ((v = pool.get(v_id))->right)
            v_id = v->right;
        return v_id;
    }

    size_t root_id;
    ObjPool<Node> pool;
};


#endif
