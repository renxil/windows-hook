#pragma once
#include "stdafx.h"

/*
resource file class 
*/
class FileH
{
public:
	FileH(HANDLE handle, DWORD oper_type, DWORD oper_len) :_file_handle(handle), _file_oper_type(oper_type), _file_oper_len(oper_len) {}
	virtual ~FileH() {}
	HANDLE  GetFileHandle() { return _file_handle; }
	DWORD GetFileOperType() { return _file_oper_type; }
	DWORD GetFileOperLen() { return _file_oper_len; }
private:
	HANDLE _file_handle;
	DWORD _file_oper_type; // 1:read ;2:write
	DWORD _file_oper_len;

};

/*
unused
*/
class HandleFile
{
public :
	HandleFile(HANDLE handle, wstring file_path) :_handle(handle), _file_path(file_path) {}
	virtual ~HandleFile() {}
	
	HANDLE GetHandle() const{ return _handle; }
	wstring GetFileName() const { return _file_path; }

private:
	HANDLE _handle;
	wstring _file_path;

};

class FileContainer
{
public:
	FileContainer();
	virtual ~FileContainer();

	static FileContainer* Instance();
	static void Desory();

	/*
	vector<HandleFile>& GetHandles() { return _vec_handles; }
	void AddHandle(HandleFile& handle) { _vec_handles.push_back(handle); }
	int GetHandleContainerLen() { return _vec_handles.size(); }
	m.emplace(std::make_pair(std::string("a"), std::string("a")));
	*/
	void AddHandle(HANDLE handle, wstring file_path) { _map_handles.emplace(make_pair(handle, file_path)); }
	int GetHandleContainerLen()const { return _map_handles.size(); }
	map<HANDLE, wstring>& GetHandles() { return _map_handles; }

	vector<FileH>& GetFileS() { return _vec_files; }
	void AddFile(FileH f) { _vec_files.push_back(f); }
	int GetFileContainerLen() { return _vec_files.size(); }
	
private:
	vector<FileH> _vec_files;
	map<HANDLE,wstring> _map_handles;

	static FileContainer* _file_container_obj;
};

