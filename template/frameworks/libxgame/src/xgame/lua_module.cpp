#include "xgame/lua_module.h"
#include "xgame/lua_crypto.h"
#include "xgame/xruntime.h"
#include "xgame/xlua.h"
#include "xgame/lua-bindings/lua_xgame.h"
#include "cjson/lua_cjson.h"
#include "md5/md5.h"
#include "pbc/pbc.h"
#include "cocos2d.h"

int luaopen_module(lua_State *L)
{
    xlua_preload(L, "protobuf.c", luaopen_protobuf_c);
    xlua_preload(L, "crypto.md5" , luaopen_md5);
    xlua_preload(L, "crypto.base64", luaopen_base64);
    xlua_preload(L, "crypto.xxtea", luaopen_xxtea);
    xlua_preload(L, "cjson", luaopen_cjson);
    xlua_preload(L, "cjson.safe", luaopen_cjson_safe);
    
    xlua_call(L, luaopen_xgame);
    
    return 0;
}