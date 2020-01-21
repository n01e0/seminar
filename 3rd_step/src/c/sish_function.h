int sish_exit(char **args) {
    return 0;
}

int sish_cd(char **args) {
    if (chdir((args[1] != NULL? args[1]: getenv("HOME"))) != 0)
        eprint("cd");
    return 1;
}

char *cmd_str[] = {
    "exit",
    "cd"
};

int (*cmd_fn[]) (char**) =  {
    &sish_exit,
    &sish_cd
};

int cmd_num() {
    return sizeof(cmd_str) / sizeof(char *);
}

