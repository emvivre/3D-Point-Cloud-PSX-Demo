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

int main(int argc, char** argv)
{
	if ( argc < 2 ) {
		printf("Usage: %s <PSX_EXE>\n", argv[0]);
		return 1;
	}
	int i = 0;
	const char* psxexe = argv[++i];
	char buffer[1024];
	getcwd(buffer, sizeof buffer);
	strcat(buffer, "\\");
	char* psxexe_absolute = strcat(buffer, psxexe);
	printf("Loading %s ...\n", psxexe_absolute);
	chdir("C:\\Documents and Settings\\coco\\My Documents\\ePSXe190");	
	const char* args[4] = { "ePSXe.exe", "-nogui", psxexe_absolute, NULL };
	_execv(args[0], args);
	return 0;
}
