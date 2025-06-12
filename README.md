# ELog - Easy logging in C: All in one header file

This project a single header-only library for logging in C.

Logging here includes writing to stdout, stderr and to a file(s). This library
supports timestamps, log levels and many different log formats and timestamp
formats with colour options.

## Installation

Installation is as easy as copying the `elog.h` file to your project directory
and including it in your projejct.

```c
#include "elog.h"
```

The file can be copied from the dropdown below if you're too lazy to leave the
readme and click on the `elog.h` file about 2.5 mouse scrolls up from this line.

<details>
<summary>Expand to view & copy elog.h</summary>

<!-- read file into here  -->
<!-- cat ./elog.h -->

```c
#ifndef ELOG_H
#define ELOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
// #include <unistd.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef enum {
    TM_ORIG,
    TM_TEA,
    TM_BURGER,
    TM_READABLE
} TimeMode;

static FILE*    log_file   = NULL;
static TimeMode time_mode  = TM_ORIG;
static int      file_color = 0;
static int      file_only  = 0;

#define COLOR_DEBUG "\x1b[36m"
#define COLOR_INFO "\x1b[32m"
#define COLOR_WARN "\x1b[33m"
#define COLOR_ERROR "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

static const char* level_strs[]   = {"DBG", "INF", "WRN", "ERR"};
static const char* level_colors[] = {COLOR_DEBUG, COLOR_INFO, COLOR_WARN, COLOR_ERROR};

// compatibility with other c std's so this is `strdup` yeet
static char* string_duplicate(const char* s) {
    size_t len = strlen(s) + 1;
    char*  p   = malloc(len);
    if (p) {
        memcpy(p, s, len);
    }
    return p;
}

static inline int logger_init(const char* filename, const char* options) {
    if (filename && filename[0]) {
        log_file = fopen(filename, "a");
        if (!log_file) return -1;
    }
    time_mode  = TM_ORIG;
    file_color = 0;
    file_only  = 0;
    if (options) {
        char* opts = string_duplicate(options);
        char* tok  = strtok(opts, " ,");
        // clang can clang deez nuts with its shitty formatting shit
        // clang-format off
        while (tok) {
            if      (strcmp(tok, "TEA")      == 0) time_mode  = TM_TEA;
            else if (strcmp(tok, "BURGER")   == 0) time_mode  = TM_BURGER;
            else if (strcmp(tok, "READABLE") == 0) time_mode  = TM_READABLE;
            else if (strcmp(tok, "FILECOLOR")== 0) file_color = 1;
            else if (strcmp(tok, "FILEONLY") == 0) file_only  = 1;
            tok = strtok(NULL, " ,");
        }
        // clang-format on
        free(opts);
    }
    return 0;
}

static inline void log_log(LogLevel lvl, const char* fmt, ...) {

    // timespect deez nuts 😎
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    time_t    now = ts.tv_sec;
    struct tm tm;
    if (localtime(&now))
        tm = *localtime(&now);
    else
        memset(&tm, 0, sizeof tm);

    int ms = ts.tv_nsec / 1000000;

    char tbuf[64];
    switch (time_mode) {
        case TM_TEA:
            snprintf(tbuf, sizeof tbuf,
                     "%02d-%02d-%02d %02d:%02d:%02d.%03d",
                     tm.tm_mday, tm.tm_mon + 1, tm.tm_year % 100,
                     tm.tm_hour, tm.tm_min, tm.tm_sec, ms);
            break;

        case TM_BURGER:

            // snprintf(tbuf, sizeof tbuf,
            //          "\n%02d\t%02d\t%02d %02d:%02d",
            //          tm.tm_mon + 1, tm.tm_mday, tm.tm_year % 100,
            //          tm.tm_hour, tm.tm_min);

            snprintf(tbuf, sizeof tbuf,
                     "%02d-%02d-%02d %02d:%02d:%02d.%03d",
                     tm.tm_mon + 1, tm.tm_mday, tm.tm_year % 100,
                     tm.tm_hour, tm.tm_min, tm.tm_sec, ms);
            break;

        case TM_READABLE: {
            // why do months have shitty names
            static const char* mon_names[] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
            char part1[32], part2[16], part3[16];
            snprintf(part1, sizeof part1,
                     "%02d%s%04d",
                     tm.tm_mday, mon_names[tm.tm_mon], tm.tm_year + 1900);
            // snprintf(part2, sizeof part2,
            //          "%02d%02dh",
            //          tm.tm_hour, tm.tm_min);
            snprintf(part2, sizeof part2,
                     "%02d:%02d",
                     tm.tm_hour, tm.tm_min);
            // int ms = ts.tv_nsec / 1000000;
            snprintf(part3, sizeof part3,
                     "%02d.%03ds",
                     tm.tm_sec, ms);
            snprintf(tbuf, sizeof tbuf, "%s %s %s", part1, part2, part3);
            break;
        }
        case TM_ORIG:
        default:
            snprintf(tbuf, sizeof tbuf,
                     "%02d:%02d:%02d.%03d",
                     tm.tm_hour, tm.tm_min, tm.tm_sec, ms);
            break;
    }

    char    mbuf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(mbuf, sizeof mbuf, fmt, ap);
    va_end(ap);

    char final_msg[1152];

    // adding pid for debug (sub testing the thing from other programs testing
    // other sub testing programs with programs in programs in programs in programs in
    // programs .await().await()?.await()?.unwrap().unwrap().unwrap().unwrap().unwrap()?)

    // snprintf(final_msg, sizeof final_msg,
    //          "%s pid=%d", mbuf, (int)getpid());

    // removed pid cos no bedugging anymore
    snprintf(final_msg, sizeof final_msg,
             "%s", mbuf);

    if (!file_only) {
        fprintf(stdout,
                "%s %s%s%s %s\n",
                tbuf,
                level_colors[lvl], level_strs[lvl], COLOR_RESET,
                final_msg);
    }

    if (log_file) {
        if (file_color) {
            fprintf(log_file,
                    "%s %s%s%s %s\n",
                    tbuf,
                    level_colors[lvl], level_strs[lvl], COLOR_RESET,
                    final_msg);
        } else {
            fprintf(log_file,
                    "%s %s %s\n",
                    tbuf,
                    level_strs[lvl],
                    final_msg);
        }
        fflush(log_file);
    }
}

#define elog_init(filename, options) logger_init(filename, options)
#define elog(level, fmt, ...) log_log(level, fmt, ##__VA_ARGS__)

// macros so instead of `elog(LOG_WARN, "test");` just do `ELOG_WARN("test");`
// cos the vibes are just objectively better and 70% of all typers can save
// many days of their life not writing those extra characters.

#define ELOG_DEBUG(fmt, ...) elog(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define ELOG_INFO(fmt, ...) elog(LOG_INFO, fmt, ##__VA_ARGS__)
#define ELOG_WARN(fmt, ...) elog(LOG_WARN, fmt, ##__VA_ARGS__)
#define ELOG_ERROR(fmt, ...) elog(LOG_ERROR, fmt, ##__VA_ARGS__)

#endif
```


