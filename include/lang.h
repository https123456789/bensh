#ifndef _BENSH_LANG_H_
#define _BENSH_LANG_H_

struct lang_token {
};

int lang_parse_line(struct lang_token** token_buf, char* src);

#endif
