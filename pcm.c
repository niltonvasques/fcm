#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"
#include "gnuplot.h"
#include "pcm.h"

double gamas[MAX_CLUSTER];

static inline int init(char *fname) {
  int i, j, r, rval;
  FILE *f;
  double s;
  if ((f = fopen(fname, "r")) == NULL) {
    printf("Failed to open input file.");
    return -1;
  }
  fscanf(f, "%d %d %d", &num_data_points, &num_clusters, &num_dimensions);
  if (num_clusters > MAX_CLUSTER) {
    printf("Number of clusters should be < %d\n", MAX_CLUSTER);
    goto failure;
  }
  if (num_data_points > MAX_DATA_POINTS) {
    printf("Number of data points should be < %d\n", MAX_DATA_POINTS);
    goto failure;
  }
  if (num_dimensions > MAX_DATA_DIMENSION) {
    printf("Number of dimensions should be >= 1.0 and < %d\n",
        MAX_DATA_DIMENSION);
    goto failure;
  }
  fscanf(f, "%lf", &fuzziness);
  if (fuzziness <= 1.0) {
    printf("Fuzzyness coefficient should be > 1.0\n");
    goto failure;
  }
  fscanf(f, "%lf", &epsilon);
  if (epsilon <= 0.0 || epsilon > 1.0) {
    printf("Termination criterion should be > 0.0 and <= 1.0\n");
    goto failure;
  }
  for (i = 0; i < num_data_points; i++) {
    for (j = 0; j < num_dimensions; j++) {
      fscanf(f, "%lf", &data_point[i][j]);
      if (data_point[i][j] < low_high[j][0])
        low_high[j][0] = data_point[i][j];
      if (data_point[i][j] > low_high[j][1])
        low_high[j][1] = data_point[i][j];
    }
  }
  for (i = 0; i < num_data_points; i++) {
    s = 0.0;
    r = 100;
    for (j = 1; j < num_clusters; j++) {
      rval = rand() % (r + 1);
      r -= rval;
      degree_of_memb[i][j] = rval / 100.0;
      s += degree_of_memb[i][j];
    }
    degree_of_memb[i][0] = 1.0 - s;
  }
  fclose(f);
  return 0;
failure:
  fclose(f);
  exit(1);
}

static inline int calculate_centre_vectors() {
  int i, j, k;
  double numerator, denominator;
  double t[MAX_DATA_POINTS][MAX_CLUSTER];
  for (i = 0; i < num_data_points; i++) {
    for (j = 0; j < num_clusters; j++) {
      t[i][j] = pow(degree_of_memb[i][j], fuzziness);
    }
  }
  for (j = 0; j < num_clusters; j++) {
    for (k = 0; k < num_dimensions; k++) {
      numerator = 0.0;
      denominator = 0.0;
      for (i = 0; i < num_data_points; i++) {
        numerator += t[i][j] * data_point[i][k];
        denominator += t[i][j];
      }
      cluster_centre[j][k] = numerator / denominator;
    }
  }
  return 0;
}

static inline double get_norm(int i, int j) {
  int k;
  double sum = 0.0;
  for (k = 0; k < num_dimensions; k++) {
    sum += pow(data_point[i][k] - cluster_centre[j][k], 2);
  }
  return sqrt(sum);
}

static inline double get_new_value(int i, int j) {
  int k;
  double t, p, sum;
  sum = 0.0;
  p = 2 / (fuzziness - 1);
  for (k = 0; k < num_clusters; k++) {
    t = get_norm(i, j) / get_norm(i, k);
    t = pow(t, p);
    sum += t;
  }
  return 1.0 / sum;
}

void estimate_gamas() {
  double k = 1;
  int i, j;

  for (j = 0; j < num_clusters; j++) {
    double numerator = 0;
    double denominator = 0;
    for (i = 0; i < num_data_points; i++) {
      double dij = get_norm(i, j); 
      double uij = degree_of_memb[i][j]; 
      uij = pow(uij, fuzziness);
      numerator += uij * dij;
      denominator += uij;
    }
    gamas[j] = k * (numerator / denominator);
    printf("gamas[%d]: %f\n", j, gamas[j]);
  }
}

double tipicality(double distance, int j){

  double denominator = distance / gamas[j];
  double exp = 1.0 / (fuzziness - 1.0);
  denominator = 1.0 + pow(denominator, exp);
  return 1.0 / denominator;
}

static inline double update_degree_of_membership() {
  int i, j;
  double new_uij;
  double tik;
  double sum_ic = 0;
  double sum_kn = 0;
  double sum_lc = 0;
  double sum_jn[num_clusters];
  double distance;

  for (j = 0; j < num_clusters; j++) {
    sum_jn[j] = 0;
  }

  for (i = 0; i < num_data_points; i++) {
    sum_ic = 0;
    for (j = 0; j < num_clusters; j++) {
      distance = get_norm(i, j);
      new_uij = tipicality(distance, j);
      tik = pow(new_uij, fuzziness);
      sum_ic += tik * distance;
      tik = pow(1 - new_uij, fuzziness);
      sum_jn[j] += tik;
      degree_of_memb[i][j] = new_uij;
    }
    sum_kn += sum_ic;
  }

  for (j = 0; j < num_clusters; j++) {
    sum_lc += sum_jn[j] * gamas[j];
  }

  return sum_kn + sum_lc;
}

int pcm(char *fname) {
  double max_diff;
  double curr_j = 0, old_j = 0;
  init(fname);
  estimate_gamas();
  do {
    calculate_centre_vectors();
    curr_j = update_degree_of_membership();
    max_diff = abs(curr_j - old_j);
    old_j = curr_j;
    printf("max_diff: %f\n", max_diff);
  } while (max_diff > epsilon);
  return 0;
}
