#include <pcq.h>

#include "lang.h"

int lang_parse_line(struct lang_token** token_buf, char* src) {
    pcq_parser_t *Executable = pcq_new("executable");

    pcqa_lang(PCQA_LANG_DEFAULT,
        " executable: /[a-zA-Z\\/]+/; ",
        Executable, NULL);

    pcq_result_t r;

    if (pcq_parse("input", src, Executable, &r)) {
        pcq_ast_print(r.output);
        pcq_ast_delete(r.output);
    } else {
        pcq_err_print(r.error);
        pcq_err_delete(r.error);
    }

    pcq_cleanup(1, Executable);

    return -1;
}
