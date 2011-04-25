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
    
   file: catharsis.cpp
  
  
 */


#include "directory.h"
#include "filesystem.h"
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CHECK_INTERVAL_SECONDS 4

#include <iostream>

int main(int argc, char **argv)
{
     string user = getlogin();
     
     Filesystem fs("Auto Managed", "/home/" + user + "/Downloads/");


     pid_t pid, sid;
     
    
     pid = fork();
     if (pid < 0) {
	  exit(EXIT_FAILURE);
     }
     
     
     
     sid = getsid(pid);
   
     if (pid > 0) {
	  exit(EXIT_SUCCESS);
     }
   
     umask(0);
   
     sid = setsid();
     
     if (sid < 0) {
	  
	  exit(EXIT_FAILURE);
     }
     
     
     if ((chdir("/")) < 0) {
	 
	  exit(EXIT_FAILURE);
     }

     close(STDIN_FILENO);
     close(STDOUT_FILENO);
     close(STDERR_FILENO);
     
     while(1)
     {
	  fs.refresh();
	 
	  sleep(CHECK_INTERVAL_SECONDS);
     }
     
     
     return 0;
}