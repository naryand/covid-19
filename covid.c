#include <string.h>
#include <stdio.h>
#include <math.h>
#define DAYS 365

// C implementation of covid.py, used for regression on to data points to get parameters
// Mostly because this is why faster than the python implementation

typedef struct country {
    char country[10];
    float suscept;
    float infect;
    float immune;
} country;

void spread(float factor, struct country *world) {
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
}

void recover(float factor, struct country *world, float days) {
    if(world->suscept == 0) {
        return;
    }
    float recovered = world->infect/(pow(factor, days));
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
    struct country world;
    strcpy(world.country, "world");
    // regression initial conditions for start
    int populn = 7800000; // world pop.
    float factor = 1.189060; // init growth factor
    float days = 14.451526; // init infectious period
    int infect = 1; // init infections
    int recovd = 0; // init recoveries
    int length = 47; // length of each simulation in days

    // regression targets
    float infend = 53.3;
    float immend = 73;

    // margin between result and target
    float margin = 20;
    // step of parameters after each loop
    float step = 0.001;

    while(1) {
        // init world struct
        world.infect =  (float) infect;
        world.immune = (float) recovd;
        world.suscept = (float) populn-infect-recovd;

        // complete a simulation over the length
        for(int i = 0; i < length; i++) {
            spread(factor, &world);
            recover(factor, &world, days);
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
        }
        // break if parameters all within margin
        if(world.infect <= infend+margin && world.infect >= infend-margin && world.immune <= immend+margin && world.immune >= immend-margin) {
            break;
        }
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

    // mostly same as in covid.py
    int infect = 46;
    int recovd = 68;
    int populn = 7800000;
    float factor = 1.15;
    int days = 14;
    int length = DAYS;

    struct country world;
    strcpy(world.country, "world");
    world.suscept = (float) populn-infect-recovd;
    world.infect =  (float) infect;
    world.immune = (float) recovd;

    int aggr[DAYS]; // DAYS defined up top
    for(int i = 1; i < length+1; i++) {
        spread(factor, &world);
        recover(factor, &world, days);
        aggr[i] = world.infect+world.immune;
        //printf("Day: %d Susceptible: %.3f Infected: %.3f Immune: %.4f Cumulative: %.3f\n", i, world.suscept, world.infect, world.immune, world.infect+world.immune);
    }
    return 0;
}
