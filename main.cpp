#include <allegro.h>
#include <iostream>
#include "Graphe.h"

int main()
{
    std::cout<<"Quel fichier voulez-vous ouvrir ? ";
    std::string nom;
    std::cin>>nom;
    Graphe piscine(nom);
    std::cout<<"Quel fichier de ponderation voulez-vous ouvrir ?";
    std::cin>>nom;
    piscine.ponderation(nom);
    piscine.affichage();
    return 0;
}
END_OF_MAIN();
