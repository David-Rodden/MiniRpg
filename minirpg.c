
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //for randomness in combat
#include <math.h>

#define DEAD -1
#define HEALTHY 0
#define DISEASED 1
#define INJURED 2
typedef struct fightNodeTag{
  int monsterLevel, monsterHealth, damageDealt, damageReceived;
}fightNode;

typedef struct cityNodeTag{
	char *name;
	int hostileness, frequency;
}cityNode;
typedef struct weaponTag{
	char *name;
	int bonus;
}weaponNode;
typedef struct armourTag{
	char *name;
	int bonus;
}armour;
void initialize(int *level, int *money, int *weapon, int *armour, int *experience, int *health);
void initializeCities(cityNode **city);
void initializeWeapons(weaponNode **wep);
void prompt(char *question);
int roomCost(int level);
int rest(int level, int *health, int *money);
int generate(int *id, int amount);
int getArea(char *area, char **choices);
int battle(int *level, int *experience, int *weapon, int *armour, int *health, int *money);
int end(int level, int money);
int experienceToLevel(int experience);
char *append(char *word, char **appended);
char *response();
char *lower(char *word);

int main(void){
  int level, money, weapon, armour, health, experience, state = HEALTHY;
  char *options[] = {"fight", "shop", "rest", "quit"}, *states[] = {"healthy", "diseased", "injured"};
  initialize(&level, &money, &weapon, &armour, &experience, &health);
	cityNode *city = malloc(sizeof(city) * 5);
	//initializeCities(&city);  
	weaponNode *wep = malloc(sizeof(weaponNode) * 6);
	//initializeWeapons(&wep);
generate(&money, 10);
 label:while(state >= HEALTHY){
    printf("\x1B[0m""Level: %d(%dXP); Money: %d, Weapon bonus: %d; Armour bonus: %d; State: %s(%d/%dHP)\n", level, experience, money, weapon, armour, states[state], health, (level * 10));
    prompt(append("What do you want to do? ", options));
    //scan user's response
    //check for response
    int area = getArea(response(), options), newState = 0;
    switch(area){
    case 0:
      state = battle(&level, &experience, &weapon, &armour, &health, &money);
      goto label;
    case 2:
      if((newState = rest(level, &health, &money)) >= 0) state = newState;
      goto label;
	case 3:
	return -1;
    default:
      printf("No such choice is available...\n");
      goto label;
    }
  }
  return ((state == DEAD)? end(level, money): 0);
}

void initialize(int *level, int *money, int *weapon, int *armour, int *experience, int *health){
  *level = 1;
  *money = 0;
  *weapon = 2;
  *armour = 0;
  *experience = 0;
  *health = 10;
}

void initializeCities(cityNode **city){
	city[0]->name = "hey";
	printf("%s\n", city[0]->name);
}
void initializeWeapons(weaponNode **wep){
	//wep[0]->name = "Iron sword";
	//printf("%s", wep[0]->name);
	wep[0]->bonus = 2;
	printf("%d", wep[0]->bonus);	
wep[1]->name = "Steel sword";
	wep[1]->bonus = 4;
	wep[2]->name = "Mithril sword";
	wep[2]->bonus = 7;
	wep[3]->name = "Adamantine sword";
	wep[3]->bonus = 11;
	wep[4]->name = "Dark Magic sword";
	wep[4]->bonus = 16;
	wep[5]->name = "Diamond sword";
	wep[5]->bonus = 23;
}
void prompt(char *question){
  printf("%s\n>", question);
}

int roomCost(int level){
  return level * 4;
}

int rest(int level, int *health, int *money){
  int cost = roomCost(level);
  printf("A room for the night costs %d coins:\tYES\tNO\n>", cost);
  if((strcmp(response(), "yes")) != 0){
    printf("Leaving...\n");
    return -1;
  }
  if(cost > *money){
    printf("You do not have enough money to rent a room\n");
    return -1;
  }
	*money-=cost;
  printf("--------------------Resting----------------------\n");
  printf("You purchase a room in the nearest tavern and rest for the night...\n");
  *health = level * 10;
  printf("-------------------------------------------------\n");
  return HEALTHY;
}

int generate(int *id, int amount){
  *id += amount;
  return amount;
}

int getArea(char *area, char **choices){
  int i;
  for(i = 0; i < 4; i++){
    if(!strcmp(area, choices[i]))return i;
  }
  //checks if word is equal to any of the areas
      return -1;
}

int battle(int *level, int *experience, int *weapon, int *armour, int *health, int *money){
  printf("-------------------Combat-Statistics------------------\n");
  fightNode *fight = malloc(sizeof(fightNode));
  srand(time(NULL));
  int *received = &(fight->damageReceived), *dealt = &(fight->damageDealt), *mHealth = &(fight->monsterHealth), *mLevel = &(fight->monsterLevel);
  *mLevel = rand() % (int)(*level * 1.5) + 1;
  *mHealth = *mLevel * *mLevel;
  printf("Monster level: %d\n", *mLevel);
  int originalHealth = *mHealth, maxHealth = *level * 10;
  while((*health > 0) && (*mHealth > 0)){
    printf("HE hits for %d damage: ", *received = (rand())? (rand() % (*mLevel / (*armour + 1))): 0);
    if(*received)((*received > *health)? (*health = 0) : (*health-=*received));
    printf("(%d/%dHP)\tYOU hit for %d damage: ", *health, (*level * 10), *dealt = (rand() % ((*weapon + *level) / 2 + 1)));
    if(*dealt)((*dealt > *mHealth)? (*mHealth = 0): (*mHealth-=*dealt));
    printf("(%d/%dHP)\n", *mHealth, originalHealth);
  }
  if(*health > 0){
    int experienceGain = (*mLevel * 5);
    *experience += experienceGain;
    printf("You've gained %d experience and %d coins\n", experienceGain, generate(money, (5 + rand() % *mLevel)));
	   free(fight);
    int newLevel = experienceToLevel(*experience);
    if(*level < newLevel)printf("Congratulations! You've leveled up!\n");
    *level = newLevel;
  }
  printf("------------------------------------------------------\n");
  return ((*health > 0)? ((*health > maxHealth/2)? HEALTHY: INJURED): DEAD);
}

int end(int level, int money){
  printf("You have perished - Game Over\n\nFinal stats:\n\nLevel:\t%d\nMoney:\t%d\n\n", level, money);
  return DEAD;
}

int experienceToLevel(int experience){
  return floor(pow((experience), (1/2.3)));
}

char *append(char *word, char **appended){
  int i, max = 4;
  for(i = 0; i < max; i++){
    char *newWord = malloc(strlen(word) + strlen(appended[i]) + ((i < max - 1)? 4: 1));
    if(!newWord)return NULL;
    strcpy(newWord, word);
    strcat(newWord, appended[i]);
    if(i < max - 1)strcat(newWord, ", ");
    word = malloc(strlen(newWord) + 1);
    if(!word)return newWord;
    strcpy(word, newWord);
    free(newWord);
  }
  return word;
}
     
     char *response(){
       char *word = malloc(sizeof(char) * 15);
       scanf("%s", word);
       return word;
     }
     
     char *lower(char *word){
       int i;
      for(i = 0; i < strlen(word); i++){
	 tolower(word[i]);
       }
       return word;
     }
     
