#ifndef LCPP_TRAMPOLINE_HPP
#define LCPP_TRAMPOLINE_HPP

#include <tuple>

#include <luacpp/lua.hpp>

namespace lua
{
    namespace
    {
        template<typename R, typename... FA, typename T, typename... A>
        inline R apply_tuple(R (*fn)(FA...), T, std::true_type, A&&... args)
        {
            return fn(args...);
        }

        template<typename R, typename... FA, typename T, typename... A>
        inline R apply_tuple(R (*fn)(FA...), T t, std::false_type, A&&... args)
        {
            return apply_tuple(fn, t, std::integral_constant<bool, std::tuple_size<T>::value == sizeof...(A) + 1>(), std::forward<A>(args)..., std::get<sizeof...(A)>(t));
        }

        template<typename R, typename...A>
        struct trampoline
        {
            static int call(lua_State *L)
            {
                typedef R (*actual_fn)(A...);
                actual_fn func = (actual_fn)(lua_touserdata(L, lua_upvalueindex(1)));
                std::tuple<A...> tuple;
                stack::make_argument_tuple<sizeof...(A)>(L, tuple);
                R r = apply_tuple(func, tuple, std::integral_constant<bool, sizeof...(A) == 0>());
                stack::push(L, r);
                return 1;
            }
        };

        template<typename R, typename...A>
        struct trampoline<void, A...>
        {
            static int call(lua_State *L)
            {
                typedef R (*actual_fn)(A...);
                actual_fn func = (actual_fn)(lua_touserdata(L, lua_upvalueindex(1)));
                std::tuple<A...> tuple;
                stack::make_argument_tuple<sizeof...(A)>(L, tuple);
                apply_tuple(func, tuple, std::integral_constant<bool, sizeof...(A) == 0>());
                return 0;
            }
        };
    }

    namespace stack
    {
        template <typename R, typename...A>
        inline void push(lua_State *L, R (*func)(A...))
        {
            lua_pushlightuserdata(L, reinterpret_cast<void *>(func));
            lua_pushcclosure(L, trampoline<R, A...>::call, 1);
        }
    }
}

#endif
