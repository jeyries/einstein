// compile with: g++ -O3 einstein.cpp
// time ./a.out 

#include <iostream>
#include <cassert>    
#include <string>   
#include <algorithm>   


/*
Enigme d'Einstein

Il y a cinq maisons de cinq couleurs différentes, alignées le long d'une route. 
Dans chacune de ces maisons vit une personne de nationalité différente. 
Chacune de ces personnes boit une boisson différente, fume une marque de cigare différente et a un animal domestique différent.

Il y a cinq maisons de cinq couleurs différentes, alignées le long d'une route. Dans chacune de ces maisons vit une personne de nationalité différente. Chacune de ces personnes boit une boisson différente, fume une marque de cigare différente et a un animal domestique différent.
1. Le Britannique vit dans la maison rouge.
2. Le Suédois a des chiens.
3. Le Danois boit du thé.
4. La maison verte est directement à gauche de la maison blanche.
5. Le propriétaire de la maison verte boit du café.
6. La personne qui fume des Pall Mall élève des oiseaux.
7. Le propriétaire de la maison jaune fume des Dunhill.
8. La personne qui vit dans la maison du centre boit du lait.
9. Le Norvégien habite dans la première maison.
10. L'homme qui fume des Blend vit à côté de celui qui a des chats.
11. L'homme qui a un cheval est le voisin de celui qui fume des Dunhill.
12. Celui qui fume des Bluemaster boit de la bière.
13. L'Allemand fume des Prince.
14. Le Norvégien vit juste à côté de la maison bleue.
15. L'homme qui fume des Blend a un voisin qui boit de l'eau.

Qui a le poisson ?

*/

enum Column { maison, nationalite, boisson, cigare, animal };
enum Maison { blanc, bleu, jaune, rouge, vert };
enum Nationalite { norvegien, britannique, danois, allemand, suedois };
enum Boisson { biere, cafe, eau, lait, the };
enum Cigare { blend, bluemaster, dunhill, pallMall, prince };
enum Animal { poisson, cheval, chiens, oiseaux, chats };

int permutation[120][5];

void precalc() {
    int values[5];
    for (int i=0; i<5; i++) { 
        values[i] = i;
    }

    int m = 0;
    do {
        for (int i=0; i<5; i++) { 
            permutation[m][i] = values[i];
        }
        m ++;
    } while ( std::next_permutation(values, values+5) );
    assert(m == 120);
}

int find( int *column, int value) {
    for (int i=0; i<5; i++) {
        if (column[i] == value) {
            return i;
        }
    }
    assert(false);
}

bool check(int *maison, int *nationalite, int *boisson, int *cigare, int *animal) {

    int i, j;
    // 1. Le Britannique vit dans la maison rouge.
    if (maison[find(nationalite, britannique)] != rouge) { return false; }
    // 2. Le Suédois a des chiens.
    if (animal[find(nationalite, suedois)] != chiens) { return false; }
    // 3. Le Danois boit du thé.
    if (boisson[find(nationalite, danois)] != the) { return false; }
    // 4. La maison verte est directement à gauche de la maison blanche.
    i = find(maison, blanc); 
    j = find(maison, vert);
    if (j != i-1) { return false; }
    // 5. Le propriétaire de la maison verte boit du café.
    if (boisson[find(maison, vert)] != cafe) { return false; }
    // 6. La personne qui fume des Pall Mall élève des oiseaux.
    if (animal[find(cigare, pallMall)] != oiseaux) { return false; }
    // 7. Le propriétaire de la maison jaune fume des Dunhill.
    if (cigare[find(maison, jaune)] != dunhill) { return false; }
    // 8. La personne qui vit dans la maison du centre boit du lait.
    if (boisson[2] != lait) { return false; }
    // 9. Le Norvégien habite dans la première maison.
    if (nationalite[0] != norvegien) { return false; }
    // 10. L'homme qui fume des Blend vit à côté de celui qui a des chats.
    i = find(cigare, blend);
    j = find(animal, chats);
    if (abs(i-j) != 1) { return false; }
    // 11. L'homme qui a un cheval est le voisin de celui qui fume des Dunhill.
    i = find(animal, cheval);
    j = find(cigare, dunhill);
    if (abs(i-j) != 1) { return false; }
    // 12. Celui qui fume des Bluemaster boit de la bière.
    if (boisson[find(cigare, bluemaster)] != biere) { return false; }
    // 13. L'Allemand fume des Prince.
    if (cigare[find(nationalite, allemand)] != prince) { return false; }
    // 14. Le Norvégien vit juste à côté de la maison bleue.
    i = find(nationalite, norvegien);
    j = find(maison, bleu);
    if (abs(i-j) != 1) { return false; }
    // 15. L'homme qui fume des Blend a un voisin qui boit de l'eau.
    i = find(cigare, blend);
    j = find(boisson, eau);
    if (abs(i-j) != 1) { return false; }    

    return true;
}

void solve(int *result) {
    for (int a=0; a<120; a++) {
        int *maison = permutation[a];
        std::cout << "crunching ... " <<  a  << std::endl; 
        for (int b=0; b<120; b++) { 
            int *nationalite = permutation[b];
            if (nationalite[0] != norvegien) { continue; } // optimisation: 9. Le Norvégien habite dans la première maison.
            for (int c=0; c<120; c++) {
                int *boisson = permutation[c];
                if (boisson[2] != lait) { continue; } // optimisation: 8. La personne qui vit dans la maison du centre boit du lait.
                for (int d=0; d<120; d++) {
                    int *cigare = permutation[d];
                    for (int e=0; e<120; e++) {
                        int *animal = permutation[e];
                        if (check(maison, nationalite, boisson, cigare, animal)) {
                            result[0] = a;
                            result[1] = b;
                            result[2] = c;
                            result[3] = d;
                            result[4] = e;
                            return;
                        }
                    }
                }
            }
        }
    }
}


int main () {

    precalc();

    int result[5];
    solve(result);

    int *nationalite = permutation[result[1]];
    int *animal = permutation[result[4]];
    int answer = nationalite[find(animal, poisson)];

    std::string str[5] = { "Norvégien", "Britannique", "Danois", "Allemand", "Suédois" };
    std::cout << str[answer] << std::endl;
  
    return 0;
}

