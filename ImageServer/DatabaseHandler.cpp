#include "StdAfx.h"
#include "DatabaseHandler.h"

#include "ROSE_CDVSLib.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#define BOOST_FILESYSTEM_NO_DEPRECATED
namespace fs = boost::filesystem;
using namespace std;

const string DB_PATH = "C:\\Dev\\uGuide_Server\\database";

void DatabaseHandler::buildIndex(void) {
	
	ROSE_InitSDK();

	iterateBuildIndex(DB_PATH, "");
	
	ROSE_FinishSDK();

}

int DatabaseHandler::iterateBuildIndex(string dir, string dir_string){
	string temp_child_dir;
	FILE_NAME child_dirs[100];	// list of child directories
	int child_dir_count = 0;
	FILE_NAME child_files[100];	// list of child files
	int child_file_count = 0;

	fs::path path = fs::system_complete(fs::path(dir));
	if (!fs::exists(path))
	{
		cout << "\nNot found: " << path.string() << endl;
		return 0;
	}

	if (fs::is_directory(path))
	{
		if (dir.compare(DB_PATH) != 0) // not the DB root directory
		{
			//std::cout << dir_string << "\n";
		}

		fs::directory_iterator end_iter;
		for (fs::directory_iterator dir_itr(path); dir_itr != end_iter; ++dir_itr)
		{
			try
			{
				if (fs::is_directory(dir_itr->status()))
				{
					temp_child_dir = dir_string;
					if (dir.compare(DB_PATH) != 0) // not the DB root directory
					{
						temp_child_dir += "-";
					}
					temp_child_dir += dir_itr->path().filename().string();
					int result = iterateBuildIndex(dir_itr->path().string(), temp_child_dir);
					
					if (result == 0)
					{
						return 0;
					}
					else
					{
						strcpy(child_dirs[child_dir_count], (DB_PATH + "\\" + temp_child_dir).c_str());
						child_dir_count++;
						std::cout << (DB_PATH + "\\" + temp_child_dir) << "\n";
					}
				}
				else if (fs::is_regular_file(dir_itr->status()))
				{
					strcpy(child_files[child_file_count], (dir + "\\" + dir_itr->path().filename().string()).c_str());
					child_file_count++;
					// std::cout << (dir + "\\" + dir_itr->path().filename().string()) << "\n";
				}
				else
				{
					// std::cout << dir_itr->path().filename() << " [other]\n";
				}

			}
			catch (const std::exception & ex)
			{
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			}
		}
	}
	else // must be a file
	{
		std::cout << "\nFound: " << path.string() << "\n";
		return 0;
	}

	if (child_dir_count > 0 || child_file_count > 0)
	{
		// std::cout << (DB_PATH + "\\" + dir_string)  << "\n";

		ROSE_INDEX_Ptr temp_index = NULL;

		if (child_dir_count > 0 && child_file_count > 0)
		{
			temp_index = ROSE_MergeIndex(child_dirs, child_dir_count);
			for (int i = 0; i < child_file_count; i++)
			{
				ROSE_AddToIndex(temp_index, child_files[i]);
			}
		}
		else if (child_dir_count > 0)
		{
			temp_index = ROSE_MergeIndex(child_dirs, child_dir_count);
		}
		else
		{
			temp_index = ROSE_CreateIndex(child_files, child_file_count);
		}

		ROSE_SaveIndex(temp_index, (DB_PATH + "\\" + dir_string).c_str());
		ROSE_FreeIndex(temp_index);
		temp_index = NULL;
	}

	return 1;
}

string replace(string source, string key, string value)
{
	int i = 0;
	for (;;) {
		i = source.find(key, i);
		if (i == string::npos) {
			break;
		}
		source.replace(i, 1, value);
	}

	return source;
}