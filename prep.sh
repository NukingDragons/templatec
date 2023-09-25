#!/bin/bash

# A simple C template
# Copyright (C) 2023  Sabrina Andersen (NukingDragons)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

if [[ $# != 1 ]]
then
	echo "Usage: $0 <name of new repository>"
	exit 1
fi

rm -rf .git custom-errno
git init
sed -i "s/template/$1/g" Makefile
sed -i "s/template/$1/g" template.c
mv template.c $1.c
git submodule add https://github.com/nukingdragons/custom-errno.git custom-errno
git submodule update --init --recursive --remote
git add -A .
git rm --cached prep.sh
git commit -m "Initial Commit"
rm -rf prep.sh
