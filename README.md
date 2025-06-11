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

```c title="elog.h"
{%include "elog.h" %}
```



You can also install it by downloading the file directly into your project
or directory of choice.

With Curl:

```bash
curl -o elog.h https://raw.githubusercontent.com/fluffysnnowman/elog/master/elog.h
```

Wtih Wget:

```bash
wget https://raw.githubusercontent.com/fluffysnnowman/elog/master/elog.h
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



