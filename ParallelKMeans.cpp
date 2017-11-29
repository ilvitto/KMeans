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
#include <omp.h>


using namespace std;


//void showPointsAssignedParallel(int n, int k, Gnuplot *gp, struct point * points,struct point * centroids);


void initPointsParallel(string file, int n, struct mypoint* points) {
	ifstream input;
	input.open(file.c_str());
	if(!input){
		cout << "Error getting input points";
		return;
	}

	#pragma omp parallel for
    for (int i = 0; i <= n; i++) {
        input >> points[i].x;
        input >> points[i].y;
        points[i].cluster = -1;
    }
	input.close();
}

void computeCentroidsParallel(int n, int k, struct mypoint * points,struct mypoint * centroids) {
	int i, cluster;
	double *sumXCluster;
	double *sumYCluster;
	int *countElementCluster;
    int tid;

	sumXCluster = (double *)malloc(sizeof(double)*k );
	sumYCluster = (double *)malloc(sizeof(double)*k );
	countElementCluster = (int *)malloc(sizeof(int)*k );
	#pragma omp parallel for
	for(cluster = 0; cluster < k; cluster++){
		sumXCluster[cluster] = 0;
		sumYCluster[cluster] = 0;
		countElementCluster[cluster] = 0;
	}

	for (i = 0; i < n; i++) {
        sumXCluster[points[i].cluster] += points[i].x;
        sumYCluster[points[i].cluster] += points[i].y;
        countElementCluster[points[i].cluster]++;
	}
	#pragma omp parallel for
    for (cluster = 0; cluster < k; cluster++) {
        centroids[cluster].x = sumXCluster[cluster] / countElementCluster[cluster];
        centroids[cluster].y = sumYCluster[cluster] / countElementCluster[cluster];
    }
}

//Not used
void seeResultsParallel(string res, int n, int cluster, struct mypoint * points,struct mypoint * centroids){

	ofstream output;
	output.open(res.c_str());

	//Get data from file
	#pragma omp parallel for
	for(int i = 0; i < n; i++){
		if(points[i].cluster == cluster){
			output << points[i].x << " ";
			output << points[i].y << endl;
		}
	}

	output.close();
}

int assignPointsToNearestClusterParallel(int n, int k, struct mypoint * points,struct mypoint * centroids) {
	double minDistance;
	int bestCluster;
	int numChanged = 0;
	int i, j;
	double dist;

	#pragma omp parallel for private(minDistance, bestCluster)
	for (i = 0; i < n; i++) {
		minDistance = mydistance(centroids[0], points[i]);
		bestCluster = 0;
		#pragma omp parallel for private(dist)
		for (j = 1; j < k; j++) {
			dist = mydistance(centroids[j], points[i]);
			#pragma omp critical
			if (dist < minDistance) {
				minDistance = dist;
				bestCluster = j;
			}
		}

        #pragma omp critical
		if (points[i].cluster != bestCluster) {
			points[i].cluster = bestCluster;
			numChanged++;
		}
	}

	return numChanged;
}

/*
void showPointsAssignedParallel(int n, int k, Gnuplot *gp, struct point * points,struct point * centroids) {
	vector<double> x = {};
	vector<double> y = {};
	vector<double> centroidX = {};
	vector<double> centroidY = {};

	centroidX = {};
	centroidY = {};
	#pragma omp parallel for
	for (int j = 0; j < k; j++){
		x = {};
		y = {};

		#pragma omp parallel for
		for (int i = 0; i < n; i++) {
			if (points[i].cluster == j) {
				#pragma omp critical
				x.push_back(points[i].x);
				y.push_back(points[i].y);
			}
		}
		//Plot cluster j
		(*gp).plot_xy(x,y,"Cluster "+j);

        #pragma omp critical
		centroidX.push_back(centroids[j].x);
		centroidY.push_back(centroids[j].y);
	}

	(*gp).plot_xy(centroidX,centroidY,"Final centroids");

}*/
