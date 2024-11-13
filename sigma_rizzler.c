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

void printAuthentication()
{

}

void authRegister()
{

}
void printLogo()
{
    printf("________  ______  ________  ___        ___       ___\n");
    printf("__          __    __        ___ __  __ ___     __   __\n");
    printf("________    __    __   ___  ___  _  _  ___    __ ___ __\n");
    printf("      __    __    __    __  ___   __   ___   __       __\n");
    printf("________  ______  ________  ___        ___  __         __\n");
}

int main()
{   
    printLogo();
    return 0;
}   