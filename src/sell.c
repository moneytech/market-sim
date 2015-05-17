#include "globals.h"

void sell_handler(struct player_t *player)
{
    printf("Enter the ticker symbol of the stock you wish to sell: ");
    char *sym = read_ticker();

    printf("Getting stock information...\n");

    struct stock_t *stock = NULL;

    stock = find_stock(player, sym);

    if(!stock)
    {
        printf("Couldn't find '%s' in portfolio.\n", sym);
        free(sym);
        return;
    }

    free(sym);

    printf("Updating prices...\n");

    get_stock_info(stock->symbol, &stock->current_price, &stock->fullname);

    printf("You currently own %llu share(s) of '%s' (%s) valued at $%llu.%02llu each.\n",
           stock->count, stock->fullname, stock->symbol, stock->current_price.cents / 100, stock->current_price.cents % 100);

    printf("How many shares do you wish to sell? ");
    ullong sell_count = read_int();

    if(!sell_count)
    {
        printf("Sale cancelled.\n");
        return;
    }

    if(stock->count < sell_count)
    {
        printf("You don't own enough shares!\n");
        return;
    }

    ullong sell_total = stock->current_price.cents * sell_count;

    printf("This will sell %llu shares for $%llu.%02llu total.\nProceed? ", sell_count, sell_total / 100, sell_total % 100);

    char *response = read_string();

    if(response[0] == 'y')
    {
        stock->count -= sell_count;

#if 0
        if(stock->count == 0)
        {
            /* remove this item from the portfolio */
            memmove(player->portfolio + stock_idx, player->portfolio + stock_idx + 1, sizeof(struct stock_t) * (player->portfolio_len - stock_idx - 1));
            player->portfolio_len -= 1;
            player->portfolio = realloc(player->portfolio, sizeof(struct stock_t) * player->portfolio_len);
        }
#endif

        player->cash.cents += sell_total;

        add_hist(stock, SELL, sell_count);

        printf("%llu shares sold for $%llu.%02llu total.\n", sell_count, sell_total / 100, sell_total % 100);
    }
    else
    {
        printf("Not confirmed.\n");
    }

    free(response);
}
