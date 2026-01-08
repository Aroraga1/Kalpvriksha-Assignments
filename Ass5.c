#include<stdio.h>
#include "players_data.h"
#include<stdlib.h>
#include<string.h>

char *roles[]={"Batsman", "Bowler", "All_rounder"};
int totalRoles = sizeof(roles) / sizeof(roles[0]);

struct Player{
    int playerId;
    char *name;
    int teamId;
    int roleId;
    int totalRun;
    float battingAvg;
    float strikeRate;
    int wicket;
    float ecoomicRate;
    float performanceIndex;
    struct Player *next;
};

struct Team{
    int teamId;
    char *name;
    int totalPlayers;
    float absr;
    struct Team *next;
};

struct Team *tHead = NULL;
struct Team *tTail = NULL;
struct Player *pHead = NULL;
struct Player *pTail = NULL;

void initializeTeamAndPlayers(){
    for(int i=0; i<teamCount; i++){
        struct Team *newTeamNode = (struct Team*)malloc(sizeof(struct Team));
        newTeamNode->teamId = i+1;
        newTeamNode->name = malloc(strlen(*(teams+i)) + 1);
        strcpy(newTeamNode->name,*(teams+i));
        newTeamNode->totalPlayers=0;
        newTeamNode->absr=0.0;
        newTeamNode->next=NULL;
        if(tHead==NULL){
            tHead = newTeamNode;
            tTail = newTeamNode;
        }else{
            tTail->next = newTeamNode;
            tTail = newTeamNode;
        }
    }
    for(int i=0; i<playerCount; i++){
        struct Player *newPlayerNode = (struct Player*)malloc(sizeof(struct Player));
        newPlayerNode->playerId = (players+i)->id;
        newPlayerNode->name = malloc(strlen((players+i)->name) + 1);
        strcpy(newPlayerNode->name,(players+i)->name);
        for(int j=0; j<teamCount; j++){
            if(strcmp((players+i)->team, *(teams+j)) == 0){
                newPlayerNode->teamId=j+1;
                break;
            }
        }
        newPlayerNode->ecoomicRate=(players+i)->economyRate;
        newPlayerNode->wicket=(players+i)->wickets;
        newPlayerNode->strikeRate=(players+i)->strikeRate;
        newPlayerNode->battingAvg=(players+i)->battingAverage;
        newPlayerNode->totalRun=(players+i)->totalRuns;
        for(int j=0; j<totalRoles; j++){
            if(strcmp((players+i)->role,*(roles+j))==0){
                newPlayerNode->roleId=j+1;
                break;
            }
        }
        if(newPlayerNode->roleId  == 1){ 
            newPlayerNode->performanceIndex = (newPlayerNode->battingAvg * newPlayerNode->strikeRate) /100; 
        }else if(newPlayerNode->roleId  == 2){
            newPlayerNode->performanceIndex  = (newPlayerNode->wicket * 2) + (100 - newPlayerNode->ecoomicRate);    
        }else if(newPlayerNode->roleId  == 3){
            newPlayerNode->performanceIndex = ((newPlayerNode->battingAvg * newPlayerNode->strikeRate ) / 100)+(newPlayerNode->wicket * 2);
        }

        newPlayerNode->next = NULL;

        if(pHead == NULL){
        pHead = pTail = newPlayerNode;
        } else{
        pTail->next = newPlayerNode;
        pTail = newPlayerNode;
        }
    }
}

void showMenu(){
printf("\n==============================================================================");
printf("\nICC ODI Player Performance Analyzer");
printf("\n==============================================================================");
printf("\n1. Add Player to Team\n2. Display Players of a Specific Team\n3. Display Teams by Average Batting Strike Rate\n4. Display Top K Players of a Specific Team by Role\n5. Display all Players of specific role Across All Teams by performance index\n6. Exit");
printf("\n==============================================================================");
}

void clearBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF){}
}

int getInput(void *var, char c){
    int result=0;
    if(c=='d') result = scanf("%d",(int *)var);
    else if(c=='f') result = scanf("%f",(float *)var);
    else result = scanf("%51s",(char *)var);
    if(result!=1){
        printf("Invailid Input!");
        clearBuffer();
        return 0;
    }
    return 1;
}

