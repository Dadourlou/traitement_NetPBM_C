#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char chiffre; // = PX du fichier d'entrée
int largeur, hauteur; // = Lazrgeur et hauteur de l'image et dimension de ma matrice
char des_chiffre[3]; // = chiffre du hichier de destination
char mode[10]="Texte"; // = mode d"écriture
int iv=255; // = intensité
int valeur_seuil; // Valeur pour la fonction seuil
int taille3D; // Troisième dimension du tableau

int NB=1; // 0 = Blanc 1 = Noir
int gris=0;
int rouge=0; // iv = blanc 0 = Noir
int vert=0;
int bleu=0;
//Couleur de base

int rouge_int=0;
int vert_int=0;
int bleu_int=0;
// Couleur de INtérieur

int x_zig;
int y_zig;
//Coordonné pour ZIG

int coordx_cercle;
int coordy_cercle;
int coordx2_cercle;
int coordy2_cercle;
//Coordonné pour cercle

int action=0;// Détermine dans quelle cas nous nous trouvons(si il y a un fichier d'zntrée et pas de LBI etc) cf le main


void test_argument(int argc, char **argv){
  //test si la ligne de commande rentré est conforme aux exigences
  if(argc < 4){
    printf("Erreur il manque un argument ! \n");
    printf("La bonne syntaxe est ./a.out fichier_entrée (ou -) fichier_lbi (ou -) fichier_sortie\n");
    exit(EXIT_FAILURE);
  }

  if(argc > 4){
    printf("Erreur il y a trop d'argument !! \n");
    printf("La bonne syntaxe est ./a.out fichier_entrée (ou -) fichier_lbi (ou -) fichier_sortie\n");
    exit(EXIT_FAILURE);
  }

  if ( argv[3][0] == '-'){
    printf("Erreur ! Vous devez impérativement spécifiez un fichier de sortie.\n");
    exit(EXIT_FAILURE);
  }
}

void commentaire(FILE* fichier){
  //cette petite fonction sert a ne pas prendre les commentaires en compte
  char comm[80],com;
  while(1){
    com = fgetc(fichier);
    fseek(fichier, -1, SEEK_CUR);
    if (com == '#'){
      fgets(comm,sizeof(comm), fichier);
    } else {
      break;
    }
  }
}

void check_ligne1(FILE* fichier){
  //cette fonction check que la première ligne est bien PX\n
  char p, retour_chariot;

  commentaire(fichier);

  p = fgetc(fichier);
  if (p != 'P'){
    printf("Erreur ! Ceci n'est pas un fichier NetPbm.\n");
    exit(EXIT_FAILURE);
  }

  commentaire(fichier);

  chiffre = fgetc(fichier);
  if (chiffre == '1'){
    printf("C'est un fichier monochrome ASCII.\n");
  } else if (chiffre == '2'){
    printf("C'est un fichier niveaux de gris ASCII.\n");
  } else if (chiffre == '3'){
    printf("C'est un fichier couleur ASCII.\n");
  } else if (chiffre == '4'){
    printf("C'est un fichier monochrome Binaire.\n");
  } else if (chiffre == '5'){
    printf("C'est un fichier niveaux de gris Binaire.\n");
  } else if (chiffre == '6'){
    printf("C'est un fichier couleur Binaire.\n");
  } else {
    printf("Erreur de type de fichier !\n");
    exit(EXIT_FAILURE);
  }

  commentaire(fichier);

  retour_chariot = fgetc(fichier);
  if (retour_chariot != '\n'){
    printf("Erreur!\n");
    exit(EXIT_FAILURE);
  }

  commentaire(fichier);
}

void fNouvImage( FILE* lbi, char ligne[80]){
  //cette fonction est appeler unniquement si il n'y as pas de fichier d'entrèe
  //elle permet de déterminer que la larguer, la hauteur et le type de fichier de sortie
char nouv[10];
char type[10];

  sscanf(ligne, "%s %d %d %s", nouv, &largeur, &hauteur, type);
  if (strcmp(type,"NB") == 0 && des_chiffre[0]=='1'){
    des_chiffre[0]='1';
    des_chiffre[1]='4';
  } else if (strcmp(type,"Gris") == 0 && des_chiffre[0]=='2'){
    iv=255;
    des_chiffre[0]='2';
    des_chiffre[1]='5';
  }else if (strcmp(type,"Couleur") == 0 && des_chiffre[0]=='3'){
    iv=255;
    des_chiffre[0]='3';
    des_chiffre[1]='6';
  } else {
    printf("Erreur ! Le type de fichier de sortie n'est pas compatible avec le type de NouvImage.\n");
    exit(EXIT_FAILURE);
  }

}

void fMode(char ligne[80], FILE* copie){
  //Cette fonction sert a déterminer si on écrit en Binaire ou en Texte
  char nouv[14];
  sscanf(ligne,"%s %s", nouv, mode);

  if (strcmp(mode,"Texte") == 0){
    fseek(copie,0,SEEK_SET);
    fprintf(copie, "P%c\n", des_chiffre[0]);
  } else if (strcmp(mode,"Binaire") == 0){
    fseek(copie,0,SEEK_SET);
    fprintf(copie, "P%c\n", des_chiffre[1]);
  } else {
    printf("Problème, quel mode d'écriture voulez vous ?\nBinaire ou Texte ?\nCar %s n'est pas valable.\n", mode);
    exit(EXIT_FAILURE);
  }
}

void fPixel(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
  //Cette fonction sert a mettre un Pixel
  char nouv[14];
  sscanf(ligne, "%s %d %d", nouv, &x_zig, &y_zig);

  if(x_zig<0 || x_zig>largeur || y_zig<0 || y_zig>hauteur) {
    printf("Warning ! Les coordonnées du Pixel sont hors de l'image, il ne serat donc pas mit dans l'image\n");
  } else {

  if(des_chiffre[0] == '1') (*tab)[x_zig][y_zig][0]=NB;
  if(des_chiffre[0] == '2') (*tab)[x_zig][y_zig][0]=gris;
  if(des_chiffre[0] == '3'){
   (*tab)[x_zig][y_zig][0]=rouge;
   (*tab)[x_zig][y_zig][1]=vert;
   (*tab)[x_zig][y_zig][2]=bleu;
  }
}
}

