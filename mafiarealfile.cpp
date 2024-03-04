#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <map>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

typedef map<int , string> inttostrmap;

 int randint(int x, int y){
     int a = rand() % (y - x + 1) + x;
     return a;
 }

int Choiceint(int a, int b){
    int y = randint(0,1);
    if (y == 0){
        return a;
    } else {
        return b;
    }
}

int Choiceint3(int a, int b, int c){
    int y = randint(0,2);
    if (y == 0){
        return a;
    } else if (y == 1){
        return b;
    } else {
        return c;
    }
}

struct Role{
    int id;
    inttostrmap mp = {{0, "hooker"},{1, "mafia"},{2, "doctor"}, {3, "civilian"}};
    string rolename = mp[id];
};

struct Player{
    int index;
    string name;
    Role r;
    int lifecount = 1;
    int lastinterractedwhis = -1;
    bool isattacked = false;
    bool isbanned = false;
    bool isprotected = false;
    vector <int> spvec = {};
};

struct lessid{
    inline bool operator() (const Player& p1, const Player p2){
        return (p1.r.id < p2.r.id);
    }
};

enum gamestate{
    cont,
    mfwin,
    cvwin,
};

void kill(int p, vector<Player>& v) {
    int g = 0;
    v[p].lifecount = v[p].lifecount - 1;
    cout << v[p].name << " was killed. his role was ";
    cout << v[p].r.rolename << "." << endl;
    auto iter = v.cbegin();
    v.erase(iter + p);
    if (v[p].r.id == 0) {
        kill(v[p].lastinterractedwhis, v);
    }
}

void listofplayers(bool showrole, vector<Player> vect){
    int i = 0;
    if (!showrole){
        for (i = 0; i+1 <= size(vect); ++i){
            cout << i + 1 << ") " << vect[i].name << ";" << endl;
        }
    }else {
        for (i = 0; i+1 <= size(vect); ++i) {
            cout << i + 1 << ") " << vect[i].name << "    " << "his role " << vect[i].r.rolename << ";" << endl;
        }
    }

};

void action(Player p, vector<Player>& vec, Player u){
    int dealer = p.index;
    int target = randint(0, vec.size()-1);
    int id = p.r.id, tg;
    if (dealer != u.index) {
        if ((id == 1) and !(vec[dealer].isbanned)) {
            vec[target].isattacked = true;
            vec[dealer].lastinterractedwhis = target;
        } else if ((id == 2) and !(vec[dealer].isbanned)) {
            vec[target].isprotected = true;
            vec[dealer].lastinterractedwhis = target;
        } else if (id == 0) {
            vec[target].isbanned = true;
            vec[dealer].lastinterractedwhis = target;
        }
    } else if (id == 1) {                              //player - active role
        cout << "It's up to you who doesn't wake up tonight." << endl;
        listofplayers(false, vec);
        cin >> tg;
        --tg;
        if (!(vec[dealer].isbanned)){
            vec[tg].isattacked = true;
            vec[dealer].lastinterractedwhis = target;
        }
    } else if ((id == 2)){
        cout << "It's up to you to decide who to safe tonight." << endl;
        listofplayers(false, vec);
        cin >> tg;
        --tg;
        if (!(vec[dealer].isbanned)){
            vec[tg].isprotected = true;
            vec[dealer].lastinterractedwhis = target;
        }
    } else if (id == 0) {
        cout << "Choose who you go to tonight." << endl;
        listofplayers(false, vec);
        cin >> tg;
        --tg;
        vec[tg].isbanned = true;
        vec[dealer].lastinterractedwhis = target;
    }
}

string printdays(int days){
    if (days == 1){
        return "1`st day";
    } else if (days == 2){
        return "2`nd day";
    } else if (days == 3){
        return "3`rd day";
    } else {
        return to_string(days) + "`th day";
    }
}

void firstnight(vector<Player> vecp, vector<Role> vecrol) {
    cout << "Thank you for using our service to play mafia!" << endl;
    /*cout << "we present to your attention today's players: " << endl;
    listofplayers(false, vecp);
    cout << "among them: " << endl;
    cout << "civilians: " << 4 << endl;
    cout << "mafia: " << 1 << endl;
    cout << "doctors: " << 1 << endl;
    cout << "hookers: " << 1 << endl;
    cout << "your role: " << vecp[6].r.rolename << endl;*/
}

int votingrez(vector <Player> vecpl){
    int targetbot = randint(0, vecpl.size()-1);
    int input;
    cout << "Who are you voting for (enter number): "<< endl;
    cin >> input;
    int targetuser = input - 1;
    return Choiceint3(targetbot, targetuser, -1);
}

