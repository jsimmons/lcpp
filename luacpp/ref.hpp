#ifndef LCPP_REF_HPP
#define LCPP_REF_HPP

#include "luacpp/lua.hpp"
#include "luacpp/types.hpp"
#include "luacpp/stack.hpp"

namespace lua
{
    class ref
    {
        template <typename T>
        friend void stack::push(lua_State *, T *);
        template <typename T>
        friend void stack::get(lua_State *, int, T &);

        protected:
            lua_State *L;
            int name;

        public:
            inline ref() : L(nullptr), name(LUA_NOREF) {}
            inline ref(const ref &&rhs) : L(rhs.L), name(rhs.name) {}
            ref(const ref &rhs) = delete;
            ref &operator=(const ref &rhs) = delete;

            inline ~ref()
            {
                luaL_unref(this->L, LUA_REGISTRYINDEX, this->name);
            }
    };

    namespace stack
    {
        template<>
        inline void push(lua_State *L, ref *value)
        {
            lua_rawgeti(L, LUA_REGISTRYINDEX, value->name);
        }

        template <>
        inline void get(lua_State *L, int index, ref &out_value)
        {
            lua_pushvalue(L, index);
            out_value.L = L;
            out_value.name = luaL_ref(L, LUA_REGISTRYINDEX);
        }
    }
}

#endif
