#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <math.h>  
#include <vector>
#include <list>
#include <iterator>   
#include <stdlib.h>     
#include <time.h>       
#include <chrono>   
#include <ilcplex/ilocplex.h>

using namespace std;


double fo_melhor;
bool pare;
bool melhorou;
double dif_transporte;
double fo_nova;
int temp;
int mais_vazia;

double penalidade_recebe;
double penalidade_retira;
int cliente_realoca;
int facilidade_recebe;

int melhor_i1;
int melhor_i2;
double pen_nova_atendei1;
double pen_nova_atendei2;

double chega_a_mais;
int desaloca;
int aloca;
double penalidade_nova;

int melhor_j1;
int melhor_j2;

int nivel;

  
void Movement1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas)
{

bool pare = false;



while (pare == false)
{

 
vector<int> clientes_de_melhora;
vector<int> destinos_de_melhora;
vector<double> melhoras;


for (int i = 0; i < n; i++){  
for (std::list<int>::iterator k3 = abertas.begin(); k3 != abertas.end(); k3++ ){

if ( (c[i][*k3]*d[i] < c[i][atende[i]]*d[i]) && (pen[*k3] + d[i] <= 0) && (s[atende[i]].size() > 1.1) && (*k3 != atende[i])  ){   

clientes_de_melhora.push_back(i);
destinos_de_melhora.push_back(*k3);
double ganho = -c[i][atende[i]]*d[i] + c[i][*k3]*d[i];
melhoras.push_back(ganho);  //valor negativo

}
}
}
//cout << "Melhoras:" << endl;



if (clientes_de_melhora.size() == 0)   //se não houve nenhum movimento deve fornecer a solução corrente
{
 pare = true;
}
else   // se houve pelo menos uma melhora 
{ 


     for (int i = 0; i < melhoras.size(); i++)
     {
     int melhor_melhora = i;
     for (int j = i; j < melhoras.size(); j++)
     {
     if (  melhoras[j]  <  melhoras[melhor_melhora] )
        {
        melhor_melhora = j;  //indice do mais proximo ocupa a posicao j   
        }    
    }
   
    if (melhor_melhora != i)
    {

    int temp = clientes_de_melhora[i];     
    clientes_de_melhora[i] = clientes_de_melhora[melhor_melhora];
    clientes_de_melhora[melhor_melhora] = temp;

    int temp2 = destinos_de_melhora[i];     
    destinos_de_melhora[i] = destinos_de_melhora[melhor_melhora];
    destinos_de_melhora[melhor_melhora] = temp2;
    
    double temp3 = melhoras[i];     
    melhoras[i] = melhoras[melhor_melhora];
    melhoras[melhor_melhora] = temp3;

    }




    }





    for (int i = 0; i < clientes_de_melhora.size(); i++)
    {
    
    if ( (c[clientes_de_melhora[i]][destinos_de_melhora[i]]*d[clientes_de_melhora[i]] < c[clientes_de_melhora[i]][atende[clientes_de_melhora[i]]]*d[clientes_de_melhora[i]]) &&  (pen[destinos_de_melhora[i]] + d[clientes_de_melhora[i]] <= 0) && (s[atende[clientes_de_melhora[i]]].size() > 1.1) && (destinos_de_melhora[i] != atende[clientes_de_melhora[i]])  )
    { 
    
    fo = fo + c[clientes_de_melhora[i]][destinos_de_melhora[i]]*d[clientes_de_melhora[i]] - c[clientes_de_melhora[i]][atende[clientes_de_melhora[i]]]*d[clientes_de_melhora[i]] ;    
    s[atende[clientes_de_melhora[i]]].remove(clientes_de_melhora[i]);
    s[destinos_de_melhora[i]].push_back(clientes_de_melhora[i]);
    pen[atende[clientes_de_melhora[i]]] = pen[atende[clientes_de_melhora[i]]] - d[clientes_de_melhora[i]];
    pen[destinos_de_melhora[i]] = pen[destinos_de_melhora[i]] + d[clientes_de_melhora[i]];
    atende[clientes_de_melhora[i]] = destinos_de_melhora[i];

    

    }



        
    }

} //fim else;

} //fim while
    
} // fim void

