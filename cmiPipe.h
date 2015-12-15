int pipeCheck(char **args);

char **getFirstCmd(char **oldArgs, int pipeIndex);

char **getSecondCmd(char **oldArgs, int pipeIndex);

int getPipeIndex(char **oldArgs);

void pipeChildExec(char **firstCmd, char **secondCmd);

void pipeExec(char **firstCmd, char **secondCmd);
