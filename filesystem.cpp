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
    
   file: filesystem.cpp
  
  
 */


#include "filesystem.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>


string getExtension(string filename);

using namespace std;



#define TOTAL_MUSIC_FORMATS 11
string MusicFormats[TOTAL_MUSIC_FORMATS] =
{
     
     ".aac",	//Advanced Audio Coding File
     ".aif",	//Audio Interchange File Format
     ".iff",	//Interchange File Format
     ".m3u",	//Media Playlist File
     ".mid",	//MIDI File
     ".mp3",	//MP3 Audio File
     ".mpa",	//MPEG-2 Audio File
     ".ra",	//Real Audio File
     ".wav",	//WAVE Audio File
     ".wma",	//Windows Media Audio File
     ".ogg"
};



#define TOTAL_DOCUMENT_FORMATS 11
string DocumentFormats[TOTAL_DOCUMENT_FORMATS] =
{
     ".doc",	//Microsoft Word Document
     ".docx",	//Microsoft Word Open XML Document
     ".log",	//Log File
     ".msg",	//Outlook Mail Message
     ".pages",	//Pages Document
     ".rtf",	//Rich Text Format File
     ".txt",	//Plain Text File
     ".wpd",	//WordPerfect Document
     ".wps",	//Microsoft Works Word Processor Document
     ".xls",
     ".pdf"
};



#define TOTAL_VIDEO_FORMATS 14
string VideoFormats[TOTAL_VIDEO_FORMATS] =
{
     ".3g2",	//3GPP2 Multimedia File
     ".3gp",	//3GPP Multimedia File
     ".asf",	//Advanced Systems Format File
     ".asx",	//Microsoft ASF Redirector File
     ".avi",	//Audio Video Interleave File
     ".flv",	//Flash Video File
     ".mov",	//Apple QuickTime Movie
     ".mp4",	//MPEG-4 Video File
     ".mpg",	//MPEG Video File
     ".rm",	//Real Media File
     ".swf",	//Flash Movie
     ".vob",	//DVD Video Object File
     ".wmv"	//Windows Media Video File
	 ".mkv"	//Windows Media Video File
};





#define TOTAL_PICTURE_FORMATS 15

string PictureFormats[TOTAL_PICTURE_FORMATS] =
{
     ".bmp",	//Bitmap Image File
     ".gif",	//Graphical Interchange Format File
     ".jpg",	//JPEG Image File
     ".jpeg",
     ".png",	//Portable Network Graphic
     ".psd",	//Adobe Photoshop Document
     ".pspimage",	//PaintShop Photo Pro Image
     ".thm",	//Thumbnail Image File
     ".tif",	//Tagged Image File
     ".yuv",	//YUV Encoded Image File 
     ".ai",	//Adobe Illustrator File
     ".drw",	//Drawing File
     ".eps",	//Encapsulated PostScript File
     ".ps",	//PostScript File
     ".svg"	//Scalable Vector Graphics Fil
};






Filesystem::Filesystem(string name, string checkingPath)
{
     
     this->checkingPath = checkingPath;
     this->root = checkingPath + name;
     this->name = name;

     //directories.push_back( new Directory(name, path+name+"/" ) );
     
     this->init();
}



Filesystem::Filesystem(string name, string checkingPath, string root)
{
     this->name = name;
     this->checkingPath = checkingPath;
     this->root = root + name;
  
     this->init();
}





Filesystem::~Filesystem(void)
{
     list <Directory *>::iterator current;
     list <Directory *>::iterator previous;

     current = this->directories.begin();

     while( current != this->directories.end() )
     {
	  previous = current;
	  current++;
	  delete *previous;
     }

     this->directories.clear();
     
}



void Filesystem::refresh(void)
{
     DIR *dir;
     struct dirent *ent;
     dir = opendir (this->checkingPath.c_str());
     
     if (dir != NULL)
     {
	  while ((ent = readdir (dir)) != NULL)
	  {
	       if( ent->d_type == DT_REG )
	       {
		    string name;
		    name = ent->d_name;

		    if(name!="." && name!="..")
		    {
			 list <Directory *>::iterator it;
			 it = this->directories.begin();
			 bool done = false;
			 while(it!=directories.end() && done==false)
			 {
			      if( (*it)->containsFormat( getExtension(name) ) )
			      {
				   (*it)->collectFile(this->checkingPath + "\"" + name + "\"");
				   done = true;
			      }
			      
			      it++;
			 }
		    }
		    
	       }	    
	  }
     }
     closedir(dir);


     // Remove Empty Directories
     
     list <Directory *>::iterator it;
     it = this->directories.begin();
   
     while(it!=directories.end() )
     {
	  if( (*it)->isEmpty() )
	  {
	       (*it)->remove();
	      
	  }  
	  it++;
     }



     
}




