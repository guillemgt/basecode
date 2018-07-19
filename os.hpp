#ifndef engine_os_h
#define engine_os_h
#include "basecode.hpp"

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#if OS == OS_WINDOWS
#include "os_windows.hpp"
#elif OS == OS_MAC
#include "os_osx.hpp"
#elif OS == OS_IOS
#include "os_ios.hpp"
#elif OS == OS_WASM
#include "os_wasm.hpp"
#endif

//struct OsFile;
typedef FILE* OsFile;
typedef u64 OsTime;

OsFile open_game_file(const char *name, const char *mode);
OsFile open_user_file(const char *name, const char *mode);
OsFile open_game_file(String name, const char *mode);
OsFile open_user_file(String name, const char *mode);
OsFile open_file(const char *name, const char *mode);
void get_game_file_path(const char *name, char *dst);
void get_user_file_path(const char *name, char *dst);
void read_file(void *dest, unsigned int size, unsigned int amount, OsFile fp);
void write_file(void *dest, unsigned int size, unsigned int amount, OsFile fp);
void close_file(OsFile);

/*Array<const char *> filesInGameDirectory(const char *);
void renameGameDirectory(const char *src, const char *dst);
void createGameDirectory(const char *name);
Array<const char *> filesInUserDirectory(const char *);
void renameUserDirectory(const char *src, const char *dst);
void createUserDirectory(const char *name);*/

OsTime file_modification_date(const char *path);

#endif
