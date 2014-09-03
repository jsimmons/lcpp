#include <luacpp.hpp>

namespace
{
    void void_add(const char* a, double b)
    {
        printf("void_add(%s, %lf)!!!!!\n", a, b);
    }

    int int_add(const char* a, double b)
    {
        printf("int_add(%s, %lf)!!!!!\n", a, b);
        return 0;
    }
}
int main()
{
    lua::state state;
    state.open_libraries();

    state.do_string("print 'Hello, World!'");

    uint8_t a = 10;
    lua::stack::push(state, 10);
    lua::stack::push(state, a);

    float b = 10.0f;
    lua::stack::push(state, 10.0f);
    lua::stack::push(state, b);

    const char *qqq = "Hello, World!";
    lua::stack::push(state, qqq);
    lua::ref ref;
    lua::stack::get(state, -1, ref);

    lua_settop(state, 0);

    lua::stack::push(state, &ref);

    // no copy.
    const char *string;
    size_t string_length;
    lua::stack::get(state, -1, string, string_length);

    // copy.
    std::string blah;
    lua::stack::get(state, -1, blah);

    printf("%s\n", blah.c_str());

    lua_settop(state, 0);

    lua::function print;
    lua_getglobal(state, "print");
    lua::stack::get(state, -1, print);
    print.call("Hello, World!", "Cat Dog", 9001, &print, true);
    lua_settop(state, 0);

    state.do_string("function add(a, b) print(a, b) return 10, 12.2 end");
    lua::function add;
    lua_getglobal(state, "add");
    lua::stack::get(state, -1, add);
    auto sum = add.call<double, double>(10, 5);
    printf("sum is %f %f\n", std::get<0>(sum), std::get<1>(sum));
    lua_settop(state, 0);

    // test with void return
    lua::stack::push(state, ::void_add);
    lua_setglobal(state, "add");

    state.do_string("add('hello', 1000)");

    // test with int return
    lua::stack::push(state, ::int_add);
    lua_setglobal(state, "add");

    state.do_string("add('hello', 1000)");

    return 0;
}
