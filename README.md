lcpp
====

C++11 Templated Bindings for the Lua Programming Lanuage.

This is a re-implementation of some of @JakobOvrum's
[LuaCpp](https://github.com/JakobOvrum/LuaCpp) with a different take on some
things. I've thrown this up because I throw everything up but I wouldn't
recommend using it or anything like it for interfacing between Lua and C++. At
the end of the day you're much better off writing the bindings by hand.
Lua has a really well designed API, make use of it.

There is no documentation, you should probably check out the aformentioned
repository if you're actually interested in using something like this.

C++11 makes this possible but it doesn't make it usable. C++ Templates have not
been fixed in C++11. (but check out D's templates as seen in [LuaD](https://github.com/JakobOvrum/LuaD))

EXAMPLE CODES
-------------

```cpp
#include <luacpp.hpp>

namespace
{
    void add(double a, double b)
    {
        printf("%f %f!!!!!\n", a, b);
    }
}
int main()
{
    lua::state state;
    state.open_libraries();

    state.do_string("print 'Hello, World!'");

    // Stack manipulation through C++ API.
    uint8_t a = 10;
    lua::stack::push(state, 10);
    lua::stack::push(state, a);

    float b = 10.0f;
    lua::stack::push(state, 10.0f);
    lua::stack::push(state, b);

    const char *qqq = "Hello, World!";
    lua::stack::push(state, qqq);

    // thin wrapper over the luaL_ref / luaL_unref API which stores reference to
    // an object in the Lua registry table and returns you an index.
    lua::ref ref;
    lua::stack::get(state, -1, ref);

    // indeed you can still call ordinary Lua functions with the state object.
    lua_settop(state, 0);

    // we can push a ref back onto the stack.
    lua::stack::push(state, &ref);

    // raw pointers don't make a copy of the string.
    const char *string;
    size_t string_length;
    lua::stack::get(state, -1, string, string_length);

    // override for std::string does full string copy.
    std::string blah;
    lua::stack::get(state, -1, blah);

    printf("%s\n", blah.c_str());

    lua_settop(state, 0);

    // no wrapper for getting / setting globals yet either.
    lua_getglobal(state, "print");

    // a reference to a function implemented in Lua
    lua::function print;
    // can be pulled straight off the stack.
    lua::stack::get(state, -1, print);
    // and then magically called!
    print.call("Hello, World!", "Cat Dog", 9001, &print, true);

    lua_settop(state, 0);

    state.do_string("function add(a, b) print(a, b) return 10, 12.2 end");

    lua::function add;
    lua_getglobal(state, "add");
    lua::stack::get(state, -1, add);

    // we can specifiy that we'd like return values too.
    auto sum = add.call<double, double>(10, 5);
    printf("sum is %f %f\n", std::get<0>(sum), std::get<1>(sum));
    lua_settop(state, 0);

    // push a standard c++ function and have the library generate an appropriate trampoline.
    lua::stack::push(state, ::add);
    lua_setglobal(state, "add");

    state.do_string("add('hello', 1000)");

    return 0;
}
```

