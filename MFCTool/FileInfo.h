#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString ConvertRelativePath(const CString& wstrFileFullPath); 
};

