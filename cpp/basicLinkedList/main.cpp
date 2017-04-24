
#include "LinkedList.h"

class Item {
public:
    int num;
public:
    // default constructor
    Item() : num(0) {};

    // constructor
    Item(int n) : num(n) {};

    friend std::ostream& operator<<(std::ostream &out, const Item &item) { out << item.num; }
};

int main() {
    dataStructures::list::LinkedList<int> xs; // default constructor

    std::cout << xs << std::endl;

    for(auto i=0; i<10; i++)
        xs.insert(i, 10*i);


    dataStructures::list::LinkedList<int> xs1{xs}; // copy constructor

    xs.prepend(-1);
    xs.append(1000);
    xs.set(2, 11);

    xs1.append(100);


    dataStructures::list::LinkedList<int> xs2; // default constructor

    xs2 = xs1; // copy assignment
    xs2.append(200);
    xs2.set(1, 12);

    std::cout << xs << std::endl;
    std::cout << xs1 << std::endl;
    std::cout << xs2 << std::endl;


    for(auto i=0; i<12; i++) {
        auto e = xs.get(i);
        std::cout << "Elem at " << i << " is " << e << std::endl;
    }

    auto x = xs[5];
    xs[6] = 600;

    // iterator
    for(auto it = xs.begin(); it != xs.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // for each
    for(auto x : xs) {
        std::cout << x << " ";
    }
    std::cout << std::endl;


    std::cout << xs << std::endl;


    dataStructures::list::LinkedList<Item> ys;


    for(int i=0; i<2; i++) {
        //Item *item = new Item(i);
        Item item{i};
        ys.insert(i, item);
        // item.num = 1000;
    }

    std::cout << std::endl;
    std::cout << ys << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    // Initializer list
    dataStructures::list::LinkedList<int> zs{1,2,3,5};
    std::cout << zs << std::endl;
    std::cout << std::endl;

    dataStructures::list::LinkedList<int> zs1;
    zs1 = {1,2,3,5,6}; // Initializer list and move assignment
    std::cout << zs1 << std::endl;
    std::cout << std::endl;

    return 0;
}