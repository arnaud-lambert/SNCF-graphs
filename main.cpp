#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, float>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<Sommet*, double>> vecteurPropre=a.vecteurPropre();
    a.dessiner();

    return 0;
}
