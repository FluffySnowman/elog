#include "elog.h"

int main(void) {

    // testing after adding macros
    // elog_init("master_log.retard", "READABLE,FILECOLOR");
    elog_init("master_log.retard", "TEA,FILECOLOR");
    ELOG_WARN("warning with macros");

    // elog_init(NULL, NULL);
    // elog(LOG_INFO, "this should be in stdou by default?");
    // elog(LOG_WARN, "test");

    // elog_init("bri_ish.log", "TEA");
    // elog(LOG_DEBUG, "this is a debug");
    // elog(LOG_INFO, "this is info");
    // elog(LOG_WARN, "this isnt a warning because we are british and hence the warning itself");
    // elog(LOG_ERROR, "imagine having errors lol");

    // elog_init("us_cheezburger.log", "BURGER,FILECOLOR");
    // elog(LOG_WARN, "should be us with ansi colours written to the file + stdout");

    // elog_init("readable.log", "READABLE");
    // elog(LOG_INFO, "actually readable timestamps and shit. way easier than the others yeet");
    // elog(LOG_ERROR, "imagine having errors lol");

    return 0;
}

// int main(void) {
//     logger_init(NULL, NULL);
//     log_log(LOG_INFO, "this should be in stdou by default?");

//     logger_init("bri_ish.log", "TEA");
//     log_log(LOG_DEBUG, "this is a debug");
//     log_log(LOG_INFO, "this is info");
//     log_log(LOG_WARN, "this isnt a warning because we are british and hence the warning itself");
//     log_log(LOG_ERROR, "imagine having errors lol");

//     logger_init("us_cheezburger.log", "BURGER,FILECOLOR");
//     log_log(LOG_WARN, "should be us with ansi colours written to the file + stdout");

//     logger_init("readable.log", "READABLE");
//     log_log(LOG_ERROR, "actually readable timestamps and shit. way easier than the others yeet");

//     return 0;
// }