void addPlayer(){
    printf("\nEnter Team ID to add player: ");
    int teamId,playerId,role,runs,wicket;
    char name[51];
    float battingAvg, StrickRate, economicRate;

    if(!getInput(&teamId,'d')) return;
    if(teamId<1 || teamId>10){
        printf("\nteam id should be under 1 to 10, Enter again: ");
        return;
    }

    printf("\nEnter Player Details: ");
    printf("\nPlayer Id: ");
    if(!getInput(&playerId,'d')) return;
    if(playerId<1 || playerId>1000){
        printf("\nplayer id must be under 1 to 1000, Enter again: ");
        return;
    }
    struct Player *p = pHead;
    while(p){
        if(p->playerId == playerId){
        printf("Player ID already exists!");
        return;
        }
        p = p->next;
    }   
    printf("\nName: ");
    if(!getInput(&name,'s')) return;

    printf("\nRole:");
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if(!getInput(&role,'d')) return;

    printf("\nTotal runs: ");
    if(!getInput(&runs,'d')) return;

    printf("\nBatting Average: ");
    if(!getInput(&battingAvg,'f')) return;

    printf("\nStrike Rate: ");
    if(!getInput(&StrickRate,'f')) return;
    
    printf("\nWickets: ");
    if(!getInput(&wicket,'d')) return;

    printf("\nEconomic Rate: ");
    if(!getInput(&economicRate,'f')) return;

    struct Player *newPlayer = malloc(sizeof(struct Player));
    newPlayer->playerId = playerId;
    newPlayer->teamId = teamId;
    newPlayer->name = malloc(strlen(name)+1);
    strcpy(newPlayer->name,name);
    newPlayer->roleId = role;
    newPlayer->totalRun = runs;
    newPlayer->wicket = wicket;
    newPlayer->battingAvg = battingAvg;
    newPlayer->ecoomicRate = economicRate;
    newPlayer->strikeRate = StrickRate;
    newPlayer->next = NULL;

    pTail->next = newPlayer;
    pTail = newPlayer;

    if(newPlayer->roleId  == 1){ 
            newPlayer->performanceIndex = (newPlayer->battingAvg * newPlayer->strikeRate) /100; 
    }else if(newPlayer->roleId  == 2){
            newPlayer->performanceIndex  = (newPlayer->wicket * 2) + (100 - newPlayer->ecoomicRate);
    }else if(newPlayer->roleId  == 3){
            newPlayer->performanceIndex = ((newPlayer->battingAvg * newPlayer->strikeRate ) / 100)+(newPlayer->wicket * 2);
    }
    struct Team *t = tHead;
    for(int i=0; i<teamCount; i++){
        if(i==newPlayer->teamId){
            t->totalPlayers++;
            break;
        }
        t=t->next;
    }
    printf("Player added successfully to Team India!");
}

void calAvgSR(){
    struct Team *team = tHead;
    while(team!=NULL){
        struct Player *player = pHead;
        int batsmanCount=0;
        float sumSR=0.0;
        while(player!=NULL){
            if((player->teamId==team->teamId) && (player->roleId==1 || player->roleId==3)){
                sumSR+=player->strikeRate;
                batsmanCount++;
            }
            player=player->next;            
        }
        if(batsmanCount>0) team->absr=sumSR/batsmanCount;
        else team->absr = 0;
        team=team->next;
    }
}

void playersByTeamId(){
    int teamId;
    printf("\nEnter Team ID:");
    if(!getInput(&teamId,'d')) return;
    if(teamId<1|| teamId>10){
        printf("\nTeam id must be between 1-10");
        return;
    }
    struct Player *temp = pHead;
    printf("Players of Team %s:\n", *(teams+teamId-1));
    printf("====================================================================================\n");
    printf("ID | Name | Runs | Avg | SR | Wkts | ER | Pref.Index\n");
    printf("====================================================================================\n");
    int teamPlayerCount = 0;
    float totalSR = 0.0;
    while (temp != NULL){
        if(temp->teamId == teamId)
            printf("%d | %s |  %d | %0.1f | %.1f | %d | %.1f | %.2f\n", temp->playerId, temp->name,  temp->totalRun, temp->battingAvg, temp->strikeRate, temp->wicket, temp->ecoomicRate, temp->performanceIndex);
        if(temp->teamId == teamId && (temp->roleId == 1 || temp->roleId == 3)){
            totalSR += temp->strikeRate;
            teamPlayerCount++;
        }
        temp = temp->next;
    }
    printf("====================================================================================\n");
    printf("Total Players : %d\n", teamPlayerCount);
    float avg = (teamPlayerCount == 0) ? 0 : totalSR / teamPlayerCount;
    printf("Average Batting Strike Rate: %.2f\n", avg);
}

void swapTeamData(struct Team *a, struct Team *b){
    int id = a->teamId;
    char *name = a->name;
    int tp = a->totalPlayers;
    float absr = a->absr;

    a->teamId = b->teamId;
    a->name = b->name;
    a->totalPlayers = b->totalPlayers;
    a->absr = b->absr;

    b->teamId = id;
    b->name = name;
    b->totalPlayers = tp;
    b->absr = absr;
}

