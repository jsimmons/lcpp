#ifndef LCPP_STACK_HPP
#define LCPP_STACK_HPP

#include <tuple>

#include "luacpp/lua.hpp"

namespace lua
{
    namespace stack
    {
        template <typename T>
        inline void push(lua_State *, const T);
template <typename T> inline void push(lua_State *, T*);

        template <typename T>
        inline void push(lua_State *, const T*, size_t);

        template <>
        inline void push(lua_State *L, const signed char value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const unsigned char value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const short value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const unsigned short value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const int value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const unsigned int value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const long value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const unsigned long value)
        {
            lua_pushinteger(L, value);
        }

        template <>
        inline void push(lua_State *L, const double value)
        {
            lua_pushnumber(L, value);
        }

        template <>
        inline void push(lua_State *L, const float value)
        {
            lua_pushnumber(L, value);
        }

        template <>
        inline void push(lua_State *L, const bool value)
        {
            lua_pushboolean(L, value);
        }

        template <>
        inline void push(lua_State *L, const char *value)
        {
            lua_pushstring(L, value);
        }

        template <>
        inline void push(lua_State *L, const char *value, size_t length)
        {
            lua_pushlstring(L, value, length);
        }

        template <>
        inline void push<>(lua_State *L, const std::string &value)
        {
            lua_pushlstring(L, value.c_str(), value.length());
        }

        template <typename T>
        inline void get(lua_State *, int, T&);

        template <typename T>
        inline void get(lua_State *, int, T*&, size_t &);

        template <>
        inline void get(lua_State *L, int index, signed char &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, unsigned char &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, short &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, unsigned short &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, int &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, unsigned int &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, long &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, unsigned long &out_value)
        {
            out_value = lua_tointeger(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, float &out_value)
        {
            out_value = lua_tonumber(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, double &out_value)
        {
            out_value = lua_tonumber(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, bool &out_value)
        {
            out_value = lua_toboolean(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, std::string &out_value)
        {
            size_t length;
            const char *string = lua_tolstring(L, index, &length);
            out_value = std::string(string, length);
        }

        template <>
        inline void get(lua_State *L, int index, const char *&out_value)
        {
            out_value = lua_tostring(L, index);
        }

        template <>
        inline void get(lua_State *L, int index, const char *&out_value, size_t &out_len)
        {
            out_value = lua_tolstring(L, index, &out_len);
        }

        template <size_t N, typename T>
        inline void check_argument(lua_State *L, int n, T &out)
        {
            luaL_checktype(L, n, static_cast<int>(lua_typeof<T>()));
            get(L, n, out);
        }

        template <int N, typename... T>
        inline typename std::enable_if<N == 0>::type make_argument_tuple(lua_State *, std::tuple<T...> &)
        {
        }

        template <int N, typename... T>
        inline typename std::enable_if<N >= 1>::type make_argument_tuple(lua_State *L, std::tuple<T...> &tuple)
        {
            check_argument<N>(L, N, std::get<N - 1>(tuple));
            make_argument_tuple<N - 1, T...>(L, tuple);
        }

        template <size_t N, typename T>
        inline void check_result(lua_State *L, int n, T &out)
        {
            int expected = static_cast<int>(lua_typeof<T>());
            int actual = lua_type(L, n);
            if(expected != actual)
            {
                luaL_error(L, "bad result #%d from function (expected %s, got %s)",
                        n - 1,
                        lua_typename(L, expected),
                        lua_typename(L, actual));
            }

            get(L, n, out);
        }

        template <int N, typename... T>
        inline typename std::enable_if<N == 0>::type make_result_tuple(lua_State *, std::tuple<T...> &)
        {
        }

        template <int N, typename... T>
        inline typename std::enable_if<N >= 1>::type make_result_tuple(lua_State *L, std::tuple<T...> &tuple)
        {
            check_result<N>(L, sizeof...(T) - N + 2, std::get<N - 1>(tuple));
            make_result_tuple<N - 1, T...>(L, tuple);
        }

    }
}

#endif
