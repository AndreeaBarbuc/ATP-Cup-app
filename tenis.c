#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INT_MIN NULL

typedef struct jucator	//structura de date pentru jucatori
{
    char*nume, *prenume;
    int scor;

} JUCATOR;

typedef struct tara	//structura de date pentru tara
{
    int nr_juc;
    char*nume_tara;
    int scor_global;
    JUCATOR* jucatori;

} TARA;

typedef TARA Data;

Data* aloc_mem_t(Data*tara, int nr)	//functie ce aloca memorie pentru tarile participante
{
    tara = malloc(nr * sizeof(Data));
    return tara;
}

typedef struct nod	//structura de date pentru lista dublu inlantuita de tari
{
    Data* val;
    struct nod *urm, *ant;

} NOD;

typedef struct nod_s   //lista dublu inlantuita de scoruri
{
    float val;
    struct nod_s *urm, *ant;

} NOD_S;

typedef struct Elem Node;  //stiva
struct Elem
{
    Data* val; 		// datele efectiv memorate (tarile memorate in stiva)
    struct Elem* urm;		 // legatura catre nodul urmator
};

typedef struct Elem_coada Nod_coada;  //nod_coada
struct Elem_coada
{
    JUCATOR *val;	 	// datele efectiv memorate
    struct Elem_coada *urm; 		// legatura catre nodul urmator
};

struct Q		//coada
{
    Nod_coada *front, *rear;	//inceputul, respectiv finalul cozii
};
typedef	struct	Q Queue;

typedef struct Nod_arbore	//structura de date pentru arbore
{
    JUCATOR* val;			//in arbore sunt stocate valori de tip JUCATOR
    struct Nod_arbore *left, *right;			//legatura catre subarborii din stanga si dreapta

} Nod_arbore;

typedef struct elem stackNode;		  //structura pentru stiva (pt arbori)
struct elem
{
    Nod_arbore *val; 		//datele memorate de tip nod
    struct elem *next; 		//legatura catre urmatorul element
};

char* setsir(char*x, FILE*f)		//functia aloca memorie pentru un sir de caractere citit din fisierul "date.in"
{
    char aux[100];		//variabila locala in care stocam valoarea citita

    fscanf(f,"%s",aux);

    x = malloc((strlen(aux) + 1) * sizeof(char));	//alocam memorie pentru "sirul utilizat" in problema (doar atat cat e nevoie)
    //aflam in strlen dimensiunea exacta a sirului citit, iar "+1" este pt null

    return strcpy(x,aux);	//copiez in x(variabila folosita in problema) informatia stocata in sirul initial si returnez valoarea
}

void ad_la_poz_db(NOD**head, Data* v, int pos)        //adaug pe pozitia pos un nod cu informatia v
{
    NOD *headcopy = *head;			//headcopy e copia listei head
    int p = 0;					//cu ajutorului lui p aflam cand ajungem la pozitia dorita (pos)

    while (headcopy->urm!=*head && p<pos)		//cat timp nodul curent e diferit de santinela si nu a ajuns la pozitia dorita
    {
        //"inaintam" in lista si crestem contorul p
        headcopy = headcopy->urm;
        p++;
    }                          				//cand una din conditii nu e indeplinita iesim din while
    //insemnand ori ca am ajuns la pozitia dorita ori ca am terminat de parcurs lista

    NOD *newNode = malloc(sizeof(NOD)); 	//alocam memorie pentru noul nod (cel pe care dorim sa-l adaugam)
    newNode->val = v;				//stocam valoarea data

    /*in problema vom da mereu ultima pozitie, deci se va adauga intotdeauna la final*/

    if ( p == pos && headcopy->urm != *head )      //daca nu se adauga la final
    {
        (headcopy->urm)->ant = newNode;		//facem legaturile pentru a adauga noul nod
        newNode->urm = headcopy->urm;
        headcopy->urm = newNode;
        newNode->ant = headcopy;
    }

    else if ( p == pos && headcopy->urm == *head)      //daca se adauga la final
    {
        newNode->urm = headcopy->urm;
        headcopy->urm = newNode;			//cream legaturile pentru a adauga noul nod
        newNode->ant = headcopy;
    }
}

void citire_elem(Data *v, int *n, FILE *f);		//functia se poate gasi mai jos

void afisare_db_nume(NOD *head, int *n, FILE *rezultate)		//parcurgerea si afisarea listei
{
    NOD *headcopy = head;						//(mai exact a numelor tarilor stocate in lista)
    int i;	//contor
    for(i=0; i < *n; i++)		//*n reprezinta numarul de tari
    {
        fprintf(rezultate,"%s\n",headcopy->val->nume_tara);		//afisam in fisier
        headcopy=headcopy->urm;		//inaintam in lista
    }
}

void sterge_lista_db(NOD **head)	//functie pentru stergerea listei de tari
{
    NOD *aux = malloc(sizeof(NOD)), *headcopy = (*head)->urm;	//copia va incepe de la valoarea imediat urmatoare santinelei

    while (headcopy != *head)				//parcurg lista_copie cat timp e diferita de santinela
    {
        aux = headcopy;			//in aux stochez nodul curent
        headcopy = headcopy->urm;		//fac legatura catre nodul urmator
        free(aux);			//sterg nodul stocat in aux
    }

    free(*head);		//sterg "lista"
    *head = NULL;		//acum va pointa catre null
}

void sterge_lista_db_simplu(NOD **head)		//functie pentru stergerea listei cu toate tarile
{
    NOD *aux = malloc(sizeof(NOD)), *headcopy = *head;		//copia listei

    while (headcopy != NULL)			//parcurg lista_copie cat timp e diferita de NULL
    {
        aux = headcopy;				//in aux stochez nodul curent
        headcopy = headcopy->urm;		//fac legatura catre nodul urmator
        free(aux);				//sterg nodul stocat in aux
    }

    free(*head);		//sterg "lista"
    *head = NULL;		//acum va pointa catre null
}

