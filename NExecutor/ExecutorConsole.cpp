#include "pch.h"
#include "ExecutorConsole.h"

#include <iostream>

void ExecutorConsole::SetColorFromMessageType(const MessageType type)
{
	auto out = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (type)
	{
	case Output:
		SetConsoleTextAttribute(out, 7);
		break;
	case Warning:
		SetConsoleTextAttribute(out, 6);
		break;
	case Error:
		SetConsoleTextAttribute(out, 4);
		break;
	}
}

bool ExecutorConsole::CreateConsole()
{
	FILE* dummy;

	return AllocConsole() &&
		SetConsoleTitle("NExecutor") &&
		freopen_s(&dummy, "CONIN$", "r", stdin) == 0 &&
		freopen_s(&dummy, "CONOUT$", "w", stdout) == 0;
}

void ExecutorConsole::Write(const MessageType type, const std::string& message)
{
	SetColorFromMessageType(type);

	std::cout << message;
}

void ExecutorConsole::WriteFormat(const MessageType type, const char* format, ...)
{
	SetColorFromMessageType(type);

	va_list args;
	va_start(args, format);

	vprintf_s(format, args);

	va_end(args);
}

void ExecutorConsole::StartExecutor(ScriptContext& scriptContext)
{
	Write(Output, "NExecutor initialized!\n\n");

	while (true)
	{
		Write(Output, "> ");

		std::string script;
		std::getline(std::cin, script);

		try
		{
			scriptContext.ExecuteIDEScript(6, script.c_str(), "NExecutor");
		}
		catch (std::runtime_error& error)
		{
			WriteFormat(Error, "Script error: %s\n", error.what());
		}
	}
}
