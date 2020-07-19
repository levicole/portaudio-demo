#include "repl.h"

int start_repl(const char *prompt, repl_cb cb, void *userData)
{
    char *line;
    repl_state *state;
    if((state = malloc(sizeof(repl_state))) == NULL) {
        printf("Couldn't start repl");
        return 0;
    }
    state->should_exit=0;
    while(!state->should_exit)
    {
        if((line = readline(prompt)) == NULL)
        {
            return 0;
        }

        state->last_line = line;
        cb(state, userData);
        free(line);

        if (state->should_exit)
        {
            free(state);
            break;
        }
        
    }
    return 1;
}