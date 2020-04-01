#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define DAYS 14

// C implementation of covid.py, used for regression on to data points to get parameters
// Mostly because this is faster than the python implementation

typedef struct country {
    char country[10];
    double suscept;
    float infect;
    float immune;
} country;

int *fortnight;

void spread(float factor, country *world, float days, int *arr) {
    if(world->suscept == 0 || world->infect == 0) {
        return;
    }
    float infected = (factor-1)*world->infect*(1-((world->infect+world->immune)/world->suscept));
    world->suscept -= infected;
    if(world->suscept < 0) {
        world->infect += world->suscept;
        world->suscept = 0;
        return;
    }
    world->infect += infected;

    int rdays = (int) nearbyintf(days);
    for(int i = 1; i < rdays; i++) {
        arr[rdays-i] = arr[rdays-i-1];
        arr[0] = world->infect;
    }
}

void recover(float factor, country *world, float days, int *arr) {
    if(world->suscept == 0) {
        return;
    }
    float recovered;
    int rdays = (int) nearbyintf(days);
    if(arr[rdays-1] == -1) {
        recovered = world->infect/(pow(factor, rdays));
    } else {
        recovered = arr[rdays-1];
    }

    world->infect -= recovered;
    if(world->infect < 0) {
        world->immune += world->infect;
        world->infect = 0;
        return;
    }
    world->immune += recovered;
}

// Implement a regression algorithm to get growth factor and
// infectious period parameters from start and end data points.

typedef struct result {
    float factor;
    float days;
} result;

result regression() {
    float initfactor = 1.3;
    float initdays = 5;
    struct country world;
    strcpy(world.country, "world");
    // regression initial conditions for start
    int populn = 7800000; // world pop.
    float factor = initfactor; // init growth factor
    float days = initdays; // init infectious period
    int infect = 2; // init infections
    int recovd = 0; // init recoveries
    int length = 66; // length of each simulation in days

    // regression targets
    float infend = 634;
    float immend = 224;

    // margin between result and target
    float margin = 100;
    // step of parameters after each loop
    float step = 0.000001;

    while(1) {
        // init world struct
        world.infect =  (float) infect;
        world.immune = (float) recovd;
        world.suscept = (double) populn-infect-recovd;
        int rdays = (int) nearbyintf(days);
        fortnight = malloc(sizeof(int)*rdays);
        for(int i = 0; i < rdays; i++) {
            fortnight[i] = -1;
        } 

        // complete a simulation over the length
        for(int i = 0; i < length; i++) {
            spread(factor, &world, days, fortnight);
            recover(factor, &world, days, fortnight);
        }

        // difference between targets and results
        float infdiffer = infend-world.infect;
        float immdiffer = immend-world.immune;

        // adjust parameters accordingly if outside margin
        if(world.infect <= infend-margin || world.infect >= infend+margin) {
            factor += step*infdiffer;
        }
        if(world.immune >= immend+margin || world.immune <= immend-margin) {
            days += step*immdiffer;
            if(days < 1) {
                days = initdays;
            }
        }
        // break if parameters all within margin
        if(world.infect <= infend+margin && world.infect >= infend-margin && world.immune <= immend+margin && world.immune >= immend-margin) {
            break;
        }
        free(fortnight);
    }

    // return regression results
    result ret;
    ret.factor = factor;
    ret.days = days;
    return ret;
}


int main() {
    // do the regression
    result ret;
    ret = regression();
    printf("factor %.6f days %.6f\n", ret.factor, ret.days);

    // // mostly same as in covid.py
    // int infect = 85;
    // int recovd = 90;
    // int populn = 7800000;
    // float factor = 1.189060;
    // int days = 14.451526;
    // int length = DAYS;
    // int rdays = (int) nearbyintf(days);
    // fortnight = malloc(sizeof(int)*rdays);
    // for(int i = 0; i < rdays; i++) {
    //     fortnight[i] = -1;
    // } 

    // country world;
    // strcpy(world.country, "world");
    // world.suscept = (float) populn-infect-recovd;
    // world.infect =  (float) infect;
    // world.immune = (float) recovd;

    // int aggr[DAYS]; // DAYS defined up top
    // for(int i = 1; i < length+1; i++) {
    //     spread(factor, &world, days, fortnight);
    //     recover(factor, &world, days, fortnight);
    //     aggr[i] = world.infect+world.immune;
    //     printf("Day: %d Susceptible: %.3f Infected: %.3f Immune: %.4f Cumulative: %.3f\n", i, world.suscept, world.infect, world.immune, world.infect+world.immune);
    // }
    // free(fortnight);
    return 0;
}