void fRectangleV(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
  //Fonction pour rectangle vide
  //J'aurais aussi put le faire avec 4 appels de ligne
  int x,y,hau,lar,i,j;
  char nouv[14];
  sscanf(ligne, "%s %d %d %d %d", nouv, &x, &y, &hau, &lar);
//if(x!=0)x-=1;
//if(y!=0)y-=1;

if(x<0 || x>largeur || y<0 || y>hauteur || x+lar>largeur || y+hau>hauteur) {
  printf("Warning ! Les coordonnées du Rectangle sont hors de l'image, il ne serat donc pas mit dans l'image\n");
}else {

  for(i=y-hau+1;i<=y;i++){
    for (j=x;j<=lar+x;j++){
      if ((j==x) || (i==y) || (i==y-hau+1) || (j==lar+x)){
        if(des_chiffre[0] == '1') (*tab)[j][i][0]=NB;
        if(des_chiffre[0] == '2'){
          (*tab)[j][i][0]=gris;
         }
         if(des_chiffre[0] == '3'){
           (*tab)[j][i][0]=rouge;
           (*tab)[j][i][1]=vert;
           (*tab)[j][i][2]=bleu;
        }
      }
    }
  }
}
}

void fRectangleP(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Je commence par faire les bords, puis je remplis


  int x,y,hau,lar,i,j;
  char nouv[14];
  sscanf(ligne, "%s %d %d %d %d", nouv, &x, &y, &hau, &lar);

  if(x<0 || x>largeur || y<0 || y>hauteur || x+lar>largeur || y+hau>hauteur) {
    printf("Warning ! Les coordonnées du Rectangle sont hors de l'image, il ne serat donc pas mit dans l'image\n");
  }else {
  x+=1;
  y-=1;
  hau-=2;
  lar-=1;

    for(i=y-hau+1;i<=y;i++){
      for (j=x;j<=lar+x;j++){
          if(des_chiffre[0] == '1') (*tab)[j][i][0]=NB;
          if(des_chiffre[0] == '2'){
            (*tab)[j][i][0]=gris;
           }
           if(des_chiffre[0] == '3'){
             (*tab)[j][i][0]=rouge_int;
             (*tab)[j][i][1]=vert_int;
             (*tab)[j][i][2]=bleu_int;
          }
        }
      }
      fRectangleV(ligne,largeur,hauteur,taille3D,tab);
}
}

void fSeuil(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
// Fonction mettant en noir ou blanc les formats PPM et PGM

char nouv[14];
  int i,j;
    if (action!=3)sscanf(ligne, "%s %d", nouv, &valeur_seuil);
    else if (action==3) valeur_seuil=iv/2;

    if((des_chiffre[0] == '2' && action!=3) || (chiffre=='2' && action==3)) {
      for(i=0;i<hauteur;i++){
        for (j=0;j<largeur;j++){
          if((*tab)[j][i][0]>=valeur_seuil) (*tab)[j][i][0]=iv;
          else (*tab)[j][i][0]=0;
        }
      }
    }

    if((des_chiffre[0] == '3' && action!=3) || (chiffre=='3' && action==3)) {
      for(i=0;i<hauteur;i++){
        for (j=0;j<largeur;j++){
          if((((*tab)[j][i][0]+(*tab)[j][i][1]+(*tab)[j][i][2])/3)>=valeur_seuil){
            (*tab)[j][i][0]=iv;
            (*tab)[j][i][1]=iv;
            (*tab)[j][i][2]=iv;
          }
          else {
            (*tab)[j][i][0]=0;
            (*tab)[j][i][1]=0;
            (*tab)[j][i][2]=0;
          }
        }
      }
    }
  }


void fInvVideo(int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Fonction ou pour PBM inversion 1 et 0 et pour PPm et PGM, = iv-valeur

int i,j,q;

  if(des_chiffre[0] == '1'){
      for(i=0;i<hauteur;i++){
        for (j=0;j<largeur;j++){
          if((*tab)[j][i][0]==0) (*tab)[j][i][0]=1;
          else  (*tab)[j][i][0]=0;
        }
      }
    }

  if(des_chiffre[0] == '3' || des_chiffre[0] == '2'){
    for(i=0;i<hauteur;i++){
      for (j=0;j<largeur;j++){
        for (q=0;q<taille3D;q++){
          if((*tab)[j][i][q]==0) (*tab)[j][i][q]=iv;
          else if ((*tab)[j][i][q]==iv) (*tab)[j][i][q]=0;
          else (*tab)[j][i][q]=iv-(*tab)[j][i][q];
         }
       }
    }
  }
}

