#include "Graphe.h"
#include <windows.h>

bool menu(Graphe& a, std::string nomFichier);

///Varibale globale qui permet de mettre de la couleur sur le texte de la console
HANDLE texteConsole=GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
    bool continuer=true;
    do
    {
        std::string nomFichier;
        Graphe a(nomFichier);
        continuer=menu(a, nomFichier);
    }
    while(continuer);
    return 0;
}


bool menu (Graphe& a, std::string nomFichier)
{
    char choix='0';
    do
    {
        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<std::endl<<"  ____    _____     ___________     ____    ____     ____     ____ "<<std::endl
                            <<"/      \\/      \\   |    _______|   |    \\  |    |   |    |   |    |"<<std::endl
                            <<"|              |   |   |           |     \\ |    |   |    |   |    |"<<std::endl
                            <<"|   |\\    /|   |   |   |____       |      \\|    |   |    |   |    |"<<std::endl
                            <<"|   | \\__/ |   |   |    ____|      |            |   |    |   |    |"<<std::endl
                            <<"|   |      |   |   |   |           |    |\\      |   |    |___|    |"<<std::endl
                            <<"|   |      |   |   |   |_______    |    | \\     |   |             |"<<std::endl
                            <<"|___|      |___|   |___________|   |____|  \\____|   |_____________|"<<std::endl;

        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<std::endl<<std::endl<<"1. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Charger";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" un graphe"<<std::endl<<std::endl<<"2. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Changer";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" le systeme de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"ponderations"<<std::endl;
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<std::endl<<"3. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Calculer";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" et ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Sauvegarder";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" les differents ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"indices";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"centralite"<<std::endl;
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<std::endl<<"4. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Tester";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" la ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"vulnerabilite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" du graphe"<<std::endl;
        std::cout<<std::endl<<"5. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Quitter";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" l'application"<<std::endl<<std::endl;

        do
        {
            std::cin>>choix;
        }while((int)choix<1 && (int)choix>5);

        switch(choix)
        {
        case '1':
            return true;
            break;

        case '2':
            a.ponderation();
            break;

        case '3':
        {
            std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
            std::vector<std::pair<double, double>> vecteurPropre=a.vecteurPropre();
            std::vector<std::pair<double, double>> vecteurProximite=a.vecteurProximite();
            std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite=a.intermediarite();
            a.sauvegarder(centralite_degres, vecteurPropre, vecteurProximite, intermediarite, nomFichier);
        }
        break;

        case '4':
            //a.testConnexite();
            break;
        }
        a.dessiner();
    }while(choix!='5');
    return false;
}
