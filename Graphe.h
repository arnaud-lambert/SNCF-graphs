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
        Graphe(std::string& nomFichier);
        virtual ~Graphe();
        void affichage()const;
        void ponderation();
        void dessiner ();
        std::vector<std::pair<double, double>> vecteurPropre();
        std::vector<Sommet*> getSommets() { return m_sommets;}
        std::vector<std::pair<int, double>> centraliteDegre ();
        void rechercheCC();
        void supprimerArete ();
        void testConnexite ();
        void sauvegarder(std::vector<std::pair<int, double>> centralite_degres, std::vector<std::pair<double, double>> vecteurPropre, std::vector<std::pair<double, double>> vecteurProximite, std::vector<std::pair<double, double>> intermediarite, std::string nomFichier);
        std::vector<std::pair<double,double>> intermediarite();
        std::vector<std::pair<double, double>> vecteurProximite();

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
