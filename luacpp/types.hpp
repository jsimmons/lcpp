#ifndef LCPP_TYPES_HPP
#define LCPP_TYPES_HPP

namespace lua
{
    enum class type
    {
        NONE = LUA_TNONE,
        STRING = LUA_TSTRING,
        NUMBER = LUA_TNUMBER,
        THREAD = LUA_TTHREAD,
        BOOLEAN = LUA_TBOOLEAN,
        FUNCTION = LUA_TFUNCTION,
        USERDATA = LUA_TUSERDATA,
        LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
        TABLE = LUA_TTABLE
    };

    template <typename T>
    inline type lua_typeof() { return type::NONE; }

    template <>
    inline type lua_typeof<char>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<unsigned char>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<short>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<unsigned short>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<int>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<unsigned int>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<long>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<unsigned long>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<float>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<double>() { return type::NUMBER; }
    template <>
    inline type lua_typeof<bool>() { return type::BOOLEAN; }
    template <>
    inline type lua_typeof<const char *>() { return type::STRING; }
    template <>
    inline type lua_typeof<std::string>() { return type::STRING; }
}

#endif
