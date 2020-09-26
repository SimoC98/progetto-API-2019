#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct nodo_utente {
	char *nome_utente;
	struct nodo_utente *left;
	struct nodo_utente *right;
	struct nodo_utente *f;
} nodo_utente;

typedef struct nodo_relazione {
	char *nome_utente;
	int num_rel;
	nodo_utente *p;
	struct nodo_relazione *left;
	struct nodo_relazione *right;
	struct nodo_relazione *f;
} nodo_relazione;

typedef struct relazione {
	char *nome_relazione;
	int max;
	int num_max;
	nodo_relazione *tree;
	struct relazione *next;
} relazione;

typedef struct utente {
	char *nome_utente;
	struct utente *next;
} utente;

typedef struct massimi {
	char *nome_relazione;
	int max;
	utente *lista;
	struct massimi *next;
} massimi;


relazione *check_relazione(relazione *lista, char *nome_relazione) {
	relazione *attuale=lista;
	
	while((attuale!=NULL)&&(strcmp(attuale->nome_relazione,nome_relazione)!=0)) {
		attuale = attuale->next;
	}
	return attuale;	
}


//passo un vettore di char e alloco dinamicamente una stringa
char *alloca_stringa(char *string, char *s) {
	int len;
	
	len = strlen(string)+1;
	s = (char*)malloc(len*sizeof(char));
	strcpy(s,string);
	return s;
}


//calcola numero massimo tra tre interi
int max_intero(int i, int j, int k) {
	int n;
	
	if(i>j) {
		n = i;
	}
	else {
		n = j;
	}
	if(k>n) {
		n = k;
	}
	return n;
}


//trova utente minimo in un bst
nodo_utente *utente_minimo(nodo_utente *x) {
	if(x==NULL) {
		return x;
	}
	while(x->left!=NULL) {
		x = x->left;
	}
	return x;
}


//trova relazione minimo in un bst
nodo_relazione *relazione_minimo(nodo_relazione *x) {
	if(x==NULL) {
		return x;
	}
	while(x->left!=NULL) {
		x = x->left;
	}
	return x;
}


//trova utente successore in bst 
nodo_utente *utente_successor(nodo_utente *x) {
	nodo_utente *s;	
	
	if(x->right!=NULL) {
		s = utente_minimo(x->right);
		return s;
	}
	s = x->f;	
	while((s!=NULL)&&(x=s->right)) {
		x = s;
		s = s->f;
	}
	return s;
}


//trova relazione successore in bst 
nodo_relazione *relazione_successor(nodo_relazione *x) {
	nodo_relazione *s;	
	
	if(x->right!=NULL) {
		s = relazione_minimo(x->right);
		return s;
	}
	s = x->f;	
	while((s!=NULL)&&(x=s->right)) {
		x = s;
		s = s->f;
	}
	return s;
}


//funzione che cancella un albero
nodo_utente *delete_tree(nodo_utente *tree) {
	if(tree==NULL) {
		return tree;
	}
	else if((tree->left==NULL)&&(tree->right==NULL)) {
		free(tree);
	}
	else if((tree->left==NULL)&&(tree->right!=NULL)){
		tree->right = delete_tree(tree->right);
	}
	else if((tree->left!=NULL)&&(tree->right==NULL)) {
		tree->left = delete_tree(tree->left);
	}
	else {
		tree->left = delete_tree(tree->left);
		tree->right = delete_tree(tree->right);
		free(tree);
	}
	tree = NULL;
	return tree;
}



//ricerca in albero con nodi di tipo nodo_utente:
nodo_utente *ricerca_utente(nodo_utente *tree, char *nome_utente) {
	
	if(tree==NULL) {
		return tree;
	}
	while(tree!=NULL) {
		if(strcmp(nome_utente,tree->nome_utente)==0) {
			break;
		}
		if(strcmp(nome_utente,tree->nome_utente)<0) {
			tree = tree->left;
		}
		else {
			tree = tree->right;
		}
	}
	return tree;
}