void sterge_lista_db_S(NOD_S **head)		//functia elibereaza memoria ocupata de lista de scoruri
{
    NOD_S *aux, *headcopy = (*head)->urm;	// procedeul e asemanator cu cel al stergerii listei dublu inlantuite de tari :)

    while (headcopy != *head)
    {
        aux = headcopy;
        headcopy = headcopy->urm;
        free(aux);
    }

    free(*head);
    *head = NULL;
}


void citire_elem(Data *v, int *n, FILE *f)		//functia citeste o tara
{
    //(cu toate datele aferente ei - se pot vedea in structura de date TARA) din fisier

    int j, ct; 	//in ct stocam numarul de jucatori corespunzatori fiecarei tari
    		//j e un contor

    fscanf(f,"%d",&(v)->nr_juc);	//citim din fisier numarul de jucatori

    ct = (v)->nr_juc;
    *n = ct;		//transmitem valoarea

    (v)->nume_tara = setsir( (v)->nume_tara, f );	//citim numele tarii cu functia setsir

    (v)->jucatori = malloc(sizeof(JUCATOR) * ct);	//alocam memorie pentru jucatorii unei tari

    for(j = 0; j < ct; j++)	//parcurgem structura de date JUCATOR corespunzatoare fiecarei tari
    {

        (v)->jucatori[j].nume=setsir((v)->jucatori[j].nume,f);	//citim numele jucatorului de pe pozitia j cu functia setsir

        (v)->jucatori[j].prenume=setsir((v)->jucatori[j].prenume,f);	//citim prenumele jucatorului de pe pozitia j cu functia setsir

        fscanf(f,"%d",&(v)->jucatori[j].scor);	//citim scorul jucatorului de pe pozitia j din fisier
    }
}


void inserareFinal(NOD **head, Data *valoare) 		//functie ce adauga valori la finalul unei liste dublu inlantuita
{
    //cream noul nod si ii atribuim valoarea din paramentru
    NOD *elem_final = malloc(sizeof(NOD));
    elem_final->val = valoare;
    elem_final->urm = *head;
    elem_final->ant = NULL;		//aceasta lista nu are santinela si nu este circulara

    if (*head == NULL) // in cazul in care lista noastra este vida, punem elementul in lista
        *head = elem_final;
    else
    {
        NOD *nod_curent = *head;

        while (nod_curent->urm != NULL)		//parcurgem lista pana la final
        nod_curent = nod_curent->urm;

        //mutam sageata ultimului element catre elementul creat anterior
        elem_final->urm = nod_curent->urm;
        nod_curent->urm = elem_final;
        elem_final->ant = nod_curent;

    }
}

float scor(Data *val)		//functia afla scorul initial al unei tari
{
    float suma = 0;
    int i;

    for(i = 0; i < (val)->nr_juc; i++)
    {
        suma = suma + (val)->jucatori[i].scor;		//se calculeaza scorurilor personale ale jucatorilor
    }

    return ( suma / (val)->nr_juc );	//se returneaza media aritmetica a scorurilor personale ale jucătorilor din lotul țării respective
}

void inserareFinal_S(NOD_S **head, float *valoare) 	//functia insereaza scorul initial al unei tari la finalul
{							//unei liste dublu inlantuita cu santinela si circulara
    //cream noul nod si ii atribuim valoarea din paramentru
    NOD_S *elem_final = malloc(sizeof(NOD_S));
    elem_final->val = *valoare;

    NOD_S *nod_curent = *head;

    while (nod_curent->urm != *head)	//parcurgem lista pana la final
        nod_curent = nod_curent->urm;

    //mutam sageata ultimului element catre elementul creat anterior
    elem_final->urm = nod_curent->urm;
    nod_curent->urm = elem_final;
    elem_final->ant = nod_curent;
}

NOD* cautarePozitie(NOD *head, int pozitie)	//functia cauta pozitia unui element intr-o lista dublu inlantuita circulara, cu santinela
{
    int i = 0;  //pozitia curenta

    //parcurgem lista pana la pozitia curenta, sau
    //pana ajungem la ultimul element al listei

    NOD*cap=head;		//copia listei

    while (cap->urm != head && i < pozitie)		//parcurgem lista cat timp nodul curent nu e egal cu santinela
    {							//si inca nu am ajuns la pozitia dorita
        cap = cap->urm;
        i++;
    }	
    //iesim din while daca am ajuns la pozitia dorita sau daca am terminat de parcurs lista

    if(cap->urm != head && i == pozitie) return cap->urm;
    //in cazul in care am gasit pozitia ceruta, returnam nodul de pe pozitia urmatoare (din cauza santinelei)

    else return NULL;	//inseamna ca nu a gasit pozitia data si returnam NULL
}

