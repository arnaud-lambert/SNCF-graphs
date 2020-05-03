#include "Arete.h"

Arete::Arete(int id, Sommet* s1, Sommet* s2)
        :m_id{id}, m_extremites{s1, s2}
{

}

Arete::Arete(Arete&source) : m_id{source.m_id}, m_poids{source.m_poids}
{

}

Arete::~Arete()
{
    //dtor
}

/*void Arete::affichage()const
{
    std::cout<<m_id<<" "<<m_extremites.first->getId()<<" "<<m_extremites.second->getId()<<std::endl;
}*/

void Arete::setExtremites (Sommet*s1, Sommet*s2)
{
    m_extremites.first=s1;
    m_extremites.second=s2;
}

//methode dessiner une arete
void Arete::dessiner(Svgfile&svgout, bool orientation, bool even, bool poids, std::string couleur)
{
    std::string nom = m_extremites.first->getNom()+"-"+ m_extremites.second->getNom();//on recupere le nom de l'arete pour son orientation (ex: A-B)
    std::string color="darkorchid";//couleur de base d'un indice

    if(couleur!="black")//si couleur de l'arete n'est pas noir
        color=couleur;

    if(orientation ==true)//si graphe oriente
        {
            //un tour sur 2 on affiche le nom de l'arete au dessus ou en dessous
            if(even)
                svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50-10, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)+30, nom, color);
            else
                svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50-10, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)-30, nom, color);
        }
    //dessin de l'arete avec sa couleur pour l'intermediarite
    svgout.addLine( (m_extremites.first)->getX()*100, (m_extremites.first)->getY()*100, (m_extremites.second)->getX()*100, (m_extremites.second)->getY()*100, couleur);
    //pareil, on intercale les indices
    if(even)
        svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)+15, m_id, color);
    else
        svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)-17, m_id, color);

    if(poids==true)//si fichier avec ponderation, on affiche le poids de l'arete
        svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)-5, m_poids, "grey" );
}
