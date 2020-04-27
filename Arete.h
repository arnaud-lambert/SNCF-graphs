#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        virtual ~Arete();
        void affichage()const;
        std::pair<Sommet*, Sommet*> getExtremites() { return m_extremites; }

    private:
        int m_id;
        std::pair<Sommet*, Sommet*> m_extremites;
};

#endif // ARETE_H