void afla_min(NOD **head, NOD_S **lista_scor, int *n)	//functia elimina tarile ( (*) n ) cu scorul initial minim
{
    //in nod_min stocam nodul (de pe pozitia) de sters, headcopy1 e copia listei de tari
    NOD *nod_min=malloc(sizeof(NOD)),*headcopy1=*head;

    //cursor2 e cursorul cu care parcurgem lista de scoruri, headcopy 2 e copia listei de scoruri
    NOD_S *cursor2=malloc(sizeof(NOD)),*headcopy2=*lista_scor;

    Data* aux;		//in aux stocam valoarea din nodul din lista de tari
    float mini;
    int p, ok = 0;	/*ok verifica daca mai exista vreun alt min in afara de cel de pe prima pozitie*/
    			//p reprezinta pozitia nodului curent in lista

    mini = headcopy2->urm->val;	//minimul din lista de scoruri, primul element din lista, excluzand santinela

    if (headcopy1->urm == *head)
        printf("\nLista de tari e goala.");

    if(headcopy2->urm == *lista_scor)
        printf("\nLista de scoruri e goala.");

    for(cursor2 = headcopy2->urm, p = 0; cursor2 != *lista_scor; cursor2 = cursor2->urm, p++) 
    {		//parcurgem lista de scoruri, sarind peste santinela

        if(cursor2->val < mini)			//daca gasim un element mai mic decat minimul
        {
            ok++;
            mini = cursor2->val;		//minimul ia valoarea acelui element
            nod_min = cautarePozitie(*head,p);	//aflam nodul din lista de tari care se afla pe acea pozitie in lista
            aux = nod_min->val;			//stim ca pozitia scorului initial aferent unei tari e identica
            		// cu pozitia acelei tari in lista, intrucat introducerea datelor in cele doua liste s-a facut "sincronizat"

        }

        if(ok == 0)		/*inseamna ca minimul e chiar pe prima pozitie*/
        {
            nod_min = cautarePozitie(*head,0);	//pozitia 0 in lista (primul element din lista de tari, exceptand santinela)
            aux = nod_min->val;	//stocam valoarea de la acel nod
        }

    }

    // mai jos stergem nodul minim


    while (headcopy2->urm != *lista_scor)  	//cat timp elm urm diferit de cap
    {
        if(headcopy2 == *lista_scor)
            headcopy2 = headcopy2->urm;  	//daca elem e = cu capul atunci inseamna ca
        					//e agal cu santinela si ii vom atribui val urm elem

        while (headcopy2->val != mini)	//parcurgem lista pana la intalnirea valorii minime
        {
            headcopy2 = headcopy2->urm;
        }

        //facem legaturile in lista (intrucat stergem nodul)
        (headcopy2 ->urm)->ant = headcopy2->ant;
        (headcopy2->ant)->urm = headcopy2->urm;

        free(headcopy2);		/*nu mai am nevoie de un auxiliar*/
        break;			//deoarece vrem sa stergem primul element gasit

        if(headcopy2->urm == *lista_scor && headcopy2->val== mini) 	//daca se afla pe ultima pozitie
            free(headcopy2);	//stergem nodul din lista
    }

    while (headcopy1->urm != *head)
    {
        if(headcopy1 == *head)
            headcopy1 = headcopy1->urm;

        while (headcopy1->val != aux)		//parcurgem lista pana la intalnirea tarii in functie de care vrem sa stergem nodul
        {
            //adica vom sterge nodul care contine valoarea structurii aux ("tara")
            headcopy1 = headcopy1->urm;		//in rest, procedeul e asemanator cu cel de la lista de scoruri (mai sus)
        }

        (headcopy1 ->urm)->ant = headcopy1->ant;
        (headcopy1->ant)->urm = headcopy1->urm;

        free(headcopy1); /*nu mai am nevoie de un auxiliar*/
        break;


        if(headcopy1->urm == *head && headcopy1->val== aux)
        {
            free(headcopy1);
        }

    }

}

int tari_sterse(int *n)		//functia calculeaza cate tari vor fi sterse
{
    int putere = 0, nr_tari_ramase = 1, copie_nr = *n, nr_tari_sterse;

    while(copie_nr != 0)
    {
        copie_nr = copie_nr / 2;

        if(copie_nr != 0)
            putere++;		//aflam cea mai mare putere a lui 2
    }

    while(putere != 0)
    {
        nr_tari_ramase = nr_tari_ramase * 2;		//inmultim cu 2 cat timp puterea devine 0
        putere--;					//aflam numarul de tari ramase (2 la cea mai mare putere a sa)
    }

    nr_tari_sterse= *n - nr_tari_ramase;	//aflam cate tari trebuie sa stergem din lista

    return nr_tari_sterse;
}

void push(Node **top, Data *v)                          //varful trebuie intors modificat din functie
{
    //functie de adaugare a unui element in stiva
    Node* newNode = malloc(sizeof(Node)); 	   //aloc memorie pentru noul nod in care voi stoca tarile

    newNode->val = v;                   	        //ii atribui valoarea stocata in parametru
    newNode->urm = *top;				//fac legatura catre urmatorul nod(initial)
    *top = newNode;				//varful stivei este acum noul nod
}


int isEmpty(Node *top)    //verificare daca stiva e goala
{
    return top == NULL;
}

Data* pop(Node **top) // se returneaza informatia stocata in varf si se sterge acest nod
{
    if (isEmpty(*top))
        return INT_MIN; //daca stiva e goala returnez o valoare standard definita de mine

    Node *temp = (*top); //stochez adresa varfului in temp

    Data* aux = temp->val;     //stochez valoarea din varf in aux
    *top = (*top)->urm;

    free(temp);		//sterg elementul din varf
    return aux;		//returnez valoarea din varf
}

Queue* createQueue()		//functia creeaza o coada
{
    Queue *q;
    q = malloc(sizeof(Queue));

    if (q == NULL)
        return NULL;

    q->front=q->rear=NULL;

    return q;
}

void enQueue(Queue *q, JUCATOR *v)		//functia introduce un nod (ce contine valoare de tip JUCATOR) in coada
{
    Nod_coada* newNode = malloc(sizeof(Nod_coada));		//alocam memorie pentru noul nod

    newNode->val = v;
    newNode->urm = NULL; 		//nodurile noi se adauga la finalul cozii

    if (q->rear == NULL)
        q->rear = newNode;  		//daca nu aveam niciun nod in coada

    else
    {
        //se adauga la finalul cozii
        (q->rear)->urm = newNode;
        (q->rear) = newNode;
    }

    if (q->front == NULL)
        q->front = q->rear; 		// daca e singurul element din coada

}

int isEmpty_coada(Queue *q)		//functie care verifica daca coada e goala
{
    return (q->front == NULL);
}

JUCATOR* deQueue(Queue *q)		//functie ce scoate un element din coada
{
    Nod_coada* aux;
    JUCATOR *d;

    if (isEmpty_coada(q))
        return INT_MIN;		//daca coada e goala returnez o valoare prestabilita de mine

    aux = q->front;		//aux e primul element din coada
    d = aux->val;		//in d stochez valoarea din nodul aux

    q->front = (q->front)->urm;			//fac legatura catre urmatorul element
    free(aux);			//eliberez memoria ocupata de aux

    return d;		//returnez valoarea ce se afla in aux (in cazul meu: jucatorul)
}

