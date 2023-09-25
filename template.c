/*
 * A simple C template
 * Copyright (C) 2023  Sabrina Andersen (NukingDragons)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <error.h>

// Name of the program if argv[0] fails
#define NAME "template"

struct args_t
{
	// Options
	uint8_t help:1;
};

void usage(char *name, FILE *f)
{
	// Set the name for the usage prompt
	char *n = NAME;
	if (name
	&&  strlen(name))
		n = name;

	fprintf(f,
			"Usage: %s [options]\n"
			"Options:\n"
			"\t--help,-h\t\tDisplay this help\n"
			,
			n);
}

struct args_t parse_args(int32_t argc, char **argv, int32_t min_args)
{
	set_errno(ESUCCESS);

	struct args_t args;
	memset(&args, 0, sizeof(struct args_t));

	// Ensure arguments provided and that the minimum are provided
	// argc should be set to at least 1 for the program name
	if (argc < (min_args + 1))
		set_errno(ETOOFEW);

	if (!errno
	&&  argv)
	{
		// Scan for arguments
		// Start at 1 so that the name of the program is not a false positive
		for (int32_t i = 1; i < argc; i++)
		{
#undef check_arg
#define check_arg(full, small)		  (!strncmp(full, argv[i], strlen(full)) || !strncmp(small, argv[i], strlen(small)))
			if      (check_arg("--help", "-h"))
				args.help = 1;
			else
				set_errno(EUNKARG);
#undef check_arg

			if (errno || args.help)
				break;
		}
	}

	return args;
}

int32_t main(int32_t argc, char **argv)
{
	int32_t r = 0;
	struct args_t args = parse_args(argc, argv, 1);

	if (!errno)
	{
		if (args.help)
			usage(argv[0], stdout);
	}
	else
	{
		fprintf(stderr, "Error: %s\n", errorstr(errno));

		if (argv)
			usage(argv[0], stderr);
		else
			usage(0, stderr);

		r = 1;
	}

	return r;
}
