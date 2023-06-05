#include "jogador.c"

#define CIMA 65
#define BAIXO 66
#define DIREITA 67
#define ESQUERDA 68
#define ENTER 13
#define ESC 27

struct Mapa
{
	char caracter;
	int visivel;
	int personagem;
	int mob;
};

void gerar_mapa(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO]){
	srand(time(0));
	int x_aleatorio, y_aleatorio;
	int area = ALTURA * COMPRIMENTO;
	int area_paredes = 0.5 * area;

	// setar as paredes:
	for(int i = 0; i < area_paredes; i++){
		x_aleatorio = rand()%COMPRIMENTO;
		y_aleatorio = rand()%ALTURA;
		if(mapa[y_aleatorio][x_aleatorio].caracter == '#'){
			i--;
		} else {
			mapa[y_aleatorio][x_aleatorio].caracter = '#';
		}
	}

	// preencher os que nao sao paredes:
	for(int i = 0; i < ALTURA; i++){
		for(int j = 0; j < COMPRIMENTO; j++){
			if(mapa[i][j].caracter == '#'){
				;
			} else {
				mapa[i][j].caracter = '.';
			}
		}
	}
}

int paredes_perto(int ALTURA, int COMPRIMENTO,struct Mapa mapa[ALTURA][COMPRIMENTO], int x, int y, int raio){
	int r = 0;
	for(int i = raio; i >= 0; i--){
		for(int j = raio; j >= 0; j--){
			if(mapa[y-i][x-j].caracter == '#') r++;
			if(mapa[y+i][x+j].caracter == '#') r++;
		}
	}
	return r;

}

void ajeitar_paredes(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], int fst_passagem, int snd_passagem){
	//fst
	for(int k = 0; k < fst_passagem; k++){
		for(int i = 1; i < ALTURA-1; i++){
			for(int j = 1; j < COMPRIMENTO-1; j++){
				if(paredes_perto(ALTURA, COMPRIMENTO, mapa,j,i,1) >= 5 || paredes_perto(ALTURA, COMPRIMENTO, mapa,j,i,2) <= 2){
					mapa[i][j].caracter = '#';
				} else {
					mapa[i][j].caracter = '.';
				}
			}
		}
	}
	//snd
	for(int k = 0; k < snd_passagem; k++){
		for(int i = 1; i < ALTURA-1 ; i++){
			for(int j = 1; j < COMPRIMENTO-1; j++){
				if(paredes_perto(ALTURA, COMPRIMENTO, mapa, j, i, 1) >= 5 ){
					mapa[i][j].caracter = '#';
				} else {
					mapa[i][j].caracter = '.';
				}
			}
		}
	}

	// bordas como parede
	for(int i = 0; i < COMPRIMENTO; i++){
		mapa[0][i].caracter = '#';
		mapa[ALTURA-1][i].caracter = '#';
	}
	for(int i = 0; i < ALTURA; i++){
		mapa[i][0].caracter = '#';
		mapa[i][COMPRIMENTO-1].caracter = '#';
	}

}

void spawn_jogador(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], struct Jogador *jogador){
	srand(time(0));
	int x_aleatorio, y_aleatorio;
	x_aleatorio = rand()%COMPRIMENTO-1;
	y_aleatorio = rand()%ALTURA-1;
	while(paredes_perto(ALTURA,COMPRIMENTO, mapa, x_aleatorio, y_aleatorio, 1) > 0){
		x_aleatorio = rand()%COMPRIMENTO-1;
		y_aleatorio = rand()%ALTURA-1;
	}
	jogador->x = x_aleatorio;
	jogador->y = y_aleatorio;
}

void spawn_torre(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], struct Torre *torre,struct Jogador jogador){
	srand(time(0));
	int x_aleatorio, y_aleatorio;
	x_aleatorio = rand()%COMPRIMENTO-1;
	y_aleatorio = rand()%ALTURA-1;
	while(paredes_perto(ALTURA,COMPRIMENTO, mapa, x_aleatorio, y_aleatorio, 2) > 0 || (x_aleatorio == jogador.x && y_aleatorio == jogador.y)){
		x_aleatorio = rand()%COMPRIMENTO-1;
		y_aleatorio = rand()%ALTURA-1;
	}
	torre->x = x_aleatorio;
	torre->y = y_aleatorio;
}

void move_jogador(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], struct Jogador *jogador,struct Torre torre, char tecla){
	switch(tecla){
	case CIMA:
		if(mapa[jogador->y-1][jogador->x].caracter == '.'){
			if(jogador->y-1 != torre.y || jogador->x != torre.x) jogador->y--;
		}
		break;
	case BAIXO:
		if(mapa[jogador->y+1][jogador->x].caracter == '.'){
			if(jogador->y+1 != torre.y || jogador->x != torre.x) jogador->y++;
		}
		break;
	case DIREITA:
		if(mapa[jogador->y][jogador->x+1].caracter == '.'){
			if(jogador->y != torre.y || jogador->x+1 != torre.x) jogador->x++;
		}
		break;
	case ESQUERDA:
		if(mapa[jogador->y][jogador->x-1].caracter == '.'){
			if(jogador->y != torre.y || jogador->x-1 != torre.x) jogador->x--;
		}
		break;
	}
}

void ataque_jogador(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], struct Jogador jogador, struct Torre *torre, char tecla){
	switch(tecla){
	case 'a':
	case 'A':
		if(mapa[jogador.y][jogador.x-1].caracter == '#' && jogador.x-1 > 0){
			mapa[jogador.y][jogador.x-1].caracter = '.';
		} else if((torre->y == jogador.y) && torre->x == jogador.x-1){
			if(dano_torre()) torre->hp--;
		}
		break;
	case 'd':
	case 'D':
		if(mapa[jogador.y][jogador.x+1].caracter == '#' && jogador.x+1 < COMPRIMENTO-1){
			mapa[jogador.y][jogador.x+1].caracter = '.';
		} else if((torre->y == jogador.y) && torre->x == jogador.x+1){
			if(dano_torre()) torre->hp--;
		}
		break;
	case 's':
	case 'S':
		if(mapa[jogador.y+1][jogador.x].caracter == '#' && jogador.y+1 < ALTURA-1){
			mapa[jogador.y+1][jogador.x].caracter = '.';
		} else if(torre->y == jogador.y+1 && (torre->x == jogador.x)){
			if(dano_torre()) torre->hp--;
		}
		break;
	case 'w':
	case 'W':
		if(mapa[jogador.y-1][jogador.x].caracter == '#' && jogador.y-1 > 0){
			mapa[jogador.y-1][jogador.x].caracter = '.';
		} else if(torre->y == jogador.y-1 && (torre->x == jogador.x)){
			if(dano_torre()) torre->hp--;
		}
		break;
	}
}

void ataque_torre(struct Jogador *jogador, struct Torre torre){
	int dx[] = {-1,-1,-1,0,0,1,1,1};
	int dy[] = {-1,0,1,-1,1,-1,0,1};
	for(int i = 0; i < 8; i++){
		int nx = torre.x + dx[i];
		int ny = torre.y + dy[i];
		if(ny == jogador->y && nx == jogador->x){
			if(dano_jogador()) jogador->hp--;
		}
	}
}

void resetar_mapa(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO]){
	for(int i = 0; i < ALTURA; i++){
		for(int j = 0; j < COMPRIMENTO; j++){
			mapa[i][j].caracter = '\0';
		}
	}
}