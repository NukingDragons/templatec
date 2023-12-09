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
#include <cpwn/tcpconnect.h>
#include <cpwn/recvuntil.h>

// Name of the program if argv[0] fails
#define NAME "template"

struct args_t
{
	// Options
	uint8_t help:1;

	char *host;
	char *port;
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
			"\t--host,-H\t\tThe hostname or IP of the target\n"
			"\t--port,-p\t\tThe port of the target\n"
			,
			n);
}

struct args_t parse_args(int32_t argc, char **argv, int32_t min_args)
{
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
			else if (check_arg("--host", "-H"))
			{
				if (argc >= i + 1)
					args.host = argv[++i];
				else
					set_errno(EINVAL);
			}
			else if (check_arg("--port", "-p"))
			{
				if (argc >= i + 1)
					args.port = argv[++i];
				else
					set_errno(EINVAL);
			}
			else
				set_errno(EUNKARG);
#undef check_arg

			if (errno || args.help)
				break;
		}
	}

	return args;
}

void send_payload(int sock, void *payload, uint64_t payload_len)
{
	if (sock > -1 && payload && payload_len)
	{
		// Overflow to RIP
		uint64_t overflow = 0;

		// Size of the buffer on the other end
		uint64_t full_payload_len = 0;

		uint8_t *full_payload = malloc(full_payload_len);

		if (full_payload)
		{
			// Fill the buffer with A's
			memset(full_payload, 0x41, full_payload_len);

			// Don't overflow ourselves
			if (payload_len > (full_payload_len - overflow))
				payload_len = (full_payload_len - overflow);

			// Apply the payload
			memcpy(&full_payload[overflow], payload, payload_len);

			// Send it
			write(sock, full_payload, full_payload_len);

			// Remove the garbage that may or may not result from sending the payload
			recvuntil(sock, "i got rekt> ", 12);

			free(full_payload);
		}
	}
	else
		set_errno(EINVAL);
}

void exploit(char *host, char *port)
{
	if (host && port)
	{
		int sock = tcpconnect(host, port);

		if (sock)
		{
			// Do the exploit :3
			uint64_t payload[10];
			memset(payload, 0, sizeof(payload));
			payload[0] = 0xDEADBEEFCAFEBABE;		// RIP

			send_payload(sock, payload, sizeof(payload));

			// Print whatever comes over the socket until it closes
			char c = 0;
			while(!errno)
			{
				if (read(sock, &c, 1) <= 0)
					break;
				putchar(c);
				fflush(stdout);
			}

			close(sock);
		}
	}
	else
		set_errno(EINVAL);
}

int32_t main(int32_t argc, char **argv)
{
	int32_t r = 0;
	struct args_t args = parse_args(argc, argv, 4);

	if (!errno)
	{
		if (args.help)
			usage(argv[0], stdout);
		else
			exploit(args.host, args.port);

		if (errno)
		{
			fprintf(stderr, "Error: %s\n", errorstr(errno));
			r = 1;
		}
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
