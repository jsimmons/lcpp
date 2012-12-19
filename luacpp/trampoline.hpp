#ifndef LCPP_TRAMPOLINE_HPP
#define LCPP_TRAMPOLINE_HPP

#include <tuple>

#include <luacpp/lua.hpp>

namespace lua
{
    namespace trampoline
    {
        template<typename R, typename...A, size_t... S>
        int trampoline(lua_State *L)
        {
            typedef R (*actual_fn)(A...);
            actual_fn func = (actual_fn)(lua_upvalueindex(1));
            std::tuple<A...> tuple;
            stack::make_argument_tuple<sizeof...(A)>(L, tuple);
        }
    }

    namespace stack
    {
        template <typename R, typename...A>
        inline void push(lua_State *L, R (*func)(A...))
        {
            lua_pushlightuserdata(L, reinterpret_cast<void *>(func));
            lua_pushcclosure(L, trampoline::trampoline<R, A...>, 1);
        }
    }
}

#endif
