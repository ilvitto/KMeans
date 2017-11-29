/*
Fabio Vittorini
K-Means in C/C++

n:				numero di punti (int)
k: 				numero di cluster (int) < n
clusters: 		vettore di cluster [#k] (int*)
centroids: 		vettore di centroidi [#k uno per cluster] (point*)
points:			elenco di punti 2D [#n] (point*)

*/


#include "SequentialKMeans.h"
//#include "gnuplot_i.hpp"



void initPoints(string file, int n, struct mypoint* points) {
	ifstream input;
	input.open(file.c_str());
	if(!input){
		cout << "Error getting input points";
		return;
	}

	for(int i = 0; i <= n; i++){
		input >> points[i].x;
		input >> points[i].y;
		points[i].cluster = -1;
	}
	input.close();
}

//Funzione di distanza tra 2 punti (euclidea)
double mydistance(struct mypoint p1,struct mypoint p2){

	return sqrt(pow(p1.x - p2.x,2)+pow(p1.y - p2.y,2));

}

void computeCentroids(int n, int k, struct mypoint * points,struct mypoint * centroids) {
	int i, cluster;
	double *sumXCluster;
	double *sumYCluster;
	int *countElementCluster;

	sumXCluster = (double *)malloc(sizeof(double)*k );
	sumYCluster = (double *)malloc(sizeof(double)*k );
	countElementCluster = (int *)malloc(sizeof(int)*k );
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
	for (cluster = 0; cluster < k; cluster++) {
		centroids[cluster].x = sumXCluster[cluster] / countElementCluster[cluster];
		centroids[cluster].y = sumYCluster[cluster] / countElementCluster[cluster];
	}

}

//Not used now
void seeResults(string res, int n, int cluster, struct mypoint * points,struct mypoint * centroids){

	ofstream output;
	output.open(res.c_str());

	//Get data from file
	for(int i = 0; i < n; i++){
		if(points[i].cluster == cluster){
			output << points[i].x << " ";
			output << points[i].y << endl;
		}
	}

	output.close();
}

int assignPointsToNearestCluster(int n, int k, struct mypoint * points,struct mypoint * centroids) {
	double minDistance;
	int bestCluster;
	int numChanged = 0;
	int i, j;
	double dist;

	for (i = 0; i < n; i++) {
		minDistance = mydistance(centroids[0], points[i]);
		bestCluster = 0;
		for (j = 1; j < k; j++) {
			dist = mydistance(centroids[j], points[i]);
			if (dist < minDistance) {
				minDistance = dist;
				bestCluster = j;
			}
		}
		if (points[i].cluster != bestCluster) {
			points[i].cluster = bestCluster;
			numChanged++;
		}
	}

	return numChanged;
}
/*

void showPointsAssigned(int n, int k, Gnuplot *gp, struct point * points,struct point * centroids) {
	vector<double> x = {};
	vector<double> y = {};
	vector<double> centroidX = {};
	vector<double> centroidY = {};

	centroidX = {};
	centroidY = {};
	for (int j = 0; j < k; j++){
		x = {};
		y = {};

		for (int i = 0; i < n; i++) {
			if (points[i].cluster == j) {
				x.push_back(points[i].x);
				y.push_back(points[i].y);
			}
		}
		//Plot cluster j
		(*gp).plot_xy(x,y,"Cluster "+j);

		centroidX.push_back(centroids[j].x);
		centroidY.push_back(centroids[j].y);
	}

	(*gp).plot_xy(centroidX,centroidY,"Final centroids");

}

*/
