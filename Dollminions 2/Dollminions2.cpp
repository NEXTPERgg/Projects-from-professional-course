//TRÊS DE MAIO OPEN DAY - VICTOR BAPTISTA 
#include <iostream> //não sei
#include <string> // modificações de strings basicas em C++
#include <stdlib.h> //varias funções de conversões e cmd
#include <random> //motor de rng
#include <cstdio> //funções basicas de C usando elas em C++
#include <cstring> //modificações de string C usando elas em C++ (strcpy strcmp)
#include <chrono> //usar funções relacionadas com o tempo do sistema (para RNG)
#include <thread> //varios "mains" ao mesmo tempo, faz com que possa ter funções a correrem ao msm tempo
#include <cmath> //funções matematicas
#include <algorithm> //comparações de valores 
#include <limits> //Impedir que o jogador meta valores fora dos maximos e minimos de um int (and such)
#include <fstream>//abrir ascii em txts durante o jogo
#include <windows.h> //para poder usar os characteres UTP-8 (Os que estão em Português) 
using namespace std;

struct MOVEAC {
    string name; 
    string type;
    int id, power, state;
    double hit_chance;
	
};

struct Bios {
    string bios[7] = {
        " ",
        "Tomatom may be small but he can surely pack a punch with his signature attack Tomato Storm!",
        "Molmoly's favorite activities involve digging, digging, digging, and sometimes throwing rocks at people!",
        "Fury Shadows prefer to hunt at night and they never need to cook their prey since their flamethrower is really powerful!",
        "These cold-hearted creatures roam in packs. It's very hard to tame one, but once you do, it's your loyal companion forever!",
        "Seadett is constantly seeking new challenges and is dedicated to satisfying its superiors. These creatures are willing to go to great lengths to accomplish their goals!",
        "The Corrupt type baffles scientists to this day, seemingly unaffected by other Types. Verderven, a timid and mysterious creature, defies containment to most trainers. Its power is wielded only by Champions!"
    };
};

struct DOLL {
	
    string name; 
    string type;
    string Bio;
    int BioID, asciiPlayer, asciiOpponent, atkup, defup, stkup, sdfup, spdup; 
    float hp, atk, def, stk, sdf, spd; 
    MOVEAC moves[4]; 
    string leftSprite;
    string rightSprite;

};

int RNG(int min, int max) {    
    
    seed_seq seed{ 
        static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()),
        static_cast<unsigned int>(hash<std::thread::id>{}(std::this_thread::get_id())),
        random_device{}()
    };
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int CharToInt(char i){
	return i - '0';
}

void confirmation(char decision = ' ', int* Confirmation = nullptr) {
    int end = 0;
    if (decision == 'y' || decision == 'Y') {
        end = 1;
        *Confirmation = end;
    }
    else if (decision == 'n' || decision == 'N') {
        end = 2;
        *Confirmation = end;
    }
    else {
        end = 3;
        *Confirmation = end;
    }
}

class Modify{
	
	public:
	bool SuperEffective;
	bool CritcalHit;
	bool NotVeryEffective;
	bool SameTypeAttackBonus;
	Modify(bool Seff = false, bool Crit = false, bool Neff = false, bool STAB = false): SuperEffective(Seff), CritcalHit(Crit), NotVeryEffective(Neff), SameTypeAttackBonus(STAB){}
 	
	void CheckModifiers(MOVEAC* modifymove, DOLL* attacker, DOLL* defender){
    int critcheck;
    critcheck = RNG(1, 100);
    if (critcheck >= 95) {
        CritcalHit = true;
    }

    if ((modifymove->type != "corrupt") &&
    ((modifymove->type == "plant" && (defender->type == "water" || defender->type == "earth")) ||
     (modifymove->type == "earth" && (defender->type == "fire" || defender->type == "ice")) ||
     (modifymove->type == "ice" && (defender->type == "plant" || defender->type == "water")) ||
     (modifymove->type == "fire" && (defender->type == "plant" || defender->type == "ice")) ||
     (modifymove->type == "water" && (defender->type == "fire" || defender->type == "earth")))) {
    SuperEffective = true;
	}

    if ((modifymove->type != "corrupt") &&
    ((modifymove->type == "plant" && (defender->type == "fire" || defender->type == "plant")) ||
     (modifymove->type == "earth" && (defender->type == "plant" || defender->type == "earth")) ||
     (modifymove->type == "ice" && (defender->type == "earth" || defender->type == "ice")) ||
     (modifymove->type == "fire" && (defender->type == "water" || defender->type == "fire")) ||
     (modifymove->type == "water" && (defender->type == "ice" || defender->type == "water")))) {
    NotVeryEffective = true;
}

    if (modifymove->type == attacker->type) {
        SameTypeAttackBonus = true;
    }
}
	