void Movement2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f )
{


bool pare = false;

while (pare == false)
{

melhorou = false;
vector<int> clientes_de_melhora1;
vector<int> clientes_de_melhora2;
vector<double> melhoras;

for(int i1=0 ; i1 < n-1 ; i1++){
for(int i2= i1+1 ; i2 < n ; i2++) {
 
if (   (atende[i1] != atende[i2]) && ( (c[i1][atende[i2]]*d[i1] + c[i2][atende[i1]]*d[i2]) < (c[i1][atende[i1]]*d[i1] + d[i2]*c[i2][atende[i2]])) && (pen[atende[i1]] - d[i1] + d[i2] <= 0) && (pen[atende[i2]] -d[i2] + d[i1] <= 0) )  
{

clientes_de_melhora1.push_back(i1);
clientes_de_melhora2.push_back(i2);
double ganho = -d[i1]*c[i1][atende[i1]] + d[i1]*c[i1][atende[i2]] -d[i2]*c[i2][atende[i2]] + d[i2]*c[i2][atende[i1]] ;
melhoras.push_back(ganho);  //valor negativo
}

}
}


if (clientes_de_melhora1.size() == 0)   //se não houve nenhum movimento deve fornecer a solução corrente
{
 pare = true;
}
else   // se houve pelo menos uma melhora 
{ 


     for (int i = 0; i < melhoras.size(); i++)
     {
     int melhor_melhora = i;
     for (int j = i; j < melhoras.size(); j++)
     {
     if (  melhoras[j]  <  melhoras[melhor_melhora] )
        {
        melhor_melhora = j;  //indice do mais proximo ocupa a posicao j   
        }    
    }
      if (melhor_melhora != i)
    {

    int temp = clientes_de_melhora1[i];     
    clientes_de_melhora1[i] = clientes_de_melhora1[melhor_melhora];
    clientes_de_melhora1[melhor_melhora] = temp;

    int temp2 = clientes_de_melhora2[i];     
    clientes_de_melhora2[i] = clientes_de_melhora2[melhor_melhora];
    clientes_de_melhora2[melhor_melhora] = temp2;
    
    double temp3 = melhoras[i];     
    melhoras[i] = melhoras[melhor_melhora];
    melhoras[melhor_melhora] = temp3;

    }




    }

    int movimentos_feitos = 0;
    for (int i = 0; i < clientes_de_melhora1.size(); i++)
    {
    
    if (  (atende[clientes_de_melhora1[i]] != atende[clientes_de_melhora2[i]])  && ( (c[clientes_de_melhora1[i]][atende[clientes_de_melhora2[i]]]*d[clientes_de_melhora1[i]] + c[clientes_de_melhora2[i]][atende[clientes_de_melhora1[i]]]*d[clientes_de_melhora2[i]]) < (c[clientes_de_melhora1[i]][atende[clientes_de_melhora1[i]]]*d[clientes_de_melhora1[i]] + d[clientes_de_melhora2[i]]*c[clientes_de_melhora2[i]][atende[clientes_de_melhora2[i]]])) &&  (pen[atende[clientes_de_melhora1[i]]] - d[clientes_de_melhora1[i]] + d[clientes_de_melhora2[i]] <= 0) && (pen[atende[clientes_de_melhora2[i]]] -d[clientes_de_melhora2[i]] + d[clientes_de_melhora1[i]] <= 0) )
    { 
    

    fo = fo + c[clientes_de_melhora1[i]][atende[clientes_de_melhora2[i]]]*d[clientes_de_melhora1[i]] + c[clientes_de_melhora2[i]][atende[clientes_de_melhora1[i]]]*d[clientes_de_melhora2[i]] - c[clientes_de_melhora1[i]][atende[clientes_de_melhora1[i]]]*d[clientes_de_melhora1[i]] - d[clientes_de_melhora2[i]]*c[clientes_de_melhora2[i]][atende[clientes_de_melhora2[i]]];
    s[atende[clientes_de_melhora1[i]]].remove(clientes_de_melhora1[i]);
    s[atende[clientes_de_melhora2[i]]].remove(clientes_de_melhora2[i]);
    s[atende[clientes_de_melhora1[i]]].push_back(clientes_de_melhora2[i]);
    s[atende[clientes_de_melhora2[i]]].push_back(clientes_de_melhora1[i]);
    pen[atende[clientes_de_melhora1[i]]] = pen[atende[clientes_de_melhora1[i]]] - d[clientes_de_melhora1[i]] + d[clientes_de_melhora2[i]];
    pen[atende[clientes_de_melhora2[i]]] = pen[atende[clientes_de_melhora2[i]]] - d[clientes_de_melhora2[i]] + d[clientes_de_melhora1[i]];
    
    int temp4 = atende[clientes_de_melhora2[i]]; 
    atende[clientes_de_melhora2[i]] = atende[clientes_de_melhora1[i]];
    atende[clientes_de_melhora1[i]] = temp4;
    movimentos_feitos = movimentos_feitos + 1;

 
    }


        
    }

} //fim else;

} // fim while;

} //fim void

void embaralhar( vector<int> &ordem, int numero_vizinhancas)
{
	

    for (int i = 0; i < numero_vizinhancas; i++)
{
    ordem[i] = i;
}
    
    for (int i = 0; i < numero_vizinhancas; i++)
	{
		int r = rand() % numero_vizinhancas;

		int temp = ordem[i];
		ordem[i] = ordem[r];
		ordem[r] = temp;
	}
}

void Movement3 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p)
{

fo_melhor = fo;
pare = false;

while (pare == false)
{

melhorou = false;




for (std::list<int>::iterator k1 = abertas.begin(); k1 != abertas.end(); k1++ ){
chega_a_mais = 0;  

for ( std::list<int>::iterator k = s[*k1].begin(); k != s[*k1].end(); k++)
{
      chega_a_mais += d[*k] ;
}

for (std::list<int>::iterator k2 = abertas.begin(); k2 != abertas.end(); k2++ ){




if ( (*k1 != *k2) && pen[*k2] + chega_a_mais <= 0)
{

std::list<int>::iterator k;
dif_transporte = 0;
for ( k = s[*k1].begin(); k != s[*k1].end(); k++)
{
      dif_transporte = dif_transporte - d[*k]*c[*k][*k1] + d[*k]*c[*k][*k2];
}

fo_nova = fo + dif_transporte - f[*k1] ;


if (fo_nova < fo_melhor) {
desaloca = *k1;
aloca = *k2;
fo_melhor = fo_nova;  
melhorou = true;


}
}
}
}

if (melhorou == false)   //se não houve nenhum movimento deve fornecer a solução corrente
{
     pare = true;

}
else   // se houve pelo menos uma melhora deve atualizar a solucao corrente
{ 
chega_a_mais = 0;  
std::list<int>::iterator k;
for ( k = s[desaloca].begin(); k != s[desaloca].end(); k++)
{
      chega_a_mais += d[*k] ;
}

pen[aloca] = pen[aloca] + chega_a_mais;
pen[desaloca] = -p[desaloca];

for (k = s[desaloca].begin(); k != s[desaloca].end(); k++)
{
      s[aloca].push_back(*k);
      atende[*k] = aloca;
}
abertas.remove(desaloca);
fechadas.push_back(desaloca);
s[desaloca].clear();

fo = fo_melhor;

}



} 
}

void VND_ils2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p)
{

double fo_antes = fo;
bool parada = false;
int iter_sem_melhora = 0;


while (parada == false)
{

 if (parada == false) 
 {
 fo_antes = fo;
  Movement1(m, n, s , fo, pen,atende, c,  d,  f, abertas, fechadas);
 if (fo_antes <= fo)
 {
     iter_sem_melhora = iter_sem_melhora + 2;
     if (iter_sem_melhora == 4)
     {
         parada = true;
     }

 }
  else
 {
     iter_sem_melhora = 0;
 }
 }
 if (parada == false) 
 {
 fo_antes = fo;
 Movement2(m, n, s , fo, pen,atende, c,  d,  f);

 if (fo_antes <= fo)
 {
     iter_sem_melhora = iter_sem_melhora + 2;
     if (iter_sem_melhora == 4)
     {
         parada = true;
     }

 }
  else
 {
     iter_sem_melhora = 0;
 }
 }
}

//}


}

