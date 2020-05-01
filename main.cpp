#include "Graphe.h"
#include <windows.h>

bool menu(Graphe& a, std::string nomFichier);
int optionVulnerabilite(int& nb, int taille, std::string saisie);

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
    std::string saisie;
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
        std::cout<<"Etude ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<"de la ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"k-sommet(s)-connexite ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<"du graphe"<<std::endl;
        std::cout<<std::endl<<"4. ";
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
        std::cout<<std::endl<<"5. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Tester";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" la ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"vulnerabilite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" du graphe"<<std::endl;
        std::cout<<std::endl<<"6. Etude ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"forte connexite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" du graphe"<<std::endl;
        std::cout<<std::endl<<"7. ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Quitter";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" l'application"<<std::endl<<std::endl;


        std::cin>>saisie;
        if(saisie.length()>1)
        {
            choix='d';
        }
        else
            choix=saisie.front();
        std::cout<<std::endl;

        switch(choix)
        {
        case '1':
            a.dessiner();
            return true;
            break;

        case '2':
            a.ponderation();
            break;

        case '3':
            a.kSommetsConnexite();
            break;

        case '4':
        {
            std::vector<std::pair<int, double>> centralite_degres = a.centraliteDegre ();
            std::vector<std::pair<double, double>> vecteurPropre=a.vecteurPropre();
            std::vector<std::pair<double, double>> vecteurProximite=a.vecteurProximite();
            std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite=a.intermediarite();
            a.sauvegarder(centralite_degres, vecteurPropre, vecteurProximite, intermediarite, nomFichier);
        }
        break;

        case '5':
        {
            int nb=0;
            if(optionVulnerabilite(nb, a.getTaille(), saisie)=='1')
                a.testConnexite(nb);
            else
                a.comparaisonIndices(nb);
        }
        break;

        case '7':
            SetConsoleTextAttribute(texteConsole, 3);
            std::cout<<" _________    ____     ____    __________ "<<std::endl
                     <<"/    _    \\   \\   \\   /   /   |   _______|"<<std::endl
                     <<"|   /_\\   |    \\   \\_/   /    |   |"<<std::endl
                     <<"|    _____/     \\       /     |   |____"<<std::endl
                     <<"|    __   \\      \\     /      |    ____|"<<std::endl
                     <<"|   /  \\   |      |   |       |   |"<<std::endl
                     <<"|   \\__/   |      |   |       |   |______"<<std::endl
                     <<"\\__________/      |___|       |__________|"<<std::endl<<std::endl;
            SetConsoleTextAttribute(texteConsole, 15);
            break;


        case '6':
            a.testForteConnexite();
            break;
        case '7':
            {
              a.kAretesConnexe();
            }
            break;

        default :
            std::cout<<"Saisie ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"invalide";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" une option"<<std::endl;
            break;
        }

        a.dessiner();
    }
    while(choix!='7');
    return false;
}


int optionVulnerabilite(int& nb, int taille, std::string saisie)
{
    char option='0';
    std::cout<<"Comment voulez vous ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"tester";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" la ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"vulnerabilite";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" du graphe ?"<<std::endl<<std::endl<<"    1. ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"Supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" une ou plusieurs";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<" aretes";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" et ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"tester";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" la ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"connexite";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" du graphe"<<std::endl<<std::endl<<"    2. ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"Supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" une ou plusieurs ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"aretes";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<", ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"recalculer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" et ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"comparer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" les ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"indices";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"centralite"<<std::endl<<std::endl;
    SetConsoleTextAttribute(texteConsole, 15);

    do
    {
        std::cin>>saisie;
        if(saisie.length()>1)
        {
            option='d';
        }
        else
            option=saisie.front();

        if(option!='1' && option!='2')
        {
            std::cout<<std::endl<<"Cette option est ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"invalide";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" : ";
        }
    }
    while(option!='1' && option!='2');

    std::cout<<std::endl<<"Combien d'";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"aretes";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" voulez vous ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" ? ";

    do
    {
        std::cin>>nb;
        if((nb<0)||(nb>taille))
        {
            std::cout<<std::endl<<"Vous avez saisie un nombre ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"trop important";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" ou ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"negatif";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" : ";
        }
    }
    while((nb<0)||(nb>taille));

    return option;
}
