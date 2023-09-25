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

CC := gcc

CFLAGS := -O2
_CFLAGS := -Iinclude -Icustom-errno/include

SRCS = custom-errno/error.c \
	   template.c

# Target based rules

.PHONY: all clean template

all: template

clean:
	find . -name "*.o" -exec rm {} \; || true
	rm -rf template 2>/dev/null || true

# File based rules

custom-errno/error.c:
	git submodule update --init --recursive --remote

template: $(SRCS:.c=.o)
	$(CC) $(_CLFAGS) $(CFLAGS) $^ -o $@

# Glob based rules

%.o: %.c
	$(CC) $(_CFLAGS) $(CFLAGS) -c $^ -o $@
