/*
 * SIGMA RIZZLER
 *
 * A simple turn-based game about Sigma and Rizz
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct{
    char username[100];
    int health;
    int score;
}Person;

typedef struct{
    char name[100];
    int health;
}Enemy;

int isvulnurable = 0;
int abundant = 0;
int poison = 1;
int poison_counter = 0;


int gacha(){
    srand(time(NULL));

    int random = (rand() % 10) + 1;
    return random;
}
int gacha_vulnuraability(){
    srand(time(NULL));

    int random = (rand() % 7) + 4;
    return random;
}

int gacha_poison(){
    srand(time(NULL));

    int random = (rand() % 7) + 2;
    return random;
}


int buff(){
    int random = gacha();
    if (random >= 5){
        srand (time(NULL));
        int buff = (rand() % 6) + 5;

        return buff;
        printf("The arrow of the hunt has assisted you with a buff of %d\n", buff);

    }


}
int pregame_announcement(int turn){
    if (turn >= 3){
        printf("Both buff and debuffs for you and the enemy are active! Push on with care!\n");
        srand (time(NULL));
        int notice = gacha();
        if (notice <= 3){
            printf("Enemy is vulnurable for this turn! Bring in the Damage!\n");
            isvulnurable = 1;
        }
        else if(notice > 3 && notice <= 6){
            printf("Goddess of Abundance have blessed you! Healing now has improved for this turn!\n");
            abundant = 1;
        }
        else if(notice >= 7 && notice <= 9){
            printf("The enemy have been afflicted by poison! They will take damage overtime for 3 turns\n");
            poison = 1;
            poison_counter = 3;

        }
    }

}

void apply_poison(int *hp) {
    if (poison_counter > 0 && poison == 1) {
        int poison_damage = gacha_poison();
        *hp -= poison_damage;
        printf("Poison damage of %d applied! Remaining poison turns: %d\n", poison_damage, poison_counter);
        poison_counter--;
    }
    else {
        poison = 0;
    }
}





void attack(int *hp, int turn){
    srand(time(NULL));

    int random = (rand() % 16) + 5;
     int atk = random;
    if (turn < 3){
    *hp -= atk;
    }
    else {
        int buff_atk = buff(atk) + atk;
        if(isvulnurable == 1){
            buff_atk += gacha_vulnuraability();
            isvulnurable = 0;
        }
        *hp -= buff_atk;

    }
}

int gacha_buffheal(){
    srand (time(NULL));
    int random = (rand() % 11) + 10;
    return random;
}

void heal (int *hp){
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;
    if (abundant == 1){
        heal += gacha_buffheal();
        if (poison == 1){
            poison == 0;
            printf("Poison has been cured!\n");

        }
    }
    *hp += heal;
}
void take_turn(int *hp, int turn) {
    apply_poison(hp);
    pregame_announcement(turn); // Check for any new effects or buffs

}




void printAuthentication()
{

}

void authRegister()
{

}
void printLogo()
{
    const char* LOGO = "________  ______  ________  ___        ___       ___      \n"
                       "__          __    __        ___ __  __ ___     __   __    \n"
                       "________    __    __   ___  ___  _  _  ___    __ ___ __   \n"
                       "      __    __    __    __  ___   __   ___   __       __  \n"
                       "________  ______  ________  ___        ___  __         __ \n";

    printf(LOGO);
}

int main()
{
    printLogo();

    return 0;
}
