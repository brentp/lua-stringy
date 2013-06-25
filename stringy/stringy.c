/***
Methods for fast string operations

@license MIT/X11
@module stringy
*/
#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "fastsearch.h"

#if LUA_VERSION_NUM < 502
# define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
# define lua_rawlen lua_objlen
#endif

inline int get_start(lua_State *L, int nargs, int string_len){
    int start = (nargs > 2) ? lua_tonumber(L, 3) - 1: 0;
    // what about when they pass 0?
    if(start < 0) { start = string_len + start + 1; }
    return start;
}
inline int get_end(lua_State *L, int nargs, int string_len){
    int end = (nargs > 3) ? lua_tonumber(L, 4) - 1: string_len;
    if(end < 0) { end = string_len + end + 1; }
    return end;
}



inline static int lua_fastsearch_wrap(lua_State *L, int mode) {
    size_t string_len;
    const char *string = luaL_checklstring(L, 1, &string_len);

    size_t token_len;
    const char *token = luaL_checklstring(L, 2, &token_len);
    
    int nargs = lua_gettop(L);
    int start = get_start(L, nargs, string_len);
    int end = get_end(L, nargs, string_len);
    if (start > end){ return -1; }

    string_len = end - start; 
    int val = fastsearch(string + start, string_len,
                            token, token_len, -1, mode);
    if (mode == FAST_COUNT) { return val; }

    if(val == -1) { return val; }
    int r = val + ((start > 0) ? start  : 0);
    return r;
}

/// count number of occurences of a substring in a string
// @param str the string to search in
// @param substr the substring to count
// @return the number of occurrences of substring substr in string str
// @function count(str, substr)
static int count(lua_State *L) {
    int scount = lua_fastsearch_wrap(L, FAST_COUNT);
    if(scount == -1){ scount = 0; }
    lua_pushnumber(L, scount);
    return 1;
}

/// find the first occurence of a substring needle in string haystack
// @param haystack the string to search in
// @param needle the substring to search for
// @return the index of first occurence of substring needle in string haystack
// @function find(haystack, needle)
static int find(lua_State *L) {
    int sloc = lua_fastsearch_wrap(L, FAST_SEARCH);
    // adjust for lua 1-based indexing.
    if (sloc != -1) { lua_pushnumber(L, sloc + 1); }
    else { lua_pushnil(L); }
    return 1;
}

/// test if a string ends with a specific substring
// @param str the string to search in
// @param substr the substring to test for
// @return true if `str` ends with `substr`, false otherwise
// @function endswith(str, substr)
static int endswith(lua_State *L) {
    size_t string_len;
    const char *string = luaL_checklstring(L, 1, &string_len);

    size_t token_len;
    const char *token = luaL_checklstring(L, 2, &token_len);

    int end = 0;
    if(token_len <= string_len){
        string += string_len - token_len;
        end = memcmp(string, token, token_len) == 0;
    }
    lua_pushboolean(L, end);
    return 1;
}

/// test if a string starts with a specific substring
// @param str the string to test
// @param substr the substring to test for
// @return true if `str` starts with `substr`, false otherwise
// @function startswith(str, substr)
static int startswith(lua_State *L) {
    const char *string = luaL_checkstring(L, 1);
    int string_len = lua_rawlen(L, 1);

    const char *token = luaL_checkstring(L, 2);
    int token_len = lua_rawlen(L, 2);
    int start = 0;
    if(token_len <= string_len)
        start = memcmp(string, token, token_len) == 0;
    lua_pushboolean(L, start);
    return 1;
}

/// split a string into an array
// @param str the string to split
// @param sep a character to use as separator
// @return an array splitted by `sep`
// @function split(str, sep)
static int split(lua_State *L) {
    const char *string = luaL_checkstring(L, 1);
    const char *sep = luaL_checkstring(L, 2);
    const char *token;
    int i = 1;
    lua_newtable(L);
    while ((token = strchr(string, *sep)) != NULL) {
        lua_pushlstring(L, string, token - string);
        lua_rawseti(L, -2, i++);
        string = token + 1;
    }
    lua_pushstring(L, string);
    lua_rawseti(L, -2, i);
    return 1;
}

/// strip whitespace from beginning and end of a string
// @param str the string to strip
// @return the stripped string
// @function strip(str)

/*  based on http://lua-users.org/lists/lua-l/2009-12/msg00951.html
    from Sean Conner */
int strip(lua_State *L) {
    const char *front;
    const char *end;
    size_t      size;

    front = luaL_checklstring(L, 1, &size);
    end   = &front[size - 1];

    for ( ; size && isspace(*front) ; size-- , front++)
    ;
    for ( ; size && isspace(*end) ; size-- , end--)
    ;

    lua_pushlstring(L, front, (size_t)(end - front) + 1);
    return 1;
}

static const luaL_Reg stringy[] = {
    {"count", count},
    {"find", find},
    {"split", split},
    {"strip", strip},
    {"startswith", startswith},
    {"endswith", endswith},
    {NULL, NULL}
};

int luaopen_stringy(lua_State *L){
    luaL_newlib(L, stringy);
    return 1;
}