void fLigne(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D], int fonction){
//3 fonctions pour fLigne
// fonction =1 => ligne normal
// fonction =2 => utilisation avec Zig
// fonction =3 => remplissage de cercle pour CercleP

  int coordx,coordy,coordx2,coordy2;
char nouv[14];
  if (fonction == 0 )sscanf(ligne, "%s %d %d %d %d", nouv, &coordx, &coordy, &coordx2, &coordy2);
  else if (fonction == 1) {
    sscanf(ligne, "%s %d %d", nouv, &coordx2, &coordy2);
    coordx=x_zig;
    coordy=y_zig;
  } else if (fonction==2){
    coordx=coordx_cercle;
    coordy=coordy_cercle;
    coordx2=coordx2_cercle;
    coordy2=coordy2_cercle;
  }

  if(coordx2<0){
    coordx2=0;
    printf("La coordonnées x étaient inférieur à 0, elle a donc été mises à 0.\n");
  }else if (coordx2>largeur){
    coordx2=largeur-1;
    printf("La coordonnées x étaient supérieur ou égale à largeur, elle a donc été mises à largeur.\n");
  } else if(coordy2<0){
    coordy2=0;
    printf("La coordonnées y étaient inférieur à 0, elle a donc été mises à 0.\n");
  }else if (coordy2>hauteur){
    coordy2=largeur-1;
    printf("La coordonnées y étaient supérieur ou égale à hauteur, elle a donc été mises à largeur.\n");
  }

    int dx = abs(coordx2-coordx), sx = coordx<coordx2 ? 1 : -1;
    int dy = abs(coordy2-coordy), sy = coordy<coordy2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
      if(des_chiffre[0] == '1') (*tab)[coordx][coordy][0]=NB;
      if(des_chiffre[0] == '2'){
        (*tab)[coordx][coordy][0]=gris;
       }
       if(des_chiffre[0] == '3' && fonction !=2){
         (*tab)[coordx][coordy][0]=rouge;
         (*tab)[coordx][coordy][1]=vert;
         (*tab)[coordx][coordy][2]=bleu;
        }
        if(des_chiffre[0] == '3' && fonction ==2){
          (*tab)[coordx][coordy][0]=rouge_int;
          (*tab)[coordx][coordy][1]=vert_int;
          (*tab)[coordx][coordy][2]=bleu_int;
         }
	    e2 = err;
	    if (e2 >-dx) { err -= dy; coordx += sx; }
	    if (e2 < dy) { err += dx; coordy += sy; }
      if (coordx==coordx2 && coordy==coordy2) break;
    }

    if (fonction == 1) {
      x_zig=coordx2;
      y_zig=coordy2;
    }
}


void fCercle(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//fonction faisant un cercle seloon l'algo de Bresenhamn, et gérant les 3 types

  int x0,y0,radius;
char nouv[14];
  sscanf(ligne, "%s %d %d %d", nouv,&x0,&y0,&radius);

  if(x0<0 || x0>largeur || y0<0 || y0>hauteur\
     || x0-radius<0 || x0+radius>largeur || y0-radius<0 || y0+radius>hauteur) {
       printf("Warning ! Les coordonnées du Cercle sont hors de l'image, elle ne serat donc pas mit dans l'image\n");
     }else {

    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
      if(des_chiffre[0] == '1'){
        (*tab)[x0 + x][y0 + y][0]=NB;
        (*tab)[x0 + y][y0 + x][0]=NB;
        (*tab)[x0 - y][y0 + x][0]=NB;
        (*tab)[x0 - x][y0 + y][0]=NB;
        (*tab)[x0 - x][y0 - y][0]=NB;
        (*tab)[x0 - y][y0 - x][0]=NB;
        (*tab)[x0 + y][y0 - x][0]=NB;
        (*tab)[x0 + x][y0 - y][0]=NB;
      }
      if(des_chiffre[0] == '2'){
        (*tab)[x0 + x][y0 + y][0]=gris;
        (*tab)[x0 + y][y0 + x][0]=gris;
        (*tab)[x0 - y][y0 + x][0]=gris;
        (*tab)[x0 - x][y0 + y][0]=gris;
        (*tab)[x0 - x][y0 - y][0]=gris;
        (*tab)[x0 - y][y0 - x][0]=gris;
        (*tab)[x0 + y][y0 - x][0]=gris;
        (*tab)[x0 + x][y0 - y][0]=gris;
      }
      if(des_chiffre[0] == '3'){
        (*tab)[x0 + x][y0 + y][0]=rouge;
        (*tab)[x0 + x][y0 + y][1]=vert;
        (*tab)[x0 + x][y0 + y][2]=bleu;// en bas a droite

        (*tab)[x0 + y][y0 + x][0]=rouge;
        (*tab)[x0 + y][y0 + x][1]=vert;
        (*tab)[x0 + y][y0 + x][2]=bleu;

        (*tab)[x0 - y][y0 + x][0]=rouge;
        (*tab)[x0 - y][y0 + x][1]=vert;
        (*tab)[x0 - y][y0 + x][2]=bleu;

        (*tab)[x0 - x][y0 + y][0]=rouge;
        (*tab)[x0 - x][y0 + y][1]=vert;
        (*tab)[x0 - x][y0 + y][2]=bleu;

        (*tab)[x0 - x][y0 - y][0]=rouge;
        (*tab)[x0 - x][y0 - y][1]=vert;
        (*tab)[x0 - x][y0 - y][2]=bleu;

        (*tab)[x0 - y][y0 - x][0]=rouge;
        (*tab)[x0 - y][y0 - x][1]=vert;
        (*tab)[x0 - y][y0 - x][2]=bleu;

        (*tab)[x0 + y][y0 - x][0]=rouge;
        (*tab)[x0 + y][y0 - x][1]=vert;
        (*tab)[x0 + y][y0 - x][2]=bleu;

        (*tab)[x0 + x][y0 - y][0]=rouge;
        (*tab)[x0 + x][y0 - y][1]=vert;
        (*tab)[x0 + x][y0 - y][2]=bleu;

      }

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
  }
}

void fDisque(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Je commence par tracer des lignes horizontales entre tous les points du cercle
//Puis j'appelle fCercle pour faire le contour dans la bonne couleur

  int fonction;
  int x0,y0,radius;
char nouv[14];
  sscanf(ligne, "%s %d %d %d", nouv,&x0,&y0,&radius);

  if(x0<0 || x0>largeur || y0<0 || y0>hauteur\
     || x0-radius<0 || x0+radius>largeur || y0-radius<0 || y0+radius>hauteur) {
       printf("Warning ! Les coordonnées du Cercle sont hors de l'image, elle ne serat donc pas mit dans l'image\n");
     }else {

    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        fonction=2;
        coordx_cercle=x0+x;
        coordy_cercle=y0+y;
        coordx2_cercle=x0-x;
        coordy2_cercle=y0+y;
        fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);

        coordx_cercle=x0+y;
        coordy_cercle=y0+x;
        coordx2_cercle=x0-y;
        coordy2_cercle=y0+x;
        fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);

        coordx_cercle=x0-y;
        coordy_cercle=y0-x;
        coordx2_cercle=x0+y;
        coordy2_cercle=y0-x;
        fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);

        coordx_cercle=x0-x;
        coordy_cercle=y0-y;
        coordx2_cercle=x0+x;
        coordy2_cercle=y0-y;
        fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
    fCercle(ligne,largeur,hauteur,taille3D,tab);
}
}

