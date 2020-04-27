#include <allegro.h>
#include <iostream>
#include "Graphe.h"

int main()
{
    std::cout<<"Quel fichier voulez-vous ouvrir ? ";
    std::string nom;
    std::cin>>nom;
    Graphe piscine(nom);
    piscine.ponderation();
    piscine.affichage();
    piscine.vecteurPropre();
    piscine.dessiner();
    return 0;
}
END_OF_MAIN();