void VND_ils1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p)
{

double fo_antes = fo;
bool parada = false;
int iter_sem_melhora = 0;


while (parada == false)
{



 if (parada == false) 
 {
 fo_antes = fo;
   Movement1(m, n, s , fo, pen,atende, c,  d,  f, abertas, fechadas);
  
 if (fo_antes <= fo)
 {
     iter_sem_melhora = iter_sem_melhora + 1;
     if (iter_sem_melhora == 3)
     {
         parada = true;
     }

 }
  else
 {
     iter_sem_melhora = 0;
 }
 }

 
 
 if (parada == false) 
 {
 fo_antes = fo;
 Movement2(m, n, s , fo, pen,atende, c,  d,  f);

 if (fo_antes <= fo)
 {
     iter_sem_melhora = iter_sem_melhora + 1;
     if (iter_sem_melhora == 3)
     {
         parada = true;
     }

 }
  else
 {
     iter_sem_melhora = 0;
 }
 }


  if (parada == false) 
 {
 fo_antes = fo;

 Movement3(m, n, s , fo, pen,atende, c,  d,  f, abertas, fechadas, p);

 if (fo_antes <= fo)
 {
     iter_sem_melhora = iter_sem_melhora + 1;
     if (iter_sem_melhora == 3)
     {
         parada = true;
     }

 }
  else
 {
     iter_sem_melhora = 0;
 }
 }







}

//}


}