void deleteStack(Node **top)   		//functie pentru stergerea stivei
{
    Node *topCopy = *top, *temp;  	//variabile locale

    while (topCopy != NULL)  		//cat timp stiva nu e goala stergem pe rand valorile din ea
    {
        temp = topCopy;    		//in variabila temporara punem primul nod(cel din cap)
        topCopy = topCopy->urm;  	//pointam catre urmatorul nod
        free(temp);  			//eliberam spatiul ocupat de variabila locala
    }

    *top=NULL;  //dupa ce operatiunea anterioara s-a terminat varful stivei va pointa catre null
}


int tara_castig(Node **stackTop, Node **WINNER, int nr_etapa, FILE *rezultate)		//functia determina ce tari vor fi puse in stiva WINNER
{
    //si le adauga in stiva
    //aloc memorie pentru cele doua tari care vor fi scoase din stiva stackTop,
    //dar si pentru variabila in care vom stoca valoarea tarii ce va fi pusa in stiva WINNER
    Data*tara1 = malloc(sizeof(Data)),*tara2=malloc(sizeof(Data)),*tara_castigatoare=malloc(sizeof(Data));

    //aloc memorie pentru cei doi jucatori ce vor fi scosi din coada q
    JUCATOR *jucator1 = malloc(sizeof(JUCATOR)),*jucator2=malloc(sizeof(JUCATOR));

    //i1,i2 contori, iar nr_tari_W reprezinta numarul tarilor din stiva WINNER
    int i1,i2,nr_tari_W=0;

    Queue* q;	//declaram coada
    q = createQueue();	//o cream/initializam

    fprintf(rezultate,"\n====== ETAPA %d ======\n",nr_etapa);		//afisam in fisier numarul etapei curente

    while(!isEmpty(*stackTop))		//cat timp stiva nu e goala
    {
        //scoatem primele doua tari din stiva (din varf)
        tara1 = pop(&*stackTop);
        tara2 = pop(&*stackTop);

        if(nr_etapa == 1)		//daca e prima etapa atunci scorul global al fiecarei tari e 0
        {
            tara1->scor_global = 0;
            tara2->scor_global = 0;
        }

        //afisam in fisier numele tarilor care concureaza cu datele initiale
        fprintf(rezultate,"\n%s %d ----- %s %d\n",tara1->nume_tara, tara1->scor_global, tara2->nume_tara, tara2->scor_global);

        //parcurgem primele doua tari (mai exact "jucatorii lor")
        //introducem in coada jucatorul primei tari, apoi toti jucatorii celei de-a doua tari
        //procesul se repeta pana cand nu mai avem ce jucatori sa adaugam in coada de meciuri(sistemul de referinta e prima tara)

        for(i1 = 0; i1 < tara1->nr_juc; i1++)
        {
            for(i2 = 0; i2 < tara2->nr_juc; i2++)
            {
                enQueue(q,tara1->jucatori+i1);
                enQueue(q,tara2->jucatori+i2);
            }
        }

        int scor_local_t1 = 0, scor_local_t2 = 0;

        while(!isEmpty_coada(q))	//cat timp coada(de meciuri) cu jucatori nu e goala
        {
            //scoatem 2 jucatori din coada
            //astfel se urmareste ca jucatorul primei tari sa joace cu toti ai celei de-a doua tari si tot asa pana cand
            //toti jucatorii primei tari au jucat cu toti jucatorii celei de-a doua tari

            jucator1=deQueue(q);
            jucator2=deQueue(q);

            //afisam in fisier confruntarea dintre cei doi jucatori
            fprintf(rezultate,"%s %s %d vs %s %s %d\n",jucator1->nume, jucator1->prenume, jucator1->scor, jucator2->nume, jucator2->prenume, jucator2->scor);

            if(jucator1->scor > jucator2->scor)			//daca scorul primului jucator este mai mare decat al celui de-al doilea
            {
                jucator1->scor = jucator1->scor + 5;			//lui i se vor adauga 5 puncte la scorul personal
                scor_local_t1 = scor_local_t1 + 3;			//iar tarii din care face parte i se vor adauga 3 puncte
               		 // la scorul local, cel specific "acestui meci"
            }

            //in caz contrar se intampla acelasi lucru, dar pentru celalalt jucator, respectiv cealalta tara
            else if(jucator1->scor < jucator2->scor)
            {
                jucator2->scor = jucator2->scor + 5;
                scor_local_t2 = scor_local_t2 + 3;
            }

            //daca cei doi jucatori au acelasi scor
            //fiecare va primi cate 2 puncte la scorul personal, iar tara din care fac parte cate un punct la scorul local
            else
            {
                jucator1->scor = jucator1->scor + 2;
                jucator2->scor = jucator2->scor + 2;
                scor_local_t1 = scor_local_t1 + 1;
                scor_local_t2 = scor_local_t2 + 1;
            }
        }

        //la scorul global al fiecarei tari se adauga scorul local acumulat in urma meciurilor dintre jucatori
        tara1->scor_global = tara1->scor_global + scor_local_t1;
        tara2->scor_global = tara2->scor_global + scor_local_t2;

        //max1 si max2 reprezinta maximurile scorurilor celor doua tari care concureaza,
        //initial acestea fiind cele de pe prima pozitie
        int max1 = tara1->jucatori->scor, max2 = tara2->jucatori->scor, j1, j2;

        //stocam in tara_castigatoare statul care are cel mai mare scor local,initial,personal
        if(scor_local_t1 > scor_local_t2)
            tara_castigatoare = tara1;

        else if(scor_local_t1 < scor_local_t2)
            tara_castigatoare = tara2;

        //daca cele doua tari au acelasi scor local
        else if(scor_local_t1 == scor_local_t2)
        {
            for(j1 = 0; j1 < tara1->nr_juc; j1++)
            {
                //se determina care este cel mai mare scor personal din prima tara
                if(tara1->jucatori[j1].scor > max1)
                    max1 = tara1->jucatori[j1].scor;
            }

            for(j2 = 0; j2 < tara2->nr_juc; j2++)
            {
                //se determina care este cel mai mare scor personal din a doua tara
                if(tara2->jucatori[j2].scor > max2)
                    max2 = tara2->jucatori[j2].scor;
            }

            //se compara cele doua scoruri maxime specifice fiecarei tari in parte
            //astfel vom stoca in tara_castigatoare valoarea statului în lotul căreia se află jucătorul
            // cu cel mai mare scor personal (dintre scorurile personale ale jucătorilor celor două țări)

            if(max1 > max2) tara_castigatoare = tara1;

            else if(max1 < max2)
                tara_castigatoare = tara2;
            //daca si de aceasta data avem egalitate intre valori tara_castigatoare ia valoarea primei tari scoase din stiva stackTop

            else if(max1 == max2)
                tara_castigatoare = tara1;
        }

        push(&*WINNER,tara_castigatoare);	//adaugam in stiva WINNER tara castigatoare
        //in urma meciurilor dintre jucatorii specifici celor doua scoase din stiva

	 nr_tari_W++;		//de fiecare data cand scoatem cate doua tari din stiva stackTop
        // creste cu 1 numarul de tari introduse in stiva WINNER

    }
    return nr_tari_W;		//returnam numarul de tari din stiva WINNER
}

