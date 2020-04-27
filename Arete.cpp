#include "Arete.h"

Arete::Arete(int indice, Sommet* s1, Sommet* s2)
        :m_indice{indice}, m_som{s1, s2}
{

}

Arete::~Arete()
{
    //dtor
}

void Arete::affichage()const
{
    std::cout<<std::endl<<m_indice<<" "<<m_som.first->getId()<<" "<<m_som.second->getId();
}
