#include "../Headers/jeu.h"

Jeu::Jeu(string NomJoueur, int nombreBateau_)
{
	// Initialisation du menu
	Menu menuBatailleNavale("Bataille Navale");
	menuBatailleNavale.ajouterOption("Jouer", "Commencer une nouvelle partie");
	menuBatailleNavale.ajouterOption("Sauvegarder", "Sauvegarder une partie en cours");
	menuBatailleNavale.ajouterOption("Charger", "Charger une partie précedemment sauvegardée");
	menuBatailleNavale.ajouterOption("Quitter", "Sortir du jeu sans sauvegarder");
	menuBatailleNavale.executer();

	// Initialisation de l'affichage
	unsigned char couleurGrille[3] = {0, 0, 255};
	unsigned char couleurGrille1[3] = {255, 0, 0};

	affichage Grille(couleurGrille, 10, 10);   // Affichage lié au bateau du joueur humain
	affichage Grille1(couleurGrille1, 10, 10); // Affichage lié à la flotte de l'I.A.

	// Initilaisation de la grille logique
	GrilleLogique grilleHumain;
	GrilleLogique grilleOrdi;

	// Initilisation des joueurs (humain et IA)
	JoueurHumain Joueur1 = JoueurHumain(NomJoueur, nombreBateau_);
	JoueurOrdi Joueur2 = JoueurOrdi("GlaDos", nombreBateau_);

	// Placement des bateaux du joueur humain
	cout << "Placez vos " << nombreBateau_ << " bateaux." << endl;
	vector<Bateau> flotteHumain = Joueur1.Personne::getFlotte();
	for (int i = 0; i < flotteHumain.size(); i++)
	{
		cout << " Placez la proue du bateau." << endl;
		int cX1 = Grille.coordonnesClic[0], cY1 = Grille.coordonnesClic[1];
		while (grilleHumain.getEtatCase(cX1, cY1) != 1)
		{
			cout << "Oups ! Ce n'est pas possible. Placez la proue du bateau." << endl;
			int cX1 = Grille.coordonnesClic[0], cY1 = Grille.coordonnesClic[1];
		}

		cout << "Selectionnez l'orientation du bateau." << endl;
		int cX2 = Grille.coordonnesClic[0], cY2 = Grille.coordonnesClic[1];
		while (grilleHumain.getEtatCase(cX2, cY2) != 1)
		{
			cout << "Oups ! Ce n'est pas possible. Selectionnez l'orientation du bateau." << endl;
			int cX2 = Grille.coordonnesClic[0], cY2 = Grille.coordonnesClic[1];
		}

		Joueur1.ajouterBateau(cX1, cY1, cX2, cY2, i);
		grilleHumain.ajouterBateau(flotteHumain[i]);
	}

	// Placement des bateaux de l'IA
	cout << "Placement des bateaux par votre adversaire." << endl;
	vector<Bateau> flotteOrdi = Joueur2.Personne::getFlotte();
	for (int i = 0; i < flotteOrdi.size(); i++)
	{
		int cX1, cX2, cY1, cY2;
		bool aReussiAPlacer1 = 0;
		bool aReussiAPlacer2 = 0;

		while (!aReussiAPlacer1) // Placement de la proue du bateau par l'I.A.
		{
			Joueur2.ChoixCible(Grille.getNombreLignes(), Grille.getNombreColonnes());
			int cX1 = Joueur2.getCibleX();							   // Première coordonnee X choisie par l'ordinateur
			int cY1 = Joueur2.getCibleY();							   // Première coordonnee Y choisie par l'ordinateur
			aReussiAPlacer1 = (grilleOrdi.getEtatCase(cX1, cY1) == 1); // Vérifie si la case sélectionnée correspond à de l'eau
		}

		while (!aReussiAPlacer2) // Placement de la poupe du bateau par l'I.A.
		{
			Joueur2.ChoixCible(Grille.getNombreLignes(), Grille.getNombreColonnes());
			cX2 = Joueur2.getCibleX();								   // Deuxième coordonnee X choisie par l'ordinateur
			cY2 = Joueur2.getCibleY();								   // Deuxième coordonnee Y choisie par l'ordinateur
			aReussiAPlacer2 = (grilleOrdi.getEtatCase(cX2, cY2) == 1); // Vérifie si la case sélectionnée correspond à de l'eau
		}
		Joueur2.ajouterBateau(cX1, cY1, cX2, cY2, i);
	}

	// Déroulement du jeu
	while (!partieTerminee)
	{
		if (!quelJoueur_) // Tour du joueur humain
		{
			cout << "C'est votre tour !" << endl;
			// Décompte des pertes dans la flotte pour condition d'arrêt
			int pertes = 0;
			for (int i = 0; i < grilleHumain.getGrille().size(); i++)
			{
				for (int j = 0; j < grilleHumain[i].size(); j++)
				{
					if (grilleHumain[i][j] == 4) // Nombre de partie de bateau déjà touchées
					{
						pertes++;
					}
				}
			}
			// Décompte du nombre de cases effectifs de la flotte pour comparaison avec les pertes
			int casesBateau = 0;
			for (int i = 0; i < Joueur1.getFlotte().size(); i++)
			{
				casesBateau += Joueur1.getFlotte()[i].getTaille();
			}
			if (pertes == casesBateau)
			{
				partieTerminee = !partieTerminee;
			}
			else
			{
				int clicX1 = Grille1.coordonnesClic[0], clicY1 = Grille1.coordonnesClic[1];
				while ((clicX1 < 0) || (clicX1 > Grille1.getNombreLignes()) || (clicY1 < 0) || (clicY1 > Grille1.getNombreColonnes()))
				{
					cout << "Oups ! Le clic enregistré n'est pas correct ! Reessayez !" << endl;
					int clicX1 = Grille1.coordonnesClic[0], clicY1 = Grille1.coordonnesClic[1];
				}
				bool aReussiATirer = 0;
				while (!aReussiATirer)
				{
					Joueur1.ChoixCible(clicX1, clicY1);
					aReussiATirer = grilleOrdi.tireSurCase(clicX1, clicY1); // Vérifie si la case correspond à une cible correcte et informe le joueur
					switch (grilleOrdi.getEtatCase(clicX1, clicY1))			// Passe en revue tous les cas de figures
					{
					case 1: // C'est de l'eau
						aReussiATirer = !aReussiATirer;
						break;
					case 3: // C'est une partie d'un bâteau non précédemment touchée
						aReussiATirer = !aReussiATirer;
						break;
					default:
						break;
					}
				}
			}
			quelJoueur_ = !quelJoueur_;
		}

		else // Tour de l'IA
		{
			cout << "C'est au tour de votre adversaire." << endl;
			// Décompte des pertes dans la flotte pour condition d'arrêt
			int pertes = 0;
			for (int i = 0; i < grilleOrdi.getGrille().size(); i++)
			{
				for (int j = 0; j < grilleOrdi[i].size(); j++)
				{
					if (grilleOrdi[i][j] == 4)
					{
						pertes++;
					}
				}
			}
			// Décompte du nombre de cases effectifs de la flotte pour comparaison avec les pertes
			int casesBateau = 0;
			for (int i = 0; i < Joueur1.getFlotte().size(); i++)
			{
				casesBateau += Joueur1.getFlotte()[i].getTaille();
			}
			if (pertes == casesBateau)
			{
				partieTerminee = !partieTerminee;
			}
			bool aReussiATirer = 0;
			while (!aReussiATirer)
			{
				Joueur2.ChoixCible(Grille.getNombreLignes(), Grille.getNombreColonnes());
				int cOX = Joueur2.getCibleX(); // Coordonnée X choisie par l'ordinateur
				int cOY = Joueur2.getCibleY(); // Coordonnée Y choisie par l'ordinateur
				switch (grilleHumain.getEtatCase(cOX, cOY))
				{
				case 1:
					aReussiATirer = !aReussiATirer;
					break;
				case 3:
					aReussiATirer = !aReussiATirer;
					break;
				default:
					break;
				}
			}
		}
		quelJoueur_ = !quelJoueur_;
	}
}
}

Jeu::~Jeu()
{
}

vector<int> Jeu::getGrille()
{
	return vector<int>();
}
