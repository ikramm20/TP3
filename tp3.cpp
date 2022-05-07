# inclure < stdio.h >
# inclure < stdlib.h >
# inclure < chaîne.h >
# définir  N  100

// définir la structure arbre
  typedef  struct  noed {
                int     num_sommet ;  // le numéro du sommet 0 , 1 , 2 ...
  	  int     borne_supérieur;  // la valeur du noeud
  	  float   valeurs_variables[N] ;  // valeur des objets variables [0,1]
  	  entier fixeur_variables     [N] ; // fixé l'objet
  	                             // 0 : fixé à zéro 1 : fixé à 1
  	                             // -1 : pas encor fixé
	  struct  noed * gauche;  // la partie gauche		
	  struct  noed * droite;  // la partie droite
	  
	}arborescence ;
typedef arborescence* arbre ;

// les variables globales
int      nb_variable ; // nb variable
float    coeff_fon_objective[N] ;  // vecteur objectif
float    coeff_contrainte[N] ;  // vecteur de contrainte
float    tab_efficacite[N] ;  // vecteur efficacité des objets (trier)
float    tab_indice[N] ; // vecteur des objets trier
int      borne_inférieur;  // la borne inférieure
int      resultat_final[N] ; // vecteur des objets (solution finale)
arbre arbre_resultat ;  // l'arbre qui contient tous les sous problème
int      choix ; // recomancer / quitter
int      voir_detaille ; // voir les détails ou pas
int      num_som = 0 ; // le numéro du sommet commence par 0

// prototype :
void  affiche_result_node (noeud arbre);
arbre init_arbre ();


// les fonctions :

int  is_entier ( float n){ // tester si un nombre est entier (dans se cas entre 0 et 1)
	int      partie_entière ;
	float    parti_non_entiere ;
	parti_entiere = int (n);
	
	parti_non_entiere = float (n) - parti_entiere ;
	
	if ((parti_non_entiere <= 0.001   && parti_non_entiere >= - 0.001 ) ||
	   (parti_non_entière < 1   && parti_non_entière >= 0.99 ) ||
	   (parti_non_entière <= - 0.99   && parti_non_entière > - 1 )
	  ){
	      retour  1 ;	
	}
  retourne  0 ;
}


int  solution_non_entiere (arbre a){   // tester si le noeud contient une solution int
	int je ;
	
	pour (i= 1 ; i<= nb_variable ; i++){
		si (! est_entier (a-> valeurs_variables [i]) ){
		     // indice_variable_non_int = je ;
		     retourner je ; // il existe une valeur non entière	
		}
	}
	
	retourne  0 ; // toutes les valeurs sont de type int (0 / 1)
}




void  lire_nb_variable (){   // lire le nombre de variables
	chaîne de caractères   [N] ;
	faire {
	    printf ( "   Donner le nombre des variables : " );
	    scanf ( " %s " ,chaine);
	    nb_variable = atoi (chaine);
	    
	} tandis que (nb_variable <= 0 );
	
	
}






void  lire_fonction_objective (){   // lire la fonction objective
     int   je ;
     printf ( " Donner la fonction objective : " );
     pour (i= 1 ; i<= nb_variable ; i++){
          	
          printf ( " \n      X%d =   " ,i);
          scanf ( " %f " ,&coeff_fon_objective[i]);
          	
     }	
	
}




void  lire_contrainte (){   // lire la contrainte
     int   je ;
     printf ( " Donner la contrainte : " );
          // lire le poids max
               printf ( " \n      Donner le poids Max : " ,i);
          scanf ( " %f " ,&coeff_contrainte[nb_variable+ 1 ]);
          
     pour (i= 1 ; i<= nb_variable ; i++){
          faire {
               printf ( " \n      X%d =   " ,i);
               scanf ( " %f " ,&coeff_contrainte[i]);
	 si (coeff_contrainte[i] > coeff_contrainte[nb_variable+ 1 ]){
	     printf ( "             X%d doit être supérieur ou égal à %.2f   \n  " ,i,coeff_contrainte[nb_variable+ 1 ]);
	 }         	
          } tandis que (coeff_contrainte[i] > coeff_contrainte[nb_variable+ 1 ]);	

          	
     }	

               	
}




void  permuter ( float *a , float *b){    // la permutation
	float   var_temporaire ;
	var_temporaire = *a;
	*a = *b ;
	*b = var_temporaire ;
	
}



