#include "basecode.hpp"
#include "os.hpp"
#include "os_wasm.hpp"

// File reading & writing

OsFile open_game_file(const char *name, const char *mode){
    char dst[MAX_PATH_LENGTH];
    sprintf(dst, "../run_tree/%s", name);
    FILE *fp = fopen(dst, mode);
    return fp;
}
OsFile open_user_file(const char *name, const char *mode){
    char dst[MAX_PATH_LENGTH];
    sprintf(dst, "../run_tree/%s", name);
    FILE *fp = fopen(dst, mode);
    return fp;
}

/*OsFile open_game_file(String name, const char *mode){
    return openGameFile((char *)name.text, mode);
}
OsFile open_user_file(String name, const char *mode){
    return openUserFile((char *)name.text, mode);
}*/

void get_game_file_path(const char *name, char *dst){
    sprintf(dst, "../run_tree/%s", name);
}
void get_user_file_path(const char *name, char *dst){
    sprintf(dst, "../run_tree/%s", name);
}

void read_file(void *dest, unsigned int size, unsigned int amount, OsFile fp){
    fread(dest, size, amount, fp);
}
void write_file(void *dest, unsigned int size, unsigned int amount, OsFile fp){
    fwrite(dest, size, amount, fp);
}
void close_file(OsFile fp){
    fclose(fp);
}