void day(vector<Player>& vecp, int& dayc) {
    int g = 0;
    cout << printdays(dayc) << endl;
    cout << "stayed alive: " << endl;
    listofplayers(false, vecp);
    int tg = votingrez(vecp);
    if (tg == -1) {
        cout << "no one was gotten out" << endl;
    } else {
        vecp[tg].lifecount -= 1;
        cout << vecp[tg].name << " was gotten out" << endl;
    }
    if (vecp[tg].lifecount <= 0) {
        cout << "his role was " << vecp[tg].r.rolename << endl;
        auto iter = vecp.cbegin();
        vecp.erase(iter + tg);
        cout << vecp.size() << endl;

    } else {
        cout << "hi`s save, because he vas lucky" << endl;
    }
}

vector<Player>& srtbyrl(vector <Player> v){
    sort(v.begin(), v.end(), lessid());
    return v;
}

void night(vector<Player>& v){
    vector<Player> vecbyroles = v;
    sort(vecbyroles.begin(), vecbyroles.end(), lessid());
    cout << "start night" << endl;
    int i = 0, y = 0, iskill = 0;
    for (i; i + 1 <= vecbyroles.size(); ++i){

        action(vecbyroles[i], v,  v[6]);
    }
    for (y = 0; y + 1 <= v.size(); ++y){
        if (v[y].isattacked and !(v[y].isprotected)){
            kill(y, v);
            ++iskill;
        }
    }
    if (iskill == 0){
        cout << "surprisingly, everyone survived that night." << endl;
    }
    for (i = 0; i + 1 <= v.size(); ++i){
        v[i].isprotected = 0;
        v[i].isbanned = 0;
        v[i].isattacked = 0;
        v[i].lastinterractedwhis = -1;
    }
    cout << "end night" << endl;

}

struct ifmf{
    inline bool operator()(Player p){
        return(p.r.id == 1);
    }
};

gamestate wininteractor(vector<Player> v) {
    auto mfit = find_if(v.begin(), v.end(), ifmf());
    if (mfit != v.end()){
         if (v.size() <= 2){
             return mfwin;
         } else {
             return cont;
         }
    } else {
        return cvwin;
    }
}

void gamemain(vector <Player>& vecpl, vector <Role> vecrl, int dc){
    firstnight(vecpl, vecrl);
    while (wininteractor(vecpl) == cont){
     day(vecpl, dc);
     cout << "endd" << endl;
     if (wininteractor(vecpl) == cont){
         night(vecpl);
     }
    }
    if (wininteractor(vecpl) == mfwin){
        cout << "mafia wins" << endl;
    } else {
        cout << "civilians win" << endl;
    }
    listofplayers(true, vecpl);
}

int main() {
    int dayscount = 1, i;
    srand(time(0));
    cout << "enter your name" << endl;
    string pln = "elena";
    //cin >> pln;
    // vector of player creating
    vector<Role> vecrole = {{.id = 0},
                            {.id = 1},
                            {.id = 2},
                            {.id = 3},
                            {.id = 3},
                            {.id = 3},
                            {.id = 3}};
    random_shuffle(vecrole.begin(), vecrole.end(), [&](int i) {
        return std::rand() % i;
    });
    Player bot1 = {.index = 0, .name = "Bot1", .r = vecrole[bot1.index]};
    Player bot2 = {.index = 1, .name = "Bot2", .r = vecrole[bot2.index]};
    Player bot3 = {.index = 2, .name = "Bot3", .r = vecrole[bot3.index]};
    Player bot4 = {.index = 3, .name = "Bot4", .r = vecrole[bot4.index]};
    Player bot5 = {.index = 4, .name = "Bot5", .r = vecrole[bot5.index]};
    Player bot6 = {.index = 5, .name = "Bot6", .r = vecrole[bot6.index]};
    Player user = {.index = 6, .name = pln, .r = vecrole[user.index]};
    Player &linkbot1 = bot1;
    Player &linkbot2 = bot2;
    Player &linkbot3 = bot3;
    Player &linkbot4 = bot4;
    Player &linkbot5 = bot5;
    Player &linkbot6 = bot6;
    Player &linkuser = user;
    vector<Player> vecpl = {linkbot1, linkbot2, linkbot3, linkbot4, linkbot5, linkbot6, linkuser};
    //firstnight(vecpl, vecrole);
    vector<Player> &alive = vecpl;
    vector<Player> &stayalive = alive;
    for (i; i + 1 <= alive.size(); ++i) {
        cout << alive[i].name << " " << alive[i].r.rolename << endl;
    }
    int &linkdc = dayscount;
    // gamemain(vecpl, vecrole, linkdc);
    gamemain(alive, vecrole, linkdc);
    return 0;
}