void ILS_1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>facilidades_potenciais, vector<double> p, int tamanho_instancia )
{

double fstar = fo;
//int vezesmax = ILSmax; 
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;
// criar s_copia abertas_copia e fechadas_copia
for (int j = 0; j < m; j++){
if (! s[j].empty() )
{
abertas_copia.push_back(j);

for ( std::list<int>::iterator  k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
// criar fo copia e atende copia
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
fo_copia = fo;


bool parada = false; 

while(parada == false)
{

parada = true;    

vector<int> facilidades_potenciais_fechadas;

//embaralha

for (int i = 0; i < facilidades_potenciais.size(); i++)
{
		int r = rand() % facilidades_potenciais.size();

		int temp = facilidades_potenciais[i];
		facilidades_potenciais[i] = facilidades_potenciais[r];
		facilidades_potenciais[r] = temp;
}



for (int i = 0; i < facilidades_potenciais.size(); i++)
{

 std::list<int>::iterator k = find(abertas.begin(), abertas.end(), facilidades_potenciais[i]);
 if (*k != facilidades_potenciais[i])
 {
     
     facilidades_potenciais_fechadas.push_back(facilidades_potenciais[i]);
 }
}
int j = 0;
while (j < facilidades_potenciais_fechadas.size() )  //explora toda vizinhança uma única vez
{
double limitante = 0;
fo_copia = fo_copia + f[facilidades_potenciais_fechadas[j]];
abertas_copia.push_back(facilidades_potenciais_fechadas[j]);
fechadas_copia.remove(facilidades_potenciais_fechadas[j]);
bool pare = false;
int conta_clientes_realocados = 0;
while (pare == false)
{
   
   
   pare = true;
   int cliente_de_menor_custo; //primeiro cliente que cabe e que ainda não foi
   for (int i = 0; i < n; i++)
   {
      if ( (pen_copia[facilidades_potenciais_fechadas[j]] + d[i] <= 0) && (c[i][facilidades_potenciais_fechadas[j]] > limitante ) )
      {
          cliente_de_menor_custo = i;
          conta_clientes_realocados = conta_clientes_realocados + 1;
          pare = false;
          break; //sai do for com primeiro cliente que cabe a ainda não foi
      }
   }
   
   if (pare == false)
   {
   for (int i = cliente_de_menor_custo; i < n; i++)
   {
       if ((pen_copia[facilidades_potenciais_fechadas[j]] + d[i] <= 0 )  &&  (c[i][facilidades_potenciais_fechadas[j]] < c[cliente_de_menor_custo][facilidades_potenciais_fechadas[j]]) && ( (c[i][facilidades_potenciais_fechadas[j]] > limitante ))   )
       {
           cliente_de_menor_custo = i; 
                 
       }

   }
   }   //selecionou cliente de menor custo que cabe
  
   if (pare == false)  //é a condicao para realocar 
   {
   // faz a realocacao do cliente 
   //cout << cliente_de_menor_custo << " ";
      if (s_copia[atende_copia[cliente_de_menor_custo]].size() < 1.1)
   {
      // cout << "causou fechamento!!!!!!!!!!!!!!!!!!!!!!!!!!!! de: " << atende_copia[cliente_de_menor_custo] << endl;
       fo_copia = fo_copia - f[atende_copia[cliente_de_menor_custo]];
       fechadas_copia.push_back(atende_copia[cliente_de_menor_custo]);
       abertas_copia.remove(atende_copia[cliente_de_menor_custo]);

   }
   s_copia[atende_copia[cliente_de_menor_custo]].remove(cliente_de_menor_custo);
   s_copia[facilidades_potenciais_fechadas[j]].push_back(cliente_de_menor_custo);
   pen_copia[atende_copia[cliente_de_menor_custo]] = pen_copia[atende_copia[cliente_de_menor_custo]] - d[cliente_de_menor_custo];
   pen_copia[facilidades_potenciais_fechadas[j]] = pen_copia[facilidades_potenciais_fechadas[j]] + d[cliente_de_menor_custo];
   fo_copia = fo_copia - c[cliente_de_menor_custo][atende_copia[cliente_de_menor_custo]]*d[cliente_de_menor_custo] + c[cliente_de_menor_custo][facilidades_potenciais_fechadas[j]]*d[cliente_de_menor_custo];

   atende_copia[cliente_de_menor_custo] = facilidades_potenciais_fechadas[j];
   limitante = c[cliente_de_menor_custo][facilidades_potenciais_fechadas[j]];


   }
   else
   {
       if (conta_clientes_realocados == 0)
       {
          //cout << "PASSOU POR AQUI" << endl;
          fo_copia = fo_copia - f[facilidades_potenciais_fechadas[j]];
          abertas_copia.remove(facilidades_potenciais_fechadas[j]);
          fechadas_copia.push_back(facilidades_potenciais_fechadas[j]);  
       }
   }
   
  

} // termino das realocações para a facilidade potencial de posicao j;

VND_ils1 (m, n, s_copia , fo_copia, pen_copia,atende_copia, c,  d,  f, abertas_copia, fechadas_copia, p);


if (fo_copia < 0.9999*fstar) //houve melhora fica com a solucao
{

fstar = fo_copia;

abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++){
s[j].clear();
if (! s_copia[j].empty() )
{
abertas.push_back(j);
for ( std::list<int>::iterator k = s_copia[j].begin(); k != s_copia[j].end(); k++ ){
s[j].push_back(*k); 
}
}
else
{
fechadas.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen[j] = pen_copia[j];
} 
for (int i = 0; i < n; i++)
{
    atende[i] = atende_copia[i];
} 
fo = fstar; 
parada = false;
break; //



}
else 
{   

fechadas_copia.clear();
abertas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();
if (! s[j].empty() )
{
abertas_copia.push_back(j);
for ( std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
// criar fo copia
fo_copia = fstar;
j = j + 1;
   
}


} //fim ils
}

} //fim void ils 2

void ILS_2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>facilidades_potenciais, int tam_facilidades_potenciais, vector<double> p, int tamanho_instancia)
{

double fstar = fo;
int iter = 0;
int melhor_iter = iter;
int nivel = 1;
int nvezes = 1; 
int ILSmax = 120;
int vezesmax = 1; 
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;

for (int j = 0; j < tam_facilidades_potenciais; j++){
if (! s[facilidades_potenciais[j]].empty() )
{
abertas_copia.push_back(facilidades_potenciais[j]);
  std::list<int>::iterator k;
for (k = s[facilidades_potenciais[j]].begin(); k != s[facilidades_potenciais[j]].end(); k++ ){
s_copia[facilidades_potenciais[j]].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(facilidades_potenciais[j]);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
fo_copia = fo;


while (iter - melhor_iter < ILSmax )
{

iter = iter + 1;

int numero_abertas = 0;
for (int j = 0; j < m; j++)
{
    if (!s[j].empty())
    {
        numero_abertas = numero_abertas +1;
    }
}


vector<int> facilidades(abertas.size());
int i = 0; 
for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ )
  {
      facilidades[i] = *k;
      i = i+1;
  }

list<int> lista_clientes_perturbados;
int trocas = 0;
int cliente_sorteado1;
int cliente_sorteado2;
while (trocas < (tamanho_instancia/250)*nivel)   
{
    cliente_sorteado1 = rand() % n;  //sorteia um clientes
    cliente_sorteado2 = (rand()+1) % n; // sorteia um outro cliente
 
    if ((cliente_sorteado1 != cliente_sorteado2) &&  (atende_copia[cliente_sorteado1] != atende_copia[cliente_sorteado2]) && (pen_copia[atende_copia[cliente_sorteado1]] + d[cliente_sorteado2] - d[cliente_sorteado1] <= 0) && (pen_copia[atende_copia[cliente_sorteado2]] + d[cliente_sorteado1] - d[cliente_sorteado2] <= 0)  )
    {
    fo_copia = fo_copia - d[cliente_sorteado1]*c[cliente_sorteado1][atende_copia[cliente_sorteado1]] + d[cliente_sorteado1]*c[cliente_sorteado1][atende_copia[cliente_sorteado2]]; 
    fo_copia = fo_copia - d[cliente_sorteado2]*c[cliente_sorteado2][atende_copia[cliente_sorteado2]] + d[cliente_sorteado2]*c[cliente_sorteado2][atende_copia[cliente_sorteado1]]; 

    //cout << "fo apos o movmento" << fo_copia << endl ;
    pen_copia[atende_copia[cliente_sorteado1]] = pen_copia[atende_copia[cliente_sorteado1]] - d[cliente_sorteado1] + d[cliente_sorteado2];
    pen_copia[atende_copia[cliente_sorteado2]] = pen_copia[atende_copia[cliente_sorteado2]] - d[cliente_sorteado2] + d[cliente_sorteado1] ;
   
    s_copia[atende_copia[cliente_sorteado1]].remove(cliente_sorteado1);
    s_copia[atende_copia[cliente_sorteado2]].remove(cliente_sorteado2);
    s_copia[atende_copia[cliente_sorteado1]].push_back(cliente_sorteado2);
    s_copia[atende_copia[cliente_sorteado2]].push_back(cliente_sorteado1);

  
    int temp = atende_copia[cliente_sorteado1]; 
    atende_copia[cliente_sorteado1] = atende_copia[cliente_sorteado2];
    atende_copia[cliente_sorteado2] = temp;
    trocas = trocas + 1;
    }
    
    else
    {
        trocas = trocas + 1;
    }


}

lista_clientes_perturbados.sort();
lista_clientes_perturbados.unique();

VND_ils2(m, n, s_copia , fo_copia, pen_copia,atende_copia, c,  d,  f, abertas_copia, fechadas_copia, p);

if (fo_copia < 0.9999999*fstar) //houve melhora fica com a solucao
{
    
melhor_iter = iter;
nivel = 1;
nvezes = 1; 
fstar = fo_copia;


abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++){
s[j].clear();
if (! s_copia[j].empty() )
{
abertas.push_back(j);
for ( std::list<int>::iterator k = s_copia[j].begin(); k != s_copia[j].end(); k++ ){
s[j].push_back(*k); 
}
}
else
{
fechadas.push_back(j);
}
}
// criar pen copia
for (int j = 0; j < m; j++)
{
    pen[j] = pen_copia[j];
} 
for (int i = 0; i < n; i++)
{
    atende[i] = atende_copia[i];
} 
fo = fstar; 
}
else {   //se nao houve melhora

fechadas_copia.clear();
abertas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();
if (! s[j].empty() )
{
abertas_copia.push_back(j);
for ( std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
// criar fo copia
fo_copia = fstar;


if (nvezes >= vezesmax){ 
        nivel = nivel + 1;  
        nvezes = 1;        
    }
else
    {
    nvezes = nvezes + 1;
    }
    
}
} 


}

int main (int argc, char *argv[]) {
ifstream arq(argv[1]);
		if (!arq) {cerr << "Error  \n"; exit(0);}
 
srand (time(NULL));
        clock_t inicio_CPU, fim_CPU;   
        inicio_CPU = clock();

        int m;
int n;
        int i;
        int AUX;
        int id;  
       
        arq >> AUX;
        if (AUX == 100) //beasley instances
        {
            cout << "OR-4 instance have been identified" << endl;
            id = 1;
            m = AUX;
            arq >> n;
        }
              if (AUX > 100) //TBED 1 instances
        {
            cout << "TBED1 instance have been identified" << endl;
            id = 2;
            n = AUX;
            arq >> m;
        }
   
       
        vector<double> p(m); // capacity
        vector<double> f(m); // fix cost
        vector<double> d(n); // demand
        vector<vector<double>> c(n, vector<double>(m)); // unit costs
        vector<double> pen(m); // penality (less and equal 0 in all algorithm)
      
//Leitura de dados

       if (id == 1)  //or 4
       {
        vector<vector<double>> caux(n, vector<double>(m)); 
for (int j = 0; j <  m; j++){
arq >> p[j]; 
            arq >> f[j]; 
}
       
for (int i = 0; i <  n; i++){
arq >> d[i];     
        for (int j = 0; j < m; j ++)
        {
        arq >> caux[i][j] ;
        }    
        }

        for (int i = 0; i <  n; i++){
                for (int j = 0; j <  m; j++){
               
                c[i][j] = (double) caux[i][j]/d[i];
                }

             }
       }

    if (id == 2) //tbed1 
    {
      vector<vector<double>> caux(m, vector<double>(n)); // 
    for (int i = 0; i <  n; i++){
arq >> d[i];
        }
    for (int j = 0; j <  m; j++){
arq >> p[j];
        }
    for (int j = 0; j <  m; j++){
arq >> f[j];
        }
    for (int j = 0; j < m; j ++){
   for (int i = 0; i <  n; i++)
        {
arq >> caux[j][i];
        }    
    }

    for (int j = 0; j < m; j ++){
   for (int i = 0; i <  n; i++)
        {

             c[i][j] = caux[j][i];
        }    
    }
    }


cout << "End of data reading" << endl;
double soma_demandas = 0;
for (int i = 0; i < n; i++)
{
    soma_demandas += d[i]; 
}

int tamanho_instancia = n;


vector<list<int>> s(m);
double fo;


for (int i = 0; i < m; i++)
{
pen[i] = -p[i]; 

}
list<int> abertas;
list<int> fechadas;
vector<int> atende(n);

int tilim1 = 1000; // time for Reduced SSCFLP
int tilim2 = 200; // time for GAP

IloEnv env2;
IloModel mod2(env2);
IloCplex cplex2(mod2);
IloNumVarArray y2(env2, m, 0, 1, ILOFLOAT); 
IloArray<IloNumVarArray> x2(env2, n);
for(int i = 0; i < n; i++) {  
			 x2[i] = IloNumVarArray(env2, m, 0, 1, ILOFLOAT);
}
// objective *****************************************************
IloExpr expfo2(env2);
for (int j = 0; j <  m; j++){
	expfo2 += f[j] * y2[j];
	for(int i = 0; i < n; i++){
		expfo2 += c[i][j] * x2[i][j] * d[i];
	}
}  
IloAdd(mod2, IloMinimize(env2, expfo2));
expfo2.end();
// assigment constraints  *****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r22(env2);
			for (int j = 0; j < m; j++){
				r22 += x2[i][j];
			}
			mod2.add(r22 == 1);
			r22.end();
		} 
// capacity constraints *****************************************************************
			   for (int j = 0; j < m; j++){
			IloExpr r32(env2);
			for (int i = 0; i < n; i++){
				r32 += d[i]*x2[i][j];
			}
			mod2.add(r32 <= p[j]*y2[j]);
			r32.end();
		} 
// redundant constraints *****************************************************************
			   for (int j = 0; j < m; j++){
			
			for (int i = 0; i < n; i++){
               mod2.add(y2[j] >= x2[i][j]);
			}
			
			
		}

cplex2.setWarning(env2.getNullStream()); // Eliminar warnings
cplex2.setOut(env2.getNullStream()); /// Eliminar os logs do solver
cout << "Running LP(SSCFLP)..." << endl;
cplex2.solve();

double otimo_problema_relaxado = cplex2.getObjValue();

float media_y = 0;
float soma_y = 0;
for (int j = 0; j < m; j++)
{
    soma_y += cplex2.getReducedCost(y2[j]);
}
media_y = (float) soma_y/m;

float media_y2 = 0;
if (media_y > 0)
{
media_y2 = media_y;
}
vector<int> vgrupo1;
list<int> grupo1;  //y[j] > 0.99   
list<int> grupo2;  
vector<int> vgrupo2;    
list<int> grupo2b;  
vector<int> vgrupo2b;   
list<int> grupo3;  //  y[j] == 0 
vector<int> vgrupo3;   


for (int j = 0; j < m; j++)
{
if (cplex2.getValue(y2[j]) > 0.9999)
{
    grupo1.push_back(j);
    vgrupo1.push_back(j);
    
}
else
{
    if (  ( cplex2.getValue(y2[j]) > 0) && (cplex2.getReducedCost(y2[j]) == 0 ) )
    {
        grupo2.push_back(j);
        vgrupo2.push_back(j); 
         
    }
    if (  ( cplex2.getValue(y2[j]) == 0) && (cplex2.getReducedCost(y2[j]) == 0 ) )
    {
        grupo2b.push_back(j);
        vgrupo2b.push_back(j); 
         
    }
       if (  ( cplex2.getValue(y2[j]) == 0)  && (cplex2.getReducedCost(y2[j]) > 0 )   )
    {
        grupo3.push_back(j);
        vgrupo3.push_back(j); 
         
    }

}
 
}

float mediah = 0;
float somah = 0;
for (int j = 0; j < vgrupo3.size(); j++)
{
     somah += cplex2.getReducedCost(y2[vgrupo3[j]]);
}
mediah = (float) somah/vgrupo3.size();

     for (int i = 0; i < vgrupo2.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo2.size(); j++)
     {
     if ( cplex2.getValue(y2[vgrupo2[j]])  >  cplex2.getValue(y2[vgrupo2[menor_custo]]) )
        {
        menor_custo = j;    
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo2[i];     
    vgrupo2[i] = vgrupo2[menor_custo];
    vgrupo2[menor_custo] = temp;
    }
     }

     for (int i = 0; i < vgrupo2b.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo2b.size(); j++)
     {
     if ( (double)f[vgrupo2b[j]]/p[vgrupo2b[j]]   <  (double)f[vgrupo2b[menor_custo]]/p[vgrupo2b[menor_custo]] )
        {
        menor_custo = j;  
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo2b[i];     
    vgrupo2b[i] = vgrupo2b[menor_custo];
    vgrupo2b[menor_custo] = temp;
    }
     }


     for (int i = 0; i < vgrupo3.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo3.size(); j++)
     {
     if ( cplex2.getReducedCost(y2[vgrupo3[j]]) <  cplex2.getReducedCost(y2[vgrupo3[menor_custo]]) )
        {
        menor_custo = j;  
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo3[i];     
    vgrupo3[i] = vgrupo3[menor_custo];
    vgrupo3[menor_custo] = temp;
    }
     }

vector<int> facilidades_potenciais;
for (int i = 0; i < vgrupo1.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo1[i]);
}
for (int i = 0; i < vgrupo2.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo2[i]);
}

for (int i = 0; i < vgrupo2b.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo2b[i]);
}


for (int i = 0; i < vgrupo3.size(); i++)
{
    if (cplex2.getReducedCost(y2[vgrupo3[i]]) < 0.25*mediah)
    {
        facilidades_potenciais.push_back(vgrupo3[i]);
    }
}

vector<list<int>> clientes_potenciais(m);
vector<list<int>> clientes_nao_potenciais(m);


double soma_capacidades = 0;

//******************************************************************************************************************************
cout << "Generating Initial Solution..." << endl;
list<int> clientes_ja_alocados;
int contador = 0;
while (soma_capacidades < soma_demandas)
{
   
    abertas.push_back(facilidades_potenciais[contador]);
    fechadas.remove(facilidades_potenciais[contador]);
    soma_capacidades += p[facilidades_potenciais[contador]];
    if (soma_capacidades < soma_demandas)
    {
     contador = contador + 1;
    }
    
}

vector<vector<int>> start(n, vector<int>(m));
bool factivel = false;

vector<int> clientes_restantes(n);
for (int i = 0; i < n; i++)
{
   clientes_restantes[i] = i;
}

for (int i = 0; i < clientes_restantes.size()- 1; i++)
{
    int maior_demanda = i;
    for (int j = i; j < clientes_restantes.size(); j++)
    {
       if (d[clientes_restantes[j]] > d[clientes_restantes[maior_demanda]])
       {
           maior_demanda = j;
       }
    }
    if (maior_demanda != i)
    {
        int temp = clientes_restantes[i];
        clientes_restantes[i] = clientes_restantes[maior_demanda];
        clientes_restantes[maior_demanda] = temp;
    }
    
}

factivel = false;
while (factivel == false)   
{

factivel = true;

for (int j = 0; j < m; j++){
    pen[j] = -p[j];
    s[j].clear();
}

vector<int> facilidades(abertas.size());  
int i = 0;
for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++)
{
     facilidades[i] = *k;
     i = i + 1;        
}



for (int i = 0; i < clientes_restantes.size();i++)   
{

if (factivel == false) 
{

break; 
}

    
int primeira_que_cabe; 
bool encontrou = false; 
bool realocou_primeira_tentativa = false;




for (int j = 0; j < facilidades.size(); j++)
{
   if (   (cplex2.getValue(x2[clientes_restantes[i]][facilidades[j]]) > 0.999) &&  (pen[facilidades[j]] + d[clientes_restantes[i]] ) <=  0  )
   {


    s[facilidades[j]].push_back(clientes_restantes[i]);
    pen[facilidades[j]] = pen[facilidades[j]] + d[clientes_restantes[i]]; 
    atende[clientes_restantes[i]] = facilidades[j];
    realocou_primeira_tentativa = true;
    break;
   } 

}


if (realocou_primeira_tentativa == false)
{
int j = 0;
while ( (encontrou == false) && (j != abertas.size()) )
    {
        if (pen[facilidades[j]] + d[clientes_restantes[i]] <= 0) 
        {
            primeira_que_cabe = facilidades[j];
            encontrou = true;
        }
        j = j+1;
    }
    if (encontrou == false) 
    {
   
    factivel = false;
    break; 
    }
 
    
int maisbarata = primeira_que_cabe;    

for (int j = 0; j < abertas.size(); j++)
    {
        if (  (c[clientes_restantes[i]][facilidades[j]]  <  c[clientes_restantes[i]][maisbarata]) && (pen[facilidades[j]] + d[clientes_restantes[i]] <= 0) )
        {
            maisbarata = facilidades[j];
        }
    }
    s[maisbarata].push_back(clientes_restantes[i]);
    pen[maisbarata] = pen[maisbarata] + d[clientes_restantes[i]]; 
    atende[clientes_restantes[i]] = maisbarata;

}

}


fo = 0; 

for (int j = 0; j < m; j++){
  if (!s[j].empty())
  {
  fo += f[j];  
  for (std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ )
  {
       fo += c[*k][j]*d[*k];
  
  }

  }
  }


if (factivel == false ) 
{
contador = contador + 1;    
abertas.push_back(facilidades_potenciais[contador]);
fechadas.remove(facilidades_potenciais[contador]);

}


} 

double fstar2 = 0;
int tam_grupo3 = grupo3.size();

int tamanho_facilidades_potenciais = facilidades_potenciais.size();
int tam_facilidades_potenciais = facilidades_potenciais.size();
cout << "Objective value: " << fo;
clock_t stop = clock();
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;
cout << "Refining solution using vnd... " << endl;
VND_ils1 (m, n, s , fo, pen,atende, c,  d,  f, abertas, fechadas, p);
cout << "Objective value: " << fo;
stop = clock();
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;
/****************************************************************************************************/
int inter = 0;    
bool parada2 = false;     
double gap = 100;             
double gap2 = 100;          
double lb = 0;             
double lb2 = 0;           
double fo_antes2 = fo;         
int conta = 0;
bool aux2 = false;
cout << "PHASE I" << endl;
while (parada2 == false) 
{
double fo_antes2 = fo;

clock_t fim_CPU2 = clock();
IloEnv env3;
IloModel mod3(env3);
IloCplex cplex3(mod3);
IloNumVarArray y3(env3, facilidades_potenciais.size(), 0, 1, ILOBOOL); 
IloArray<IloNumVarArray> x3(env3, n);
for(int i = 0; i < n; i++) {  
    x3[i] = IloNumVarArray(env3, facilidades_potenciais.size(), 0, 1, ILOBOOL);
}

// declaração da expressão da funcao objetivo*****************************************************
IloExpr expfo(env3);
for (int j = 0; j <  facilidades_potenciais.size(); j++){
	expfo += f[facilidades_potenciais[j]] * y3[j];
	for(int i = 0; i < n; i++){
		expfo += c[i][facilidades_potenciais[j]] * x3[i][j] *d[i];
	}
}  
IloAdd(mod3, IloMinimize(env3, expfo));
expfo.end();

//restricao de indivisibilidade*****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r3(env3);
			for (int j = 0; j < facilidades_potenciais.size(); j++){
				r3 += x3[i][j];
			}
			mod3.add(r3 == 1);
			r3.end();
		} 
//restricao de de capacidade *****************************************************************
			   for (int j = 0; j < facilidades_potenciais.size(); j++){
			IloExpr r3(env3);
			for (int i = 0; i < n; i++){
				r3 += d[i]*x3[i][j];
			}
			mod3.add(r3 <= p[facilidades_potenciais[j]]*y3[j]);
			r3.end();
		} 


if (  id == 1  )  // redundant sentences for OR-4 data sets.
{
  for (int j = 0; j < facilidades_potenciais.size(); j++){

for (int i = 0; i < n; i++){
               mod3.add(y3[j] >= x3[i][j]);
}


}
}

IloExpr r5(env3);
for (int j = 0; j <  facilidades_potenciais.size(); j++){
	r5 += f[facilidades_potenciais[j]] * y3[j];
	for(int i = 0; i < n; i++){
		r5 += c[i][facilidades_potenciais[j]] * x3[i][j] *d[i];
	}
} 
mod3.add(r5 >= lb2);
r5.end();

vector<vector<int>> start1(n, vector<int>(facilidades_potenciais.size()));

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < facilidades_potenciais.size(); j++)
    {
       start1[i][j] = 0;
    }
}


for (int i = 0; i < n ; i++)
{
    for (int j = 0; j < facilidades_potenciais.size(); j++)
    {
    if (atende[i] == facilidades_potenciais[j])
    { 
      start1[i][j] = 1;

    }
    }
    
  
}


IloNumVarArray startVar(env3);
IloNumArray startVal(env3);
for (int i = 0; i < n; i++)
         for (int j = 0; j < facilidades_potenciais.size(); j++) {
             startVar.add(x3[i][j]);
             startVal.add(start1[i][j]);
         }
     cplex3.addMIPStart(startVar, startVal);
     startVal.end();
     startVar.end();



cplex3.setWarning(env3.getNullStream()); // Eliminar warnings
cplex3.setOut(env3.getNullStream()); /// Eliminar os logs do solver
cplex3.setParam(IloCplex::Threads,1);

cplex3.setParam(IloCplex::EpGap, 0.00001);
cplex3.setParam(IloCplex::TiLim, tilim1);
cout << "Running cplex for Reduced SSCFLP...." << endl;
cplex3.solve();
fstar2 = cplex3.getObjValue();	
lb2 = cplex3.getBestObjValue();	
gap2 = 100*cplex3.getMIPRelativeGap();


fim_CPU2 = clock();
float tempo = (float)(fim_CPU2 - inicio_CPU)/CLOCKS_PER_SEC ;
if ((gap2 <= 0.0099) || (tempo > 21600))   
{
    parada2 = true;
}
fo = 0;
for (int j = 0; j < facilidades_potenciais.size(); j++)
{
   s[facilidades_potenciais[j]].clear();
   pen[facilidades_potenciais[j]] = -p[facilidades_potenciais[j]];
   for (int i = 0; i < n; i++)
   {
   if ( abs(cplex3.getValue(x3[i][j])-1) < 0.0001)
   {
       s[facilidades_potenciais[j]].push_back(i);
       atende[i] = facilidades_potenciais[j];
       pen[facilidades_potenciais[j]] = pen[facilidades_potenciais[j]] + d[i];
       fo = fo + c[i][facilidades_potenciais[j]] *d[i];
   }
   }
  // fo = fstar2;
   
}
abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++)
{
   if (!s[j].empty())
   {
       abertas.push_back(j);
       fo = fo + f[j];
   }
   else
   {
       fechadas.push_back(j);
   }
}
cout << "Objective value: " << fo;
 stop = clock();
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;