void  initialiser_efficacite (){   // calculer l'efficacité
	int je ;
	pour (i= 1 ; i<= nb_variable ; i++){
	     tab_efficacite[i] = coeff_fon_objective[i] / coeff_contrainte[i] ;
	}
	
}




void  initialiser_indices (){   // initialiser les indices des variables
	int je ;
	pour (i= 1 ; i<= nb_variable ; i++){
	    tab_indice[i] = i ;
	}
		
}


void  initialise_var_fixe (){ // toutes les variables n'ont pas été fixées
	int   je ;
	pour (i= 1 ; i<= nb_variable ; i++){
		// pas encor fixé
		arbre_resultat-> fixer_variables [i] = - 1 ;
	}
}


void  tri_efficacite (){	 // trier les efficacite par ordre décroissant
	int   je, j ;
	

	pour (i= 1 ; i<= nb_variable ; i++){
		for (j= i+ 1 ;j<= nb_variable ; j++){
		   si (tab_efficacite[i] < tab_efficacite[j]){
		     // tester l'efficacité
		     permuter (&tab_efficacite[i],&tab_efficacite[j]);
		     // trier l'objet
		     permuter (&tab_indice[i],&tab_indice[j]);
	                 }
		}
	}

	
}

void  calcul_sol_realisable (){   // calculer la première solution réalisable
	flottant    volume_max ;
	int      je , indice ;
	// trouver la valeur des objets (1 / 0 )
	volume_max = coeff_contrainte[nb_variable+ 1 ];
	
	pour (i= 1 ; i<= nb_variable ; i++){
	       indice = tab_indice[i] ;
	       si (coeff_contrainte[indice] <= volume_max){
	           volume_max = volume_max - coeff_contrainte[indice] ;
	           resultat_final[indice] = 1 ; // on peut prendre l'objet   	
	       } sinon {
	           resultat_final[indice] = 0 ; // on peut pas le prendre	
	       }	
	}
	
	// calcule la borne inférieure
	borne_inférieur =   0 ;
	pour (i= 1 ; i<= nb_variable ;i++){
        	   borne_inferieur = borne_inferieur +
	           ( coeff_fon_objective[i] * resultat_final[i]); 			
	}
}

float  take_object (arbre le_node){
	int je , indice ;
	float vol_max = coeff_contrainte[nb_variable + 1 ];
	
	pour (i= 1 ;i<=nb_variable ; i++){
	     indice = tab_indice[i] ;
	     if (le_node-> fixer_variables [indice] == 1 ){
	          vol_max = vol_max - coeff_contrainte[indice] ;
	     }	
	}	
	retourne vol_max ;	
}

void  remplire_valeur (arbre le_node){
	int je ;
	pour (i= 1 ; i<= nb_variable ; i++){
		
		if (le_node-> fixer_variables [i] == 1 ){
			le_node-> valeurs_variables [i] = 1 ;
			
		} sinon {
			le_node-> valeurs_variables [i] = 0 ;	
		}
		
	}
}

void  iliminate_object (arbre le_node, float volume_max){
	int je ;
	indice entier ;
	pour (i= 1 ; i<= nb_variable ; i++){
		indice = tab_indice[i] ;	
		if (le_node-> fixer_variables [indice] == - 1 ){
			si (coeff_contrainte[indice] > volume_max){
			   le_node-> fixer_variables [indice] = 0 ;		
			}	
		}
	}
}
void  calcul_borne_superieur (arbre le_node ){ // la borne supérieur du sommet S0

	flottant    volume_max ;
	int      je , indice ;

	// prend l'objet fixé à 1
	volume_max = take_object (le_node);
	
	// iliminer l'objet qu'on peut pas les prendre
	éliminer_objet (le_node, volume_max);
	
	// remplir la valeur des variables du noeud
	remplir_valeur (le_node);


	
	// trouver la valeur de X
	pour (i= 1 ; i<= nb_variable ; i++){
		
	       indice = tab_indice[i] ;
	       
	       if (le_node-> fixer_variables [indice] == - 1 ){
	       	si (coeff_contrainte[indice] <= volume_max){
		    volume_max = volume_max - coeff_contrainte[indice] ;
	                  le_node-> valeurs_variables [indice] = 1 ;	       		
		} sinon {
	            		le_node-> valeurs_variables [indice]=
	              	 volume_max / coeff_contrainte[indice] ;
	            		casser ;	
	      	 }	
	       }
	       if (le_node-> fixer_variables [indice] == 0 ){
	       	le_node-> valeurs_variables [indice] = 0 ;	       	       	  	
	      }	

	}	
	
	
	// calcule la borne supérieure
	// arbre_resultat->borne_superieur = 0 ;// bahnes : a supprimer
	
    le_node-> borne_superieur = 0 ; // / ajouté par bahnes
	pour (i= 1 ; i<= nb_variable ;i++){
        	   le_node-> borne_superieur = le_node-> borne_superieur   +
	           ( coeff_fon_objective[i] * le_node-> valeurs_variables [i]);
              		
	}


}

