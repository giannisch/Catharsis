/* 
    An Automatic File Manager - Daemon
  
    Copyright (C) 2011 Giannis Christakis
  
    Catharsis is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
   file: filesystem.h
  
  
 */



#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_


#include "directory.h"
#include <list>
#include <string>

using namespace std;



class Filesystem
{
	private:
	string name;
	string root;
	string checkingPath;
	
	list <Directory *> directories;

	void init(void);
	
	
	public:	
	
	Filesystem(string name, string checkingPath);
	Filesystem(string name, string checkingPath, string root);
	~Filesystem(void);

	void refresh(void);
	
};




#endif /* FILESYSTEM_H_ */
