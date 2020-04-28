#include "Graphe.h"

int main()
{
    Graphe a;
    a.ponderation();
    a.affichage();
    std::vector<std::pair<int, float>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<Sommet*, double>> vecteurPropre=a.vecteurPropre();
    std::vector<std::map<Sommet*, std::pair<Sommet*, double>>> vecteurProximite=a.vecteurProximite();
    a.intermediarite();
    a.dessiner();

    return 0;
}
