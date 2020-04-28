#include "Graphe.h"
void menu();


int main()
{
    menu();
    Graphe a;
    a.ponderation();
    a.affichage();
    a.dessiner();
    std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<Sommet*, double>> vecteurPropre=a.vecteurPropre();
    a.testConnexite();
    a.dessiner();

    std::vector<std::map<Sommet*, std::pair<Sommet*, double>>> vecteurProximite=a.vecteurProximite();
    a.intermediarite();//plante quand plus d'une arete supprimée
    a.dessiner();

    return 0;
}



void menu ()
{
    std::cout<<"           ***** Menu *****"<<std::endl
             <<"1. Charger un graphe"<<std::endl
             <<"2. Changer le systeme de ponderations"<<std::endl
             <<"3. Calculer l'indice de centralite de degre"<<std::endl
             <<"4. Calculer l'indice de centralite de vecteur propre"<<std::endl
             <<"5. Tester la connexite du graphe"<<std::endl<<std::endl;

}
