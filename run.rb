#!/bin/env ruby

cluster = ARGV[0]

system "./main --#{cluster} --input input.dat"
system "gnuplot gnuplot.script"
system "eog cluster_plot.png"
