#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & wstrFileFullPath)
{
	TCHAR szCurDir[MAX_PATH]{}; 
	GetCurrentDirectory(MAX_PATH, szCurDir); 
	PathRelativePathTo(szCurDir, szCurDir, FILE_ATTRIBUTE_DIRECTORY, wstrFileFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY); 

	return CString(szCurDir);
}