void fcoul(char ligne[80]){
  // détermine la couleur de dessin
  int coul1,coul2,coul3;
char nouv[14];
  sscanf(ligne, "%s %d %d %d", nouv, &coul1,&coul2,&coul3);
  if(strcmp(nouv,"Blanc")==0){
    NB=0;
    gris=iv;
    rouge=iv;
    vert=iv;
    bleu=iv;
  } else if (strcmp(nouv,"Gris")==0){
    NB=1;
    gris=coul1;
    rouge=0;
    vert=0;
    bleu=0;
  } else if (strcmp(nouv,"Coul")==0){
    NB=1;
    gris=coul1;
    rouge=coul1;
    vert=coul2;
    bleu=coul3;
  } else if (strcmp(nouv,"Noir")==0){
    NB=1;
    gris=0;
    rouge=0;
    vert=0;
    bleu=0;
  }
}

void fInterieur(char ligne[80]){
  // détermine la couleur de remplissage
  int coul1,coul2,coul3;
  char nouv[14];
  sscanf(ligne, "%s %d %d %d", nouv, &coul1,&coul2,&coul3);
    rouge_int=coul1;
    vert_int=coul2;
    bleu_int=coul3;
}

void fRot(char ligne[80],int largeur,int hauteur,int taille3D ,int (*tab)[largeur][hauteur][taille3D]){
// Je commence par copier la matrice dans une matrice
// Puis je copie en changeant largeur et hauteur
//Comme je ne change pas la matrice, cela signifie que ma fonction rot ne marche bien que avec les images carré ou largeur = hauteur

  int rot;
char nouv[14];
  sscanf(ligne,"%s %d", nouv, &rot);
      int i,j,q;



  if (rot==90){

    int tab_rot[hauteur][largeur][taille3D];

  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
         tab_rot[i][j][q]=(*tab)[j][i][q];
       }
     }
  }

    for(i=0;i<largeur;i++){
      for (j=0;j<hauteur;j++){
        for (q=0;q<taille3D;q++){
           (*tab)[j][i][q] = tab_rot[j][hauteur-i-1][q];
         }
       }
    }
  } else if (rot==270){

    int tab_rot[hauteur][largeur][taille3D];

  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
         tab_rot[i][j][q]=(*tab)[j][i][q];
       }
     }
  }
    for(i=0;i<hauteur;i++){
      for (j=0;j<largeur;j++){
        for (q=0;q<taille3D;q++){
           (*tab)[j][i][q] = tab_rot[largeur-j-1][i][q];
         }
       }
    }
  } else if (rot == 180){

  int tab_rot[largeur][hauteur][taille3D];

  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
         tab_rot[j][i][q]=(*tab)[j][i][q];
       }
     }
  }

    for(i=0;i<hauteur;i++){
      for (j=0;j<largeur;j++){
        for (q=0;q<taille3D;q++){
           (*tab)[j][i][q] = tab_rot[largeur-j-1][hauteur-i-1][q];
         }
       }
    }
  }

}

void fRemplit(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D],int x,int y){
// Cette focntion regarde si le pixel est blanc et regarde ces 4 pixels adjacent, si ils sont blancs alors les colories
// Puis appels récursif sur les 4 pixels adjacents

/*if(y<=0)y=0;
if(x<=0)x=0;
if(x>(largeur-1))x=(largeur-1);
if(y>(hauteur-1))y=(hauteur-1);*/

if(x<0)x=0;
if(y<0)y=0;

if(x<0 || x>largeur || y<0 || y>hauteur){
printf("Warning ! Les coordonnées que vous essayer de sélectionner sont hors de l'image.\n");
}else{




  if (des_chiffre[0]=='1'){
    if ((*tab)[x][y][0] == 0) {
      (*tab)[x][y][0] = 1;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x-1,y);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x+1,y);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y-1);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y+1);
    }
  } else if (des_chiffre[0]=='2'){
    if ((*tab)[x][y][0] == iv) {
      (*tab)[x][y][0] = 0;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x-1,y);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x+1,y);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y-1);
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y+1);
    }
  } else if (des_chiffre[0]=='3'){
    if ((*tab)[x][y][0] == iv && (*tab)[x][y][1] == iv && (*tab)[x][y][2] == iv) {
      (*tab)[x][y][0] = rouge_int;
      (*tab)[x][y][1] = vert_int;
      (*tab)[x][y][2] = bleu_int;
    }

    if ((*tab)[x-1][y][0] == iv && (*tab)[x-1][y][1] == iv && (*tab)[x-1][y][2] == iv && x>=0) {
      (*tab)[x-1][y][0] = rouge_int;
      (*tab)[x-1][y][1] = vert_int;
      (*tab)[x-1][y][2] = bleu_int;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x-1,y);

    }
    if ((*tab)[x][y+1][0] == iv && (*tab)[x][y+1][1] == iv && (*tab)[x][y+1][2] == iv && y<=(hauteur-1)) {
      (*tab)[x][y+1][0] = rouge_int;
      (*tab)[x][y+1][1] = vert_int;
      (*tab)[x][y+1][2] = bleu_int;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y+1);

    }
    if ((*tab)[x+1][y][0] == iv && (*tab)[x+1][y][1] == iv && (*tab)[x+1][y][2] == iv && x<=(largeur-1)) {
      (*tab)[x+1][y][0] = rouge_int;
      (*tab)[x+1][y][1] = vert_int;
      (*tab)[x+1][y][2] = bleu_int;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x+1,y);

    }
    if ((*tab)[x][y-1][0] == iv && (*tab)[x][y-1][1] == iv && (*tab)[x][y-1][2] == iv && y>=0) {
      (*tab)[x][y-1][0] = rouge_int;
      (*tab)[x][y-1][1] = vert_int;
      (*tab)[x][y-1][2] = bleu_int;
      fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y-1);

    }

  }
}

}

