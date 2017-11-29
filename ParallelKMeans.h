#include "SequentialKMeans.h"

void initPointsParallel(string file, int n, struct mypoint* points);
void computeCentroidsParallel(int n, int k, struct mypoint * points,struct mypoint * centroids);
void seeResultsParallel(string res, int n, int cluster, struct mypoint * points,struct mypoint * centroids);
int assignPointsToNearestClusterParallel(int n, int k, struct mypoint * points,struct mypoint * centroids);

