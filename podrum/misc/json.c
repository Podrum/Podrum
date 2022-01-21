/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./json.h"
#include <string.h>

char *some_json = "{\"test\" : \"hii\"}";

void test() {
	int i;
	int open_brace_count = 0;
	int open_bracket_count = 0;
	int open_quotes_count = 0;
	int ignore = 0;
	int esc_code_size = 0;
	for (i = 0; i < strlen(some_json); ++i) {
		switch (some_json[i]) {
		case '{':
			++open_brace_count;
			break;
		case '}':
			--open_brace_count;
			break;
		case '[':
			++open_bracket_count;
			break;
		case ']':
			--open_bracket_count;
			break;
		case '\"':
			if (ignore == 0) {
				if (open_quotes_count == 0) {
					open_quotes_count = 1;
				} else {
					open_quotes_count = 0;
				}
			} else {
				ignore = 1;
			}
			break;
		case 'n':
			break;
		}
	}
}