void ad_inceput_db(NOD **head, Data *v)		//functia adauga la inceputul unei liste dublu inlantuite
{
    NOD* newNode = malloc(sizeof(NOD)),*headcopy=*head;		//alocam spatiu pentru noul nod(newNode), iar headcopy reprezinta copia listei
    newNode->val = v;			//stocam valoarea data in parametrul v

    //facem legaturile
    newNode->urm = *head;
    newNode->ant = NULL;
    (headcopy)->ant = newNode;
    *head = newNode;
}

void sterge_nod(NOD **head, NOD* nod_sters)		//functie ce sterge un nod dintr-o lista dublu inlantuita
{
    if (*head == NULL || nod_sters == NULL)	// daca lista e goala
        return;

    if (*head == nod_sters)		// daca nod de sters e capul listei
        *head = nod_sters->urm;

    // daca nodul de sters nu e ultimul nod: se leaga urmatorul nod de cel anterior celui de sters
    if (nod_sters->urm != NULL)
        nod_sters->urm->ant = nod_sters->ant;

    // daca nodul de sters nu e primul in lista: se leaga nodul anterior de nodul care urmeaza dupa cel de sters
    if (nod_sters->ant != NULL)
        nod_sters->ant->urm = nod_sters->urm;

    free(nod_sters); 	// eliberam memoria ocupata de nodul de sters
}

void sterge_toate_nodurile_dorite(NOD **head, char *nume)
{
    //functie care sterge toate nodurile ce contin valoarea stocata in parametrul nume dintr-o lista dublu inlantuita

    NOD* headcopy = *head;
    NOD* contor;

    if ((*head) == NULL)		// daca lista e goala iese din functie (inseamna ca nu avem ce sterge)
        return;

    while (headcopy != NULL)		 // parcurge lista pana la final
    {
	
        if (headcopy->val->nume_tara == nume) 		// daca gasim nodul ce contine valoarea data
        {
            contor = headcopy->urm;		// salvam in pointerul contor adresa urmatorului nod

            sterge_nod(head, headcopy);			// sterge nodul in care se gaseste valoarea data

            headcopy = contor;			// actualizeaza nodul headcopy
        }

        // daca nu s-a gasit merge mai departe
        else
            headcopy = headcopy->urm;
    }
}

void deleteQueue(Queue *q)		//functie ce elibereaza memoria ocupata de coada creata
{
    Nod_coada* aux;

    while ( ! isEmpty_coada(q) )		//cat timp coada nu e goala
    {
        aux = q->front;			//in aux stocam primul nod din coada
        q->front = q->front->urm;			//facem legatura catre urmatorul nod din coada
        free(aux);			//eliberam memoria ocupata de aux
    }

    free(q);		//eliberam memoria ocupata de coada
}

Nod_arbore* newNode(JUCATOR* data)		//functia creeaza un nod nou pentru arbore
{
    Nod_arbore* node = malloc(sizeof(Nod_arbore));		//alocam memorie pentru noul nod
    node->val= data;				//stocam in el valoarea data prin parametru
    node->left = node->right = NULL;		//copiii pointeaza catre NULL

    return node;		//returnam nodul creat
}

Nod_arbore* insert(Nod_arbore *node, JUCATOR* key_juc)			//functia insereaza un jucator in arbore
{
    // daca (sub)arborele e gol – se creaza un nod si se returneaza adresa
    if (node == NULL)
        return newNode(key_juc);

    if(key_juc->scor == node->val->scor)		//daca scorul jucatorului este egal cu cel din nodul curent
    {

        if(strcmp(key_juc->nume, node->val->nume) == 0)			//daca cei doi jucatori au acelasi nume
        {

            //le comparam prenumele
            //daca prenumele jucatorului "e mai mic" d.p.d.v lexicografic (adica, alfabetic)
            //se compara litera cu litera pana cand una din ele nu e egala
            //astfel,daca jucatorul dat ca parametru se afla inaintea celui din arbore, alfabetic vorbind,
            // atunci key_juc va fi pus in arbore
            if(strcmp(key_juc->prenume,node->val->prenume) < 0)
                node->val = key_juc;
        }
		
        	//altfel, inseamna ca numele celor doi jucatori nu sunt identice si se respecta procedeul descris anterior
        else if(strcmp(key_juc->nume,node->val->nume) < 0)
            node->val = key_juc;

    }

    //altfel, cum nu se afla alt jucator cu acelasi scor, va fi introdus in arbore jucatorul
    //daca scorul e mai mic decat celui din radacina curenta va fi pus in subarborele stang, altfel in subarborele drept

    else if (key_juc->scor < node->val->scor)
        node->left  = insert(node->left, key_juc);

    else if (key_juc->scor > node->val->scor)
        node->right = insert(node->right, key_juc);
    //este o functie recursiva,deci se proceseaza fiecare nod radacina pana e introdus jucatorul care trebuie :)

    return node;	//returnam noul arbore creat cu jucatorul dat ca parametru
}