	double CalcModifies(double basedmg){
        int RFcheck; //random factor check
        RFcheck = RNG(85, 100);
        if (SuperEffective) {
            basedmg *= 2;
        } else if (NotVeryEffective) {
            basedmg /= 2;
        } else if (CritcalHit) {
            basedmg *= 1.5;
        } else if (SameTypeAttackBonus) {
            basedmg *= 1.5;
        }
        basedmg *= RFcheck;
        basedmg /= 100;
        double modifiedDamage = basedmg;
        return modifiedDamage;
    }
	
	void MessageMiss(DOLL* attacker, MOVEAC* modifymove, bool Miss){
	cout << attacker->name << " used " << modifymove->name << " and it missed!" << endl;	
		SuperEffective = false;
        CritcalHit = false;
        NotVeryEffective = false;
        SameTypeAttackBonus = false;
	}
	
	void Message(DOLL* attacker, DOLL* defender, MOVEAC* modifymove, double basedmg, bool Seff, bool Neff, bool Crit)
	{
    string msg[5] = {" used ", " it missed!", " critical hit!", " it was super effective!", " it was not very effective..." };
    if (!Crit && !Seff && !Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << endl; 
    }
    else if (!Crit && Seff && !Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << msg[3] << endl; 
    }
    else if (!Crit && !Seff && Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << msg[4] <<endl; 
    }
	else if (Crit && !Seff && !Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << msg[2] << endl; 
    }
    else if (Crit && Seff && !Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << msg[2] << msg[3] << endl; 
    }
    else if (Crit && !Seff && Neff) 
	{
        cout << attacker->name << msg[0] << modifymove->name << msg[2] << msg[4] << endl; 
    }
		
		SuperEffective = false;
        CritcalHit = false;
        NotVeryEffective = false;
        SameTypeAttackBonus = false;
        
	}
	
	void Status_Message(DOLL* attacker, MOVEAC* modifymove)
	{
    string msg [50] = {
	" used ", //0
	" it missed!", //1
	
	", it's attack greatly increassed!", //2
	", it's defense greatly increassed!",  //3
	", it's special attack greatly increassed!",//4
	", it's special Defense greatly increassed!",//5
	", it's speed greatly increassed!",//6
	
	", it's attack was increassed!",//7
	", it's defense was increassed!",//8
	", it's special attack was increassed",//9
	", it's special Defense was increassed!",//10
	", it's speed was increassed!",//11
	
	", it's foe caught on fire! it migh be difficult to attack!",//12 fire
	", it's foe got sleepy! it migh fall asleep!",//13 plant
	", it's foe got confused! it migh hurt itself!",//14 earth
	", it's foe got frostbite! it migh be difficult to attack!",//15 ice
	", it's foe got sleepy! it migh fall asleep",//16 water unfinished
	
	", it's attack won't go any higher!",//17
	", it's defense won't go any higher!",//18
	", it's special attack won't go any higher!",//19
	", it's special defense won't go any higher!",//20
	", it's speed won't go any higher!"//21
	};
	switch (modifymove->id)
	{
		
		case 25:
			if(attacker->atkup <= 6)
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[2] <<endl;
			}
			else
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[17] <<endl;
			}
		break;
		
