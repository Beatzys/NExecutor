#pragma once

#include "ScriptContext.h"

class ExecutorConsole
{
public:
	enum MessageType
	{
		Output,
		Warning,
		Error
	};
private:
	void SetColorFromMessageType(const MessageType type);
public:
	bool CreateConsole();

	void Write(const MessageType type, const std::string& message);
	void WriteFormat(const MessageType type, const char* format, ...);

	void StartExecutor(ScriptContext& scriptContext);
};
