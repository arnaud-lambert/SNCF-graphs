#include <iostream>
#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
    a.testConnexite();
    a.dessiner();

    return 0;
}
