#include <stdlib.h>
#include <stdio.h>
#include "shared.h"
#include "gnuplot.h"
#include "pcm.h"
#include "fcm.h"

#define __MAIN_FILE__ 

#include <argp.h>
#include <stdbool.h>

const char *argp_program_version = "Fuzzy Clustering";
const char *argp_program_bug_address = "<nilton.vasques@openmailbox.org>";
static char doc[] = "Implementing some fuzzy clustering algorithms.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
    { "fcm", 'f', 0, 0, "Run fuzzy c means."},
    { "pcm", 'p', 0, 0, "Run possibilistc c means."},
    { "input", 'i', "INPUT", 0, "Path for input data."},
    { 0 } 
};

struct arguments {
    enum { FCM, PCM } mode;
    char *input;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'f': arguments->mode = FCM; break;
    case 'p': arguments->mode = PCM; break;
    case 'i': arguments->input = arg; break;
    case ARGP_KEY_ARG: return 0;
    default: return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
    struct arguments arguments;
    arguments.input = NULL;

    arguments.mode = FCM;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if(!arguments.input) return 0;

    printf ("------------------------------------------------------------------------\n");
    if(arguments.mode == PCM){
      pcm(arguments.input);
    }else if(arguments.mode == FCM){
      fcm(arguments.input);
    }

    printf("Number of data points: %d\n", num_data_points);
    printf("Number of clusters: %d\n", num_clusters);
    printf("Number of data-point dimensions: %d\n", num_dimensions);
    printf("Accuracy margin: %lf\n", epsilon);
    print_membership_matrix("membership.matrix");
    gnuplot_membership_matrix();
    printf
      ("------------------------------------------------------------------------\n");
    printf("The program run was successful...\n");
    printf("Storing membership matrix in file 'membership.matrix'\n\n");
    printf("If the points are on a plane (2 dimensions)\n");
    printf("the gnuplot script was generated in file 'gnuplot.script', and\n");
    printf("the gnuplot data in files cluster.[0]... \n\n");
    printf
      ("Process 'gnuplot.script' to generate graph: 'cluster_plot.png'\n\n");
    printf
      ("NOTE: While generating the gnuplot data, for each of the data points\n");
    printf("the corresponding cluster is the one which has the highest\n");
    printf("degree-of-membership as found in 'membership.matrix'.\n");
    printf
      ("------------------------------------------------------------------------\n");
    return 0;
}
