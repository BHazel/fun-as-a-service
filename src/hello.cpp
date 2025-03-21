#include <print>

int main()
{
#ifndef NO_CGI
    std::println("Content-Type: text/plain\n");
#endif
    std::println("Hello, World!");
}