//ricerca in albero con nodi di tipo nodo_relazione:
nodo_relazione *ricerca_relazione(nodo_relazione *tree, char *nome_utente) {
	if(tree==NULL) {
		return tree;
	}
	while(tree!=NULL) {
		if(strcmp(nome_utente,tree->nome_utente)==0) {
			break;
		}
		if(strcmp(nome_utente,tree->nome_utente)<0) {
			tree = tree->left;
		}
		else {
			tree = tree->right;
		}
	}
	return tree;
}

//inserimento nell'albero con nodi nodo_utente
nodo_utente *insert_utente(nodo_utente *tree, char *nome_utente) {
	nodo_utente *new, *x=tree, *y=NULL;
	char *nome=NULL;
		
	nome = alloca_stringa(nome_utente,nome);
	
	new = (nodo_utente*)malloc(sizeof(nodo_utente));

	new->nome_utente = nome;
	new->left = NULL;
	new->right = NULL;
	
	while(x!=NULL) {
		y = x;
		if(strcmp(nome_utente,x->nome_utente)<0) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	//albero vuoto
	if(y==NULL) {
		new->f = NULL; //è la radice
		tree = new;		
	}
	else if(strcmp(new->nome_utente,y->nome_utente)<0) {
		new->f = y;
		y->left = new;
	}
	else {
		new->f = y;
		y->right = new;
	}
	return tree;
}


//inserimento nell'albero con nodi nodo_relazione
nodo_relazione *insert_relazione(nodo_relazione *tree, char *nome_utente1, char *nome_utente2) {
	nodo_relazione *new, *x=tree, *y=NULL;
	char *nome=NULL;
	
	nome = alloca_stringa(nome_utente2,nome);
	new = (nodo_relazione*)malloc(sizeof(nodo_relazione));
	
	new->nome_utente = nome;
	new->num_rel = 1;
	new->p = NULL;
	new->p = insert_utente(new->p,nome_utente1);	//inserisco l'utente 1 (quello che "compie" la relazione sull'utente 2) nell'albero;
	new->left = NULL;
	new->right = NULL;
	
	while(x!=NULL) {
		y = x;
		if(strcmp(nome_utente2,x->nome_utente)<0) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	//albero vuoto
	if(y==NULL) {
		new->f = NULL; //è la radice
		tree = new;		
	}
	else if(strcmp(new->nome_utente,y->nome_utente)<0) {
		new->f = y;
		y->left = new;
	}
	else {
		new->f = y;
		y->right = new;
	}
	return tree;
}


//eliminazione nodo_utente
nodo_utente *delete_utente(nodo_utente *tree, nodo_utente *z) {
	nodo_utente *y, *x;
	
	
	if((z->left==NULL)||(z->right==NULL)) {
		y = z;
	}
	else {
		y = utente_successor(z);
	}
		
		
	if(y->left!=NULL) {
		x = y->left;
	}
	else {
		x = y->right;
	}
		
	if(x!=NULL) {
		x->f = y->f;
	}
		
	if(y->f==NULL) {
		tree = x;
	}
	else {
		if(y==y->f->left) {
			y->f->left = x;
		}
		else {
			y->f->right = x;
		}
	}
		
	if(y!=z) {
		free(z->nome_utente);
		z->nome_utente = alloca_stringa(y->nome_utente,z->nome_utente);
	}
	free(y->nome_utente);
	free(y);
	return tree;
}	



nodo_relazione *delete_relazione(nodo_relazione *tree, nodo_relazione *z) {
	nodo_relazione *y, *x;
	
	
	if((z->left==NULL)||(z->right==NULL)) {
		y = z;
	}
	else {
		y = relazione_successor(z);
	}
		
		
	if(y->left!=NULL) {
		x = y->left;
	}
	else {
		x = y->right;
	}
		
	if(x!=NULL) {
		x->f = y->f;
	}
		
	if(y->f==NULL) {
		tree = x;
	}
	else {
		if(y==y->f->left) {
			y->f->left = x;
		}
		else {
			y->f->right = x;
		}
	}
		
	if(y!=z) {
		free(z->nome_utente);
		z->nome_utente = alloca_stringa(y->nome_utente,z->nome_utente);
		z->num_rel = y->num_rel;
		z->p = y->p;
		y->p = NULL;
	}
	
	free(y->nome_utente);
	free(y);
	return tree;
}



	


//inserimento di una nuova relazione in testa nella lista
relazione *new_relazione(relazione **lista, char *nome_relazione) {
	relazione *new_rel;
	char *nome=NULL;
	
	new_rel = (relazione*)malloc(sizeof(relazione));
	nome = alloca_stringa(nome_relazione,nome);
	
	new_rel = (relazione*)malloc(sizeof(relazione));
	new_rel->nome_relazione = nome;
	new_rel->max = 1;
	new_rel->num_max = 1;
	new_rel->tree = NULL;
	
	if(*lista==NULL) {
		new_rel->next = NULL;
		*lista = new_rel;
	}
	else {
		new_rel->next = *lista;
		*lista = new_rel;
	}
	return new_rel;
}



//inserimento nodo nella lista degli utenti in ordine alfabetico
utente *aggiungi_utente(utente *lista, char *nome_utente) {
	utente *new, *attuale=lista, *prec=NULL;
	char *nome=NULL;
	
	nome = alloca_stringa(nome_utente,nome);
	
	new = (utente*)malloc(sizeof(utente));
	new->nome_utente = nome;
	
	while((attuale!=NULL)&&(strcmp(attuale->nome_utente,nome_utente)<0)) {
		prec = attuale;
		attuale = attuale->next;
	}
	
	if(prec==NULL) {
		new->next = attuale;
		lista = new;
	}
	else {
		new->next = attuale;
		prec->next = new;
	}
	return lista;	
}


//inserimento di un nuovo nodo nella lista dei massimi in ordine alfabetico
massimi *new_massimo(massimi *lista, char *nome_relazione, char *nome_utente) {
	massimi *new_rel, *attuale=lista, *prec=NULL;
	char *nome_rel=NULL;
	
	nome_rel = alloca_stringa(nome_relazione,nome_rel);
	
	new_rel = (massimi*)malloc(sizeof(massimi));
	
	new_rel->nome_relazione = nome_rel;
	new_rel->max = 1;
	new_rel->lista = NULL;
	new_rel->lista = aggiungi_utente(new_rel->lista,nome_utente);
	
	while((attuale!=NULL)&&(strcmp(attuale->nome_relazione,nome_relazione)<0)) {
		prec = attuale;
		attuale = attuale->next;
	}
	if(prec==NULL) {
		new_rel->next = attuale;
		lista = new_rel;
	}
	else {
		new_rel->next = attuale;
		prec->next = new_rel;
	}
	return lista;	
}


//eliminare intera lista di utenti
utente *cancella_lista(utente *lista) {
	utente *attuale=lista, *succ=NULL; 
	
	while(attuale!=NULL) {
		succ = attuale->next;
		free(attuale);
		attuale = succ;		
	}
	lista = NULL;
	return lista;
}


//aggiorna la lista dei massimi
massimi *aggiorna_massimi(massimi *lista, char *nome_relazione, char *nome_utente, int n) {
	massimi *attuale=lista;
	
	while(strcmp(attuale->nome_relazione,nome_relazione)!=0) {
		attuale = attuale->next;
	}
	if(n==attuale->max) {
		attuale->lista = aggiungi_utente(attuale->lista,nome_utente);
	}
	else if((n>attuale->max)||(n<attuale->max)) {
		attuale->max = n;
		if(attuale->lista!=NULL) {
			attuale->lista = cancella_lista(attuale->lista);
		}
		attuale->lista = aggiungi_utente(attuale->lista,nome_utente);
	}
	return lista;
}



//funzione chiamata nel caso in cui il tipo di relazione esisteva già
relazione *addrel(relazione *rel, char *nome_utente1, char *nome_utente2, massimi *lista_massimi) {
	nodo_relazione *ricerca_rel;
	nodo_utente *ricerca_u;
	
	ricerca_rel = ricerca_relazione(rel->tree,nome_utente2);
	if(ricerca_rel==NULL) {
		rel->tree = insert_relazione(rel->tree,nome_utente1,nome_utente2);
		if(rel->max==1) {
			rel->num_max = rel->num_max + 1;;
			lista_massimi = aggiorna_massimi(lista_massimi,rel->nome_relazione,nome_utente2,1);
		}
		return rel;
	}
	else {
		ricerca_u = ricerca_utente(ricerca_rel->p,nome_utente1);
		if(ricerca_u==NULL) {
			ricerca_rel->p = insert_utente(ricerca_rel->p,nome_utente1);
			ricerca_rel->num_rel = ricerca_rel->num_rel + 1;;
			if(ricerca_rel->num_rel==rel->max) {
				rel->num_max = rel->num_max + 1;
				lista_massimi = aggiorna_massimi(lista_massimi,rel->nome_relazione,nome_utente2,rel->max); 
			}
			else if(ricerca_rel->num_rel>rel->max) {
				rel->max = ricerca_rel->num_rel;
				rel->num_max = 1;
				lista_massimi = aggiorna_massimi(lista_massimi,rel->nome_relazione,nome_utente2,rel->max); 
			}
			return rel;
		}
		else {
			return rel;
		}
	}
}


//funzione che elimina un utente dalla lista dei massimi
massimi *elimina_massimo(massimi *lista_massimi, char *nome_relazione, char *nome_utente) {
	massimi *attuale=lista_massimi;
	utente *del=NULL, *prec=NULL;
	
	while(strcmp(attuale->nome_relazione,nome_relazione)!=0) {
		attuale = attuale->next;
	}
	del = attuale->lista;
	while(strcmp(del->nome_utente,nome_utente)!=0) {
		prec = del;
		del = del->next;
	}
	if(prec==NULL) {
		attuale->lista = del->next;
		del->next = NULL;
	}
	else {
		prec->next = del->next;
		del->next = NULL;
	}
	free(del);
	
	return lista_massimi;
}


//funzione ricorsiva che scansiona tutto l'albero e calcola il massimo
int calcola_massimo(nodo_relazione *tree) {
	int left, right, num, max;

	if(tree==NULL) {
		return 0;
	}
	else {
		num = tree->num_rel;
		left = calcola_massimo(tree->left);
		right = calcola_massimo(tree->right);
		max = max_intero(num,left,right);
		return max;
	}	
}


//funzione ricorsiva che visita albero per aggiornare la lista dei massimi
massimi *visita_massimi(massimi *lista_massimi, nodo_relazione *tree, relazione *rel) {
	if(tree==NULL) {
		return lista_massimi;
	}
	else {
		if(tree->num_rel==rel->max) {
			(rel->num_max)++;
			lista_massimi = aggiorna_massimi(lista_massimi,rel->nome_relazione,tree->nome_utente,rel->max);
		}
		lista_massimi = visita_massimi(lista_massimi,tree->left,rel);
		lista_massimi = visita_massimi(lista_massimi,tree->right,rel);
		return lista_massimi; 
	}
}


//funzione che elimina nodo relazione dalla lista_relazioni
relazione *elimina_nodo_relazione(relazione *lista_relazioni, char *nome_relazione) {
	relazione *prec=NULL, *del=lista_relazioni;
	
	while(strcmp(del->nome_relazione,nome_relazione)!=0) {
		prec = del;
		del = del->next;
	}
	if(prec==NULL) {
		lista_relazioni = del->next;
		del->next = NULL;
	}
	else {
		prec->next = del->next;
		del->next = NULL;
	}
	free(del->nome_relazione);
	free(del);
	del = NULL;	
	return lista_relazioni;	
}



//funzione che elimina nodo massimi dalla lista_massimi
massimi *elimina_nodo_massimi(massimi *lista_massimi, char *nome_relazione) {
	massimi *prec=NULL, *del=lista_massimi;
	
	while(strcmp(del->nome_relazione,nome_relazione)!=0) {
		prec = del;
		del = del->next;
	}
	if(prec==NULL) {
		lista_massimi = del->next;
		del->next = NULL;
	}
	else {
		prec->next = del->next;
		del->next = NULL;
	}
	del->lista = cancella_lista(del->lista);
	free(del->nome_relazione);
	free(del);
	del = NULL;	
	return lista_massimi;
}




//funzione chiamata per comando delrel
relazione *delrel(relazione *lista_relazioni, relazione *rel, massimi **lista_massimi, char *utente1, char *utente2) {
	nodo_relazione *ricerca_rel;
	nodo_utente *ricerca_ut;
	int num;
	
	ricerca_rel = ricerca_relazione(rel->tree,utente2);
	if(ricerca_rel==NULL) {		//non trova utente2
		return lista_relazioni;
	}
	
	ricerca_ut = ricerca_utente(ricerca_rel->p,utente1);	
	if(ricerca_ut==NULL) {		//non trova utente1
		return lista_relazioni;
	}
	num = ricerca_rel->num_rel;
	ricerca_rel->num_rel = ricerca_rel->num_rel - 1;	
	ricerca_rel->p = delete_utente(ricerca_rel->p,ricerca_ut);
	
	if(ricerca_rel->p==NULL) {
		rel->tree = delete_relazione(rel->tree,ricerca_rel);
	}
	
	if(rel->tree==NULL) {
		*lista_massimi = elimina_nodo_massimi(*lista_massimi,rel->nome_relazione);
		lista_relazioni = elimina_nodo_relazione(lista_relazioni,rel->nome_relazione);
		return lista_relazioni;
	}
	else {
		if(num==rel->max) {
			(rel->num_max)--;
			if(rel->num_max>0) {
				*lista_massimi = elimina_massimo(*lista_massimi,rel->nome_relazione,utente2);
				return lista_relazioni;
			}
			else if(rel->num_max==0) {
				rel->max = calcola_massimo(rel->tree);
				rel->num_max = 0;
				*lista_massimi = visita_massimi(*lista_massimi,rel->tree,rel);
				return lista_relazioni;
			}
			else {
				return lista_relazioni;
			}
		}
		else {
			return lista_relazioni;
		}
	}	
}





//da sistemare
void delent_visita(relazione *rel, nodo_relazione *t, massimi **lista, char *nome) {
	nodo_utente *ric=NULL;
	int num;

	if((t->left==NULL)&&(t->right!=NULL)) {
		delent_visita(rel,t->right,lista,nome);
	}
	else if((t->left!=NULL)&&(t->right==NULL)) {
		delent_visita(rel,t->left,lista,nome);
	}
	else if((t->left!=NULL)&&(t->right!=NULL)) {
		delent_visita(rel,t->left,lista,nome);
		delent_visita(rel,t->right,lista,nome);
	}

	
	if(strcmp(t->nome_utente,nome)==0) {
		num = t->num_rel;
		rel->tree = delete_relazione(rel->tree,t);
		if(num==rel->max) {
			*lista = elimina_massimo(*lista,rel->nome_relazione,nome);
			(rel->num_max)--;
		}
	}
	else {
		ric = ricerca_utente(t->p,nome);
		if(ric!=NULL) {
			num = t->num_rel;
			if(num==rel->max) {
				*lista = elimina_massimo(*lista,rel->nome_relazione,t->nome_utente);
				(rel->num_max)--;
			}
			t->p = delete_utente(t->p,ric);
			if(t->p==NULL) {
				rel->tree = delete_relazione(rel->tree,t);
			}
			else {
				(t->num_rel)--;
			}
		}
	}
	return;
}


//funzione per delent da sistemare
relazione *delent(relazione *lista_relazioni, massimi **lista_massimi, char *nome_utente) {
	relazione *attuale=lista_relazioni, *del=NULL;
	int num;
	
	while(attuale!=NULL) {
		delent_visita(attuale,attuale->tree,lista_massimi,nome_utente);
		if(attuale->tree==NULL) {
			*lista_massimi = elimina_nodo_massimi(*lista_massimi,attuale->nome_relazione);
			del = attuale;
			attuale = attuale->next;
			lista_relazioni = elimina_nodo_relazione(lista_relazioni,del->nome_relazione);
		}
		else {	
			num = calcola_massimo(attuale->tree);
			if(num<attuale->max) {		 
				attuale->max = num;
				attuale->num_max = 0;
				*lista_massimi = visita_massimi(*lista_massimi,attuale->tree,attuale);
			}
			attuale = attuale->next;
		}
	}
	return lista_relazioni;
}


void report(massimi *lista) {
	massimi *attuale=lista;
	utente *p=NULL;
	
	if(attuale==NULL) {
		fputs("none",stdout);
	}
	else {
		while(attuale!=NULL) {
			fputs(attuale->nome_relazione,stdout);
			p = attuale->lista;
			printf(" ");
			while(p!=NULL) {
				fputs(p->nome_utente,stdout);
				printf(" ");
				p = p->next;
			}
			printf("%d",attuale->max);
			fputs(";",stdout);
			printf(" ");
			attuale = attuale->next;
		}	
	}
	printf("\n");
}



int main() {
	nodo_utente *tree=NULL, *u1=NULL, *u2=NULL;
	relazione *lista_relazioni=NULL, *check=NULL;
	massimi *lista_massimi=NULL;
	char *stringa, comando[10], utente1[100], utente2[100], nome_relazione[100];
	size_t dim=1000;
	int i, cont=0;
	
	stringa = (char*)malloc(1000*sizeof(char));
	
	while(1) {
		fgets(stringa,dim,stdin);
	
		i = 0;
		while((stringa[i+1]!=' ')&&(stringa[i+1]!='\n')&&(stringa[i+1]!='\r')) {
			comando[i] = stringa[i];
			i++;
		}
		comando[i] = stringa[i];
		comando[i+1] = '\0';
		
		if(strcmp(comando,"end")==0) {
			break;
		}
		else if(strcmp(comando,"addent")==0) {
			sscanf(stringa, "%s %s", comando, utente1);
			u1 = ricerca_utente(tree,utente1);
			if(u1==NULL) {
				tree = insert_utente(tree,utente1);
			}
			u1 = NULL;
		}
		else if(strcmp(comando,"addrel")==0) {						
			sscanf(stringa, "%s %s %s %s", comando, utente1, utente2, nome_relazione);	
			u1 = ricerca_utente(tree,utente1);
			if(u1!=NULL) {
				u2 = ricerca_utente(tree,utente2);
				if(u2!=NULL) {		
					check = check_relazione(lista_relazioni,nome_relazione);
					if(check==NULL) {
						check = new_relazione(&lista_relazioni,nome_relazione);
						check->tree = insert_relazione(check->tree,utente1,utente2);
						lista_massimi = new_massimo(lista_massimi,nome_relazione,utente2);
					}
					else {
						check = addrel(check,utente1,utente2,lista_massimi);
					}
				}	
			}
			u1 = NULL;
			u2 = NULL;		
		}
		else if(strcmp(comando,"delent")==0) {
			sscanf(stringa, "%s %s", comando, utente1);
			u1 = ricerca_utente(tree,utente1);
			if(u1!=NULL) {
				tree = delete_utente(tree,u1);
				lista_relazioni = delent(lista_relazioni,&lista_massimi,utente1);
			}
			
		}
		else if(strcmp(comando,"delrel")==0) {
			sscanf(stringa, "%s %s %s %s", comando, utente1, utente2, nome_relazione);
			u1 = ricerca_utente(tree,utente1);
			if(u1!=NULL) {
				u2 = ricerca_utente(tree,utente2);
				if(u2!=NULL) {
					check = check_relazione(lista_relazioni,nome_relazione);
					if(check!=NULL) {
						lista_relazioni = delrel(lista_relazioni,check,&lista_massimi,utente1,utente2);
					} 
				}
			}
			u1=NULL;
			u2=NULL;
			check=NULL;
		}		
		else if(strcmp(comando,"report")==0) {
			report(lista_massimi);
		}
		
		
		
		strcpy(stringa,"");
		strcpy(comando,"");
		strcpy(utente1,"");
		strcpy(utente2,"");
		strcpy(nome_relazione,"");
		cont++;
	}
	


	exit(0);
}