void  change_result (arbre a){
	int   je ;
	// changer le résultat si en une nouvelle solution optimale
	pour (i= 1 ; i<= nb_variable ; i++){
		resultat_final[i] = a-> valeurs_variables [i] ;
	}
}





 
     // tester si l'arbre est vide
int  est_vide (arbre a){
	si (a == NULL )
	   retour   1 ;
	retourne  0 ;   
}


void  fixe_les_indice (arbre fils_gauche,arbre le_node, int fixe_or_no , int indice){
	int je ;
	pour (i= 1 ; i<= nb_variable ; i++){
		fils_gauche-> fixer_variables [i] = le_node-> fixer_variables [i] ;
	}
	fils_gauche-> fixer_variables [indice] = fixe_or_no ;

}

void  separation_evaluation (arbre le_node){   // construire les sommets
	
	int   je ;
	int   indice_variable_non_int ;

	
	// calculer la borne supérieure		
	calcul_borne_superieur (le_node);
	
	// le numéro du sommet
	le_node-> num_sommet = num_som ;
	num_som++ ;

	si (voir_détail){
	  // affiche le résultat de ce noeud
	  affiche_result_node (le_node);		
	}
	
	
	// séparer le nœud si nécessaire
	indice_variable_non_int = solution_non_entiere (le_node);
	
	si (indice_variable_non_int){
	     if (le_noeud-> borne_superieur > borne_inferieur){
	     	
	     	// crée un fils gauche et fixe la variable par 0
	     	arbre fils_gauche = init_arbre () ;
       		fils_gauche=(arbre) malloc ( taille de (arborescence));
	     	fixe_les_indice (fils_gauche,le_node, 0 ,indice_variable_non_int);
	     	le_node-> gauche = fils_gauche ;
	     	
	     	
	     	// créer un fils droit et fixer la variable par 1
	     	arbre fils_droite = init_arbre () ;
       		fils_droite =(arbre) malloc ( taille de (arborescence));
       		fixe_les_indice (fils_droite,le_node, 1 ,indice_variable_non_int);
		le_node-> droite = fils_droite ;
		
		// Appliquer la séparation :
		séparation_evaluation (fils_gauche);
		séparation_évaluation (fils_droite) ;    		
       		
	     } sinon {
	     	// arbre point ver NULL (pas de fils)	
		le_node-> droite =    NULL ;
		le_node-> gauche =    NULL ;
	     }	
		
	} sinon {
	     if (le_noeud-> borne_superieur > borne_inferieur){
	         	// alor on change la borne inférieure
    	   	borne_inferieur = le_noeud-> borne_superieur ;
    	   	// et on change le résultat
    	   	change_result (le_node);	
	     }
		// arbre point ver NULL (pas de fils)	
		le_node-> droite =    NULL ;
		le_node-> gauche =    NULL ;		    
		
	}


}


arbre init_arbre (){ // initialiser l'arbre
	retourne  NULL ;
}


//    l'affichage : ********************************************* ******
void  affiche_fonction_objective (){
	int    je ;
	je =   1 ;
	printf ( "         La fonction objectif : \n " );
	printf ( "               MaxZ = %.2fX%d " , coeff_fon_objective[i],i);
	pour (i= 2 ; i<= nb_variable ; i++){
	     printf ( "   + %.2fX%d " , coeff_fon_objective[i],i);	
	}
	printf ( " \n " );	
	
}


vide  affiche_contrainte (){
     	int   je ;
	je =   1 ;		
	printf ( "         La contrainte : \n " );
	printf ( "               %.2fX%d " , coeff_contrainte[i],i);	
	pour (i= 2 ; i<= nb_variable ; i++){
	     printf ( " + %.2fX%d " , coeff_contrainte[i],i);	
	}	
	printf ( " <= %.2f " , coeff_contrainte[i]);
	printf ( " \n " );	
}



