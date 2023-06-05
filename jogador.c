#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

struct Jogador
{
	int x;
	int y;
	int hp;
	int visao;
	int visao_total;
};

struct Torre
{
	int x;
	int y;
	int hp;
};

int dano_torre(){
	srand(time(0));
	int a = rand()%200;
	if(a < 120) return 1;
	return 0;
}

int dano_jogador(){
	srand(time(0));
	int a = rand()%10;
	if(a < 3) return 1;
	return 0;
}

void inicializa_jogador(struct Jogador *jogador){
	jogador->hp = 10;
	jogador->visao = 15;
	jogador->visao_total = FALSE;
}

void inicializa_torre(struct Torre *torre){
	torre->hp = 10;
}