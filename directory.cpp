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
    
   file: directory.cpp
  
  
 */


#include "directory.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>
#include <sstream>




string getCMDpath(string path);
string convertInt(int number);
string getNewName(string name);




Directory::Directory(string path)
{
     this->path = path;
     this->parent = NULL;
     this->allowHidden = false;
}




Directory::~Directory()
{
     //delete subDirectories;
     //delete formats;

     /*
     set<Directory *>::iterator previous;
     set<Directory *>::iterator current;
     
     current = this->subDirectories.begin();
     
     while(current != this->subDirectories.end())
     {
	  previous = current;
	  current++;
	  delete *previous;
     }

     this->subDirectories.clear();
     this->formats.clear();
     */
     
     return;
     
}




string Directory::getPath()
{
     return path;
}







void Directory::addFormat(string format)
{
     this->formats.insert(format);    
}




void Directory::setParent(Directory *parent)
{
     this->parent = parent;
}


void Directory::allowHiddenFiles(bool value)
{
     this->allowHidden = value;
}


/*

void Directory::addSubDirectory(string subname)
{
     
     this->subDirectories.insert(new Directory(subname, this->path + subname + "/"));
}









Directory * Directory::getSubDirectory(string subname)
{
     set<Directory *>::iterator it;
     
     it = this->subDirectories.begin();
     
     while(it != this->subDirectories.end())
     {
	  if((*it)->getName() == subname)
	       return *it;     
	  it++;
     }
     
     return NULL;
}

*/





int Directory::containsFormat(string format)
{ 
     set<string>::iterator it;
     
     it = this->formats.find(format);
     
     if(it == this->formats.end())
	  return 0;
     
     return 1;
}




int Directory::fileExists(string filename)
{
     DIR *dir;
     struct dirent *ent;
     dir = opendir (this->path.c_str());
     
     if (dir != NULL)
     {
	  while ((ent = readdir (dir)) != NULL)
	  {
	       string name;
	       name = ent->d_name;
	       
	       if(name==filename)
	       {
		    closedir(dir);
		    return 1;
	       }
	       
	  }
     }
     closedir(dir);
     return 0;
}




void Directory::renameFile(string filename, string newfilename)
{
     DIR *dir;
     struct dirent *ent;
     dir = opendir (this->path.c_str());
     
     if (dir != NULL)
     {
	  while ((ent = readdir (dir)) != NULL)
	  {
	       string name;
	       name = ent->d_name;
		    
	       if(name==filename)
	       {
		    string cmd;
		    cmd = "mv " + this->path + "\"" + filename + "\"" + " " + this->path + "\"" +newfilename + "\"";
		    system(cmd.c_str());
		    closedir(dir);
		    return;
	       }
	       
	  } 
     }
     closedir(dir);
}




void Directory::collectFile(string filepath)
{
     this->create();


     string filename = filepath;
     filename.erase(0,filename.find_last_of("/")+1 );
     filename.erase(0,1);
     filename.erase(filename.size()-1, filename.size());


     while( this->fileExists(filename) )
     {
	  filename = getNewName(filename);
     }
     
     
     string cmd;
     cmd = "mv " + filepath + " " + getCMDpath(this->path) + "\"" + filename + "\"";
    
     system(cmd.c_str());
     
}




void Directory::create(void)
{

     if(this->parent!=NULL)
	  this->parent->create();
     
     DIR *dir;
    
     dir = opendir(this->path.c_str());
     
     if(dir==NULL)
     {
	 mkdir(this->path.c_str(), 0777);  
     } 
     else
     {
	  closedir(dir);
     }
}




void Directory::remove(void)
{
   
     if( this->isEmpty() )
     {
	  string cmd;
	  cmd = "rm -rf " + getCMDpath(this->path);
	  system(cmd.c_str());
	  //rmdir(this->path.c_str());
     }
     
     if(this->parent!=NULL)
     {
	  this->parent->remove();
     }
}




int Directory::isEmpty(void)
{
     DIR *dir;
     struct dirent *ent;
    
     dir = opendir (this->path.c_str());
     
     if (dir != NULL)
     {
	  while ((ent = readdir (dir)) != NULL)
	  {
	       if(ent->d_type == DT_REG || ent->d_type == DT_DIR )
	       {
		    string name;
		    name = ent->d_name;

		    if(name=="." || name=="..")
			 continue;
		    
		    if( this->allowHidden && name[0] == '.')
		    {
			    closedir (dir);
			    return 0;
		    }

		    if(name[0]!='.')
		    {
			 closedir (dir);
			 return 0;
		    } 
	       }

	  }
	       
     }  

     closedir (dir);

     return 1;
}




string getCMDpath(string path)
{
     for(int i=0; i<path.length(); i++)
     {
	  if(path[i]==' ')
	  {
	       path.insert(i++, "\\");
	  }
     }
     
     return path;
}







string convertInt(int number)
{
     stringstream ss;//create a stringstream
     ss << number;//add number to the stream
     return ss.str();//return a string with the contents of the stream
}

string getNewName(string name)
{
     
     int index0 = name.find_last_of("(");
     int index1 = name.find_last_of(")");
     
     if(index0 == -1 || index1 == -1)
     {
	  int i = name.find_first_of(".");
	  if(i==-1)
	       name.insert(name.size(), "(1)");
	  else
	       name.insert( i, "(1)");
	  
     }
     else
     {
	  string tmp = name;
	  tmp = tmp.erase(0, index0+1);
	  tmp = tmp.erase(tmp.find_last_of(")"), tmp.length());
	  int num = atoi(tmp.c_str());
	  num++;

	  if(	(index1 - index0) > 0	)
	  {    name = name.erase(index0+1, tmp.length());
	  name = name.insert(index0+1, convertInt(num));
	  }
	  else
	       name = name.insert(index1, convertInt(num));
     }
     
     
     return name;
}
