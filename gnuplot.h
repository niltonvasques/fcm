#ifndef __GNUPLOT_UTILS__
#define __GNUPLOT_UTILS__

#include "shared.h"

inline int gnuplot_membership_matrix() {
    int i, j, cluster;
    char fname[100];
    double highest;
    FILE * f[MAX_CLUSTER];
    if (num_dimensions != 2) {
        printf("Plotting the cluster only works when the\n");
        printf("number of dimensions is two. This will create\n");
        printf("a two-dimensional plot of the cluster points.\n");
        exit(1);
    }
    for (j = 0; j < num_clusters; j++) {
        sprintf(fname, "cluster.%d", j);
        if ((f[j] = fopen(fname, "w")) == NULL) {
            printf("Could not create %s\n", fname);
            for (i = 0; i < j; i++) {
                fclose(f[i]);
                sprintf(fname, "cluster.%d", i);
                remove(fname);
            }
            return -1;
        }
        fprintf(f[j], "#Data points for cluster: %d\n", j);
    }
    for (i = 0; i < num_data_points; i++) {
        cluster = 0;
        highest = 0.0;
        for (j = 0; j < num_clusters; j++) {
            if (degree_of_memb[i][j] > highest) {
                highest = degree_of_memb[i][j];
                cluster = j;
            }
        }
        fprintf(f[cluster], "%lf %lf\n", data_point[i][0], data_point[i][1]);
    }
    for (j = 0; j < num_clusters; j++) {
        fclose(f[j]);
    }
    if ((f[0] = fopen("gnuplot.script", "w")) == NULL) {
        printf("Could not create gnuplot.script.\n");
        for (i = 0; i < j; i++) {
            fclose(f[i]);
            sprintf(fname, "cluster.%d", i);
            remove(fname);
        }
        return -1;
    }
    fprintf(f[0], "set terminal png medium\n");
    fprintf(f[0], "set output \"cluster_plot.png\"\n");
    fprintf(f[0], "set title \"FCM clustering\"\n");
    fprintf(f[0], "set xlabel \"x-coordinate\"\n");
    fprintf(f[0], "set ylabel \"y-coordinate\"\n");
    fprintf(f[0], "set xrange [%lf : %lf]\n", low_high[0][0], low_high[0][1]);
    fprintf(f[0], "set yrange [%lf : %lf]\n", low_high[1][0], low_high[1][1]);
    fprintf(f[0],
            "plot 'cluster.0' using 1:2 with points pt 7 ps 1 lc 1 notitle");
    for (j = 1; j < num_clusters; j++) {
        sprintf(fname, "cluster.%d", j);
        fprintf(f[0],
                ",\\\n'%s' using 1:2 with points  pt 7 ps 1 lc %d notitle",
                fname, j + 1);
    }
    fprintf(f[0], "\n");
    fclose(f[0]);
    return 0;
}

#endif
