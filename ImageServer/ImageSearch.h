#ifndef ImageSearch__INCLUDED
#define ImageSearch__INCLUDED

#pragma once
class ImageSearch
{
public:
	static void run(void);
	static void ImageSearch::handleSearch(char* file, string locale);
private:
	static void search(char* file, const char* localePath);
	static bool match(string name, string key);
};

#endif