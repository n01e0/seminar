#define _GNU_SOURCE

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tool.h"
#include "sish_function.h"

static char **env;

int launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvpe(args[0], args, env) == -1)
            perror("exec");
    } else if (pid < 0) {
        perror("fork error");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int execute(char **args) {
    if (args[0] ==NULL)
        return 1;

    for (int i = 0; i < cmd_num(); i++) {
        if (strmatch(args[0], cmd_str[i]))
            return (*cmd_fn[i])(args);
    }
    return launch(args);
}

#define RL_BUFSIZE 1024

char *readln() {
    int bufsize = RL_BUFSIZE;
    int pos = 0;
    char *buf = malloc(sizeof(char) * RL_BUFSIZE);
    int c;

    if (!buf) {
        eprint("malloc in reading line");
    }

    while (1) {
        c = getchar();

        if (c == EOF) {
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buf[pos] = '\0';
            return buf;
        } else {
            buf[pos] = c;
        }
        pos++;

        if (pos >= RL_BUFSIZE) {
            bufsize += RL_BUFSIZE;
            buf = realloc(buf, bufsize);
            if (!buf) {
                eprint("realloc in reading line");
            }
        }
    }
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char **cmd_tokenize(char *line) {
    int bufsize = TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_bk;

    if (!tokens) {
        eprint("malloc in tokenize");
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;

        if (pos >= bufsize) {
                bufsize += TOK_BUFSIZE;
                tokens_bk = tokens;
                tokens = realloc(tokens, bufsize * sizeof(char ));
                if (!tokens) {
                    free(tokens_bk);
                    eprint("realloc in tokenize");
                }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
}

void loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = readln();
        args = cmd_tokenize(line);
        status = execute(args);

        free(line);
        free(args);
    } while(status);
}

int main(int argc, const char **argv, char **envp) {
    env = envp;
    loop();
    return EXIT_SUCCESS;
}
