#ifndef REPL_H
#define REPL_H

#include <readline/readline.h>
#include <stdlib.h>

typedef struct {
    char *last_line;
    int should_exit;
} repl_state;

// callback function typedef:
// is called after every entry
typedef void (*repl_cb)(repl_state *state, void *userData);
int start_repl(const char *prompt, repl_cb cb, void *userData);

#endif