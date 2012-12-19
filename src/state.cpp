#include "luacpp/state.hpp"

#include <stdexcept>

namespace
{
    int panic(lua_State *L)
    {
        throw std::runtime_error(lua_tostring(L, -1));
    }
}

namespace lua
{
    state::state() : L(luaL_newstate()), owns_state(true)
    {
        lua_atpanic(L, panic);
    }

    void state::do_string(const char *code)
    {
        luaL_dostring(this->L, code);
    }

    void state::do_file(const char *filename)
    {
        luaL_dofile(this->L, filename);
    }
}
