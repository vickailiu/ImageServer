
#pragma once


//#define DLL_EXPORTS

#if defined(_WIN32) || defined(_WIN64)
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define ROSE_INDEX_Ptr void*
#define MAX_NAME_LENGTH  512

// Define image file type
typedef char FILE_NAME[MAX_NAME_LENGTH];

// Define struct to save retrieval result
typedef struct{
	FILE_NAME image_id; // Image id
	float score;         // Similarity score with query image
}ROSE_Result;


//----------------Init and Finish SDK----------------

/************************************************************************/
/* ROSE_InitSDK: Initialize visual search SDK                            */
/*      Must be invoked before use other function for only one time     */
/*		Params:															*/
/*			None	                                                    */
/*		return: int(0: success)											*/
/************************************************************************/
DLL_API int ROSE_InitSDK();

/************************************************************************/
/* ROSE_FinishSDK: Finish visual search SDK                              */
/*      Must be invoked after all function for only one time     		*/
/*		Params:															*/
/*			None	                                                    */
/*		return: int(0: success)										    */
/************************************************************************/
DLL_API int ROSE_FinishSDK();


//----------------Index----------------
/************************************************************************/
/* ROSE_CreateIndex: Create a index for database                         */
/*		Params:															*/
/*			image_list[]: list of image files	                        */
/*			len: length of image files list(must < limited length)      */
/*		return: ROSE_INDEX_Ptr(NULL: failed)								*/
/************************************************************************/
DLL_API ROSE_INDEX_Ptr ROSE_CreateIndex(FILE_NAME *image_list, int len);

/************************************************************************/
/* ROSE_AddToIndex: Add a new image to exiting index                     */
/*		Params:															*/
/*			ptr_index: the handle of index   	                        */
/*			image_path: the path of new image                           */
/*		return: int(0: success)						             		*/
/************************************************************************/
DLL_API int ROSE_AddToIndex(ROSE_INDEX_Ptr ptr_index, const char *image_path);

/************************************************************************/
/* ROSE_MergeIndex: Merge existing index                                 */
/*		Params:															*/
/*			index_list: list of index files                             */
/*			len: length of file list                                    */
/*		return: ROSE_INDEX_Ptr(NULL: failed)						        */
/************************************************************************/
DLL_API ROSE_INDEX_Ptr ROSE_MergeIndex(FILE_NAME *index_list, int len);

/************************************************************************/
/* ROSE_SaveIndex: Store index to file                                   */
/*		Params:															*/
/*			ptr_index: the handle of index   	                        */
/*			index_file: the index path to save index                    */
/*		return: int(0: success)						             		*/
/************************************************************************/
DLL_API int ROSE_SaveIndex(ROSE_INDEX_Ptr ptr_index,  const char *index_file);

/************************************************************************/
/* ROSE_LoadIndex: Load existing index from file                         */
/*		Params:															*/
/*			index_file: the index path to save index                    */
/*		return: ROSE_INDEX_Ptr(NULL: failed)				         		*/
/************************************************************************/
DLL_API ROSE_INDEX_Ptr ROSE_LoadIndex(const char *index_file);

/************************************************************************/
/* ROSE_FreeIndex: Free existing handle of a index                       */
/*		Params:															*/
/*			ptr_index: the handle of index   	                        */
/*		return: int(0: success)						             		*/
/************************************************************************/
DLL_API int ROSE_FreeIndex(ROSE_INDEX_Ptr ptr_index);


//----------------Retrieval and Matching----------------
/************************************************************************/
/* ROSE_Retrieval: Perform image retrieval                               */
/*		Params:															*/
/*			ptr_index: the handle of index   	                        */
/*			query_image: path of query image  	                        */
/*			ROSE_result[]: array to store retrieval results              */
/*			max_ret_num: max size of ROSE_result[] or actual return number*/
/*		return: int(0: success)						             		*/
/************************************************************************/
DLL_API int ROSE_Retrieval(ROSE_INDEX_Ptr ptr_index, 
	                      const char *query_image, 
						  ROSE_Result *rose_result,
						  int        *max_ret_num
						  );

/************************************************************************/
/* ROSE_Matching: Perform image matching                                 */
/*		Params:												 			*/
/*			image1: path of image1   	                                */
/*			image2: path of image2  	                                */
/*		return: float(Similarity score of two images)					*/
/************************************************************************/
DLL_API float ROSE_Matching(const char *image1, const char *image2);


#ifdef __cplusplus
}
#endif