</details>


You can also install it by downloading the file directly into your project
or directory of choice.

With Curl:

```bash
curl -o elog.h https://raw.githubusercontent.com/FluffySnowman/elog/refs/heads/master/elog.h
```

Wtih Wget:

```bash
wget https://raw.githubusercontent.com/FluffySnowman/elog/refs/heads/master/elog.h
```

## Usage

Simply specify the log file and which format you want to use and then use the
logging functions (macros) to log messages to stdout and to a file.

```c
#include "elog.h"

int main() {

    // initilizes the logger with a file and the format. The list of formats can
    // be found below this code block

    // `TEA` is the log format for timestamps and `FILECOLOR` writes ansi codes
    // to the log file itself if you wish to `tail -f ...` your logs.
    //
    // If you don't wanna have ansi codes all over your log file, you can remove
    // the `,FILECOLOR` in the string and just use `TEA`. This works for all log
    // formats.
    elog_init("cool_log.log", "TEA,FILECOLOR");

    ELOG_INFO("this is an info log");
    ELOG_DEBUG("this is a debug log");
    ELOG_WARN("this is a warning log");
    ELOG_ERROR("this is an error log");
    return 0;
}
```

### Log formats

The following log formats are available:

- `READABLE` - Readable time format as `ddmmmyyyy HH:MM ss.sss`
- `TEA` - British time format as `DD-MM-YY HH:MM`
- `BURGER` - American time format as `MM-DD-YY HH:MM`
- The default log format is not specifiable by an option and is very basic and
  not recommended due to it only including hours and minutes with no date etc.

The `READABLE` format is recomended for most use cases/projects that don't
require a specific format and just want to have a human readable log
file/console.

### Options

#### Colours / Colors (*if you're in the divided states of america*)

If you wish to have ANSI codes written to the log file itself, use

- `,FILECOLOR` after the log format (include the comma) to enable it.

^ To disable- remove `,FILECOLOR` from the string.

#### File logging only

If you only want to log to a file and not to stdout, specify the

- `,FILEONLY` option in the string after the log format in the init function-
  AND IT MUST BE THE LAST OPTION IN THE STRING.

This will write to the file and nothing else. ANSI codes can be written to the
file too by specifying `,FILECOLOR` before it.

### Option Examples

To have colours (ANSI codes) in the log file, use:

```c
elog_init("master_log.log", "TEA,FILECOLOR");
```

To only log to a file and not to stdout use:

```c
elog_init("master_log.log", "TEA,FILEONLY");
```

To log to the file with colors and NOT to stdout use

```c
elog_init("master_log.log", "TEA,FILECOLOR,FILEONLY");
```


