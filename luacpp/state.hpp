#ifndef LCPP_STATE_HPP
#define LCPP_STATE_HPP

#include <string>

#include "luacpp/lua.hpp"

namespace lua
{
    class state
    {
        public:
            state();
            state(lua_State *L) : L(L), owns_state(false) {}
            state(const state &) = delete;
            state(const state &&rhs) : L(rhs.L), owns_state(rhs.owns_state) {}
            ~state() { if(this->owns_state) lua_close(this->L); }

            state &operator=(const state &) = delete;
            inline operator lua_State*() const { return this->L; }

            void open_libraries() { luaL_openlibs(this->L); }
            void do_string(const char *code);
            void do_file(const char *filename);

        private:
            lua_State *L;
            bool owns_state;
    };
}

#endif
