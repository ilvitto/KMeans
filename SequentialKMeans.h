#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>

//Struttura di punti 2D
struct mypoint{
	double x;
	double y;
	int cluster;
};

using namespace std;

void initPoints(string file, int n, struct mypoint* points);
double mydistance(struct mypoint p1,struct mypoint p2);
void computeCentroids(int n, int k, struct mypoint * points,struct mypoint * centroids);
void seeResults(string res, int n, int cluster, struct mypoint * points,struct mypoint * centroids);
int assignPointsToNearestCluster(int n, int k, struct mypoint * points,struct mypoint * centroids);