void fSymH(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
// Fonction de Symétrie horizontal ou le reste de l'image reste inchangé

char nouv[14];
  int y;
  sscanf(ligne,"%s %d",nouv,&y);

  if (y<0 || y>hauteur) {y = hauteur/2;
    printf("Attention soit,\n- vous avez choisis un axe qui n'est pas dans l'image\n- vous n'avez pas spécifié d'axe\nNous avons donc choisis l'axe médian.\n");
}
  int i,j,q,z=2,max=0;

  int tab_SymH[largeur][hauteur][taille3D];

for(i=0;i<hauteur;i++){
  for (j=0;j<largeur;j++){
    for (q=0;q<taille3D;q++){
       tab_SymH[j][i][q]=(*tab)[j][i][q];
     }
   }
}

if (y*2 < hauteur) max=y*2;
else max =hauteur;

  for(i=y+1;i<max;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
         (*tab)[j][i][q] = tab_SymH[j][i-z][q];
       }
     }
     z+=2;
  }


  if(y*2 < largeur){
    for(i=max;i<hauteur;i++){
      for (j=0;j<largeur;j++){
        for (q=0;q<taille3D;q++){
           if(des_chiffre[0]=='1')(*tab)[j][i][q] = 0;
           if(des_chiffre[0]!='1')(*tab)[j][i][q] = iv;
         }
       }
    }
  }

}
void fCopyRect(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Fonction copiant un rectangle et le mettant a des coordonnées données dans l'image
char nouv[14];
    int x,y,l,h,x2,y2,i,j,q;
    sscanf(ligne,"%s %d %d %d %d %d %d",nouv,&x,&y,&l,&h,&x2,&y2);

    if(x<0 || x>largeur || y<0 || y>hauteur\
       || x+l>largeur || y-h<0 || x2+l>largeur || y2-h<0 ||\
     x2<0 || x2>largeur || y2<0 || y2>hauteur) {
         printf("Warning ! Les coordonnées du Cercle sont hors de l'image, elle ne serat donc pas mit dans l'image\n");
       }else {

    int rectangle_copie[l][h][taille3D];
    int w=0,z=0;

    for(i=y-h+1;i<=y;i++){
      for (j=x;j<=l+x;j++){
        for(q=0;q<taille3D;q++){
          rectangle_copie[w][z][q]=(*tab)[j][i][q];
          }
          w++;
        }
        w=0;
        z++;
      }

      w=0;
      z=0;
      for(i=y2-h+1;i<=y2;i++){
        for (j=x2;j<=l+x2;j++){
          for(q=0;q<taille3D;q++){
            (*tab)[j][i][q]=rectangle_copie[w][z][q];
            }
            w++;
          }
          w=0;
          z++;
        }
}
}

int rouge_flou=0,vert_flou=0,bleu_flou=0,division=0;

void bleu_existe(int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D],int i, int j){

         bleu_flou+=(*tab)[j][i][2]*3;
         division+=3;
         if(j<largeur-1) {
           bleu_flou+=(*tab)[j+1][i][2]*2;
           division+=2;
         }
          if (j>0) {
           bleu_flou+=(*tab)[j-1][i][2]*2;
           division+=2;
         }
          if(i<hauteur-1){
           bleu_flou+=(*tab)[j][i+1][2]*2;
           division+=2;
         }
          if(i>0){
            bleu_flou+=(*tab)[j][i-1][2]*2;
            division+=2;
         }

         if(j<largeur-1 && i<hauteur-1) {
           bleu_flou+=(*tab)[j+1][i+1][2];
           division+=1;
        }
          if (j>0 && i>0) {
           bleu_flou+=(*tab)[j-1][i-1][2];
           division+=1;
        }
          if(i<hauteur-1 && j>0) {
           bleu_flou+=(*tab)[j-1][i+1][2];
           division+=1;
        }
          if(i>0 && j<largeur-1){
           bleu_flou+=(*tab)[j+1][i-1][2];
           division+=1;
        }

  }

void vert_existe(int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D],int i, int j){

         vert_flou+=(*tab)[j][i][1]*3;
         division+=3;
         if(j<largeur-1) {
           vert_flou+=(*tab)[j+1][i][1]*2;
           division+=2;
         }
          if (j>0) {
           vert_flou+=(*tab)[j-1][i][1]*2;
           division+=2;
         }
          if(i<hauteur-1){
           vert_flou+=(*tab)[j][i+1][1]*2;
           division+=2;
         }
          if(i>0){
            vert_flou+=(*tab)[j][i-1][1]*2;
            division+=2;
         }

         if(j<largeur-1 && i<hauteur-1) {
           vert_flou+=(*tab)[j+1][i+1][1];
           division+=1;
        }
          if (j>0 && i>0) {
           vert_flou+=(*tab)[j-1][i-1][1];
           division+=1;
        }
          if(i<hauteur-1 && j>0) {
           vert_flou+=(*tab)[j-1][i+1][1];
           division+=1;
        }
          if(i>0 && j<largeur-1){
           vert_flou+=(*tab)[j+1][i-1][1];
           division+=1;
        }

  }

void rouge_existe(int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D],int i, int j){

         rouge_flou+=(*tab)[j][i][0]*3;
         division+=3;

         if(j<largeur-1) {
           rouge_flou+=(*tab)[j+1][i][0]*2;
           division+=2;
         }
         if (j>0) {
           rouge_flou+=(*tab)[j-1][i][0]*2;
           division+=2;
         }
         if(i<hauteur-1){
           rouge_flou+=(*tab)[j][i+1][0]*2;
           division+=2;
         }
         if(i>0){
            rouge_flou +=(*tab)[j][i-1][0]*2;
            division+=2;
         }

         if(j<largeur-1 && i<hauteur-1) {
           rouge_flou+=(*tab)[j+1][i+1][0];
           division+=1;
        }
          if (j>0 && i>0) {
           rouge_flou+=(*tab)[j-1][i-1][0];
           division+=1;
        }
          if(i<hauteur-1 && j>0) {
           rouge_flou+=(*tab)[j-1][i+1][0];
           division+=1;
        }
          if(i>0 && j<largeur-1){
           rouge_flou +=(*tab)[j+1][i-1][0];
           division+=1;
        }
  }


