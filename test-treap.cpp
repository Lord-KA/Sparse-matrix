#include "treap.hpp"

void test_treap()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.print(std::cerr);
    t.graph_check();
    t.insert(1, 1);
    t.print(std::cerr);
    t.insert(2, 2);
    t.insert(2, 4);
    

    t.print(std::cerr);
    std::cerr << "\n";
    t.pop(2);
    t.print(std::cerr);
}
void test_treap_2()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.graph_check();
    
    t.print_graph(std::cerr);
    
    t.insert(1, 1);
    t.insert(2, 2);
    t.insert(2, 4);

    t.print_graph(std::cerr);
    std::cerr << "\n";
    t.pop(2);
    t.print_graph(std::cerr);
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
    test_treap_2();
    //test_pool();
    
    return 0;
}
