#ifndef SOMMET_H
#define SOMMET_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>

class Sommet
{
    public:
        Sommet(int id, std::string nom, double x, double y);
        virtual ~Sommet();
        void affichage()const;
        int getId() { return m_id; }
        std::vector<Sommet*> getAdjacents() { return m_adjacents; }
        void ajouterAdjacent(Sommet* adjacent) { m_adjacents.push_back(adjacent); }

    private:
        int m_id;
        std::string m_nom;
        double m_x;
        double m_y;
        std::vector<Sommet*> m_adjacents;
};

#endif // SOMMET_H
