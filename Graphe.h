#ifndef GRAPHE_H
#define GRAPHE_H
#include "Arete.h"
#include <fstream>

class Graphe
{
    public:
        Graphe(std::string nomFichier);
        virtual ~Graphe();
        void affichage()const;

    private:
        std::vector<Sommet*> m_som;
        std::vector<Arete*> m_arr;
        bool m_orientation;
        int m_ordre;
        int m_taille;
};

#endif // GRAPHE_H
