#pragma once



class CFileManager
{
public:
	CFileManager(void);
	~CFileManager(void);

	CString FILE_SYSTEM;


	BOOL LoadSystemInfo();
	void SaveSystemInfo();
	BOOL IsExistFile(CString strFile);
	void InitSystemDir();

	CString m_FilePath;
	BOOL MakeDirectory(CString strDir);


};

