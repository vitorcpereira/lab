#include <sys/ioctl.h>
#include <unistd.h>
#include "graficos.c"

int main(){
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    #define ALTURA w.ws_row-2
    #define COMPRIMENTO w.ws_col-1

    char c = '\0';
    char sair = '\0';
    int nova_fase = 0; int fase = 1; int menuv = 1;
    struct Mapa mapa[ALTURA][COMPRIMENTO];
    struct Jogador jogador;
    struct Torre torre;

    /*gerar_mapa(ALTURA,COMPRIMENTO,mapa);
    ajeitar_paredes(ALTURA,COMPRIMENTO,mapa,3,5);
    spawn_jogador(ALTURA,COMPRIMENTO,mapa,&jogador);*/

    keypad(initscr(),TRUE);
    start_color();
    while(sair != 'q' && sair != 'Q'){
        if(menuv == 1){
            menu(ALTURA,COMPRIMENTO,&sair);
            if(sair == 'q') break;
            menuv = 0; fase = 1;
            nova_fase = 1;
        }
        if(nova_fase == 1){
            inicializa_jogador(&jogador);
            inicializa_torre(&torre);
            resetar_mapa(ALTURA,COMPRIMENTO,mapa);
            gerar_mapa(ALTURA,COMPRIMENTO,mapa);
            ajeitar_paredes(ALTURA,COMPRIMENTO,mapa,3,5);
            spawn_jogador(ALTURA,COMPRIMENTO,mapa,&jogador);
            spawn_torre(ALTURA,COMPRIMENTO,mapa,&torre,jogador);
            nova_fase = 0;
        }
    	mostrar_stats(ALTURA,COMPRIMENTO,jogador,torre,fase);
    	mostrar_mapa(ALTURA,COMPRIMENTO,mapa,jogador,torre);
    	refresh();
    	c = getchar();
    	if(c == 'v' || c == 'V'){
    		/*if(jogador.visao_total){
    			jogador.visao_total = FALSE;
    		} else {
    			jogador.visao_total = TRUE;
    		}*/

    	} else {
            if(c == CIMA || c == BAIXO || c == ESQUERDA || c == DIREITA){
    	        move_jogador(ALTURA,COMPRIMENTO,mapa,&jogador,torre,c);
                ataque_torre(&jogador,torre);
            } else if (c == 'a' || c == 'A' || c == 's' || c == 'S' || c == 'w' || c == 'W' || c == 'd' || c == 'D'){
                ataque_jogador(ALTURA,COMPRIMENTO,mapa,jogador,&torre,c);
                ataque_torre(&jogador,torre);
            }
    	}
        if(torre.hp <= 0) {
            fase_completa(ALTURA,COMPRIMENTO,fase);
            fase++;
            if(fase > 5){
                menuv = 1;
            } else {
                nova_fase = 1;
            }
        } else if(jogador.hp <= 0){
            game_over(ALTURA,COMPRIMENTO);
            menuv = 1;
        }
    }
 
    endwin();


	return 0;
}