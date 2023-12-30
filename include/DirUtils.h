// A static class to manipulate files, directories and paths
// This is a substitute for std::filesystem os C++20, which does not work here
#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <vector>

class DirUtils
{
public:
	DirUtils();
	virtual ~DirUtils();

	static bool m_PathExists(const std::string &path);
	static bool m_IsPathRelative(const std::string &path);
	static bool m_IsDirectory(const std::string &path);
	static bool m_IsFile(const std::string &path);

	static void m_DeleteFile(const std::string &path);
	static void m_CreateDir(const std::string &path);
	static void m_SetCurrentDir(const std::string &path);

	static std::string m_JoinPaths(const std::string &upper_path, const std::string &sub_path); //pode dar problema com as \ e /
	static std::string m_GetCurrentDir(); 
	static std::string m_GetFileName(const std::string &filepath);
	static std::string m_GetFileExtension(const std::string &filepath);
	static std::string m_GetParentDir(const std::string &filepath);
	static std::string m_GetSeparator();

	static std::vector<std::string> m_ListFiles(const std::string &path, bool recursive = true);

	static void m_ReadAllLines(const std::string &filepath, std::vector<std::string> &lines);
	static void m_Tokenize(const std::string &text, const std::string &delimiters, std::vector<std::string> &tokenized);
};


#endif