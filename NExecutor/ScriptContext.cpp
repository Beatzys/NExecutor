#include "pch.h"
#include "ScriptContext.h"

ScriptContext::ScriptContext(void* const _instance)
	: instance(_instance)
{
}

void ScriptContext::ExecuteIDEScript(int identity, const char* script, const char* name)
{
	const auto _ExecuteIDEScript = (void(__thiscall*)(void* _this, int _identity, const char* _script, const char* _name))0x006083E0;
	_ExecuteIDEScript(instance, identity, script, name);
}
