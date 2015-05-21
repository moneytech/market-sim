#ifndef _MARKET_SIM_H_
#define _MARKET_SIM_H_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))

/* VERSION_INFO is supplied by the compiler */
#define PROGRAM_VERSION "prerelease (" VERSION_INFO ")"

#define ARG_LOADED (1<<0)
#define ARG_FAILURE (1<<1)
#define ARG_VERBOSE (1<<2)

/* don't change this, it will corrupt existing saves */
#define EPOCH_YEAR 2000

typedef unsigned long long ullong;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

/* money is represented internally as cents */
struct money_t {
    ullong cents;
};

enum history_action { BUY = 0, SELL };

struct history_time {
    ushort year; /* since EPOCH_YEAR */
    uchar month; /* 0 = jan, 11 = dec */
    uchar day;
    uchar hour; /* 0-23 */
    uchar minute;
    uchar second;
};

struct history_item {
    enum history_action action;
    ullong count;
    struct money_t price;

    struct history_time action_time;

    struct history_item *next;
};

struct stock_t {
    char *symbol;
    char *fullname;
    ullong count;
    struct money_t current_price;

    struct history_item *history;

    uint history_len;
};

struct player_t {
    struct money_t cash;
    uint portfolio_len;
    struct stock_t *portfolio;
    bool need_to_free_portfolio;
};

struct command_t {
    const char *name;
    const char *command;
    void (*handler)(struct player_t*);
};

/*** prototypes ***/
void cleanup(void);
int compare_stocks(const void*, const void*);
void all_lower(char*);
void all_upper(char*);
bool get_stock_info(char *sym, struct money_t*, char **name);
uint64_t to_sys64(uint64_t);
uint64_t to_be64(uint64_t);
uint32_t to_sys32(uint32_t);
uint32_t to_be32(uint32_t);
uint16_t to_sys16(uint16_t);
uint16_t to_be16(uint16_t);
struct stock_t *find_stock(struct player_t*, char*);
void add_hist(struct stock_t*, enum history_action, ullong count);
void print_history(struct stock_t*);
char *read_ticker(void);
char *read_string(void);
ullong read_int(void);
void print_usage(int argc, char *argv[]);
void print_version(void);

uint parse_args(struct player_t*, int argc, char *argv[]);
char *csv_read(char**);
void load_portfolio(struct player_t*, const char*);

void buy_handler(struct player_t*);
void sell_handler(struct player_t*);
void info_handler(struct player_t*);
void update_handler(struct player_t*);
void save_handler(struct player_t*);
void load_handler(struct player_t*);
void quit_handler(struct player_t*);
void print_handler(struct player_t*);

#ifndef NDEBUG

void debug_handler(struct player_t*);
void debug_init(void);

#endif

void do_menu(struct player_t*, const struct command_t*, uint len, const char *prompt);

#endif
