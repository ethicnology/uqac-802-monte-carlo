#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Card{
    char rank;
    char suit[8];
    int weight;
};

int Random(){
  return(rand() % 52);
}

/* Assign value to the given pointer on struct card */
void DrawCard(struct Card *Pick){
  int random, suit, rank;
  random = Random();
  switch (random / 13){
    case 0:
      strcpy(Pick->suit, "\u2660"); //Spades ♠
      break;
    case 1:
      strcpy(Pick->suit, "\u2663"); //Club ♣
      break;
    case 2:
      strcpy(Pick->suit, "\u2665"); //Heart ♥
      break;
    case 3:
      strcpy(Pick->suit, "\u2666"); //Diamond ♦
      break;
    default:
      strcpy(Pick->suit, "WTF");
  }

  rank = random % 13;
  switch (rank){
    case 9:
      Pick->rank = '1';
      Pick->weight = 10;
      break;
    case 10:
      Pick->rank = 'J';
      Pick->weight = 11;
      break;
    case 11:
      Pick->rank = 'Q';
      Pick->weight = 12;
      break;
    case 12:
      Pick->rank = 'K';
      Pick->weight = 13;
      break;
    case 0:
      Pick->rank = 'A';
      Pick->weight = 14;
      break;
    default:
      Pick->rank = '1' + rank;
      Pick->weight = '1' + rank;
  }
  // if(Pick->rank == '1')
  //   printf("\nCard from dealer is the 10 of %s - weight is %d", Pick->suit, Pick->weight);
  // else
  //   printf("\nCard from dealer is the %c of %s - weight is %d", Pick->rank, Pick->suit, Pick->weight);
}

/* Check equality between two cards*/
int IsEqual(struct Card *one, struct Card *two){
  return((one->rank == two->rank) && (strcmp(one->suit, two->suit) == 0));
}

/* Verify if a card belong to Face cards aka Jack, Queen, King, Ace */
int IsFace(char letter){
  int i;
  char *face = "JQKA";
  for (i = 0; i < strlen(face); i++){
    if(letter == face[i])
      return(1);
  }
  return(0);
}

/* Draw different cards of size N */
void DrawDifferentCards(struct Card *cards[], size_t size){
  int i, j, k;
  for (i = 0; i < size; i++){
    DrawCard(cards[i]);
  }
  for (j = 0; j < size; j++){
    for (k = j + 1; k < size; k++){
      if(IsEqual(cards[j], cards[k])){
        DrawDifferentCards(cards, size);
      }
    }
  }
}

/* Le joueur tire une carte. Si celle-ci est un as, alors le joueur gagne 10$. Autrement, le joueur perd. */
int Game1(){
  struct Card card, *cardPtr = &card;
  DrawCard(cardPtr);
  if(card.rank == 'A')
    return(10);
  else return(0);
}

/* Le joueur tire deux cartes. Si les deux cartes sont exactement identiques, alors le joueur gagne 50 $. Sinon, le joueur perd.(Avec remise). */
int Game2(){
  struct Card one, two, *onePtr = &one, *twoPtr = &two;
  DrawCard(onePtr);
  DrawCard(twoPtr);
  if(IsEqual(onePtr, twoPtr))
    return(50);
  else return(0);
}

/* Le joueur tire deux cartes. Si la deuxième carte tirée a un rang strictement supérieur à la première carte tirée, alors le joueur gagne 2 $. Sinon, le joueur perd.(Sans remise). */
int Game3(){
  int prize = 2;
  struct Card one, two, *onePtr = &one, *twoPtr = &two, *drawPtr[2];
  drawPtr[0] = onePtr;
  drawPtr[1] = twoPtr;
  DrawDifferentCards(drawPtr, 2);
  if(one.weight < two.weight){
    return(prize);
  }
  else return(0);
}

