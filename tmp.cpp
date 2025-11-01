#include "libftpp/memory.hpp"
#include "libftpp/movable.hpp"
#include "libftpp/utility.hpp"
#include <climits>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Base {
};

class Derived : public Base {

};

void take(ft::nullptr_t nptr)
{}

void take(ft::shared_ptr<Base>) 
{}

int main()
{
    ft::shared_ptr<Derived> dptr;
    take(ft::move(dptr));

    ft::nullptr_t nptr;
    take(ft::move(nptr));
    ft::unique_ptr<std::string> uptr1(ft::move(nptr));

    ft::unique_ptr<std::string> uptr(FT_NULLPTR);
    std::unique_ptr<std::string> std_uptr(nullptr);
    ft::shared_ptr<std::string> sptr(FT_NULLPTR);
    std::shared_ptr<std::string> std_sptr(nullptr);


    std::unique_ptr<Base> std_bptr = std::make_unique<Derived>();
    ft::unique_ptr<Base> bptr1(ft::make_unique<Derived>());
    ft::unique_ptr<Base> bptr2 = ft::make_unique<Derived>();


    std::unique_ptr<Base> std_bptr1(new Derived());
    std::unique_ptr<Base> std_bptr2 = new Derived();

    std_bptr1.reset();
}
