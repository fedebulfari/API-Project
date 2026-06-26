#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 256       //nomi
#define H 9999999   //hash*
#define O 2200      //coda ordini
#define S 50        //profondità scadenze*
#define K 6
//--------------------------------------------------------------------
typedef struct{
    int t;                                              //set time
    int s;                                              //set space
}coriere;
//--------------------------------------------------------------------
typedef struct link{
    int k;
    int x;
}link;
//--------------------------------------------------------------------
typedef struct ingre{
    int k;
    int x;
    int q;                                              //set quantità
    struct ingre *next;
}ingre;
typedef struct{
    int peso;                                           //set peso
    short ts;
    ingre *ing;
    char *n_ric;
}ricetta;
typedef struct{
    ricetta **r;
}ric;
ric ricettario[H];
//--------------------------------------------------------------------
typedef struct{
    int h;
    int i;
    int peso;                                           //set peso
    int q;                                              //set quantità
    int t;                                              //set tempo
}dolci;
typedef struct ord{
    dolci dolce;
    struct ord *next;
}ord;
typedef struct queue_o{
    ord *inizio;
    ord *coda;
}queue_o;
queue_o *ordini=NULL;
//------------------
struct pacchi{
    dolci *dolce;
    int hs;
    int cap;
};
typedef struct pacchi heap;
//--------------------------------------------------------------------
typedef struct{
    int q;                                              //set quantità
    int s;                                              //set scadenza
}scadenza;
struct scadenze {
    scadenza *scad;
    int hs;
    int cap;
};
typedef struct scadenze s_heap;
//--------------------------------------------------------------------
typedef struct{
    int q;                                              //set quantità
    char *n_ing;                                        //set nome
}ingredienti;
typedef struct magaz{
    ingredienti i;
    s_heap *s;                                          //set scadenze
}magaz;
typedef struct{
    magaz **m;
}mag;
mag magazino[H];
//--------------------------------------------------------------------
typedef struct att{
    int h;
    int i;
    int q;
    int t;
    struct att *next;
    struct att *prec;
}att;
typedef struct queue{
    att *inizio;
    att *coda;
}queue;
queue *attesa=NULL;
//--------------------------------------------------------------------
ord *aggiungi_in_testa_o(ord *testa,int h,int i,int q,int t,int peso);
queue_o *aggiungi_in_coda_o(queue_o *testa,int h,int i,int q,int t,int peso);
queue_o *aggiungi_in_ordine_o(queue_o *testa,int h,int i,int q,int t,int peso);
int cerca_o(ord *testa,int h,int i);
queue *aggiungi_in_coda(queue *testa,int h,int i,int q,int t);
int cerca(att *testa,int h, int i);
ingre *aggiungi_in_testa_i(ingre *testa,link cord,int q);
s_heap *createheap_s(int cap);
void minheapify(s_heap *s,int i);
void insertmin(s_heap *s,int scad,int q);
void inserthmin(s_heap *s,int i);
scadenza estraimin(s_heap *s);
heap *createheap(int cap);
void maxheapify(heap *o,int j);
void insert(heap *o,int peso,int h,int i,int q,int t);
void inserth(heap *o,int j);
dolci estraimax(heap *o);
int hash(int k);
void setup(coriere *c);
link linker(char *n_i);
void unlinker(int k,int x);
int aggiungi_ricetta();
int rimuovi_ricetta();
void rifornimento();
int ordine(int t);
void coda(int t);
void cor(int c);
//--------------------------------------------------------------------
int main(){
    int t=0;
    coriere c={0,0};
    short ok=0,m;
    char input[N]={'\0'};
    ordini=(queue_o*)malloc(sizeof(queue_o));
    ordini->inizio=NULL;
    ordini->coda=NULL;
    attesa=(queue*)malloc(sizeof(queue));
    attesa->inizio=NULL;
    attesa->coda=NULL;
    setup(&c);                                          //setup
    while(feof(stdin)==0){
        if(t%c.t==0&&t>0){                              //coriere
            cor(c.s);
        }
        m=scanf("%s",input);
        if(input[0]=='a'){                              //aggiungi
            ok=aggiungi_ricetta();
            if(ok==1){
                printf("aggiunta\n");
            }else{
                printf("ignorato\n");
            }
        }else if(input[0]=='r'&&input[2]=='m'){         //togli
            ok=rimuovi_ricetta();
            if(ok==1){
                printf("rimossa\n");
            }else if(ok==2){
                printf("ordini in sospeso\n");
            }else{
                printf("non presente\n");
            }
        }else if(input[0]=='r'&&input[2]=='f'){         //rifornimento
            rifornimento();
            if(attesa->inizio!=NULL&&attesa->inizio->q!=0)              //sfoltimeno coda
                coda(t);
        }else if(input[0]=='o'){                        //ordine
            ok=ordine(t);
            if(ok==1){
                printf("accettato\n");
            }else{
                printf("rifiutato\n");
            }
        }
        t=t+1;                                          //time
        ok=0;
        fflush(stdout);
        input[0]='\0';
    }
    /*for(h=0;h<H;h++){
     *        if(ricettario[h].r!=NULL){
     *            for(i=0;i<R&&ricettario[h].r[i]!=NULL;i++){
     *                while(ricettario[h].r[i]->ing!=NULL){
     *                    ingre *tmp=ricettario[h].r[i]->ing;
     *                    ricettario[h].r[i]->ing=ricettario[h].r[i]->ing->next;
     *                    free(tmp);
     *                    tmp=NULL;
}
free(ricettario[h].r[i]->n_ric);
ricettario[h].r[i]->n_ric=NULL;
free(ricettario[h].r[i]);
ricettario[h].r[i]=NULL;
}
free(ricettario[h].r);
ricettario[h].r=NULL;
}
if(magazino[h].m!=NULL){
    for(i=0;magazino[h].m[i]!=NULL&&i<M;i++){
        if(magazino[h].m[i]->s!=NULL){
            free(magazino[h].m[i]->s->scad);
            magazino[h].m[i]->s->scad=NULL;
            free(magazino[h].m[i]->s);
            magazino[h].m[i]->s=NULL;
}
free(magazino[h].m[i]->i.n_ing);
magazino[h].m[i]->i.n_ing=NULL;
free(magazino[h].m[i]);
magazino[h].m[i]=NULL;
}
free(magazino[h].m);
magazino[h].m=NULL;
}
}
while(attesa->inizio!=NULL){
    att *tmp=attesa->inizio;
    attesa->inizio=attesa->inizio->next;
    if(attesa->inizio==NULL)
        attesa->coda=NULL;
    free(tmp);
    tmp=NULL;
}
free(attesa);
attesa=NULL;
while(ordini!=NULL){
    ord *tmp=ordini;
    ordini=ordini->next;
    free(tmp);
    tmp=NULL;
}
free(ordini);
ordini=NULL;*/
    t=m;
    return 0;
}
//--------------------------------------------------------------------
void cor(int c){
    int s=0;
    heap *o=createheap(O);
    if(ordini!=NULL){
        if(ordini->inizio->dolce.peso!=0&&ordini->inizio->dolce.peso<c){
            while(ordini->inizio!=NULL&&s+ordini->inizio->dolce.peso<c){
                s=s+ordini->inizio->dolce.peso;
                if(s<c){
                    ord *tmp=ordini->inizio;
                    insert(o,tmp->dolce.peso,tmp->dolce.h,tmp->dolce.i,tmp->dolce.q,tmp->dolce.t);
                    ordini->inizio=ordini->inizio->next;
                    if(ordini->inizio==NULL){
                        ordini->coda=NULL;
                    }
                    free(tmp);
                    tmp=NULL;
                }
            }
            while(o->hs!=0){
                printf("%d %s %d\n",o->dolce[0].t,ricettario[o->dolce[0].h].r[o->dolce[0].i]->n_ric,o->dolce[0].q);
                estraimax(o);
            }
        }else
            printf("camioncino vuoto\n");
    }else if(s==0)
        printf("camioncino vuoto\n");
    if(o->dolce!=NULL){
        free(o->dolce);
        o->dolce=NULL;
    }
    free(o);
    o=NULL;
}
void setup(coriere *c){
    char e;
    int m;
    m=scanf("%d",&c->t);
    m=scanf("%d",&c->s);
    m=scanf("%c",&e);
    return;
    e=(char)m;
}
ord *aggiungi_in_testa_o(ord *testa,int h,int i,int q,int t,int peso){
    if(testa!=NULL){
        ord *testasucc=testa;
        testa=(ord*)malloc(sizeof(ord));
        testa->dolce.h=h;
        testa->dolce.i=i;
        testa->dolce.q=q;
        testa->dolce.t=t;
        testa->dolce.peso=peso;
        testa->next=testasucc;
    }else{
        testa=(ord*)malloc(sizeof(ord));
        testa->dolce.h=h;
        testa->dolce.i=i;
        testa->dolce.q=q;
        testa->dolce.t=t;
        testa->dolce.peso=peso;
        testa->next=NULL;
    }
    return testa;
}
queue_o *aggiungi_in_coda_o(queue_o *testa,int h,int i,int q,int t,int peso){
    ord *nuovo=(ord*)malloc(sizeof(ord));
    nuovo->dolce.h=h;
    nuovo->dolce.i=i;
    nuovo->dolce.q=q;
    nuovo->dolce.t=t;
    nuovo->dolce.peso=peso;
    nuovo->next=NULL;
    if (testa->coda!=NULL) {
        testa->coda->next=nuovo; // Link the old rear to the new node
    }
    testa->coda=nuovo; // Update the rear to the new node
    if (testa->inizio==NULL) { // If the queue was empty, update front
        testa->inizio=nuovo;
    }
    return testa;
}
queue_o *aggiungi_in_ordine_o(queue_o *testa,int h,int i,int q,int t,int peso){
    if(testa->inizio==NULL||testa->inizio->dolce.t>=t){
        testa->inizio=aggiungi_in_testa_o(testa->inizio,h,i,q,t,peso);
    }else{
        ord *nuovo=aggiungi_in_testa_o(NULL,h,i,q,t,peso);
        ord *cor=testa->inizio;
        while(cor->next!=NULL&&cor->next->dolce.t<t){
            cor=cor->next;
        }
        if(cor->next==NULL)
            testa->coda=nuovo;
        nuovo->next=cor->next;
        cor->next=nuovo;
    }
    return testa;
}
int cerca_o(ord *testa,int h,int i){
    int p=-1,trovato=0,j=0;
    for(j=0;testa!=NULL&&trovato==0;j++){
        if(testa->dolce.h==h){
            if(testa->dolce.i==i){
                p=j;
                trovato=1;
            }
        }
        testa=testa->next;
    }
    return p;
}
queue *aggiungi_in_coda(queue *testa,int h,int i,int q,int t){
    att *nuovo=(att*)malloc(sizeof(att));
    nuovo->h=h;
    nuovo->i=i;
    nuovo->q=q;
    nuovo->t=t;
    nuovo->next=NULL;
    nuovo->prec=NULL;
    if (testa->coda!=NULL) {
        testa->coda->next=nuovo; // Link the old rear to the new node
        nuovo->prec=testa->coda;
    }
    testa->coda=nuovo; // Update the rear to the new node
    if (testa->inizio==NULL) { // If the queue was empty, update front
        testa->inizio=nuovo;
    }
    return testa;
}
int cerca(att *testa,int h, int i){
    int p=-1,trovato=0,j=0;
    for(j=0;testa!=NULL&&trovato==0;j++){
        if(testa->h==h){
            if(testa->i==i){
                p=j;
                trovato=1;
            }
        }
        testa=testa->next;
    }
    return p;
}
ingre *aggiungi_in_testa_i(ingre *testa,link cord,int q){
    if(testa!=NULL){
        ingre *testasucc=testa;
        testa=(ingre*)malloc(sizeof(ingre));
        testa->k=cord.k;
        testa->x=cord.x;
        testa->q=q;
        testa->next=testasucc;
    }else{
        testa=(ingre*)malloc(sizeof(ingre));
        testa->k=cord.k;
        testa->x=cord.x;
        testa->q=q;
        testa->next=NULL;
    }
    return testa;
}
s_heap *createheap_s(int cap){
    s_heap *s=(s_heap*)malloc(sizeof(s_heap));
    if(s==NULL)
        return NULL;
    s->hs=0;
    s->cap=cap;
    s->scad=(scadenza*)malloc(cap*sizeof(scadenza));
    if(s->scad==NULL)
        return NULL;
    return s;
}
void minheapify(s_heap *s,int i){
    int l=i*2+1,r=i*2+2,min=i;
    if(l<s->hs&&l>=0&&s->scad[l].s==s->scad[min].s){
        if(l<s->hs&&l>=0&&s->scad[l].q<s->scad[min].q)
            min=l;
    }else if(l<s->hs&&l>=0&&s->scad[l].s<s->scad[min].s)
        min=l;
    if(r<s->hs&&r>=0&&s->scad[r].s==s->scad[min].s){
        if(r<s->hs&&r>=0&&s->scad[r].q<s->scad[min].q)
            min=r;
    }else if(r<s->hs&&r>=0&&s->scad[r].s<s->scad[min].s)
        min=r;
    if(min!=i){
        scadenza temp=s->scad[min];
        s->scad[min]=s->scad[i];
        s->scad[i]=temp;
        minheapify(s,min);
    }
}
void insertmin(s_heap *s,int scad,int q){
    if(s->hs<s->cap){
        s->scad[s->hs].q=q;
        s->scad[s->hs].s=scad;
        inserthmin(s,s->hs);
        s->hs++;
    }else{
        printf("errore S in scadenze,spazio insufficente\n");
        exit(1);
    }
}
void inserthmin(s_heap *s,int i){
    int parent=(i-1)/2;
    if(s->scad[parent].s==s->scad[i].s){
        if(s->scad[parent].q>s->scad[i].q){
            scadenza temp=s->scad[parent];
            s->scad[parent]=s->scad[i];
            s->scad[i]=temp;
            inserthmin(s,parent);
        }
    }else if(s->scad[parent].s>s->scad[i].s){
        scadenza temp=s->scad[parent];
        s->scad[parent]=s->scad[i];
        s->scad[i]=temp;
        inserthmin(s,parent);
    }
}
scadenza estraimin(s_heap *s){
    scadenza elemcn={0,0};
    scadenza scad={0,0};
    if(s->hs==0)
        return scad;
    elemcn=s->scad[0];
    s->scad[0]=scad;
    s->scad[0]=s->scad[s->hs-1];
    s->hs--;
    minheapify(s,0);
    return elemcn;
}
heap *createheap(int cap){
    heap *o=(heap*)malloc(sizeof(heap));
    if(o==NULL)
        exit(1);
    o->hs=0;
    o->cap=cap;
    o->dolce=(dolci*)malloc(cap*sizeof(dolci));
    if(o->dolce==NULL)
        return NULL;
    return o;
}
void maxheapify(heap *o,int j){
    int l=j*2+1,r=j*2+2,max=j;
    if(l<o->hs&&l>=0&&o->dolce[l].peso==o->dolce[max].peso){
        if(l<o->hs&&l>=0&&o->dolce[l].t<o->dolce[max].t)
            max=l;
    }else if(l<o->hs&&l>=0&&o->dolce[l].peso>o->dolce[max].peso)
        max=l;
    if(r<o->hs&&r>=0&&o->dolce[r].peso==o->dolce[max].peso){
        if(r<o->hs&&r>=0&&o->dolce[r].t<o->dolce[max].t)
            max=r;
    }else if(r<o->hs&&r>=0&&o->dolce[r].peso>o->dolce[max].peso)
        max=r;
    if(max!=j){
        dolci temp=o->dolce[max];
        o->dolce[max]=o->dolce[j];
        o->dolce[j]=temp;
        maxheapify(o,max);
    }
}
void insert(heap *o,int peso,int h,int i,int q,int t){
    if(o->hs<o->cap){
        o->dolce[o->hs].h=h;
        o->dolce[o->hs].i=i;
        o->dolce[o->hs].peso=peso;
        o->dolce[o->hs].q=q;
        o->dolce[o->hs].t=t;
        inserth(o,o->hs);
        o->hs++;
    }else{
        printf("errore O in ordinazione,spazio insufficente\n");
        exit(1);
    }
}
void inserth(heap *o,int j){
    int parent=(j-1)/2;
    if(o->dolce[parent].peso==o->dolce[j].peso){
        if(o->dolce[parent].t>o->dolce[j].t){
            dolci temp=o->dolce[parent];
            o->dolce[parent]=o->dolce[j];
            o->dolce[j]=temp;
            inserth(o,parent);
        }
    }else if(o->dolce[parent].peso<o->dolce[j].peso){
        dolci temp=o->dolce[parent];
        o->dolce[parent]=o->dolce[j];
        o->dolce[j]=temp;
        inserth(o,parent);
    }
}
dolci estraimax(heap *o){
    dolci elemcn={0,0,0,0,0};
    dolci dol={0,0,0,0,0};
    if(o->hs==0)
        return elemcn;
    elemcn=o->dolce[0];
    o->dolce[0]=dol;
    o->dolce[0]=o->dolce[o->hs-1];
    o->hs--;
    maxheapify(o,0);
    return elemcn;
}
int hash(int k){
    int h=0,m=H,b=0;
    float a=(sqrt(5)-1)/2,x=0;
    x=a*k;
    b=(int)(a*k);
    h=(int)m*(x-b);
    return h;
}
link linker(char *n_i){
    int k=0,x=0;
    short f=0;
    link cord={0,0};
    k=(int)(n_i[0]+n_i[K]);
    k=hash(k);
    if(magazino[k].m==NULL){
        magazino[k].m=malloc(sizeof(magaz*));
        if(magazino[k].m==NULL){
            printf("ehi,link");
            exit(1);
        }
        magazino[k].m[0]=NULL;
    }
    for(x=0;magazino[k].m[x]!=NULL;x++){
        if(magazino[k].m[x]->i.n_ing!=NULL&&strcmp(magazino[k].m[x]->i.n_ing,n_i)==0){
            f=1;
            break;
        }
    }
    if(f==0){
        magazino[k].m=realloc(magazino[k].m,sizeof(magaz*)*(x+2));
        magazino[k].m[x]=malloc(sizeof(magaz));
        magazino[k].m[x]->s=NULL;
        magazino[k].m[x+1]=NULL;
        magazino[k].m[x]->i.q=0;
        magazino[k].m[x]->i.n_ing=malloc((strlen(n_i)+1)*sizeof(char));
        strcpy(magazino[k].m[x]->i.n_ing,n_i);
        magazino[k].m[x]->s=createheap_s(S);
    }
    cord.k=k;
    cord.x=x;
    return cord;
}
int aggiungi_ricetta(){
    int q=0,h=0,i=0,v=0,m;
    short f=0;
    char n_r[N]={'\0'},n_i[N]={'\0'},e='0';
    link cord={0,0};
    m=scanf("%s",n_r);
    h=(int)(n_r[0]+n_r[K]);
    h=hash(h);
    if(ricettario[h].r==NULL){
        ricettario[h].r=malloc(sizeof(ricetta*));
        ricettario[h].r[0]=NULL;
    }
    for(i=0;ricettario[h].r[i]!=NULL;i++){
        if(f==0&&ricettario[h].r[i]->ts==1){
            v=i;
            f=1;
        }
        if(ricettario[h].r[i]->n_ric!=NULL&&strcmp(ricettario[h].r[i]->n_ric,n_r)==0){
            do{
                m=scanf("%s",n_i);
                m=scanf("%d",&q);
                m=scanf("%c",&e);
            }while(e!='\n');
            return 0;
        }
    }
    if(f==1)
        i=v;
    if(f==0){
        ricettario[h].r=realloc(ricettario[h].r,sizeof(ricetta)*(i+2));
        ricettario[h].r[i]=malloc(sizeof(ricetta));
        ricettario[h].r[i+1]=NULL;
    }
    ricettario[h].r[i]->ts=0;
    ricettario[h].r[i]->peso=0;
    ricettario[h].r[i]->ing=NULL;
    ricettario[h].r[i]->n_ric=malloc((strlen(n_r)+1)*sizeof(char));
    strcpy(ricettario[h].r[i]->n_ric,n_r);
    do{
        m=scanf("%s",n_i);
        m=scanf("%d",&q);
        m=scanf("%c",&e);
        cord=linker(n_i);
        ricettario[h].r[i]->ing=aggiungi_in_testa_i(ricettario[h].r[i]->ing,cord,q);
        ricettario[h].r[i]->peso=ricettario[h].r[i]->peso+q;
    }while(e!='\n');
    i=m;
    return 1;
}
int rimuovi_ricetta(){
    int h=0,i=0,m;
    char n_r[N]={'\0'};
    m=scanf("%s",n_r);
    h=(int)(n_r[0]+n_r[K]);
    h=hash(h);
    if(ricettario[h].r==NULL)
        return 0;
    for(i=0;ricettario[h].r[i]!=NULL;i++){
        if(ricettario[h].r[i]->n_ric!=NULL&&strcmp(ricettario[h].r[i]->n_ric,n_r)==0)
            break;
    }
    if(cerca_o(ordini->inizio,h,i)!=-1)
        return 2;
    if(cerca(attesa->inizio,h,i)!=-1)
        return 2;
    if(ricettario[h].r[i]==NULL)
        return 0;
    if(ricettario[h].r[i]->n_ric[0]=='\0')
        return 0;
    free(ricettario[h].r[i]->n_ric);
    ricettario[h].r[i]->n_ric=NULL;
    ricettario[h].r[i]->ts=1;
    while(ricettario[h].r[i]->ing!=NULL){
        ingre *tmp=ricettario[h].r[i]->ing;
        ricettario[h].r[i]->ing->k=0;
        ricettario[h].r[i]->ing->x=0;
        ricettario[h].r[i]->ing=ricettario[h].r[i]->ing->next;
        free(tmp);
        tmp=NULL;
    }
    ricettario[h].r[i]->ing=NULL;
    ricettario[h].r[i]->peso=0;
    i=m;
    return 1;
}
void rifornimento(){
    int s=0,q=0,k=0,x=0,m;
    short f=0;
    char n_i[N]={'\0'},e='\0';
    do{
        m=scanf("%s",n_i);
        m=scanf("%d",&q);
        m=scanf("%d",&s);
        m=scanf("%c",&e);
        k=(int)(n_i[0]+n_i[K]);
        k=hash(k);
        if(magazino[k].m==NULL){
            magazino[k].m=malloc(sizeof(magaz*));
            if(magazino[k].m==NULL){
                return;
            }
            magazino[k].m[0]=NULL;
        }
        for(x=0;magazino[k].m[x]!=NULL;x++){
            if(magazino[k].m[x]->i.n_ing!=NULL&&strcmp(magazino[k].m[x]->i.n_ing,n_i)==0){
                f=1;
                break;
            }
        }
        if(f==1){
            magazino[k].m[x]->i.q=magazino[k].m[x]->i.q+q;
        }
        if(f==0){
            magazino[k].m=realloc(magazino[k].m,sizeof(magaz*)*(x+2));
            magazino[k].m[x]=malloc(sizeof(magaz));
            magazino[k].m[x]->s=NULL;
            magazino[k].m[x+1]=NULL;
            magazino[k].m[x]->i.n_ing=malloc((strlen(n_i)+1)*sizeof(char));
            strcpy(magazino[k].m[x]->i.n_ing,n_i);
            magazino[k].m[x]->i.q=q;
            magazino[k].m[x]->s=createheap_s(S);
        }
        insertmin(magazino[k].m[x]->s,s,q);
        f=0;
    }while(e!='\n');
    printf("rifornito\n");
    x=m;
}
int ordine(int t){
    int q=0,h=0,i=0,v=0,m;
    char n_r[N]={'\0'};
    m=scanf("%s",n_r);
    m=scanf("%d",&q);
    h=(int)(n_r[0]+n_r[K]);
    h=hash(h);
    if(ricettario[h].r==NULL)
        return 0;
    for(i=0;ricettario[h].r[i]!=NULL;i++){
        if(ricettario[h].r[i]->n_ric!=NULL&&strcmp(ricettario[h].r[i]->n_ric,n_r)==0)
            break;
    }                                                                                                       //cerca ricetta in ricettario
    if(ricettario[h].r[i]==NULL)                                                                    //no ricetta?
        return 0;
    ingre *cur_ing=ricettario[h].r[i]->ing;
    while(cur_ing!=NULL){                                                                        //ingredienti
        while(magazino[cur_ing->k].m[cur_ing->x]->i.q>0&&magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].s<=t){                                    //scadenze
            magazino[cur_ing->k].m[cur_ing->x]->i.q=magazino[cur_ing->k].m[cur_ing->x]->i.q-magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
            estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
        }
        v=0;
        if(magazino[cur_ing->k].m[cur_ing->x]->i.q<cur_ing->q*q){                             //no ingredienti in magazino?
            attesa=aggiungi_in_coda(attesa,h,i,q,t);
            return 1;
        }
        cur_ing=cur_ing->next;
    }
    cur_ing=ricettario[h].r[i]->ing;
    while(cur_ing!=NULL){
        magazino[cur_ing->k].m[cur_ing->x]->i.q=magazino[cur_ing->k].m[cur_ing->x]->i.q-(cur_ing->q*q);                                         //togli gli ingredienti necessari
        magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q-(cur_ing->q*q);
        v=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
        if(magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q<=0)
            estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
        while(v<0){
            magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q+v;
            v=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
            if(magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q<=0)
                estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
        }
        v=0;
        cur_ing=cur_ing->next;
    }
    ordini=aggiungi_in_coda_o(ordini,h,i,q,t,ricettario[h].r[i]->peso*q);
    i=m;
    return 1;
}
void coda(int t){
    int v=0;
    short f=0;
    att *prec=NULL;
    att *testa=attesa->inizio;
    while(testa!=NULL){
        ingre *cur_ing=ricettario[testa->h].r[testa->i]->ing;
        while(cur_ing!=NULL){
            while(magazino[cur_ing->k].m[cur_ing->x]->i.q>0&&magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].s<=t){                                //scadenze
                magazino[cur_ing->k].m[cur_ing->x]->i.q=magazino[cur_ing->k].m[cur_ing->x]->i.q-magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
                estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
            }
            if(magazino[cur_ing->k].m[cur_ing->x]->i.q<cur_ing->q*testa->q){                  //no ingredienti in magazino?
                f=1;
                prec=testa;
                testa=testa->next;
                break;
            }else{
                cur_ing=cur_ing->next;
            }
        }
        if(f==0){
            cur_ing=ricettario[testa->h].r[testa->i]->ing;
            while(cur_ing!=NULL){
                magazino[cur_ing->k].m[cur_ing->x]->i.q=magazino[cur_ing->k].m[cur_ing->x]->i.q-(cur_ing->q*testa->q);                              //togli gli ingredienti necessari
                magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q-(cur_ing->q*testa->q);
                v=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
                if(magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q<=0){
                    estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
                }
                while(v<0){
                    magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q+v;
                    v=magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q;
                    if(magazino[cur_ing->k].m[cur_ing->x]->s->scad[0].q<=0){
                        estraimin(magazino[cur_ing->k].m[cur_ing->x]->s);
                    }
                }
                v=0;
                cur_ing=cur_ing->next;
            }
            ordini=aggiungi_in_ordine_o(ordini,testa->h,testa->i,testa->q,testa->t,ricettario[testa->h].r[testa->i]->peso*testa->q);
            if(prec==NULL){
                attesa->inizio=testa->next;
            }else{
                prec->next=testa->next;
            }
            att *tmp=testa;
            if(testa==attesa->coda)
                attesa->coda=attesa->coda->prec;
            if (tmp->prec)
                tmp->prec->next=tmp->next;
            if (tmp->next)
                tmp->next->prec=tmp->prec;
            testa=testa->next;
            if(attesa->inizio==NULL&&attesa->coda!=NULL){
                attesa->coda->prec=NULL;
                attesa->coda=NULL;
            }
            free(tmp);
            tmp=NULL;
        }
        f=0;
    }
}
// -Wall -Werror -std=gnu11 -O2 -lm -g3 -fsanitize=address
/* valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
 * valgrind --tool=callgrind    kcachegrind callgrind.out.PID
 * valgrind --tool=massif    massif-visualizer massif.out.PID
 * --dump-instr=yes
 */
