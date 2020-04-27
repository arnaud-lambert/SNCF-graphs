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
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_aretes;
        bool m_orientation;
        bool m_ponderation;
        int m_ordre;
        int m_taille;
};

#endif // GRAPHE_H