string getExtension(string filename)
{
     string tmp = filename;
     int index = filename.find_last_of(".");
     
     if(index!=-1)
	  tmp = tmp.substr(index , tmp.length());


     //Convert to lowercase
     string::iterator it;
     for ( it=tmp.begin() ; it < tmp.end(); it++ )
	  *it = tolower(*it);
     

     return tmp;
     
}




void Filesystem::init(void)
{
     
    Directory *rootDir = new Directory(this->root);
    this->directories.push_back( rootDir );


    Directory *d, *parent;

    // ====================== MUSIC =======================
     
     d = new Directory(this->root + "/Music/");
     d->setParent(rootDir);
     for(int i=0; i<TOTAL_MUSIC_FORMATS;i++)
	  d->addFormat(MusicFormats[i]);

     this->directories.push_back(d);



     // ====================== PICTURES =======================
    
     d = new Directory(this->root + "/Pictures/");
     d->setParent(rootDir);
     for(int i=0; i<TOTAL_PICTURE_FORMATS;i++)
	  d->addFormat(PictureFormats[i]);
     
     this->directories.push_back(d);



     // ====================== VIDEOS =======================
    
     d = new Directory(this->root + "/Videos/");
     d->setParent(rootDir);
     for(int i=0; i<TOTAL_VIDEO_FORMATS;i++)
	  d->addFormat(VideoFormats[i]);
     
     this->directories.push_back(d);



     
    // ====================== DOCUMENTS ======================= 
    d = new Directory(this->root + "/Documents/");
    d->setParent(rootDir);
     
  
    this->directories.push_back(d);
     

    parent = d;
    
    d = new Directory(this->root + "/Documents/PDF/");
    d->addFormat(".pdf");
    d->setParent(parent);
    this->directories.push_back(d);

    d = new Directory(this->root + "/Documents/Spreadsheets/");
    d->addFormat(".xls");
    d->setParent(parent);
    this->directories.push_back(d);

    d = new Directory(this->root + "/Documents/Presentations/");
    d->addFormat(".ppt");
    d->setParent(parent);
    this->directories.push_back(d);

    d = new Directory(this->root + "/Documents/Text/");
    d->addFormat(".txt");
    d->addFormat(".doc");
    d->setParent(parent);
    this->directories.push_back(d);

    
    d = new Directory(this->root + "/Documents/Programming/");
    d->addFormat(".c");
    d->addFormat(".cpp");
    d->addFormat(".h");
    d->addFormat(".java");
    d->addFormat(".html");
    d->setParent(parent);
    this->directories.push_back(d);

    

    d = new Directory(this->root + "/Other/");
    d->setParent(rootDir);
    
    this->directories.push_back(d);
    
    parent = d;
    d = new Directory(this->root + "/Other/Compressed/");
    d->addFormat(".zip");
    d->addFormat(".gz");
    d->addFormat(".rar");
    d->addFormat(".bz2");
    d->addFormat(".7z");
    d->addFormat(".zz");
    d->setParent(parent);
    this->directories.push_back(d);


    d = new Directory(this->root + "/Other/Torrents/");
    d->addFormat(".torrent");
    d->setParent(parent);
    this->directories.push_back(d);
    

    d = new Directory(this->root + "/Other/Subtitles/");
    d->addFormat(".sub");
    d->addFormat(".srt");
    d->setParent(parent);
    this->directories.push_back(d);


    d = new Directory(this->root + "/Other/ISO Images/");
    d->addFormat(".iso");
    d->setParent(parent);
    this->directories.push_back(d);


    d = new Directory(this->root + "/Other/Executables/");
    d->addFormat(".exe");
    d->addFormat(".out");
    d->addFormat(".deb");
    d->addFormat(".rpm");
    d->setParent(parent);
    this->directories.push_back(d);
}






