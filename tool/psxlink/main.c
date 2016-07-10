/*
  ===========================================================================

  Copyright (C) 2014 Emvivre

  This file is part of PSXRUN.

  PSXRUN is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  PSXRUN is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with PSXRUN.  If not, see <http://www.gnu.org/licenses/>.

  ===========================================================================
*/

#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <dirent.h>
#include <stdlib.h>

int main()
{
	char buffer[1024];
	getcwd(buffer, sizeof buffer);
	DIR *d = opendir(buffer);
	struct dirent *s;
	const char* cmdline[255] = { "C:\\psyq\\bin\\ccpsx", "-Xo$80010000" };
	int i = 2;
	while((s = readdir(d))) {
		const char* f_name = s->d_name;
		int l = strlen(f_name);
		if ( l > 4 && strcmp(&f_name[l-4], ".obj") == 0 ) {
			char* f = (char*)malloc(l+1);
			strcpy(f, f_name);
			cmdline[i++] = f;
		}
	}
	if ( i == 2 ) return 1;
	cmdline[i++] = "-omain.cpe,main.sym,mem.map";
	cmdline[i] = NULL;
	_execv(cmdline[0], cmdline);
	return 0;
}
