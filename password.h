#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

namespace password
{
void stdecho(bool enable = true)
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	if (!enable)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode);
}

// Disable echo, take user input, re-enable echo
std::string GetPassword()
{
	std::string pwd{};

	stdecho(false);
	getline(std::cin, pwd);
	stdecho(true);
	return pwd;
}

} // namespace pwd
