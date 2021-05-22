#include "treap.hpp"
#include <cassert>

void test_treap()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.insert(1, 1);
    // t.insert(2, 2);
    t.insert(4, 4);

    t.insert(5, 5);

    t.insert(6, 6);
    t.insert(7, 7);

    t.insert(8, 8);
    t.insert(9, 9);
    t.insert(19, 19);
    t.print(std::cerr);
    t.print_graph(std::cerr);

    
    for(auto elem : t)
        std::cout << elem.second << ' ';
    std::cout << '\n';

    t.erase(7);
    for(auto elem : t){
        ++elem.second;
        std::cout << elem.second << ' ';
    }
    std::cout << '\n';

    for(auto elem : t)
        std::cout << elem.second << ' ';
    std::cout << '\n';

    auto iter = t.begin();
    ++iter;
    ++iter;
    ++iter;
    std::cout << (*iter).second<< ' ' << (*(--iter)).second << ' ' << (*(--iter)).second << ' ' << (*(--iter)).second << '\n';
    
    std::cout << '\n';

    Treap<int, int> t2(t);

    for(auto elem : t2)
        std::cout << elem.second << ' ';
    std::cout << '\n';
    std::cout << '\n';


    Treap<int, int> t3;
    t3 = std::move(t2);
    Treap<int, int> t4(t3);
    std::cout << '\n';
    t.erase(8);
    for(auto elem : t)
        std::cout << elem.second << ' ';
    std::cout << '\n';
    for(auto elem : t3)
        std::cout << elem.second << ' ';
    std::cout << '\n';
    std::cout << (bool)(t == t3) << ' ' << (bool)(t3 == t4) << '\n';
    assert(((int*)&t3)[0] != 0xDEAD);   
}
void test_treap_2()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.graph_check();
    
    t.print_graph(std::cerr);
    t.insert(1, 1);
    t.print_graph(std::cerr);
    t.insert(2, 2);
    t.print_graph(std::cerr);
    t.insert(2, 4);
    t.print_graph(std::cerr);

    std::cerr << "\n";
    t.erase(3);
    t.erase(3);
    t.erase(3);
    t.erase(3);
    t.print_graph(std::cerr);
}

void test_pool()
{
    const int n = 6;
    ObjPool<int> pool;
    pool.print(std::cerr);
    
    for (int i = 0; i < n + 1; ++i)
    {
        std::cerr << pool.alloc() << "\n";
        pool.print(std::cerr);
    }
    pool.free(0);
    pool.print(std::cerr);
    pool.free(2);
    
    pool.print(std::cerr);
    
    pool.alloc();
    pool.print(std::cerr);

    pool.alloc();
    pool.print(std::cerr);


    pool.print(std::cerr);

    std::cerr << "\n\n";
    ObjPool<int> othr(pool);
    othr.print(std::cerr);
    pool.alloc();
    pool.alloc();
    ObjPool<int> pool3;
    pool3 = pool;
    othr = std::move(pool);
    othr.print(std::cerr);
}

void test_treap_3()
{
    Treap<int, int> t;
    
    t.insert(3, 3);
    t.insert(1, 1);
    // t.insert(2, 2);
    t.insert(4, 4);

    t.insert(5, 5);

    t.insert(6, 6);
    t.insert(7, 7);

    t.insert(8, 8);
    t.insert(9, 9);
    t.insert(19, 19);
    t.print(std::cerr);
    t.print_graph(std::cerr);

    for(auto elem : t)
        std::cout << "(" << elem.first << ", " <<  elem.second << ");  ";
    std::cout << '\n';
    Treap<int, int> t2(t);
    t2.erase(3);
    t2.erase(4);
    
    Treap<int, int> t3(t2);
    for(auto elem : t2)
        std::cout << "(" << elem.first << ", " <<  elem.second << ");  ";
    std::cout << '\n';

    for(auto elem : t3)
        std::cout << "(" << elem.first << ", " <<  elem.second << ");  ";

    std::cout << '\n';
    assert( t2 != t ); 
    assert( t2 == t2 ); 
    assert( t2 == t3 ); 
    std::cout << "All good!\n"; 
}


int main()
{
    test_treap();
    std::cerr << "\n\n\n";

    test_treap_2();
    std::cerr << "\n\n\n";
    test_pool();
    
    std::cerr << "\n\n\n";
    test_treap_3();
    return 0;
}

