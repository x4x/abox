#ifndef APPLETS_H
#define APPLETS_H

int cat_main(int argc, char **argv);
int echo_main(int argc, char **argv);
int ls_main(int argc, char **argv);
int true_main(int argc, char **argv);
int false_main(int argc, char **argv);
int rm_main(int argc, char **argv);
int touch_main(int argc, char **argv);
int mkdir_main(int argc, char **argv); 
int pwd_main(int argc, char **argv);
int cp_main(int argc, char **argv);
int wc_main(int argc, char **argv);

struct applet {
    const char *name;
    int (*mainfunc)(int argc, char **argv);
};

static struct applet applets[] = {
    {"cat", cat_main},
    {"echo", echo_main},   
    {"touch", touch_main},
    {"ls", ls_main},
    {"true", true_main},
    {"false", false_main},
    {"rm", rm_main},
    {"mkdir", mkdir_main},
    {"pwd", pwd_main},
    {"cp", cp_main},
    {"wc", wc_main},
    {NULL, NULL}
};
#endif // APPLETS_H