int checkInternal (char *cmd, char **args);
int cd (char *cmd, char **args);
void pwd (char *cmd);
void addToPath(char *cwd);
char **redirectionCheck(char **oldArgs);
