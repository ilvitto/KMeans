/*
Fabio Vittorini
K-Means in C/C++

n:				numero di punti (int)
k: 				numero di cluster (int) < n
clusters: 		vettore di cluster [#k] (int*)
centroids: 		vettore di centroidi [#k uno per cluster] (point*)
points:			elenco di punti 2D [#n] (point*)

*/

#include "ParallelKMeans.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;


int main(){
    int n, k;
    int i;
    int numChanged;
    int limit;
    int iteration;
    int ntest;
    int ktest;

    struct mypoint *points;
	struct mypoint *centroids;
	vector<double> centroidX (32);
	vector<double> centroidY (32);

    srand(time(NULL));

    ntest = 4;
    ktest = 5;

    int ntests[4] = {3000,30000,300000,3000000};
    int ktests[5] = {2,3,8,16,32};

    points = (struct mypoint *)malloc(ntests[3]* sizeof(struct mypoint));
    centroids = (struct mypoint *)malloc(ktests[4]* sizeof(struct mypoint));

    const string filename = "/home/fabiovittorini/cuda-workspace/KMeansMiccFinal/tests/GaussianDataset3000000.txt";
    //Gnuplot gp("");


    clock_t startTime=0, endTime=0;
    double sequentialTime=0, parallelTime=0;
    vector<double> sequentialTimes, parallelTimes;
    double speedUp=0;


//***********START SEQUENTIAL*************
/*
    //Repeat sequential algorithm ntest x ktest times
    for(int ni = 0; ni < ntest; ni++){
		n = ntests[ni];

		if(n/1000 > 300)	limit = 300;
		else				limit = n/100;

		cout << "*******************"<<endl;
		cout << "n = " << n << endl;

		//Prendo i punti da file
		initPoints(filename, n, points);

		for(int ki = 0; ki < ktest; ki++){
			k = ktests[ki];

			cout << "k = " << k << endl;

			centroidX.empty();
			centroidY.empty();

			startTime = clock();

			for(i = 0; i < k; i++){
				int r = (int)(rand()%n);
				centroids[i].x = points[r].x;
				centroids[i].y = points[r].y;
				centroids[i].cluster = i;
				centroidX.push_back(centroids[i].x);
				centroidY.push_back(centroids[i].y);
			}

			//gp.plotfile_xy(filename, 1, 2, "Dataset");
			//gp.plot_xy(centroidX,centroidY,"Initial Centroid "+i);

			iteration = 0;
			//Assegno ad ogni punto il cluster piu vicino usando i centroidi
			do {
				iteration++;

				numChanged = assignPointsToNearestCluster(n, k, points, centroids);

				cout << "#" << iteration << " changed: " << numChanged << endl;

				computeCentroids(n, k, points, centroids);

				//showPointsAssigned(n,k,&gp,points,centroids);

			}while(numChanged > limit && iteration < 100);

			endTime=clock();

			cout << "KMeans end" << endl;

			sequentialTime = ((double) (endTime - startTime)) * 1000 / CLOCKS_PER_SEC;

			sequentialTimes.push_back(sequentialTime);

			cout << "Final time: " << sequentialTime << endl;

			cout << "Final centroids: " << endl;
			for(int i = 0; i < k; i++)
				cout << i << ": " << centroids[i].x << ";" << centroids[i].y << endl;

		}

	}

//*************END SEQUENTIAL********************
*/

//*************START PARALLEL********************

    cout << "********PARALLEL KMEANS WITH OMP************";

    //Repeat sequential algorithm ntest x ktest times
	for(int ni = 0; ni < ntest; ni++){
		n = ntests[ni];

		if(n/1000 > 300)	limit = 300;
		else				limit = n/100;

		cout << "*******************"<<endl;
		cout << "n = " << n << endl;

		//Prendo i punti da file
		initPoints(filename, n, points);

		for(int ki = 0; ki < ktest; ki++){
			k = ktests[ki];

			cout << "k = " << k << endl;

			centroidX.empty();
			centroidY.empty();

			startTime = clock();

			int r;
			//Scelgo k punti casuali per l'inizializzazione dei centroidi
			#pragma omp parallel for private(r)
			for(i = 0; i < k; i++){
				r = (int)(rand()%n);
				centroids[i].x = points[r].x;
				centroids[i].y = points[r].y;
				centroids[i].cluster = i;
			}

			for(i = 0; i < k; i++){
				centroidX.push_back(centroids[i].x);
				centroidY.push_back(centroids[i].y);
			}

			//gp.plotfile_xy(filename, 1, 2, "Dataset");
			//gp.plot_xy(centroidX,centroidY,"Initial Centroid "+i);

			iteration = 0;
			//Assegno ad ogni punto il cluster piu vicino usando i centroidi
			do {
				iteration++;

				numChanged = assignPointsToNearestClusterParallel(n, k, points, centroids);

				cout << "#" << iteration << " changed: " << numChanged << endl;

				computeCentroidsParallel(n, k, points, centroids);

				//showPointsAssigned(n,k,&gp,points,centroids);

			}while(numChanged > limit && iteration < 100);

			endTime=clock();

			cout << "End KMeans Parallel" << endl;

			parallelTime = ((double) (endTime - startTime)) * 1000 / CLOCKS_PER_SEC;

			parallelTimes.push_back(parallelTime);

			cout << "Final time: " << parallelTime << endl;

			cout << "Final centroids: " << endl;
			for(int i = 0; i < k; i++)
				cout << i << ": " << centroids[i].x << ";" << centroids[i].y << endl;

		}
	}


    //***************END PARALLEL************************

    cout << "Sequential Times: {";
    for(int i = 0; i < sequentialTimes.size(); i++)
    	cout << sequentialTimes[i] << " ";
    cout << "}" << endl;

    cout << "Parallel Times: {";
	for(int i = 0; i < parallelTimes.size(); i++)
		cout << parallelTimes[i] << " ";
	cout << "}" << endl;

    cout << "SpeedUps: {";
	for(int i = 0; i < parallelTimes.size(); i++)
		cout << sequentialTimes[i]/parallelTimes[i] << " ";
	cout << "}" << endl;


    //gp.plotfile_xy(filename, 1, 2, "Dataset");

    //showPointsAssigned(n,k,&gp,points,centroids);
    //showPointsAssignedParallel(n,k,&gp,points,centroids);

    return 0;
}