void afisare_arbore(Nod_arbore *root, FILE *rezultate)	//functia afiseaza arborele
{
    if (root)
    {
        afisare_arbore(root->right,rezultate);	//mai intai parcurgem subarborele drept
        //afisam in fisier datele stocate in arbore

        fprintf(rezultate,"\n%s ",root->val->nume);
        fprintf(rezultate,"%s ",root->val->prenume);
        fprintf(rezultate,"%d ",root->val->scor);

        afisare_arbore(root->left,rezultate);		//apoi pe cel stang
    }
}

void push_arbore(stackNode **top, Nod_arbore *v) 		 //functie ce adauga un element de tip nod arbore intr-o stiva
{
    stackNode* newNode = malloc(sizeof(Nod_arbore));  		//aloc memorie pentru noul nod din stiva

    newNode->val = v; 		//valoarea stocata in noul nod al stivei va fi un nod al arborelui
    newNode->next = *top;  		//cream legatura
    *top = newNode;  		//acum noul nod este varful stivei
}

int isEmpty_arbore(stackNode* top)
{
    return top == NULL;  			//functia verifica daca stiva e goala
}

Nod_arbore* pop_arbore(stackNode**top)  	//functie pentru scoaterea unui element (nod_arbore) din stiva
{	
    if (isEmpty_arbore(*top))  		//verific daca stiva e goala, adica daca am elemente de scos
        return NULL;

    stackNode *temp=(*top);  		 //variabila locala,temporara
    Nod_arbore* d=temp->val; 		//in stiva se afla elemente de tip nod_arbore, variabila d salveaza valoarea memorata in varful curent al stivei

    *top=(*top)->next; //varful stivei trece la urmatorul element
    free(temp); //eliberam memoria ocupata de variabila locala, adica stergem acel nod

    return d; // returnam valoarea memorata in varful stivei
}

void deleteStack_arbore(stackNode** top)   //functie pentru stergerea stivei
{
    stackNode* topCopy=*top, *temp;  //variabile locale
    while (topCopy!=NULL)  //cat timp stiva nu e goala stergem pe rand valorile din ea
    {
        temp=topCopy;    //in variabila temporara punem primul nod(cel din cap)
        topCopy=topCopy->next;   //pointam catre urmatorul nod
        free(temp);  //eliberam spatiul ocupat de variabila locala
    }
    *top=NULL;  //dupa ce operatiunea anterioara s-a terminat varful stivei va pointa catre null
}

int cheie(Nod_arbore*root,JUCATOR* val_interv)		//functia determina cheia pe care se afla o anumita valoare data
{
    stackNode*S = NULL;	//stiva in care adaugam valorile
    int cheie=0;   //numarul de noduri scoase din stiva

    while (1) //cat timp am elemente de procesat
    {
        while (root) //daca ranacina nu e NULL
        {
            push_arbore(&S, root);	//il adaug in stiva
            root=root->right; //cat timp exista nod la dreapta – il adaug in stiva
        }

        if (isEmpty_arbore(S))
            break;  //daca stiva e goala "iese"

        root = pop_arbore(&S); //scot primele k elemente
        (cheie)++;  //creste contorul(el reprezinta "pozitia",cheia pe care se afla o anumita valoare)

        if((strcmp(root->val->nume,val_interv->nume)==0) && (strcmp(root->val->prenume,val_interv->prenume)==0) && (root->val->scor == val_interv->scor))
            return cheie;
        //daca valoarea radacinii curente este egala cu cea cautata
        //atunci returnez "pozitia" pe care se afla

        root = root ->left; //si ma deplasez la stanga
    }  // procedeul se repeta
    deleteStack_arbore(&S);  //sterg stiva dupa ce am terminat
    return -1; 	//daca nu am gasit elementul cautat
}

int interval(Nod_arbore *root, JUCATOR *juc1, JUCATOR *juc2)        	//functia afiseaza elementele aflate intre cele doua valori ale nodurilor arborelui
{
    int key = 0, cheie1, cheie2,ct = 0;			//key reprezinta cheia unde gasim valorile arborelui
    cheie1 = cheie(root,juc1); 			 //cheia primei valori a arborelui

    cheie2=cheie(root,juc2);	//cheia celeilalte valori a arborelui

    if(cheie1 == -1 )
        return -1;	//daca nu a fost gasit elementul returnam -1 pentru prima valoare

    if(cheie2 == -1 )
        return -2;	//daca nu a fost gasit elementul returnam -2 pentru a doua valoare

    stackNode *S = NULL; //creez stiva

    while (1)   //cat timp mai am elemente de procesat
    {
        while (root)   		//daca exista nod radacina neprocesat
        {
            push_arbore(&S, root);  		//adaug nodul radacina in stiva
            root = root->right;  		//trec la urmatorul element din subarborele drept
        }

        if (isEmpty_arbore(S))  //verific daca stiva e goala
            break;

        root = pop_arbore(&S);   //scot un element din stiva
        key++;  //creste contorul, el reprezentand cheia nodului(valorii) curent(e)

        if(((key > cheie1) && (key < cheie2)) || ((key < cheie1) && (key > cheie2)))
        {
            //daca cheia "valorii" curente se afla in intervalul cautat,adica intre cheile celorlalte doua noduri

            /*printf ("\n%s ", root->val->nume); 	 //afisam valoarea de la acel nod
            printf ("%s ", root->val->prenume);
            printf ("%d ", root->val->scor);*/

            ct++;	  //numara cate elemente se afla in intervalul cautat
        }

        root = root ->left; 	//si ma deplasez la stanga
    }  // procedeul se repeta

    deleteStack_arbore(&S);  		//sterg stiva dupa ce am terminat

    return ct;		//returnez numarul de elemente dintre cele 2 valori
}

