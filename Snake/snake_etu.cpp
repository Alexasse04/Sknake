#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>




using namespace std;

#define SIZE_MAXI 200
#define LARGEUR 40
#define HAUTEUR 20

#define TEMPS 200000

struct fruit {
    int fruitX;
    int fruitY;
};

enum direction { STOP = 0, GAUCHE, DROITE, HAUT, BAS };

struct jeu {
    bool jeuEnCours;
    int teteX;
    int teteY;
    int score;
    int tailleQueue;
    direction d;
    fruit f;
};

jeu initialiser(jeu j,int queueX[], int queueY[]);
int dessiner(jeu j, int tabX[], int tabY[]);
direction entrerDirection(direction d);
jeu deplacer(jeu j, int queueX[], int queueY[]);
int userInput();



int main() {
    jeu j;
    int queueX[SIZE_MAXI];
    int queueY[SIZE_MAXI];
    srand(time(NULL));
    j = initialiser(j,queueX,queueY);

    while (j.jeuEnCours) {
        dessiner(j, queueX, queueY);

        if (userInput())
            j.d = entrerDirection(j.d);
        j = deplacer(j, queueX, queueY);
        
        usleep(TEMPS);
    }

    cout << "Game Over! Votre score est: " << j.score << endl;

    return 0;
}

jeu initialiser(jeu j,int queueX[], int queueY[]) {
     //declaration des variable
    bool sofarsogood;
    int i;

    j.jeuEnCours = true;
    j.teteX = LARGEUR/2;
    j.teteY = HAUTEUR/2;
    j.tailleQueue = 1; 
    j.score = 0;
    j.d = STOP;

    do{
        sofarsogood = true;
        j.f.fruitX = rand() % (LARGEUR - 1);
        j.f.fruitY = rand() % (HAUTEUR - 1);
        //parcour queue serpent
        for(i=0;i<j.tailleQueue;i++){
            if(queueX[i]==j.f.fruitX && queueY[i]==j.f.fruitY){
                sofarsogood = false;
            }
            
        }
    } while((j.teteX==j.f.fruitX && j.teteY==j.f.fruitY) || !sofarsogood);
     
    return j;
}

int dessiner (jeu game, int tabX[], int tabY[]) {
    //declaration des variable
    int i,j, k;

    system("clear"); // Efface l'écran

    // Dessine la bordure supérieure
    for (i = 0; i < LARGEUR; i++)
        std::cout << "#";
    std::cout << std::endl;
    
    char c =' ';
    // Dessine le contenu de l'écran
    for (i = 0; i < HAUTEUR-1; i++) {
        for (j = 0; j < LARGEUR; j++) {
            c=' ';
            if (j == 0){
                c='#'; // Bordure gauche
            }else if (j == LARGEUR - 1){
                c= '#'; // Bordure droite
            }else if (i == game.teteY && j == game.teteX) {
                // Vérifier si la position actuelle est celle de la tête du serpent
                c='O'; //dessine tête
            }else if (j == game.f.fruitX && i == game.f.fruitY){
                // Si ce n'est ni la tête ni le corps du serpent, vérifier le fruit
                c= '*'; // Dessine le fruit
            }else {
                  //Vérifier si la position actuelle est celle du corps du serpent
                for (k = 0; k < game.tailleQueue; k++) {
                    if (j == tabX[k] && i == tabY[k])
                    c='o'; //dessine corp
                }
            }  std::cout << c;

            
        }
        std::cout << std::endl;
    }
    // Dessine la bordure inférieure
    for (int i = 0; i < LARGEUR; i++)
        std::cout <<'#';
    std::cout << std::endl;


    std::cout << "coordonées x:" << game.teteX << " "<< "y:"<< game.teteY<<" ""\n";
    std::cout << "score = "<< game.score <<" ";

    return 0;
    
}


direction entrerDirection(direction dire) {
    //declaration des variable
    int input = getchar();
    int c,d,z,s,q,Z,S,Q,D;

    // mise en place des commande de direction
    if (c=='q'|| c=='Q'){
        if (dire!=DROITE){
            dire=GAUCHE;
        }
    }
     if (c=='d'|| c=='D'){
        if (dire!=GAUCHE){
            dire=DROITE;
        }
    }
    if (c=='z'|| c=='Z'){
        if (dire!=BAS){
            dire=HAUT;
        }
    }
    if (c=='s'|| c=='S'){
        if (dire!=HAUT){
            dire=BAS;
        }
    }
   
    return dire;
}

jeu deplacer(jeu game, int queueX[], int queueY[]) {
    //declaration des variable
    int tmpX, tmpY, stockX, stockY, i;
    bool sofarsogood;
    // Mettre à jour la position de la tête du serpent en fonction de la direction
    if(game.d==HAUT){
        game.teteY--;
    }
    if(game.d==BAS){
        game.teteY++;
    }
    if(game.d==DROITE){
        game.teteX++;
    }
    if(game.d==GAUCHE){
        game.teteX--;
    }
    // Vérifiez les collisions avec les bords de l'écran
    if (game.teteX <= 0 || game.teteX >= LARGEUR -1 || game.teteY < 0 || game.teteY >= HAUTEUR -1 ) {
        // Le jeu est terminé en cas de collision avec les bords
        game.jeuEnCours = false;
        return game; // Ajoutez ce return pour sortir de la fonction immédiatement
    }

    // Vérifiez si la tête du serpent atteint le fruit
    if (game.teteX == game.f.fruitX && game.teteY == game.f.fruitY) {
        game.score += 1;
        game.tailleQueue++;
        // Générez un nouveau fruit à une position aléatoire
        do{
            sofarsogood = true;
            game.f.fruitX = rand() % (LARGEUR - 1);
            game.f.fruitY = rand() % (HAUTEUR - 1);
            //parcour queue serpent
            for(i=0;i<game.tailleQueue;i++){
                 if(queueX[i]==game.f.fruitX && queueY[i]==game.f.fruitY){
                sofarsogood = false;
            }
            }
    } while((game.teteX==game.f.fruitX && game.teteY==game.f.fruitY) || !sofarsogood);   
        
    }

    tmpX=queueX[0];
    tmpY=queueY[0];
    queueX[0]=game.teteX;
    queueY[0]=game.teteY;

    for (i=1;i<game.tailleQueue;i++){
        stockX=queueX[i];
        stockY=queueY[i];
        queueX[i]=tmpX;
        queueY[i]=tmpY;
        tmpX=stockX;
        tmpY=stockY;
    }

    // Vérifiez les collisions avec le corps du serpent
    for (i = 1; i < game.tailleQueue; i++) {
        if (game.teteX == queueX[i] && game.teteY == queueY[i]) {
            game.jeuEnCours = false; // Le jeu est terminé en cas de collision avec le corps
            return game;
        }
    }

    return game;
}




//http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int userInput() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
