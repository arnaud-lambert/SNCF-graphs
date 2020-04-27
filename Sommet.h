#ifndef SOMMET_H
#define SOMMET_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>

class Sommet
{
    public:
        Sommet(int indice, char nom, int x, int y);
        virtual ~Sommet();
        void affichage()const;
        int getId();

    private:
        int m_indice;
        char m_nom;
        std::pair<int, int> m_coordonnees;

};

#endif // SOMMET_H