void elib_arbore(Nod_arbore *root) //functie ce elibereaza spatiul ocupat de un arbore binar
{
    if (root)  //daca radacina nu e nula
    {
        elib_arbore(root->left);  //parcurg subarborele stang
        elib_arbore(root->right);  //parcurg subarborele drept

        free(root);  //sterg valorile stocate si eliberez memoria
    }

    root = NULL;  //radacina e acum null
}

int main(int nr_fisiere,char**fisier)
{
    Node* stackTop= NULL, *WINNER=NULL;  
    //declare- creare stive (stiva initiala si cea in care punem tarile castigatoare)
    //in val memoram tarile din fisierul "date.in", in tara_castigatoare memorez valoarea tarii pe care o scot din stiva WINNER
    //in tara_castigatoare stochez valoarea curenta a tarii scoase din stiva WINNER(si ii aloc memorie)
    //in tara_coada stochez valoarea unui stat din lista_tari(ii aloc si memorie)

    Data* val = malloc(sizeof(Data)), *tara_castigatoare = malloc(sizeof(Data)), *tara_coada = malloc(sizeof(Data));

    JUCATOR* juc_in_arbore = malloc(sizeof(JUCATOR));	//aloc spatiu pentru variabila ce va retine jucatorul curent adaugat in arbore
	
    //valoarea unui jucator scos din coada
    //alocam memorie pentru cei doi jucatori din fisier[1]
    JUCATOR *juc1 = malloc(sizeof(JUCATOR)),*juc2 = malloc(sizeof(JUCATOR));

    //head e lista initiala, lista_tari reprezinta lista cu TOATE tarile
    NOD* head = malloc(sizeof(NOD)), *lista_tari = malloc(sizeof(NOD));

    NOD*copie1_lista_initiala = head, *copie2_lista_initiala = head;		//cream doua copii ale listei head

    NOD_S *lista_scor=malloc(sizeof(NOD));  //lista de scoruri

    Nod_arbore *root = NULL;//declarare arbore
	
    Queue *q;				//declarare coada
    q = createQueue();			//initializare coada

    head->urm = head;
    lista_scor->urm = lista_scor;		//astfel,listele head si lista_scor sunt circulare, cu santinela la inceput

    lista_tari = NULL;			//initializam lista_tari (nu e circulara si nu are santinela)

    int i, nr, n, nr_tari_coada = 0, nr_tari_sterse, contor, nr_tari_W, nr_etapa_init = 0;

    int cifra1, cifra2, cifra3, cifra4, cifra5;	//valorile din fisierul "cerinte.in"
    float scor_initial;
    
    FILE *f = fopen(fisier[2],"rb"), *cerinte = fopen(fisier[1],"rb"), *rezultate = fopen(fisier[3],"wb");

    //testam daca cele 3 fisiere s-au deschis
    if(f == NULL)
    {
        printf("Fisierul nu s-a putut deschide!");
        exit(1);
    }

    if(cerinte == NULL)
    {
        printf("Fisierul nu s-a putut deschide!");
        exit(1);
    }

    if(rezultate == NULL)
    {
        printf("Fisierul nu s-a putut deschide!");
        exit(1);
    }

    //citim cele 5 cifre din fisier[1]
    fscanf(cerinte,"%d",&cifra1);
    fscanf(cerinte,"%d",&cifra2);
    fscanf(cerinte,"%d",&cifra3);
    fscanf(cerinte,"%d",&cifra4);
    fscanf(cerinte,"%d",&cifra5);

    //daca cifra1 e 1 inseamna ca se poate rezolva prima cerinta
    //situatia de mai sus e valabila si pentru celelalte 4 cifre ramase
    if(cifra1 == 1)
    {
        fscanf(f,"%d",&n);	//citim numarul de tari din fisier[2]

        int p = 0;	//p reprezinta pozitia pe care adaugam tara citita in lista head
        val = aloc_mem_t(val,n);

        for(i=0; i<n; i++)	//se repeta procesul pana cand contorul i este egal cu (numarul de tari participante -1)
        {
            citire_elem(val + i,&nr,f);		//citesc un element, o structura de date de tip Data(TARA)
            scor_initial = scor(val + i);		//calculez scorul initial pentru elementul citit

            ad_la_poz_db(&head,val+i,p);	//introduc elementul citit la finalul listei head

            inserareFinal_S(&lista_scor,&scor_initial);		//introduc scorul calculat la finalul listei lista_scor

            p++;	//creste pozitia pe care voi adauga(adica la final)
        }

        if((cifra1 == 1) && (cifra2 == 0) && (cifra3 == 0) && (cifra4 == 0) && (cifra5 == 0))
        {
            afisare_db_nume(head->urm,&n,rezultate);		//afisez in fisier[3] numele tarilor din lista head
        }							//numele tarilor vor fi adaugate doar daca se respecta conditia
        //astfel, in viitoarele teste nu vor fi vizibile
    }	
							
    /*pana aici a fost cerinta 1*/

    if(cifra2 == 1)
    {
        nr_tari_sterse = tari_sterse(&n);

        for(contor = 0; contor < nr_tari_sterse; contor++)	//sterge atatea tari cate indica nr_tari_sterse
        {
            afla_min(&head,&lista_scor,&n);		//sterge tarile ce au scorul initial minim*/
        }

        n=n-nr_tari_sterse;	//noul numar de tari = nr tarilor ramase in competitie*/

        afisare_db_nume(head->urm,&n,rezultate);	//afisam in fisier[3] numele tarilor ramase in competitie*/

    }

    /*pana aici a fost cerinta 2*/


    if(cifra3 == 1)
    {

        while(copie1_lista_initiala->urm != head)
        {
            copie1_lista_initiala = copie1_lista_initiala->urm;		//ne "deplasam" catre urmatorul element
            								//astfel nu vom adauga santinela in stiva
            push(&stackTop,copie1_lista_initiala->val);			//adaugam in stiva stackTop tarile ramase in competitie
        }

        while(copie2_lista_initiala->urm != head)
        {
            //adaugam la finalul unei liste fara santinela si necirculara tarile ramase in competitie
            inserareFinal(&lista_tari,copie2_lista_initiala->urm->val);

            //trecem la urmatorul element
            copie2_lista_initiala = copie2_lista_initiala->urm;
        }

        do
        {
            nr_etapa_init++;	//contorizam numarul etapei

            //stocam in nr_tari_W nr de tari din stiva WINNER
            nr_tari_W = tara_castig(&stackTop,&WINNER,nr_etapa_init,rezultate);	//cream stiva WINNER

            fprintf(rezultate,"\n=== WINNER ===\n");

            while(! isEmpty(WINNER))		//cat timp stiva WINNER nu e goala
            {
                //mutam elementele din ea in stiva stackTop care acum e goala
                tara_castigatoare = pop(&WINNER);

                fprintf(rezultate,"%s --- %d\n",tara_castigatoare->nume_tara,tara_castigatoare->scor_global);

                push(&stackTop,tara_castigatoare);

                ad_inceput_db(&lista_tari,tara_castigatoare);	//adaugam tarile din stiva WINNER in lista cu toate tarile
            }							// (cream o lista cu toate tarile)
        }

        while(nr_tari_W != 1);		//procesul se repeta pana cand in stiva WINNER ramane doar o tara

    }

    deleteStack(&stackTop);
    deleteStack(&WINNER);

    /*pana aici a fost cerinta 3*/

    if(cifra4 == 1)
    {
        int ct_juc;
        while(lista_tari != NULL && nr_tari_coada != 4)		//cat timp lista cu toate tarile nu e goala
        {							//si inca nu am adaugat jucatorii a 4 tari in coada

            tara_coada = lista_tari->val;	//stocam in tara_coada prima valoare din lista de tari
            //in lista_tari statele sunt pozitionate astfel: prima valoare corespunde celei ramase in stiva WINNER
            //apoi se afla cele din stiva WINNER precedenta si tot asa, pana ajungem la tarile din lista head modificata
            //unde sunt in ordinea in care erau si in lista

            //stergem toate tarile cu numele primei tari din lista
            //astfel vom reusi sa adaugam in coada jucatorii ultimelor patru tari ramase in competitie
            sterge_toate_nodurile_dorite(&lista_tari,tara_coada->nume_tara);

            for(ct_juc = 0; ct_juc < tara_coada->nr_juc; ct_juc++)
            {
                enQueue(q,tara_coada->jucatori + ct_juc);		//adaugam in coada toti jucatorii primei tari din lista
            }

            nr_tari_coada++;		//crestem contorul

        }		//procedeul se repeta


        while(! isEmpty_coada(q))		//cat timp coada cu jucatori nu e goala
        {
            juc_in_arbore = deQueue(q);			//scoatem cate un jucator din coada
            root=insert(root,juc_in_arbore);		//si il introducem in arbore
        }

        fprintf(rezultate,"\n====== CLASAMENT JUCATORI ======");
        afisare_arbore(root,rezultate);		//afisam arborele cu toti jucatorii celor 4 tari in fisier[3]

        deleteQueue(q);		//eliberam memoria ocupata de coada
    }

    /* pana aici a fost cerinta 4 */

    if(cifra5 == 1)
    {

        //citim datele celor doi jucatori din fisier[1]

        juc1->nume = setsir(juc1->nume,cerinte);
        juc1->prenume = setsir(juc1->prenume,cerinte);
        fscanf(cerinte,"%d",&juc1->scor);

        juc2->nume = setsir(juc2->nume,cerinte);
        juc2->prenume = setsir(juc2->prenume,cerinte);
        fscanf(cerinte,"%d",&juc2->scor);

        int nr_juc_interv;

        nr_juc_interv=interval(root,juc1,juc2);			//"numarul" de jucatori aflati intre cei doi (cei din fisier[1])

        if(nr_juc_interv > 0)		//daca nr_juc_interv>0 inseamna ca am gasit cei doi jucatori si ca se afla si altii intre ei
        {
            fprintf(rezultate,"\n\n%d",nr_juc_interv);	//afisam numarul de jucatori in fisier[3]
        }

        if(nr_juc_interv == -1)		//daca e indeplinita conditia inseamna ca nu s-a gasit
        {
            //primul jucator in arborele de jucatori si se afiseaza un mesaj corespunzator
            fprintf(rezultate,"\n\n%s %s nu poate fi identificat!",juc1->nume,juc1->prenume);
        }

        if(nr_juc_interv == -2)		//daca e indeplinita conditia inseamna ca nu s-a gasit
        {
            //al doilea jucator in arborele de jucatori si se afiseaza un mesaj corespunzator
            fprintf(rezultate,"\n\n%s %s nu poate fi identificat!",juc2->nume,juc2->prenume);
        }
    }

    elib_arbore(root);			//eliberam memoria ocupata de arbore

    //eliberez memoria ocupata de cei doi jucatori din fisierul fisier[1]
    free(juc1);
    free(juc2);

    /*pana aici a fost cerinta 5*/

    sterge_lista_db_S(&lista_scor);		 //elibereaza memoria ocupata de lista de scoruri

    sterge_lista_db_simplu(&lista_tari);		//elibereaza memoria ocupata de lista cu toate

    free(val);	//eliberam memoria ocupata de "vectorul" de tari

    fclose(f);			//inchidem fisierul "date.in"(fisier[2])
    fclose(cerinte);		//inchidem fisierul "cerinte.in"(fisier[1])
    fclose(rezultate);		//inchidem fisierul fisier[3]

    //eliberez memoria ocupata de variabilele locale
    free(juc_in_arbore);	
    free(tara_castigatoare);
    free(tara_coada);

    sterge_lista_db(&copie1_lista_initiala);			//eliberam memoria ocupata de prima copie a listei modificate de tari

    sterge_lista_db(&copie2_lista_initiala);			//eliberam memoria ocupata de a doua copie a listei modificate de tari

    sterge_lista_db(&head);				//eliberam memoria ocupata de lista de tari (lista head modificata)

    return 0;
}

