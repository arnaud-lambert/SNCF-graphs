#include <allegro.h>
#include <iostream>
#include "Graphe.h"

int main()
{
    std::cout<<"Quel fichier voulez-vous ouvrir ? ";
    std::string nom;
    std::cin>>nom;
    Graphe piscine(nom);
    piscine.affichage();
    return 0;
}
END_OF_MAIN();
