#ifndef __SHARED_H__
#define __SHARED_H__

#define MAX_DATA_POINTS 10000
#define MAX_CLUSTER 100
#define MAX_DATA_DIMENSION 5

#ifndef __MAIN_FILE__

int num_data_points;
int num_clusters;
int num_dimensions;
double low_high[MAX_DATA_DIMENSION][2];
double degree_of_memb[MAX_DATA_POINTS][MAX_CLUSTER];
double epsilon;
double fuzziness;
double data_point[MAX_DATA_POINTS][MAX_DATA_DIMENSION];
double cluster_centre[MAX_CLUSTER][MAX_DATA_DIMENSION];

#else

extern int num_data_points;
extern int num_clusters;
extern int num_dimensions;
extern double low_high[MAX_DATA_DIMENSION][2];
extern double degree_of_memb[MAX_DATA_POINTS][MAX_CLUSTER];
extern double epsilon;
extern double fuzziness;
extern double data_point[MAX_DATA_POINTS][MAX_DATA_DIMENSION];
extern double cluster_centre[MAX_CLUSTER][MAX_DATA_DIMENSION];

#endif

static inline void print_data_points(char *fname) {
    int i, j;
    FILE *f;
    if (fname == NULL)
        f = stdout;
    else if ((f = fopen(fname, "w")) == NULL) {
        printf("Cannot create output file.\n");
        exit(1);
    }
    fprintf(f, "Data points:\n");
    for (i = 0; i < num_data_points; i++) {
        printf("Data[%d]: ", i);
        for (j = 0; j < num_dimensions; j++) {
            printf("%.5lf ", data_point[i][j]);
        }
        printf("\n");
    }
    if (fname == NULL)
        fclose(f);
}

static inline void print_membership_matrix(char *fname) {
    int i, j;
    FILE *f;
    if (fname == NULL)
        f = stdout;
    else if ((f = fopen(fname, "w")) == NULL) {
        printf("Cannot create output file.\n");
        exit(1);
    }
    fprintf(f, "Membership matrix:\n");
    for (i = 0; i < num_data_points; i++) {
        fprintf(f, "Data[%d]: ", i);
        for (j = 0; j < num_clusters; j++) {
            fprintf(f, "%lf ", degree_of_memb[i][j]);
        }
        fprintf(f, "\n");
    }
    if (fname == NULL)
        fclose(f);
}

#endif
