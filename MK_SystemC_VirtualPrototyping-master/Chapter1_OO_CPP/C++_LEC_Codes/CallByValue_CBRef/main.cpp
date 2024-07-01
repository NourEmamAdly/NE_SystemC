#include<iostream>
void callByValue(int x) {
    x += 10;
}
void callByReference(int *x) {
    *x += 10;
}
void callByReference2(int &x) {
    x += 10;
}
int main() {
    int a=10;
    std::cout << "a = " << a << std::endl;      // 10
    callByValue(a);
    std::cout << "a = " << a << std::endl;  // 10
    callByReference(&a);
    std::cout << "a = " << a << std::endl;  // 20
    callByReference2(a);
    std::cout << "a = " << a << std::endl;  // 30
    return 0;
}