		case 26:
			if(attacker->defup <= 6)
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[3] <<endl;
			}
			else
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[18] <<endl;
			}
		break;
		
		case 27:
			if(attacker->stkup <= 6)
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[4] <<endl;
			}
			else
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[19] <<endl;
			}
		break;
		
		case 28:
			if(attacker->sdfup <= 6)
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[5] <<endl;
			}
			else
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[20] <<endl;
			}
		break;
		
		case 29:
			if(attacker->spdup <= 6)
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[6] <<endl;
			}
			else
			{
				cout << attacker->name << msg[0] << modifymove->name << msg[21] <<endl;
			}
		break;
	}
		SuperEffective = false;
        CritcalHit = false;
        NotVeryEffective = false;
        SameTypeAttackBonus = false;	
	}	

	};	

class BattleLogic{
	public:
	Modify modifier;
	string Battle;
	string names[5] = {"", "Roblox noob", "cRaZy DaVe", "Master Maria" ,"Champion"};
	int battlesWon = 1;
	
		void attack(DOLL* attacker, DOLL* defender, int moveindex)
		{
    	double basedmg;
    	bool Miss, crit; 
    	int finaldmg, misscheck;
    	struct MOVEAC chosenmove = attacker->moves[moveindex];

    	Miss = false;
    	misscheck = RNG(1,100);

    		if (misscheck >= chosenmove.hit_chance)
			{
        		Miss = true;
        		modifier.MessageMiss( attacker, &chosenmove, Miss);
    		}
    	
			else 
			{
        
				switch (chosenmove.state) 
				{
            	case 1:
                	modifier.CheckModifiers(&chosenmove, attacker, defender);

					basedmg = ((chosenmove.power * (attacker->atk / defender->def)) / 10) + 2; 

					finaldmg = static_cast<int>(floor(basedmg));
				
					finaldmg = max(finaldmg, 1);
				
					finaldmg = modifier.CalcModifies(finaldmg);

					defender->hp -= finaldmg;

					modifier.Message(attacker, defender, &chosenmove, finaldmg, modifier.SuperEffective, modifier.NotVeryEffective, modifier.CritcalHit);

        
                break;
            
            	case 2:
                	modifier.CheckModifiers(&chosenmove, attacker, defender);

					basedmg = ((chosenmove.power * (attacker->stk / defender->sdf)) / 50) + 2; 

					finaldmg = static_cast<int>(floor(basedmg));
				
					finaldmg = max(finaldmg, 1);
				
					finaldmg = modifier.CalcModifies(finaldmg);

					defender->hp -= finaldmg;

					modifier.Message(attacker, defender, &chosenmove, finaldmg, modifier.SuperEffective, modifier.NotVeryEffective, modifier.CritcalHit);

                break;
            
            	case 3:
                	basedmg = 0;
                	switch (chosenmove.id) 
					{
                    	case 25:
                        if (attacker->atkup <7) {
                            attacker->atk *= 2;
                            attacker->atkup += 2;
                            modifier.Status_Message(attacker, &chosenmove);
                            
                        } else {
                        	modifier.Status_Message(attacker, &chosenmove);
                        }
                        break;
                        
                        case 26:
                        if (attacker->defup <7) {
                            attacker->def *= 2;
                            attacker->defup += 2;
                            modifier.Status_Message(attacker, &chosenmove);
                            
                        } else {
                        	modifier.Status_Message(attacker, &chosenmove);
                        }
                        break;
                        
                        case 27:
                        if (attacker->stkup <7) {
                            attacker->stk *= 2;
                            attacker->stkup += 2;
                            modifier.Status_Message(attacker, &chosenmove);
                            
                        } else {
                        	modifier.Status_Message(attacker, &chosenmove);
                        }
                        break;
                        
                        case 28:
                        if (attacker->sdfup <7) {
                            attacker->sdf *= 2;
                            attacker->sdfup += 2;
                            modifier.Status_Message(attacker, &chosenmove);
                            
                        } else {
                        	modifier.Status_Message(attacker, &chosenmove);
                        }
                        break;
                        
                        case 29:
                        if (attacker->spdup <7) {
                            attacker->spd *= 2;
                            attacker->spdup += 2;
                            modifier.Status_Message(attacker, &chosenmove);
                            
                        } else {
                        	modifier.Status_Message(attacker, &chosenmove);
                        }
                        break;
                    }
				break;    
                }
            
        	}
    	}
		