void teamsByAvgStrickRate(){
    calAvgSR();

    struct Team *i = tHead;
    while(i){
        struct Team *j = i->next;
        while(j){
            if(i->absr < j->absr){
                swapTeamData(i, j);
            }
            j = j->next;
        }
        i = i->next;
    }

    printf("===========================================================\n");
    printf("ID | Team Name | Avg Bat SR | Total Players\n");
    printf("===========================================================\n");

    struct Team *t = tHead;
    while(t){
        printf("%d | %s | %.2f | %d\n",
               t->teamId, t->name, t->absr, t->totalPlayers);
        t = t->next;
    }
}

void topPlayersByRole(){
    printf("\nChoice4 -> Display Top K Players of a Specific Team of specific role\n");
    int teamId, roleId, k;

    printf("EnterTeamID:");
    if(!getInput(&teamId,'d')) return;
    if(teamId<0 || teamId>10){
        printf("\nTeamId must be between 1-10");
        return;
    }

    printf("\nEnterRole(1-Batsman,2-Bowler,3-All-rounder):");
    if(!getInput(&roleId,'d')) return;
    if(roleId<0 && roleId>3){
        printf("\nRoleId must be between 1-3");
        return;
    }

    printf("\nEnter number of players:");
    if(!getInput(&k,'d')) return;
    if(k<0 && k>50){
        printf("\nplayers must be between 1-50");
        return;
    } 

    struct Player *player = pHead;
    int count=0;

    printf("\nTop %d Players:\n", k);
    printf("=============================================================\n");
    printf("ID | Name | Runs |  Avg  | SR  | Wkts | PerfIdx\n");
    printf("=============================================================\n");

    while(player && k){
        if(player->teamId==teamId && player->roleId==roleId && k!=0){
            printf("\n%d | %s | %d | %f | %f | %f | %d | %f",player->playerId,player->name,player->totalRun,player->battingAvg,player->strikeRate,player->wicket,player->performanceIndex);
            count++;    
            k--;
        }
        player=player->next;
    }
    if(count==0) printf("No players found!");
}

void specificPlayersByPerformance(){
    int roleId;
    char roleName[51],teamName[51];
    printf("\nEnterRole(1-Batsman,2-Bowler,3-All-rounder):");
    if(!getInput(&roleId,'d')) return;
    if(roleId<1 || roleId>3){
        printf("\nRoleId must be between 1-3");
        return;
    }
    printf("\n%s of all Teams:\n", *(roles + roleId - 1));
    printf("\n======================================================================================");
    printf("\nID  |     Name     |     Team     |  Role  |  Runs  |  Avg  |  SR  |  Wkts  |  ER  |  Perf.Index");
    struct Player *player = pHead;
    while(player!=NULL){
        if(player->roleId==roleId){
            struct Team *t = tHead;
        while(t){
            if(t->teamId == player->teamId){
                strcpy(teamName, t->name);
                break;
            }
            t = t->next;
        }
        strcpy(roleName, *(roles + roleId - 1));
        printf("\n%d | %s | %s | %s | %d | %.2f | %.2f | %d | %.2f | %.2f",
        player->playerId,player->name,teamName,roleName,player->totalRun,player->battingAvg,player->strikeRate,player->wicket,player->ecoomicRate,player->performanceIndex);
        }
        player = player->next;
    }
}

void freePlayersAndTeams(){
    struct Team *temp = tHead;
    while (temp != NULL) {
        struct Team *next = temp->next;
        free(temp->name);    
        free(temp);          
        temp = next;
    }
    struct Player *temp1 = pHead;
    while (temp1 != NULL) {
        struct Player *next1 = temp1->next;
        free(temp1->name);    
        free(temp1);          
        temp1 = next1;
    }
}

int main(){
    initializeTeamAndPlayers();
    do{
        showMenu();
        printf("\nEnter your choice: ");
        int inp;
        if(!getInput(&inp,'d')) return 0;
        switch (inp){
        case 1: addPlayer();
            break;
        case 2: playersByTeamId();
            break;
        case 3: teamsByAvgStrickRate();
            break;
        case 4: topPlayersByRole();
            break;
        case 5: specificPlayersByPerformance();
            break;
        case 6: {
            freePlayersAndTeams();
            printf("exiting..");
            exit(0);
        }
            break;
        default: printf("Choice a number between 1 to 6.");
            break;
        }
    } while (1);
}