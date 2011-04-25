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
    
   file: directory.h
  
  
 */



#ifndef DIRECTORY_H_
#define DIRECTORY_H_



#include <string>
#include <set>


using namespace std;


class Directory
{
	private:
	string path;
	Directory * parent;
	set<string> formats;
	bool allowHidden;
	
	public:	
	
	Directory(string path);
	~Directory(void);

	string getPath(void);
	void addFormat(string filename);
	//void addSubDirectory(string subname);
	void collectFile(string filepath);

	void setParent(Directory *parent);
	//void moveOneLevelUp(void);
	//void moveOneLevelDown(Directory *parent);
	//Directory *getParent(void);
	int fileExists(string filename);
	int containsFormat(string format);
	void renameFile(string filename, string newfilename);
	void create(void);
	void remove(void);
	int isEmpty(void);
	void allowHiddenFiles(bool value);
};




#endif /* DIRECTORY_H_ */
