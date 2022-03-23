/*
* Student: Spaczai Carla Noemi
* Grupa: 30229
* Am implementat 3 algoritmi: Quicksort, Quickselect, Heapsort.
*
* Algoritmul "Quicksort" incepe prin alegerea unui 'element pivot' din sir si prin partitionarea celorlaltor elemente in doua subsiruri (daca elementele sunt mai
* mici sau mai mari decat pivotul). Ulterior, cele 2 subsiruri vor fi ordonate recursiv (apeland in continuare Quicksort). La final, obtinem sirul ordonat.
* Complexitate: O(n*log(n)). Complexitate Worst Case: O(n^2). Complexitate Best Case: O(n*log(n)).
* Algoritmul nu e stabil.
* Cazul favorabil: are loc atunci cand apelurile recursive se fac pe vectori de aproximativ aceeasi marime (pivotul ajunge sa fie mediana vectorului).
*
* Algoritmul "Heapsort" incepe prin a schimba valoarea radacinii cu valoarea ultimei frunze (cea mai din dreapta si cea mai de jos). Se vor face interschimbari cu
* nodurile copii de cea mai mare valoare (daca valoarea e mai mare decat cea a nodului parinte) pana cand noua radacina va ajunge la pozitia ei corecta in arbore. Se
* reia acest proces cu urmatorul nod de acest tip pana cand va fi ordonat tot arborele (fiecare nod parinte trebuie sa aiba o valoare mai mare decat cele doua noduri
* copil).
* Complexitate: O(n*log(n)). Complexitate Worst Case: O(n*log(n)). Complexitate Best Case: O(n*log(n)).
* Algoritmul nu e stabil.
*
* Comparand Quicksort si Heapsort observam ca:
* Cazul mediu: Quicksort e mai eficient decat Heapsort.
* Cazul defavorabil (sirul e sortat crescator): Heapsort e mai eficient decat Quicksort (acesta are crestere exponentiala din punct de vedere al operatiilor).
*/

#include <stdio.h>
#include "Profiler.h"

Profiler pr("QUICK");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

int partition(int v[], int p, int r, int n)
{
	Operation OP = pr.createOperation("Quicksort", n);

	OP.count();
	int x = v[r];
	int i = p - 1;
	for (int j = p; j <= (r - 1); j++)
	{
		OP.count();
		if (v[j] <= x)
		{
			i = i + 1;
			OP.count(3);
			int y = v[i];
			v[i] = v[j];
			v[j] = y;
		}
	}
	OP.count(3);
	int z = v[i + 1];
	v[i + 1] = v[r];
	v[r] = z;
	return (i + 1);
}

void quicksort(int v[], int p, int r, int n)
{
	if (p < r)
	{
		int q = partition(v, p, r, n);
		quicksort(v, p, q - 1, n);
		quicksort(v, q + 1, r, n);
	}
}

int quickselect(int v[], int p, int r, int i, int n)
{
	if (p == r)
	{
		return v[p];
	}
	int q = partition(v, p, r, n);
	int k = q - p + 1;
	if (i == k)
	{
		return v[q];
	}
	else if (i < k)
	{
		return quickselect(v, p, q - 1, i, n);
	}
	else return quickselect(v, q + 1, r, i - k, n);
}

void bottom_up_2(int v[], int i, int s, int n) //heapify
{
	Operation OP = pr.createOperation("Heapsort", n);

	int k;
	int left = i * 2 + 1;
	int right = (i + 1) * 2;
	if (left < s)
	{
		OP.count();
		if (v[left] > v[i])
		{
			k = left;
		}
		else
		{
			k = i;
		}
	}
	else
	{
		k = i;
	}
	if (right < s)
	{
		OP.count();
		if (v[right] > v[k])
		{
			k = right;
		}
	}
	if (k != i)
	{
		OP.count(3);
		int x = v[i];
		v[i] = v[k];
		v[k] = x;
		bottom_up_2(v, k, s, n);
	}
}

void bottom_up(int v[], int n)
{
	for (int i = (n - 1) / 2; i >= 0; i--)
	{
		bottom_up_2(v, i, n, n);
	}
}

void heapsort(int v[], int s, int n)
{
	Operation OP = pr.createOperation("Heapsort", n);

	bottom_up(v, n);
	for (int i = n - 1; i >= 0; i--)
	{
		OP.count(3);
		int x = v[0];
		v[0] = v[i];
		v[i] = x;
		s -= 1;
		bottom_up_2(v, 0, s, n);
	}
}

void demo()
{
	int v[] = { 7, 2, 8, 9, -1, 2, 4, 1, 3, 6 };
	int n = sizeof(v) / sizeof(v[0]);

	/*quicksort(v, 0, n - 1, n);
	printf("Quicksort: ");*/
	/*heapsort(v, n, n);
	printf("Heapsort: ");

	for (int i = 0; i < n; i++)
	{
		printf("%d ", v[i]);
	}*/

	int i;
	printf("Introduceti 'i': ");
	scanf_s("%d", &i);
	int q = quickselect(v, 0, n - 1, i, n);
	printf("Quickselect: %d", q);
}

void perf(int order)
{
	int v1[MAX_SIZE], v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v1, n, 10, 50000, false, order);
			CopyArray(v2, v1, n);
			quicksort(v1, 0, n - 1, n);
			heapsort(v2, n, n);
		}
	}

	pr.divideValues("Quicksort", NR_TESTS);
	pr.divideValues("Heapsort", NR_TESTS);
	pr.createGroup("Comparare", "Quicksort", "Heapsort");
	pr.showReport();
}

void perf_all()
{
	perf(UNSORTED);
	pr.reset("Worst Case");
	perf(ASCENDING);
	pr.showReport();
}

int main()
{
	demo();
	//perf_all();
	return 0;
}