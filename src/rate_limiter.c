#include <stdio.h>
#include <time.h>

#define MAX_BYTES_PER_SECOND 1000

typedef struct{
    time_t last_time;
    size_t bytes_count;
} rate_limiter_t;

int check_rate_limit(rate_limiter_t *limiter){
    time_t now = time(NULL);
    if(difftime(now, limiter->last_time)>=1){
        limiter->bytes_count = 0;
        limiter->last_time = now;
    }
    if (limiter->bytes_count + 100 <= MAX_BYTES_PER_SECOND){
        limiter->bytes_count += 100;
        return 1;
    }
    return 0;
}
