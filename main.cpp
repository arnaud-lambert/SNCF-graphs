#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<Sommet*, double>> vecteurPropre=a.vecteurPropre();
    a.testConnexite();
    a.dessiner();

    return 0;
}
