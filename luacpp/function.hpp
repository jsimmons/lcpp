#ifndef LCPP_FUNCTION_HPP
#define LCPP_FUNCTION_HPP

#include <tuple>

#include "luacpp/lua.hpp"
#include "luacpp/types.hpp"
#include "luacpp/stack.hpp"
#include "luacpp/ref.hpp"

namespace lua
{
    class function : public ref
    {
        template<typename T>
        friend void stack::push(lua_State *, T *);
        template<typename T>
        friend void stack::get(lua_State *, int, T &);

        private:
        inline void push_args() {}

        template <typename T, typename... A>
        inline void push_args(T &&arg, A... args)
        {
            stack::push(this->L, arg);
            push_args(args...);
        }

        public:
        template <typename... R, typename... A>
        inline std::tuple <R...> call(A&&... args)
        {
            stack::push(this->L, this);
            push_args(args...);
            lua_call(this->L, sizeof...(A), sizeof...(R));
            std::tuple<R...> tuple;
            stack::make_result_tuple<sizeof...(R)>(this->L, tuple);
            return tuple;
        }
    };

    namespace stack
    {
        template <>
        inline void push(lua_State *L, function *value)
        {
            stack::push(L, static_cast<ref *>(value));
        }

        template <>
        inline void get(lua_State *L, int index, function &out_value)
        {
            stack::get(L, index, static_cast<ref&>(out_value));
        }
    }
}

#endif