void fFlou(int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
  int i,j;
  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      if(des_chiffre[0]=='3'){

        rouge_existe(largeur,hauteur,taille3D,tab,i,j);
        (*tab)[j][i][0]=rouge_flou/division;
        rouge_flou=0;
        division=0;

        vert_existe(largeur,hauteur,taille3D,tab,i,j);
        (*tab)[j][i][1]=vert_flou/division;
        vert_flou=0;
        division=0;

        bleu_existe(largeur,hauteur,taille3D,tab,i,j);
        (*tab)[j][i][2]=bleu_flou/division;
        bleu_flou=0;
        division=0;

       } else if(des_chiffre[0]=='2'){
         rouge_existe(largeur,hauteur,taille3D,tab,i,j);
         (*tab)[j][i][0]=rouge_flou/division;
         rouge_flou=0;
         division=0;
       }
     }
  }

}


void fSymV(char ligne[80],int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
  // Fonction de Symétrie vertical ou le reste de l'image reste inchangé

char nouv[14];
  int y;
  sscanf(ligne,"%s %d",nouv,&y);

  if (y<0 || y>largeur){
     y = largeur/2;
     printf("Attention soit,\n- vous avez choisis un axe qui n'est pas dans l'image\n- vous n'avez pas spécifié d'axe\nNous avons donc choisis l'axe médian.\n");
}

  int i,j,q,z=0,max=0;

  int tab_SymV[largeur][hauteur][taille3D];

for(i=0;i<hauteur;i++){
  for (j=0;j<largeur;j++){
    for (q=0;q<taille3D;q++){
       tab_SymV[j][i][q]=(*tab)[j][i][q];
     }
   }
}

if (y*2 < largeur) max=y*2;
else max =largeur;

  for(i=0;i<hauteur;i++){
    for (j=y+1;j<max;j++){
      for (q=0;q<taille3D;q++){
         (*tab)[j][i][q] = tab_SymV[j-z][i][q];
       }
       z+=2;
     }
     z=0;
  }


  if(y*2 < largeur){
    for(i=0;i<hauteur;i++){
      for (j=max;j<largeur;j++){
        for (q=0;q<taille3D;q++){
           if(des_chiffre[0]=='1')(*tab)[j][i][q] = 0;
           if(des_chiffre[0]!='1')(*tab)[j][i][q] = iv;
         }
       }
    }
  }
}

int compteurLBI=0;
int compteur=1;
//Mer permet de Savoir quand arrêter la lecture de LBI