/* Le joueur tire trois cartes. Si au moins une carte est de cœur, alors le joueur gagne k$, où k est le nombre de cartes tirées qui sont des cœurs. Sinon, le joueur perd. (Sans remise) */
int Game4(){
  int i = 0, nbDrawn = 0, prize = 0;
  struct Card one, two, three, *onePtr = &one, *twoPtr = &two, *threePtr = &three, *drawPtr[3];
  drawPtr[0] = onePtr;
  drawPtr[1] = twoPtr;
  drawPtr[2] = threePtr;
  DrawDifferentCards(drawPtr, 3);
  if(strcmp(one.suit, "\u2665") == 0) prize++;
  if(strcmp(two.suit, "\u2665") == 0) prize++;
  if(strcmp(one.suit, "\u2665") == 0) prize++;
  if(prize > 0){
    return(prize);
  } else return(0);
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

/* Le joueur tire cinq cartes. S'il existe un sous-ensemble de ces cartes qui fait une série de trois valeurs de cartes consécutives ou plus (par exemple, 5-6-7 ou 10-J-Q), alors le joueur gagne 5 $. Sinon, le joueur perd.(Sans remise) */
int Game5(){
  int nbCards = 5, handWeight[nbCards], i, weight, serial, prize = 5;
  struct Card one, two, three, four, five, *onePtr = &one, *twoPtr = &two, *threePtr = &three, *fourPtr = &four, *fivePtr = &five, *drawPtr[nbCards];
  drawPtr[0] = onePtr;
  drawPtr[1] = twoPtr;
  drawPtr[2] = threePtr;
  drawPtr[3] = fourPtr;
  drawPtr[4] = fivePtr;
  DrawDifferentCards(drawPtr, nbCards);
  for (i = 0; i < nbCards; i++){
    handWeight[i] = drawPtr[i]->weight;
  }
  qsort(handWeight, nbCards, sizeof(int), cmpfunc);
  for (i = 0; i < nbCards-1; i++){
    if((handWeight[i + 1] - handWeight[i]) == 1){
      serial++;
    } else serial = 0;
  }
  if(serial >= 2){
    return(prize);
  }else return(0);
}

/* ./a.out ARG 
*  ex : ./a.out 2
*/
int main(int argc, char *argv[]){
  int i = 0, game = 0, win = 0, loss = 0, gain = 0, iterate = 1000000;
  long arg;
  srand(time(NULL));

  if(argc == 2){
    arg = strtol(argv[1], NULL, 10);
  } else arg = 1;

  if(arg == 1) {
    for(i; i < iterate; i++){
      game = Game1();
      if(game > 0){
        win++;
        gain += game;
      }
      else loss++;
    }
  }
  else if(arg == 2){
    for(i; i < iterate; i++){
      game = Game2();
      if(game > 0){
        win++;
        gain += game;
      }
      else loss++;
    }
  }
  else if(arg == 3){
      for(i; i < iterate; i++){
      game = Game3();
      if(game > 0){
        win++;
        gain += game;
      }
      else loss++;
    }
  }
  else if(arg == 4){
    for(i; i < iterate; i++){
      game = Game4();
      if(game > 0){
        win++;
        gain += game;
      }
      else loss++;
    }
  }
  else if(arg == 5){
    for(i; i < iterate; i++){
      game = Game5();
      if(game > 0){
        win++;
        gain += game;
      }
      else loss++;
    }
  }  
  else if( argc > 2 ) {
    printf("Provide a number from 1 to 5 corresponding to the game you want to simulate  ex : ./exemple.out 5.\n");
  }
  else if (arg < 1 || arg > 5){
    printf("Provide a number from 1 to 5 corresponding to the game you want to simulate  ex : ./exemple.out 5.\n");
  }
  else {
    printf("Provide a number from 1 to 5 corresponding to the game you want to simulate  ex : ./exemple.out 5.\n");
  }
  double probability = (double)win / ((double)win + (double)loss);
  printf("\n###Game-%ld### \nwin : %d \nloss : %d \nprob : %f \nprize : %d$ \n############\n",arg,win,loss,probability,gain);
  return (0);
}