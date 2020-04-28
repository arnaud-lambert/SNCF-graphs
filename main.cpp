#include <iostream>
#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, float>> centralite_degres = a.centraliteDegre ();
    a.supprimerArete();
    a.rechercheCC();
    a.dessiner();

    return 0;
}
