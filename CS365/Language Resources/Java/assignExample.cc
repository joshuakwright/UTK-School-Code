#include <iostream>

class Test{
public:
    int i = 10;
};

int main(){
    Test c;
    c.i = 1;

    Test d = c;
    d.i = 2;
    std::cout << "Value: " << c.i << " " << d.i << "\n";
}
