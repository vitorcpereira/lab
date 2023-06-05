#include <ncurses.h>
#include "mapa.c"

void menu(int ALTURA, int COMPRIMENTO, char *sair){
	int comp_centro = COMPRIMENTO/2;
	int tamanhopal;
	int opcoes = 3;
	int selecionada = 0; int atual = 1;
	int altura_op = ALTURA/2 - 2;
	char c = '\0';
	char opcoestr[][20] = {"  Jogar","  Ajuda","  Sair"};
	char ajuda[][50] = {"Movimentacao:","Up arrow - cima","Down arrow - baixo","Right arrow - direita","Left arrow - esquerda","","Combate:","Jogador","W - ataque cima","S - ataque baixo","A - ataque esquerda","D - ataque direita","Torre","Ataca quando jogador esta a 1 de distancia","","Objetivo:","Elime a Torre em 5 fases diferentes"};
	while(selecionada != 1 && selecionada != 3){
		clear();
		for(int i = 0; i < opcoes; i++){
			tamanhopal = strlen(opcoestr[i]);
			move(altura_op + 2*i, comp_centro - (tamanhopal/2));
			printw("%s",opcoestr[i]);
		}
		tamanhopal = strlen(opcoestr[atual-1]);
		move(altura_op + 2*(atual-1), comp_centro - ((tamanhopal)/2));
		printw(">");
		move(altura_op + 2*(atual-1), comp_centro - ((tamanhopal)/2));
		refresh();

		c = getchar();
		switch(c){
		case CIMA:
			if(atual-1 > 0){
				atual--;
			}
			break;
		case BAIXO:
			if(atual+1 <= opcoes){
				atual++;
			}
			break;
		case ENTER:
			selecionada = atual;
			break;
		}

		while(selecionada == 2){
			clear();
			for(int i = 0; i < 17; i++){
				tamanhopal = strlen(ajuda[i]);
				move(5+2*i,comp_centro - (tamanhopal/2));
				printw("%s",ajuda[i]);
			}
			refresh();
			c = getchar();
			switch(c){
			case ESC:
				selecionada = 0;
				break;
			}
		}

	}
	if(selecionada == 3){
		*sair = 'q';
	}
}

void mostrar_stats(int ALTURA, int COMPRIMENTO, struct Jogador jogador,struct Torre torre, int fase){
	int hp = jogador.hp;
	int thp = torre.hp;
	move(ALTURA,0);
	printw("HP:");
	init_pair(1,COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	for(int i = 0; i < 10; i++){
		if(hp > 0){
			printw("o");
			hp--;
		} else if (hp == 0){
			attroff(COLOR_PAIR(1));
			printw("-");
			hp--;
		} else {
			printw("-");
		}
	}
	if(hp == 0){
		attroff(COLOR_PAIR(1));
	}

	printw("      Torre HP:");
	init_pair(1,COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	for(int i = 0; i < 10; i++){
		if(thp > 0){
			printw("o");
			thp--;
		} else if (thp == 0){
			attroff(COLOR_PAIR(1));
			printw("-");
			thp--;
		} else {
			printw("-");
		}
	}
	if(thp == 0){
		attroff(COLOR_PAIR(1));
	}

	move(ALTURA,COMPRIMENTO - 8);
	printw("fase: %d",fase);

}

void fase_completa(int ALTURA,int COMPRIMENTO, int fase){
	char frase[] = "fase x completa";
	int t = strlen(frase);
	clear();
	move(ALTURA/2,(COMPRIMENTO/2)-(t/2));
	printw("fase %d completa",fase);
	refresh();
	getchar();
	clear();
}

void game_over(int ALTURA, int COMPRIMENTO){
	char frase[] = "Game Over";
	int t = strlen(frase);
	clear();
	move(ALTURA/2,(COMPRIMENTO/2)-(t/2));
	printw("Game Over");
	refresh();
	getchar();
	clear();
}

void verificar_paredes_adjacentes(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], int x, int y){
	int dx[] = {-1,-1,-1,0,0,1,1,1};
	int dy[] = {-1,0,1,-1,1,-1,0,1};
	for(int i = 0; i < 8; i++){
		int nx = x + dx[i];
		int ny = y + dy[i];
		if(nx >= 0 && nx < ALTURA && ny >= 0 && ny < COMPRIMENTO && mapa[nx][ny].caracter == '#'){
			mvaddch(nx,ny,mapa[nx][ny].caracter);
		}
	}
}

void mostrar_mapa(int ALTURA, int COMPRIMENTO, struct Mapa mapa[ALTURA][COMPRIMENTO], struct Jogador jogador, struct Torre torre){
    bool parede_visivel;
    if(jogador.visao_total){
	  	for(int i=0;i<ALTURA;i++){
	    	for(int j=0;j<COMPRIMENTO;j++){
	      		mvaddch(i,j,mapa[i][j].caracter);
	      	}
	    }
	} else {    
	    for (int i = 0; i < ALTURA; i++) {
	        for (int j = 0; j < COMPRIMENTO; j++) {
	            parede_visivel = false;

	            for (int k = (i < jogador.y ? i : jogador.y); k <= (i < jogador.y ? jogador.y : i); k++) {
	                for (int l = (j < jogador.x ? j : jogador.x); l <= (j < jogador.x ? jogador.x : j); l++) {
	                    if (mapa[k][l].caracter == '#' || (torre.y == k && torre.x == l)) {
	                        parede_visivel = true;
	                        break;
	                    }
					}	
					if (parede_visivel) {break;}
	            }

	            if (sqrt(pow(j - jogador.x, 2) + pow(i - jogador.y, 2)) <= jogador.visao && !parede_visivel) {
                	mvaddch(i, j, mapa[i][j].caracter);
                	if(mapa[i][j].caracter == '.'){
                		verificar_paredes_adjacentes(ALTURA,COMPRIMENTO,mapa,i,j);
                	}
	            } else {
					mvaddch(i, j, ' ');
	            }
			}
		}	
	}

	mvaddch(torre.y,torre.x,'T');
  	mvaddch(jogador.y,jogador.x,'@');
  	move(jogador.y,jogador.x);
}