#ifndef GRAPHE_H
#define GRAPHE_H
#include "Arete.h"
#include <fstream>
#include <math.h>
#include <cmath>
#include <unordered_map>
#include <queue>

class Graphe
{
    public:
        Graphe();
        virtual ~Graphe();
        void affichage()const;
        void ponderation();
        void dessiner ();
        std::vector<std::pair<Sommet*, double>> vecteurPropre();
        std::vector<Sommet*> getSommets() { return m_sommets;}
        std::vector<std::pair<int, float>> centraliteDegre ();
        std::vector<double> intermediarite();

    private:
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_aretes;
        bool m_orientation;
        bool m_ponderation;
        int m_ordre;
        int m_taille;
};

void recursif (int &k, Sommet* i, Sommet* current, std::unordered_map<Sommet*, std::pair<std::vector<Sommet*>,double>> &predecesseurs);

#endif // GRAPHE_H
