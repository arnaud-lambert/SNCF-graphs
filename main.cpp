#include "Graphe.h"
void menu();


int main()
{
    menu();
    std::string nomFichier;
    Graphe a(nomFichier);
    a.ponderation();
    a.affichage();
    a.dessiner();
    std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
    std::vector<std::pair<double, double>> vecteurPropre=a.vecteurPropre();
//    a.testConnexite();
    std::vector<std::pair<double, double>> vecteurProximite=a.vecteurProximite();
    std::vector<std::pair<double, double>> intermediarite=a.intermediarite();
    a.dessiner();
//a.sauvegarder(centralite_degres, vecteurPropre, vecteurProximite, intermediarite, nomFichier);
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