		void battleimage(DOLL*player, DOLL*opponent)
		{
		
    		string BattleNameHolder = "battle_" + to_string(player->asciiPlayer) + "_" + to_string(opponent->asciiOpponent) + ".txt";
    		
    		ifstream battleFile(BattleNameHolder);
    
   			if (battleFile.is_open()) 
   			{
        		
        		while (getline(battleFile, Battle)) 
				{
            	cout<<Battle<<endl;
        		}
			}
		
			else
			{
     			cerr << "Failed to open sprite files." << endl;
    		}
    		battleFile.close();
       		
	
		}
		
		int MoveChoice(DOLL*player, DOLL*opponent)
		{
			int moveChoice;
				while (true)
				{
					battleimage(player, opponent);
					cout<< "chose a move for "<< player->name<<" :"<<endl;
					for(int c = 0 ; c < 4; c++)
					{
						if(player->moves[c].state == 3){
							cout<<c + 1 <<". "<< player->moves[c].name <<" status move can\'t miss"<<endl;	
						}
						else{
							cout<<c + 1 <<". "<< player->moves[c].name <<" power:"<< player->moves[c].power<<" "<< player->moves[c].hit_chance<<"%"<<endl;	
						}
						
					}
					
					cout<<"Enter the number of the move: "<<endl;
					cin>> moveChoice;
			
					if (moveChoice >= 1 && moveChoice <= 4) 
	    			{
	        			return moveChoice;
	    			}
	    		
					else
	    			{
	        			cout << "Invalid input! Please enter a number between 1 and 4." <<endl;
	        			cin.clear();
	        			cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        			system("pause");
	        			system("cls");
	        			continue;
	   				} 
			}
		}
	
		void PlayerTurn(DOLL*attacker, DOLL*defender, int movechosen)
		{
			attack(attacker, defender, movechosen -1);
		}
		
		void AIturn(DOLL*attacker, DOLL*defender)
		{
			int moveChoice;
			moveChoice = RNG(1,4);
			attack(attacker, defender, moveChoice - 1);
		}	
		
		int isBattleOver(DOLL*attacker, DOLL*defender) {return (attacker->hp <= 0 || defender->hp <= 0);};
		
		void preBattle(DOLL*player, DOLL*opponent, int numberOfOpponent )
		{
			cout<<names[0]<<"\'s dollminion:"<<player->name<<endl;
			
			cout<<player->leftSprite<<endl;
			
			cout<<names[numberOfOpponent]<<"\'s dollminion:"<<opponent->name<<endl;
			
			cout<<opponent->rightSprite<<endl;
			system("pause");
					system("cls");
		}
		
		void StartBattle(DOLL*player, DOLL*opponent)
		{
		int endofbattle = isBattleOver(player,opponent);
		preBattle(player, opponent, battlesWon);
			do
			{
				
				int movechoice = MoveChoice(player, opponent);
					if (player->spd > opponent->spd)
					{	
						
						cout<<endl;
						PlayerTurn(player, opponent, movechoice );
						cout<<endl;
						AIturn(opponent, player);
						cout<<endl;
						cout<<"("<<player->name<<" "<<player->hp<<"hp "<<opponent->name<<" "<<opponent->hp<<"hp)"<<endl;
						movechoice = 0;
						endofbattle = isBattleOver(player,opponent);
						system("pause");
						system("cls");	
					}
				
					else if (player->spd < opponent->spd)
					{	
						
						cout<<endl;
						AIturn(opponent, player);
						cout<<endl;
						PlayerTurn(player, opponent, movechoice );
						cout<<endl;
						cout<<"("<<player->name<<" "<<player->hp<<"hp "<<opponent->name<<" "<<opponent->hp<<"hp)"<<endl;
						endofbattle = isBattleOver( player, opponent);
						system("pause");
						system("cls");	
					}
				
					else if (player->spd == opponent->spd)
					{
						int tie = RNG (1,2);
							if (tie == 1)
							{
								
								cout<<endl;
								PlayerTurn(player, opponent, movechoice );
								cout<<endl;
								AIturn(opponent, player);
								cout<<endl;
								cout<<"("<<player->name<<" "<<player->hp<<"hp "<<opponent->name<<" "<<opponent->hp<<"hp)"<<endl;
								endofbattle = isBattleOver( player, opponent);
								system("pause");
								system("cls");
							}
						
							else
							{
								
								cout<<endl;
								AIturn(opponent, player);
								cout<<endl;
								PlayerTurn(player, opponent, movechoice );
								cout<<endl;
								cout<<"("<<player->name<<" "<<player->hp<<"hp "<<opponent->name<<" "<<opponent->hp<<"hp)"<<endl;
								endofbattle = isBattleOver(player,opponent);
								system("pause");
								system("cls");
							}
					}	
			}while(!endofbattle);
		
				if (player->hp > opponent->hp )
				{
					cout<<"\nYou won!\n";
					cout<<player->leftSprite;
					system("pause");
					system("cls");
				}
			
				else
				{
					
					cout<<"\nYou Lost!\n";
					cout<<opponent->rightSprite;
					system("pause");
					system("cls");
					exit(0);
				}
		system("pause");
		system("cls");
	}	
};

