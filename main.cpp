#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<Sommet*, double>> vecteurPropre=a.vecteurPropre();

    a.testConnexite();

    std::vector<std::map<Sommet*, std::pair<Sommet*, double>>> vecteurProximite=a.vecteurProximite();
    a.intermediarite();

    a.dessiner();

    return 0;
}
