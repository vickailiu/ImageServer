#ifndef DatabaseHandler__INCLUDED
#define DatabaseHandler__INCLUDED

#pragma once
class DatabaseHandler
{
public:
	static void buildIndex(void);
private:
	static int DatabaseHandler::iterateBuildIndex(string dir, string dir_string);
	// static string replace(string source, string key, string value);
};

#endif