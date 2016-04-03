#!/bin/env ruby

cluster = ARGV[0]
data = ARGV[1]

#system "make clean"
system "make"
#system "./main --#{cluster} --input input.dat"
if data.nil?
  system "./main --#{cluster} --input points"
else
  system "./main --#{cluster} --input #{data}"
end
system "gnuplot gnuplot.script"
system "eog cluster_plot.png"