class Game{
	public:
	BattleLogic obj;
	int playerID;
	string tournamentAscii;
	string tutorialAscii;
	int gameBeaten = 0;
	
	void giveSprite(DOLL *spritegiven) {
		
    string leftFileName = "doll_" + to_string(spritegiven->asciiPlayer) + ".txt";
    string rightFileName = "doll_" + to_string(spritegiven->asciiOpponent) + ".txt";

    ifstream tutorialFile("tutorial.txt");
    ifstream tournamentlFile("tournament.txt");
    	
    ifstream rightFile(rightFileName);
    ifstream leftFile(leftFileName);

   
   if (rightFile.is_open() && leftFile.is_open() && tutorialFile.is_open() && tournamentlFile.is_open()) 
   {
        string line;
        while (getline(rightFile, line)) 
		{
            spritegiven->rightSprite += line + "\n"; 
        }
        
       	while (getline(leftFile, line)) 
		{
         	   spritegiven->leftSprite += line + "\n"; 
    	}
		while (getline(tutorialFile, line)) 
		{
         	   tutorialAscii += line + "\n"; 
    	}
		while (getline(tournamentlFile, line)) 
		{
         	   tournamentAscii += line + "\n"; 
    	} 
    	
	}
		
	else
	{
     	cerr << "Failed to open sprite files." << endl;
    }	
    	tutorialFile.close();
    	tournamentlFile.close();
    	rightFile.close();
       	leftFile.close();
	}
	
	void showcaseEach(DOLL* dollArray, int size) {
    for (int i = 0; i < size; i++){
         
        cout << dollArray[i].leftSprite << endl; 
        cout << i + 1 << "-" << dollArray[i].name << endl;
        cout << "Type: " << dollArray[i].type << endl;
        cout << "Bio: " << dollArray[i].Bio << endl;
        cout << "(Press enter)" << endl;
        system("pause");
        system("cls");
    }
}
	
	void showcase(DOLL*dollminion) {
	
	cout << dollminion->leftSprite <<endl;
    cout << "Name: " << dollminion->name << endl;
    cout << "Type: " << dollminion->type << endl;
    cout << "Moves:" << endl;
    for (int i = 0; i < 4; ++i) {
        if(dollminion->moves[i].state == 3){
			cout<<i + 1 <<". "<< dollminion->moves[i].name <<" status move can\'t miss"<<endl;	
		}
		else{
		cout<<i + 1 <<". "<< dollminion->moves[i].name <<" power:"<< dollminion->moves[i].power<<" "<< dollminion->moves[i].hit_chance<<"%"<<endl;	
			}
    	}
	}
	
