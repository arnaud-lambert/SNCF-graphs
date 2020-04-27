#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"

class Arete
{
    public:
        Arete(int indice, Sommet* s1, Sommet* s2);
        virtual ~Arete();
        void affichage()const;

    private:
        int m_indice;
        std::pair<Sommet*, Sommet*> m_som;
};

#endif // ARETE_H
