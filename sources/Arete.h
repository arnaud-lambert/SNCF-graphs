#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        Arete(Arete&source);
        virtual ~Arete();
        void affichage()const;
        std::pair<Sommet*, Sommet*> getExtremites() { return m_extremites; }
        void setExtremites (Sommet*s1, Sommet*s2);
        void setPoids(double poids) { m_poids = poids; }
        double getPoids() { return m_poids; }
        void dessiner (Svgfile&svgout, int orientation, bool even, std::string couleur);
        int getId () const { return m_id; }

    private:
        int m_id;
        std::pair<Sommet*, Sommet*> m_extremites;
        double m_poids = 1.0;
};

#endif // ARETE_H
