#ifndef SOMMET_H
#define SOMMET_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include "svg/svgfile.h"

class Arete;

class Sommet
{
    public:
        Sommet(int id, std::string nom, double x, double y);
        Sommet(Sommet&source);
        virtual ~Sommet();
        void affichage()const;
        int getId() { return m_id; }
        void setId(int id) { m_id=id; }
        std::vector<std::pair<Sommet*,Arete*>> getAdjacents() { return m_adjacents; }
        void ajouterAdjacent(Sommet* adjacent, Arete* arete) { m_adjacents.push_back({adjacent,arete}); }
        double getX() const { return m_x; }
        double getY() const { return m_y; }
        void dessiner(Svgfile&svgout);
        std::string getNom () const { return m_nom; }
        void suppAdjacent (Sommet*adjacent);
        void dfsReverse(std::vector<bool>& sommetCouleur, std::vector<std::vector<int>> reverseAdjacents, std::vector<int>& composanteFortementConnexe, std::vector<Sommet*> m_sommets);
        void dfs(std::vector<bool>& sommetCouleur, std::vector<int>& ordreSommet);

    private:
        int m_id;
        std::string m_nom;
        double m_x;
        double m_y;
        std::vector<std::pair<Sommet*,Arete*>> m_adjacents;

};

#endif // SOMMET_H