if (gap2 > 0.01)
{
cout << "Refining solution using ils1... " << endl;    
ILS_1(m, n, s ,fo,pen, atende, c,d, f,abertas,fechadas, facilidades_potenciais,  p, tamanho_instancia);
stop = clock();
cout << "Objective value: " << fo;
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;
cout << "Refining solution using ils2... " << endl;   
ILS_2 (m, n, s ,fo,pen, atende, c,d, f,abertas,fechadas, facilidades_potenciais, tam_facilidades_potenciais, p, tamanho_instancia);
stop = clock();
cout << "Objective value: " << fo;
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;

}
aux2 = false;

fim_CPU2 = clock();
if( (fo >= 0.9999*fo_antes2) || ((float)(fim_CPU2 - inicio_CPU)/CLOCKS_PER_SEC > 21600))  
{

    parada2 = true;
}
}

cout << "Phase I ended" << endl;
if ( (parada2 == true) && (gap2 > 0.01)  )
{
cout << "Starts phase II" << endl;;
parada2 = false;
while (parada2 == false) 
{

double fo_antes2 = fo;

vector<int> vetor_abertas;
double fo1 = 0;
for (std::list<int>::iterator k = abertas.begin() ; k !=  abertas.end() ; k++)
{
vetor_abertas.push_back(*k);
fo1 = fo1 + f[*k]; 
}

IloEnv env;
IloModel mod(env);
IloCplex cplex(mod);
IloNumVarArray y(env, vetor_abertas.size(), 0, 1, ILOBOOL); 
IloArray<IloNumVarArray> x(env, n);
for(int i = 0; i < n; i++) {  
    x[i] = IloNumVarArray(env, vetor_abertas.size(), 0, 1, ILOBOOL);
}

// declaração da expressão da funcao objetivo*****************************************************
IloExpr expfo(env);
for (int j = 0; j <  vetor_abertas.size(); j++){
	//expfo += f[j] * y[j];
	for(int i = 0; i < n; i++){
		expfo += c[i][vetor_abertas[j]] * x[i][j] * d[i];
	}
}  
expfo += fo1;
IloAdd(mod, IloMinimize(env, expfo));
expfo.end();

//restricao de indivisibilidade*****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r2(env);
			for (int j = 0; j < vetor_abertas.size(); j++){
				r2 += x[i][j];
			}
			mod.add(r2 == 1);
			r2.end();
		} 
//restricao de de capacidade *****************************************************************
			   for (int j = 0; j < vetor_abertas.size(); j++){
			IloExpr r3(env);
			for (int i = 0; i < n; i++){
				r3 += d[i]*x[i][j];
			}
			mod.add(r3 <= p[vetor_abertas[j]]*y[j]);
			r3.end();
		} 

IloExpr r5(env);
for (int j = 0; j <  vetor_abertas.size(); j++){
	
	for(int i = 0; i < n; i++){
		r5 += c[i][vetor_abertas[j]] * x[i][j] * d[i];
	}
} 
r5 += fo1;
mod.add(r5 >= lb);  //adiciona restricao do lower bound do subproblema
r5.end();

vector<vector<int>> start1(n, vector<int>(vetor_abertas.size()));

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < vetor_abertas.size(); j++)
    {
       start1[i][j] = 0;
    }
}

