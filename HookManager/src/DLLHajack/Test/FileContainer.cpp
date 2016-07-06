#include "FileContainer.h"


FileContainer* FileContainer::_file_container_obj = NULL;

FileContainer::FileContainer()
{
}


FileContainer::~FileContainer()
{
}


FileContainer* FileContainer::Instance()
{
	if (_file_container_obj == NULL)
		_file_container_obj = new FileContainer();
	return _file_container_obj;
}
void FileContainer::Desory()
{
	if (_file_container_obj)
		delete _file_container_obj;
	_file_container_obj = NULL;
}