void lecture_lbi(FILE* lbi, FILE* copie, char **argv,int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
// Focntion lisant LBI et utilisant les fonctions correspondantes

  char ligne[80],mot[80];
  int parsing=0;
  int fonction=0;
  int x,y;
  char commande[10];

  fgets(ligne,80,lbi);
  sscanf(ligne,"%s",mot);

  printf("%s\n",mot);

  if (strcmp(mot,"Rem") == 0){
  } else if (strcmp(mot,"NouvImage") == 0){
  } else if (strcmp(mot,"Mode") == 0){
    fMode(ligne,copie);
  } else if ((strcmp(mot,"Blanc") == 0) || (strcmp(mot,"Noir") == 0) || (strcmp(mot,"Gris") == 0) || (strcmp(mot,"Coul") == 0)){
    fcoul(ligne);
  } else if (strcmp(mot,"Interieur") == 0){
    fInterieur(ligne);
  } else if (strcmp(mot,"Pixel") == 0){
    fPixel(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"InvVideo") == 0){
    fInvVideo(largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Ligne") == 0){
    fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);
  } else if (strcmp(mot,"Zig") == 0){
    fonction=1;
    fLigne(ligne,largeur,hauteur,taille3D,tab,fonction);
  } else if (strcmp(mot,"RectangleP") == 0){
    fRectangleP(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"RectangleV") == 0){
    fRectangleV(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Seuil") == 0){
    fSeuil(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Cercle") == 0){
    fCercle(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Disque") == 0){
    fDisque(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Rot") == 0){
    if(largeur==hauteur)fRot(ligne,largeur,hauteur,taille3D,tab);
    else printf("Désolé, mais la fonction Rot n'est pas encore implémenté pour les images rectangulaire.\n");
  } else if (strcmp(mot,"Remplit") == 0){
    sscanf(ligne, "%s %d %d", commande,&x,&y);
    fRemplit(ligne,largeur,hauteur,taille3D,tab,x,y);
  } else if (strcmp(mot,"SymH") == 0){
    fSymH(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"SymV") == 0){
    fSymV(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"CopyRect") == 0){
    fCopyRect(ligne,largeur,hauteur,taille3D,tab);
  } else if (strcmp(mot,"Flou") == 0){
    fFlou(largeur,hauteur,taille3D,tab);
  } else {
    printf("Warning, la commande %s n'est pas reconnu par ce programme\n", mot);
  }

  if (compteurLBI==compteur){
     printf("Fin du fichier Lbi\n");
     parsing=1;
     compteur=0;
   }

  if (parsing==0){
    compteur++;
    lecture_lbi(lbi,copie,argv,largeur,hauteur,taille3D,tab);
  }

}

char lecturebit(char c, int i){
     return ((c>>i) & 1);
}


void quatretoquatre(int compteur,FILE* fichier,FILE* copie,int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Lit le type P4

  int i=0,j=0,q=0,z,g;
  char test;

        while( fscanf(fichier, "%c", &test) != EOF)
        {
          for (g=7;g>=0;g--){
            z=lecturebit(test,g);
            (*tab)[j][i][q]=z;
            j++;
            if(j==largeur){
                j=0;
                i++;
                break;
              }
            }
          }
}


void untoun_deuxtodeux_troistotrois(int compteur,FILE* fichier,FILE* copie,int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Lir les formats P1,P2 et P3

  int test,i,j,q;
  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
        fscanf(fichier, "%d", &test);
         (*tab)[j][i][q] = test;
       }
     }
  }
}

void cinqtocinq_sixtosix(int compteur,FILE* fichier,FILE* copie,int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Lit les formats P5 et P6

  int i,j,q;
  char test;
  for(i=0;i<hauteur;i++){
    for (j=0;j<largeur;j++){
      for (q=0;q<taille3D;q++){
        fscanf(fichier, "%c", &test);
         (*tab)[j][i][q] = test;
       }
     }
  }
}


void printmatrice(FILE* copie,int largeur,int hauteur,int taille3D,int (*tab)[largeur][hauteur][taille3D]){
//Print la matrice en focntion du type demander

  if(strcmp(mode,"Texte") == 0){
    int i,j,q;
      for(i=0;i<hauteur;i++){
        for (j=0;j<largeur;j++){
          for (q=0;q<taille3D;q++){
            fprintf(copie, "%d ", (*tab)[j][i][q]);
            if ((j%35==0 && j!=0) || j == largeur-1) fprintf(copie, "\n");
          }
        }
      }
    }


  if((strcmp(mode,"Binaire") == 0) && des_chiffre[1] != '4'){
    int i,j,q;
      for(i=0;i<hauteur;i++){
        for (j=0;j<largeur;j++){
          for (q=0;q<taille3D;q++){
            fprintf(copie, "%c", (*tab)[j][i][q]);
          }
        }
      }
    }

    if((strcmp(mode,"Binaire") == 0) && des_chiffre[1] == '4'){

        int i=0,j=0,q=0,z,x,m=0;
        char test=0;

              for(z=0;z<((hauteur*largeur)/7);z++){
                for (x=7;x>=0;x--){
                  if((*tab)[j][i][q]==0 && m==0) test=test<<1;
                  else if((*tab)[j][i][q]==1 && m==0) test=(test<<1)|1;
                  if(m==0)j++;

                  if(j==largeur && x==0){
                    j=0;
                    i++;
                    m=0;
                    test=0;
                  } else if (j==largeur){
                    test=test<<1;
                    m=1;
                  }
                }
                fprintf(copie, "%c", test);

              }
            }

}




int main(int argc, char **argv){

    FILE* fichier = NULL;
    FILE* lbi = NULL;
    FILE* copie = NULL;
    static int compteur=1;

    test_argument(argc, argv);// Cette fonction sert a vérifier que la ligne de commmande entrée est bonne

    copie = fopen(argv[3], "w");

    if (argv[1][0] != '-' && argv[2][0] != '-') action=1;// Fichier entrée + actions LBI
    if (argv[1][0] == '-' && argv[2][0] != '-') action=2;// NouvImage + LBI
    if (argv[1][0] != '-' && argv[2][0] == '-') action=3;// Fichier entrée uniquement == Conversion


    if (action==1 || action==2){
      lbi = fopen(argv[2], "r");
      if(lbi == NULL){
        printf("Ce fichier contenant les commande LBI n'existe pas\n");
        exit(EXIT_FAILURE);
      }
    }

      if (action==1 || action==3){
        fichier = fopen(argv[1], "r");
        if (fichier == NULL) {
          printf("Le fichier d'entrée que vous essayer d'ouvrir n'existe pas.\n");
          exit(EXIT_FAILURE);
        }
        check_ligne1(fichier);// Cela permet de vérifier si la fichier commence bien par PX\n
      }                       // Cette fonction sert aussi a déterminer "chiffre" qui correspond au X de PX

      char test;
      test = strlen(argv[3]);
      if ( argv[3][test-2] == 'b'){
        des_chiffre[0] = '1';
        des_chiffre[1] = '4';
      } else if ( argv[3][test-2] == 'g'){
        des_chiffre[0] = '2';
        des_chiffre[1] = '5';
      } else if ( argv[3][test-2] == 'p'){
        des_chiffre[0] = '3';
        des_chiffre[1] = '6';
      }

      // des_chiffre est l'abréviation de "Chiffre du fichier de destination"
      //C'estun tableau de 2 car tant que l'on as pas lus le mode d'écriture,
      //alors le fichier peut être Binaire ou Texte, donc 1 ou 4, 2 ou 5 et 3 ou 6

      if (action==1 || action==3){
        char taille[11]; // J'autorise une image de maximum 10⁵ * 10⁵ (ce qui est déjà trop)
        fgets(taille, 70, fichier);
        sscanf(taille, "%d %d", &largeur, &hauteur);
        printf("Largeur = %d, Hauteur = %d\n",largeur, hauteur);

        commentaire(fichier);

        if ( chiffre!='1'){ //car si chiffre =1, alors il n'y as pas d'itensité
          fgets(taille,70,fichier);
          sscanf(taille, "%d", &iv);
          printf("Intensité = %d\n",iv);
        }

        commentaire(fichier);

        int current;
        int x;

        current = ftell(fichier);
        do{
          x = fgetc(fichier);
          if (x=='\n')compteur++;
        }while(x!=EOF);
        fseek(fichier, current, SEEK_SET);

        //cette boucle me permet de déterminer combien de fgets du chier je vais avoir a faire

        taille3D=1;
        if(chiffre=='3' || chiffre=='6')taille3D=3;
        //cela me permet de déterminer si c'est un tableau a 2 dimension ou a 3 dimension pour PPM
      }

      if (action==2){
        char ligne[80];
        fgets(ligne,80,lbi);
        printf("NouvImage\n");
        fNouvImage(lbi,ligne);
        //cette fonction me permet de déterminer la largeur, la hauteur et le type de fichier de sortie

        taille3D=1;
        if(des_chiffre[0]=='3' || des_chiffre[1]=='6')taille3D=3;
        //cela me permet de déterminer si c'est un tableau a 2 dimension ou a 3 dimension pour PPM
      }


    int tab[largeur][hauteur][taille3D];
    //Je crée ma matrice temporaire

    if (action==2){
      int i,j,q;
      for(i=0;i<hauteur;i++){
      for (j=0;j<largeur;j++){
        for (q=0;q<taille3D;q++){
          if(des_chiffre[0]!='1')tab[j][i][q]=iv;
          if(des_chiffre[0]=='1')tab[j][i][q]=0;
        }
      }
      }
      //Si je n'ai pas de fichier d'entrée, alors, je remplis ma matrice de blanc
    }

    if (action==1 || action==3){

    if ((chiffre == '1') || (chiffre == '2') || (chiffre == '3')){
    untoun_deuxtodeux_troistotrois(compteur,fichier,copie,largeur,hauteur,taille3D,&tab);
  }// Cette foncion lis les fichiers ascii et les mets dans ma matrice tab

    if (chiffre == '4'){
    quatretoquatre(compteur,fichier,copie,largeur,hauteur,taille3D,&tab);
  } // cette fonction lit les fichiers Binaire encodé Bit a Bit

    if ((chiffre == '5') || (chiffre == '6')){
    cinqtocinq_sixtosix(compteur,fichier,copie,largeur,hauteur,taille3D,&tab);
  } //Cette focntion lit les fichier binaires, encodé octet a octet

    }

    if(action==1 || action==2){

      int current;
      int x;

      current = ftell(lbi);
      do{
        x = fgetc(lbi);
        if (x=='\n')compteurLBI++;
      }while(x!=EOF);
      fseek(lbi, current, SEEK_SET);
      //Cette petite boucle me permet de dterminer le nombre de ligne dans mon fichier LBI

      lecture_lbi(lbi,copie,argv,largeur,hauteur,taille3D,&tab);
    }
    // Je lis le fichier LBI, c'est là que toute les modification de la matrice auront lieu.

    if (action==3){
      char ligne[]="Mode Texte";
      fMode(ligne,copie);
      //Si il n'y as pas de fichier LBI,alors le mode est mis a Texte
    }

    fprintf(copie,"%d %d \n",largeur, hauteur);
    if (des_chiffre[0]!='1') fprintf(copie,"%d\n", iv);
    //Je print dans le fichier de sortie les 2 ou 3 premières ligne de description

    if (action==1 || action==2) printmatrice(copie,largeur,hauteur,taille3D,&tab);
    // Cette fonction sert a print tab dans le fichier de sortie en focntion du mode ultilisé

    if (action==3){
      // Cette partie recense toute les conversion possible lorsue l'on ne met pas de fichier LBI

      if ((((chiffre == '3') || (chiffre == '6')) && (des_chiffre[0]=='3')) \
       || (((chiffre == '2') || (chiffre == '5')) && (des_chiffre[0]=='2'))\
       || (((chiffre == '1') || (chiffre == '4')) && (des_chiffre[0]=='1'))){
        printmatrice(copie,largeur,hauteur,taille3D,&tab);
            //Si l'on fais juste une Copie
    }

      if (((chiffre == '3') || (chiffre == '6')) && (des_chiffre[0]=='2')){
        int i,j,q,temp=0;

        for(i=0;i<hauteur;i++){
          for (j=0;j<largeur;j++){
            for (q=0;q<3;q++){
              temp+=tab[j][i][q];
              if(q==2){
                fprintf(copie, "%d ", temp/3);
                temp=0;
              }
            }
            if ((j%35==0 && j!=0) || j == largeur-1) fprintf(copie, "\n");
        }
      }
      // Si l'on passe de Couleur a gris, on moyenne les 3 pixels
    }

      if ((((chiffre == '3') || (chiffre == '6')) && (des_chiffre[0]=='1')) || \
        (((chiffre == '2') || (chiffre == '5')) && (des_chiffre[0]=='1'))){
        char *ligne=0;
        fSeuil(ligne,largeur,hauteur,taille3D,&tab);

        int i,j;

        for(i=0;i<hauteur;i++){
          for (j=0;j<largeur;j++){
              if(tab[j][i][0]==iv) fprintf(copie, "0 ");
              else if (tab[j][i][0]==0) fprintf(copie, "1 ");
              if ((j%35==0 && j!=0) || j == largeur-1) fprintf(copie, "\n");
        }
      }
      // Si l'on passe de Gris ou de couleur a NoirBlanc
      //Fseuil détermine su on met un Blanc ou un Noir puis on Print
      }

      if (((chiffre == '1') || (chiffre == '4')) && ((des_chiffre[0]=='2') || (des_chiffre[0]=='3'))){
        int i,j;

        for(i=0;i<hauteur;i++){
          for (j=0;j<largeur;j++){
            if(tab[j][i][0]==0 && (des_chiffre[0]=='3')) fprintf(copie, "%d %d %d ",iv,iv,iv);
            else if (tab[j][i][0]==1 && (des_chiffre[0]=='3')) fprintf(copie, "0 0 0 ");
            else if(tab[j][i][0]==0 && (des_chiffre[0]=='2')) fprintf(copie, "%d ",iv);
            else if (tab[j][i][0]==1 && (des_chiffre[0]=='2')) fprintf(copie, "0 ");
            if ((j%35==0 && j!=0) || j == largeur-1) fprintf(copie, "\n");
          }
        }
      }
      //Si l'on passe du NoirBlanc au format PPM ou PGM
}

if (argv[1][0] == '-' && argv[2][0] == '-') printf("Cette ligne de commande ne fais absolument rien.\n");


  if (argv[1][0] != '-')fclose(fichier);
  if (argv[2][0] != '-')fclose(lbi);
  fclose(copie);
  // On ferme tout les fichiers qui ont été ouvert

  char code_entree[80], code_sortie[80];
  sprintf(code_entree, "xdg-open %s", argv[1]);
  sprintf(code_sortie, "xdg-open %s", argv[3]);
  if (argv[1][0] != '-')system(code_entree);
  if (argv[2][0] != '-')system(code_sortie);
  //OUvre le fichier d'entré si il y en as un et ouvre aussi le fichier de sortie

  return 0;
}
