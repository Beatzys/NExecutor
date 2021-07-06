#pragma once

// Wrapper for ScriptContext
class ScriptContext
{
private:
	void* instance;
public:
	ScriptContext(void* const _instance);

	void ExecuteIDEScript(int identity, const char* script, const char* name);
};

void* const scriptContextVftable = (void*)0x00A60E7C;