	int chooseDoll(DOLL doll[], int didHeWin) {
    int selection = 0;
    int flag4 = 0;
    
    showcaseEach(doll, 6);
    if(didHeWin>0){
    do {
		
        cout << "Choose your dollmon (1-6): ";
        cin >> selection;

        if (cin.fail() || selection < 1 || selection > 7) {
            cout << "Invalid input! Please enter a number between 1 and 6." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        showcase(&doll[selection - 1]);

        char confirmation2;
        cout << "Are you sure?(Y/N): ";
        cin >> confirmation2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        confirmation(confirmation2, &flag4);
        
        if (flag4 == 1) {
        	break;
        } else if (flag4 == 2) {
            system("cls");
            do {
    			char again = '\0';
    			int flagAgain = 0;
    			cout << "Do you want to see the Dollminions again? (Y/N): ";
    			cin >> again;
    			cin.ignore(numeric_limits<streamsize>::max(), '\n');
				system("cls");
    			confirmation(again, &flagAgain);

    			if (cin.fail() || flagAgain == 3) {
        			cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
        			cin.clear();
        			cin.ignore(numeric_limits<streamsize>::max(), '\n');
        			continue;
    			} else if (flagAgain == 1) {
        			showcaseEach(doll, 6);
        			system("cls");
        			break;
    			} else if (flagAgain == 2) {
        			break; 
    			} else {
        			cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
        			system("pause");
        			system("cls");
        			continue;
    			}
			} while (true);
			continue;
        } else {
            cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
            system("pause");
            system("cls");
            continue;
		}
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (true);
	
	system("pause");
	system("cls");
	
    return selection - 1;
		
	}else{
	do {
		
        cout << "Choose your dollmon (1-5): ";
        cin >> selection;

        if (cin.fail() || selection < 1 || selection > 6) {
            cout << "Invalid input! Please enter a number between 1 and 5." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (selection == 6){
        	cout<< "That one is only available when you defeat the champion!"<<endl;
        	cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("pause");
            system("cls");
            continue;
		}

        showcase(&doll[selection - 1]);

        char confirmation2;
        cout << "Are you sure?(Y/N): ";
        cin >> confirmation2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        confirmation(confirmation2, &flag4);
        
        if (flag4 == 1) {
        	break;
        } else if (flag4 == 2) {
            system("cls");
            do {
    			char again = '\0';
    			int flagAgain = 0;
    			cout << "Do you want to see the Dollminions again? (Y/N): ";
    			cin >> again;
    			cin.ignore(numeric_limits<streamsize>::max(), '\n');
				system("cls");
    			confirmation(again, &flagAgain);

    			if (cin.fail() || flagAgain == 3) {
        			cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
        			cin.clear();
        			cin.ignore(numeric_limits<streamsize>::max(), '\n');
        			continue;
    			} else if (flagAgain == 1) {
        			showcaseEach(doll, 6);
        			system("cls");
        			break;
    			} else if (flagAgain == 2) {
        			break; 
    			} else {
        			cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
        			system("pause");
        			system("cls");
        			continue;
    			}
			} while (true);
			continue;
        } else {
            cout << "Invalid input! Please type either Y or N to confirm or cancel." << endl;
            system("pause");
            system("cls");
            continue;
		}
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (true);
	
	system("pause");
	system("cls");
	
    return selection - 1;	
	}
	
}
	
	void choseName() {
    int flag1 = 0;
    int flag3 = 0;
    string flag2;
	
    do {
        flag1 = 0;
        flag3 = 0;
        
        cout << "Welcome to Dollminions! What is your name?" << endl;
        getline(cin, obj.names[0]); 
        
        
        if (obj.names[0] == "") 
		{
        obj.names[0] = "Ermac";
    	}
        cout << "\"" << obj.names[0] << "\"" << " Are you sure? (Y/N): ";
        getline(cin, flag2);
		char confirmationChar = (flag2.length() > 0) ? flag2[0] : ' ';

        confirmation(confirmationChar, &flag1);
	
        if (flag1 == 1) {
            flag3 = 1;
        } else if (flag1 == 2) {
            system("cls");
            obj.names[0] = " "; 
            
            
        } else {
            cout << "\nInvalid input, please type either Y or N to confirm or not." << endl;
            system("pause");
            system("cls");
            obj.names[0] = ""; 
        }

    } while (!flag3); 
    system("cls");
}
	
	void tournament(){
		system("cls");
		cout<<"It is time to Join the tournament!"<<endl;
		cout<<tournamentAscii;
		system("pause");
		system("cls");
	}
	
	void tutorial(){
		do{
		char wantToSee = '\0';
		int yesOrNo = 0;
		cout<<"Do you want to see the tutorial?/Queres ver o tutorial? (Y/N): ";
		cin>>wantToSee;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		confirmation(wantToSee, &yesOrNo);
		if(yesOrNo == 1){
			cout<<tutorialAscii;
		
			cout << "\nIn Dollminion, you, " << obj.names[0] << ", are a Dollminion Trainer and will fight against other Dollminion trainers.\nEach Dollminion has a type, and there are 5 types in total: Fire, Water, Plant, Ice, Earth, as shown in the graphic above, with their strengths and weaknesses.\nYou will participate in a Dollminion tournament and fight against 3 different trainers and then the Champion!\nGood luck!\n";

			cout << "\nNo mundo dos Dollminions, tu, " << obj.names[0] << ", és um Treinador de Dollminions e vais lutar contra outros treinadores.\nCada Dollminion tem um tipo, e existem 5 tipos no total: Fogo, Água, Planta, Gelo, Terra, como está mostrado no gráfico acima, com seus pontos fortes e fracos.\nParticiparás num torneio de Dollminions e lutarás contra 3 treinadores diferentes e depois o Campeão!\nBoa sorte!\n";

    		system("pause");
    		
    		
    		tournament();
    		break;
		}else if(yesOrNo == 2){
			tournament();
			break;
		}else{
			cout << "Invalid input! Please type either Y or N to confirm or cancel./Escreve Y para \"sim\" e N para \"não\"" << endl;
            system("pause");
            system("cls");
            continue;
		}
    	
    	}while(true);
	}
	
	void credits(DOLL*player){
		
		cout<<player->leftSprite<<endl;
		cout<<"\nCongrats with becoming the Champion of the Dollminion world!\nIf you enjoyed and want more, this world has just began!\n\n Credits:\n Programing made by Victor Baptista\n Art made by Geraldine Vazquez\n If you want awesome art: https://www.instagram.com/wumy_02/\n\n"<<endl;
		cout<<"\nParabens por te teres tornado o Campeão do mundo dos Dollminons!\nSe gostaste e queres mais, este mundo acabou de começar!\n\n Creditos:\n Programação feita por Victor Baptista\n Arte feita por Geraldine Vazquez\n Se quiseres arte incrivel: //www.instagram.com/wumy_02/\n\n"<<endl;
		system("pause");
	}
	
	void newGame(DOLL *player, DOLL *dolly) {
    if (player == nullptr || dolly == nullptr) {
        cout << "Error: Invalid pointers to player or doll array." << endl;
        return;
    }
    
    for(int i = 0; i<6 ; i++)
	{
		tutorialAscii.clear();
    	tournamentAscii.clear();
		giveSprite(&dolly[i]);
	}
	
	choseName();
	
    playerID = chooseDoll(dolly, gameBeaten);
    
    *player = dolly[playerID];
    
    tutorial();
	
	}

};	

int main(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  

	setlocale(LC_ALL, "Portuguese");
	
	BattleLogic logicbattle;
	Game game;
	Bios biosobj;
	
	MOVEAC moverefined[50] = {
    {"grass blade", "plant",1 ,40 ,1 ,90.0}, //0
    {"pickaxe smash", "earth",2 ,50 ,1 ,65.0}, //1
    {"fire clash", "fire",3 ,50 ,1 ,70.0}, //2
    {"ice spiner", "ice",4 , 40, 1, 100.0}, //3
    {"water jet", "water",5 , 45, 1, 95.0}, //4
    
    {"tomato storm","plant",6, 60, 2, 65.0}, //5
    {"earth control", "earth",7, 55, 2, 70.0}, //6
    {"flamethrower", "fire",8, 45, 2, 100.0}, //7
    {"ice beam", "ice",9, 45, 2, 85.0}, //8
    {"water cannon", "water",10, 60, 2, 70.0}, //9
    
    {"grass knife", "plant",11, 25, 1, 100.0}, //10
    {"rock throw", "earth",12, 20, 1, 85.0}, //11
    {"flame tackle", "fire",13,  30, 1, 90.0}, //12
    {"snowball throw", "ice",14, 15, 1, 100.0}, //13
    {"bubble punch", "water",15, 30, 1, 90.0}, //14
    
    {"tomato bowl", "plant",16, 25, 2, 95.0},//15
    {"mud control", "earth",17, 20, 2, 85.0}, //16
    {"ember", "fire",18, 35, 2, 95.0}, //17
    {"ice shot", "ice",19, 20, 2, 90.0}, //18
    {"bubble", "water",20, 30, 2, 90.0}, //19
    
    {"smack down", "corrupt",21, 55, 1, 100.0}, //20
    {"tri attack", "corrupt",22, 55, 2, 100.0}, //21
    {"pound", "corrupt",23, 15, 1, 100.0}, //22
    {"sound wave", "corrupt",24, 15, 2, 100.0}, //23
    
    {"atkboost", "status",25, 1, 3, 101.0}, //24
    {"defboost", "status",26, 1, 3, 101.0}, //25
    {"stkboost", "status",27, 1, 3, 101.0}, //26
    {"sdfboost", "status",28, 1, 3, 101.0}, //27
    {"spdboost", "status",29, 1, 3, 101.0} //28
};

	DOLL doll[6] = {
        //name type Bio id asplayer asopponent exp lv atkup defup stkup sdfup spdup hp atk def stk sdf spd 4moverefineds
        {"Tomatom", "plant", " ", 1, 1, 2, 0, 0, 0, 0, 0, 140, 15, 25, 40, 30, 100, {moverefined[5],moverefined[16],moverefined[14], moverefined[27]}},
        {"Molmoly", "earth"," ", 2, 3, 4, 0, 0, 0, 0, 0, 90, 50, 25, 10, 60, 40, {moverefined[1], moverefined[6], moverefined[11], moverefined[28]}}, 
        {"Fury Shadow","fire"," ", 3, 5, 6, 0, 0, 0, 0, 0, 80, 5, 20, 90, 75, 80, { moverefined[7],  moverefined[18],  moverefined[1],  moverefined[27]}},
        {"Ficen", "ice"," ", 4, 7, 8, 0, 0, 0, 0, 0, 90, 25, 35, 80, 60, 70, { moverefined[9],  moverefined[8],  moverefined[2],  moverefined[3]}},
        {"Seadett", "water"," ", 5, 9, 10, 0, 0, 0, 0, 0, 90, 45, 20, 45, 40, 60, { moverefined[9],  moverefined[19],  moverefined[11],  moverefined[15]}},
        {"Verderven", "corrupt"," ", 6, 12, 12, 0, 0, 0, 0, 0, 85, 60, 200, 30, 200, 50, { moverefined[21],  moverefined[22],  moverefined[26],  moverefined[28]}},
    };
     
    for (int i = 0; i < 6; ++i) {
        doll[i].Bio = biosobj.bios[doll[i].BioID];
    }
    //populate bios
    
	struct DOLL player, opponent;
	
	while(true){
		game.newGame(&player, doll);
	
	for(int i = 0; i < 3; i++)
	{
		opponent = doll[RNG(0,4)]; 
		logicbattle.StartBattle(&player, &opponent);
		player = doll[game.playerID];
		logicbattle.battlesWon++;
	}
	
	opponent = doll[5];
	logicbattle.StartBattle(&player, &opponent);
	
	game.credits(&player);
	
	char credits1 = '\0';
	int credits2 = 0;
	
	game.gameBeaten += 1;
	cout<<"Verdernvern Unlocked do you wish to play again? (Y/N): ";
	cin>>credits1;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	confirmation(credits1,&credits2);
		if(credits2 == 1){
			continue;
			system("cls");
		}else{
			break;
		}
	}
	
	cout<<"Utill Next time!"<<endl;
	system ("pause");
	
	return 0;
}
//Erik is gay
