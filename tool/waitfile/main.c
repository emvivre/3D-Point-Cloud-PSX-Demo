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

#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	if ( argc < 2 ) {
		printf("Usage: %s <FILE_TO_WAIT>\n", argv[0]);
		return 1;
	}
	const char* file = argv[1];
	while ( open(file, O_RDONLY) < 0 );
	return 0;
}