for (int i = 0; i < n ; i++)
{
    for (int j = 0; j < vetor_abertas.size(); j++)
    {
    if (atende[i] == vetor_abertas[j])
    { 
      start1[i][j] = 1;

    }
    }

  
}


IloNumVarArray startVar(env);
IloNumArray startVal(env);
for (int i = 0; i < n; i++)
         for (int j = 0; j < vetor_abertas.size(); j++) {
             startVar.add(x[i][j]);
             startVal.add(start1[i][j]);
         }
     cplex.addMIPStart(startVar, startVal);
     startVal.end();
     startVar.end();


cplex.setParam(IloCplex::Threads,1);
cplex.setWarning(env.getNullStream()); // Eliminar warnings
cplex.setOut(env.getNullStream()); /// Eliminar os logs do solver
cplex.setParam(IloCplex::EpGap, 0.00001);
cplex.setParam(IloCplex::TiLim, tilim2);
cout << "Running cplex for GAP...." << endl;
cplex.solve();
fstar2 = cplex.getObjValue();	
lb = cplex.getBestObjValue();	
gap = 100*cplex.getMIPRelativeGap();

cout << "Objective value: " << fo;
 stop = clock();
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;

	if(  fstar2 < fo) // o cplex não alterou solucão
{
fo = fo1;
for (int j = 0; j < vetor_abertas.size(); j++)
{
   s[vetor_abertas[j]].clear();
   pen[vetor_abertas[j]] = -p[vetor_abertas[j]];
   for (int i = 0; i < n; i++)
   {
   if ( abs(cplex.getValue(x[i][j])-1) < 0.0001)
   {
       s[vetor_abertas[j]].push_back(i);
       atende[i] = vetor_abertas[j];
       pen[vetor_abertas[j]] = pen[vetor_abertas[j]] + d[i];
       fo = fo + c[i][vetor_abertas[j]] * d[i];
   }
   }
  
   
}




}
clock_t parex_CPU = clock();
if ((gap <= 0.0099) /*|| (tempo > 21600)*/)    //se encontrou o ótimo do problema geral restrito ou o tempo extrapolou o máximo para o algoritmo
{
    parada2 = true;
}
else
{
cout << "Refining solution using ils2... " << endl;  
ILS_2(m,n,s,fo, pen,atende,c,d, f,abertas,fechadas,facilidades_potenciais, tam_facilidades_potenciais,p,tamanho_instancia);
stop = clock();
cout << "Objective value: " << fo;
cout << "  cpu time : " << (float)(stop - inicio_CPU)/CLOCKS_PER_SEC << endl;

}

if( (fo >= fo_antes2) )   //se não houver melhora de pelo menos 0,01% pare o algoritmo
{

    parada2 = true;
}

} // fim while parada 2 = true;

} // fim if vai resolver o gap
cout << endl;
cout << "Matheuristic algorithm ended" << endl;

cout << endl;
cout << fixed << setprecision(4);
cout << "objetive value: " << fo ;
clock_t pare_CPU = clock();
cout << "  cpu time: " << (float)(pare_CPU - inicio_CPU)/CLOCKS_PER_SEC << endl;
cout << endl;
cout << "Solution:" << endl;
for (int i = 0; i < n; i++)
{
    cout << atende[i] << " ";
}
cout << endl;

}//fim algoritmo