void  afficher_objet_trier (){
	int    je ;
	printf ( "         Trier les objets : \n " );
	printf ( "               " );
	pour (i= 1 ; i<= nb_variable ; i++){
	     printf ( " X%.0f     " ,tab_indice[i]);	
	}
	printf ( " \n " );
	
	
}

void  affiche_result_node (noeud arbre){
	int   je ;
    	printf ( "      S%d : " ,noeud-> num_sommet );
	pour (i= 1 ; i<= nb_variable ; i++){
		printf ( " \n                 X%d = %.2f   \n " ,i,noeud-> valeurs_variables [i]);
	}
	printf ( "       E = %d " ,noeud-> borne_superieur );
	printf ( " \n       La borne inférieure = %d \n " ,borne_inférieure);
}


annuler  affiche_result_solution (){
	int    je ;
	printf ( "         La solution : : \n " );
	pour (i= 1 ;i<= nb_variable ; i++){
	    printf ( "               X%d = %d    \n " ,i, resultat_final[i]);
	}
	printf ( "               Z = %d   \n  " ,borne_inferieur);
	printf ( " \n " );
}



void  affich_arbre (arbre a){
	si (! est_vide (a)){
		printf ( " S%d :: %d \n " ,a-> num_sommet ,a-> borne_superieur );
		printf ( " \n " );
		affich_arbre (a-> gauche );
		affich_arbre (a-> droite );
	}
}


void   affiche_result (){
   printf ( " \n\n ========================================== ============================ " );
   printf ( " \n\n ========================================== =========================== \n\n " );
              si (voir_détail){
              	système ( " pause " );
	}
	printf ( "   Le resultat :    \n " );
	
	// afficher la fonction objectif
  	affiche_fonction_objectif ();
		
	// afficher la contarinte
	affiche_contrainte ();
	
	// affiche les objets par ordre d'efficacité
	afficher_objet_trier ();
	// affiche le résultat pour la première solution réalisable
	affiche_result_solution ();
	
	// affiche l'arbre
	   // affich_arbre(arbre_resultat);

	

}

    // recomancer ou quiter
int  recomancer (){
        char a choisi[N] ;
        faire {
            printf ( " \n     Voulez-vous recomancer ou quitter : " );
            printf ( " \n               1 : recomencer \n " );
            printf ( " \n               2 : quitter \n " );
           
            scanf ( " %s " ,&choisi);
            choix = atoi (choisi);	
        } tandis que (choix != 1   && choix != 2 );
        
        si (choix == 1 ){
        	retour  1 ;
        } sinon {
        	retourne  0 ;
        }	
	 
}
  // afficher les détails ou pas
void  detaille_non (){
       détail char    [N] ;
       faire {
            printf ( "      -- Voulez-vous voir le détail de la solution ou pas : " );
            printf ( " \n              1 : voir les détails " );
            printf ( " \n              2 : ne pas voir les détails \n " );
            scanf ( " %s " ,détail);
            voir_detaille = atoi (détail);
       } tandis que (voir_detaille != 1 && voir_detaille != 2 );
       si (voir_detaille == 2 ){
            voir_detaille = 0 ;	
       }
}


// la fonction principale
int  principal (){
    système ( " couleur f0 " );  // changer de couleur
    // lire le nombre de variables
        lire_nb_variable ();

    // lire la fonction objectif :
        lire_fonction_objectif ();

    // lire la contrainte
        lire_contrainte ();
   	 
    // initialiser efficacité (calculer l'efficacité)
        initialiser_efficacite ();
    
    // initialiser tableu d'indice (l'ordre des objets)
        initialiser_indices ();
    
    // trier efficacement avec et trier les objets
        tri_efficacite ();
    
    // trouver une solution réalisable ( la borne inférieure )
       calcul_sol_réalisable ();
    
    // initialiser l'arbre et lui donner un emplacement mémoire
       arbre_resultat = init_arbre () ;
       arbre_resultat=(arbre) malloc ( taille de (arborescence));
     
    // affichez les détails ou pas :
       detaille_non (); // si on veut voir les détails

    // initialiser le vecteur des variables fixé par(-1) pas fixé
       initialise_var_fixe ();

        
    // calculer S0 et faire la séparation
       separation_evaluation (arbre_resultat);
     

    // afficher le résultat
       affiche_result ();

    
    // recomencer ou quitter
    si ( recomancer ()){
         num_som = 0 ; // recomancer du sommet S0
         principal ();  // recomancer	
    }	
    

    	
    retourne  0 ;
    
}