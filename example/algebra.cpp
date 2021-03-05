#include <iostream>
#include <nyaruga_util/algebra.hpp>

using namespace nyaruga::util;

int main(){

    N q1 = 3u;
    N q2 = 3u;
    N q3 = 4u;
    N q4 = 6u;

    module_t m1 = {q1,q2};
    module_t m2 = {q3,q4};

   std::cout <<(m1+m2)[1] << "\n";
   std::cout << (q1*m2)[0] << "\n";
};