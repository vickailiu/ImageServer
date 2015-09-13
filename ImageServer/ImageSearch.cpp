#include "StdAfx.h"
#include "ImageSearch.h"

#include "Path.h"

#include "ROSE_CDVSLib.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

#define BOOST_FILESYSTEM_NO_DEPRECATED
namespace fs = boost::filesystem;

void ImageSearch::run(void)
{
	//** First of all, Init SDK
	ROSE_InitSDK();

	ROSE_INDEX_Ptr pku_index = NULL;

	{
		///////Read database_0
		//** 0. Read all image files to a list
		FILE_NAME image_list0[10];
		int len = 10;

		FILE *fin = fopen("database0.txt", "r");
		for (int i = 0; i < len; i ++)
			fscanf(fin, "%s", image_list0[i]);
		fclose(fin);

		//** 1. Create Index
		cout << "1. Create Index...." << endl;
		pku_index = ROSE_CreateIndex(image_list0, len);

		//** 2. Save index to File
		ROSE_SaveIndex(pku_index, "db_0.bin");

		//** 3. Free index
		ROSE_FreeIndex(pku_index);
		pku_index = NULL;
	}
	{
		///////Read database_1
		//** 0. Read all image files to a list
		FILE_NAME image_list1[12];
		int len = 12;

		FILE *fin = fopen("database1.txt", "r");
		for (int i = 0; i < len; i ++)
			fscanf(fin, "%s", image_list1[i]);
		fclose(fin);

		//** 1. Create Index
		cout << "1. Create Index...." << endl;
		ROSE_INDEX_Ptr pku_index = NULL;
		pku_index = ROSE_CreateIndex(image_list1, len);

		//** 2. Save index to File
		ROSE_SaveIndex(pku_index, "db_1.bin");

		//** 3. Free index
		ROSE_FreeIndex(pku_index);
		pku_index = NULL;
	}

	//** 4. Merge index
	FILE_NAME index_list[2] = {"db_0.bin", "db_1.bin"};
	int len = 2;

	pku_index = ROSE_MergeIndex(index_list, len);
	ROSE_SaveIndex(pku_index, "db.bin");
    ROSE_FreeIndex(pku_index);
	pku_index = NULL;

	//** 4. Load index
	pku_index = ROSE_LoadIndex("db.bin");

	//** 5. Add new image to index
	ROSE_AddToIndex(pku_index, "database/wangbook.jpg");

	//** 6. Retrieval
	ROSE_Result pku_result[100];
	int max_ret_num = 100;
	ROSE_Retrieval(pku_index, "1.jpg", pku_result, &max_ret_num);

	printf("Retrieval return %d\n", max_ret_num);
	for (int i = 0; i < max_ret_num; i ++)
		printf("i = %d, %s, %.3lf\n", i, pku_result[i].image_id, pku_result[i].score);

	ROSE_SaveIndex(pku_index, "db.bin");
	ROSE_FreeIndex(pku_index);
	pku_index = NULL;

	//** 7. Matching
	float score1 = ROSE_Matching("1.jpg", "11.jpg");
	printf("score1 = %.3lf\n", score1);

	float score2 = ROSE_Matching("1.jpg", "2.jpg");
	printf("score2 = %.3lf\n", score2);

	//** Remember to finish SDK
	ROSE_FinishSDK();
	printf("Completed!");
}

void ImageSearch::handleSearch(char* file, string locale)
{
	string indexFile = DEFAULT_INDEX;

	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_itr(DB_PATH); dir_itr != end_iter; ++dir_itr)
	{
		try
		{
			if (fs::is_regular_file(dir_itr->status()))
			{
				string fileName = dir_itr->path().stem().string();
				if ( match(fileName, locale) )
				{
					indexFile = fileName;
					break;
				}
			}
		}
		catch (const std::exception & ex)
		{
			cout << dir_itr->path().filename() << " " << ex.what() << endl;
		}
	}

	if (indexFile == DEFAULT_INDEX)
	{
		cout << "no such locale" << endl;
	}

	try {
		ROSE_InitSDK();
		search(file,(DB_PATH + "\\" + indexFile).c_str());
		ROSE_FinishSDK();
	}
	catch(exception & ex)				// catch any exception, including CdvsException
	{
		cerr << "Exception: " << ex.what() << endl;
	}
}

void ImageSearch::search(char* file, const char* localePath)
{
	cout << "Search in " << localePath << endl;

	ROSE_INDEX_Ptr index = ROSE_LoadIndex(localePath);

	ROSE_Result result[10];
	int max_ret_num = 10;
	ROSE_Retrieval(index, file, result, &max_ret_num);

	printf("Retrieval return %d\n", max_ret_num);
	for (int i = 0; i < max_ret_num; i ++)
	{
		printf("i = %d, %s, %.3lf\n", i, result[i].image_id, result[i].score);
	}

	ROSE_FreeIndex(index);
	index = NULL;
}

bool ImageSearch::match(string name, string key)
{
	int nameLength = name.length();
	int keyLength = key.length();
	
	return (name.find(key) == nameLength - keyLength);
}