Instrumente si tehnici de baza in informatica (ITBI), ID anul 1:
===============================================================

### Problema 1/2 (De la Fisiere / Directoare)
Problema 8: Implementarea matricilor dreptunghiulare de numere reale ca fisiere binare:  O matrice va fi stocata intr-un fisier binar continand elementele sale pe linii (in format intern), plus o informatie suplimentara din care sa se poata deduce numarul de linii si de coloane. 
Scrieti functii pentru urmatoarele operatii:
* void new(f,m,n) : creaza in fisierul f o matrice m x n initializata cu 0;
* float get(f,i,j) : returneaza elementul de pe pozitia i,j din matricea stocata in fisierul f;
* void set(f,i,j,x) : scrie elementul real x pe pozitia i,j in matricea stocata in fisierul f.

Se vor folosi functii de nivel superior (tipul ’FILE’).

Scrieti programe care folosesc aceste functii pentru a calcula suma si produsul a doua matrici. Programele se vor apela sub forma:
* sum f1 f2 f
* pro f1 f2 f
unde f1, f2 sunt specificatorii fisierelor continand matricile sursa, iar f specificatorul fisierului care va contine matricea destinatie. Fisierele f1 si f2 vor fi generate in prealabil cu alte programe ajutatatoare.
