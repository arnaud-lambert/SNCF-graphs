#include <iostream>
#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, float>> centralite_degres = a.centraliteDegre ();
    a.connexite();
    a.dessiner();

    return 0;
}
