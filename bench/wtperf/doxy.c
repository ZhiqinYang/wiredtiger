/*-
 * Public Domain 2008-2014 WiredTiger, Inc.
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdio.h>

#define	DEF_OPT_AS_BOOL(name, initval, desc)				\
	{ #name, desc, #initval, BOOL_TYPE },
#define	DEF_OPT_AS_CONFIG_STRING(name, initval, desc)			\
	{ #name, desc, initval, CONFIG_STRING_TYPE },
#define	DEF_OPT_AS_STRING(name, initval, desc)				\
	{ #name, desc, initval, STRING_TYPE },
#define	DEF_OPT_AS_UINT32(name, initval, desc)				\
	{ #name, desc, #initval, UINT32_TYPE },

typedef enum {
	BOOL_TYPE, CONFIG_STRING_TYPE, INT_TYPE, STRING_TYPE, UINT32_TYPE
} CONFIG_OPT_TYPE;

static struct __config {
	const char *name;
	const char *description;
	const char *defaultval;
	CONFIG_OPT_TYPE type;
} const config_opts[] = {
#include "wtperf_opt.i"
};

/*
 * pretty_print --
 *	Print out lines of text for a 80 character window.
 */
static void
pretty_print(const char *p, const char *indent)
{
	const char *t;

	for (;; p = t + 1) {
		if (strlen(p) <= 70)
			break;
		for (t = p + 70; t > p && *t != ' '; --t)
			;
		if (t == p)			/* No spaces? */
			break;
		printf("%s%.*s\n",
		    indent == NULL ? "" : indent, (int)(t - p), p);
	}
	if (*p != '\0')
		printf("%s%s\n", indent == NULL ? "" : indent, p);
}

/*
 * config_doxygen --
 *	Output the configuration information for doxgen.
 */
static void
config_doxygen(void)
{
	size_t i, nopt;
	const char *defaultval, *typestr;

	nopt = sizeof(config_opts)/sizeof(config_opts[0]);
	for (i = 0; i < nopt; i++) {
		defaultval = config_opts[i].defaultval;
		typestr = "string";
		switch (config_opts[i].type) {
		case BOOL_TYPE:
			typestr = "boolean";
			if (strcmp(defaultval, "0") == 0)
				defaultval = "false";
			else
				defaultval = "true";
			break;
		case CONFIG_STRING_TYPE:
		case STRING_TYPE:
			break;
		case INT_TYPE:
			typestr = "int";
			break;
		case UINT32_TYPE:
			typestr = "unsigned int";
			break;
		}
		printf("@par %s (%s, default=%s)\n",
		    config_opts[i].name, typestr, defaultval);
		pretty_print(config_opts[i].description, NULL);
	}
}

/*
 * config_doxygen --
 *	A standalone program to output the configuration options in a doxygen
 * format.
 */
int
main()
{
	config_doxygen();
	return (0);
}