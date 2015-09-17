#!/bin/env ruby

cluster = ARGV[0]

#system "make clean"
system "make"
system "./main --#{cluster} --input points"
system "gnuplot gnuplot.script"
system "eog cluster_plot.png"
