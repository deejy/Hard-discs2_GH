# General description of program suite {#general}

The **2D_Monte_Carlo** suite of programmes described here are designed to provide a simple system
for investigating very coarse grained models of membrane proteins. The programmes
operate with several different types of file that describe the system being modelled

<img src="./Images/Aqp.png" alt="an example configuration" width="300"/>
Included are programmes for designing and creating a system (called a configuration), 
manipulating a system (for example expanding or compressing it), 
analysing a system or displaying it, 
and doing different forms of integraton.

## The programmes and utilities

* [makeconfig](@ref makeconfig) - create a new configuration file.
* [shrinkconfig](@ref shrinkconfig) - change the size of a configuration (with agitation if necessary).
* [config2eps](@ref config2eps) - create a postscript file from a configuration file.

* [pcf](@ref analysis/analysis.md) - calculate pair correlation functions from a configuration.
* [2DOrder](@ref analysis/analysis.md) - analyse the local environment of the objects

* [NVT](@ref NVT) - perform a NVT monte-carlo integration in the NVT ensemble.
* [Gibbs](@ref Gibbs) - perform a monte-carlo integration in the Gibbs ensemble.

<!--
Others that exist and might be fun...
* [delaunay] - do a delaunay tesselation and calculate a voronoi diagramme
* [diffusion] - calculate diffusion information from time series
* [diff_tracer] - do a tracer diffusion calculation
* [crystallite] - identify crystalline regions in a configuration
* [diff_config] - calclate difference between 2 configurations. 
* [NPT]
* [g6r]
-->

## Files and file formats

The programmes have in common that they all operate on configurations.
A configuration can be thought of as a series of objects organized on 
a surface. 
These objects might, for example, be proteins and the surface
a flat two dimensional representation of a membrane. 
There can be several different types of object in the configuration.
Configurations can be stored in human readable files and their structure 
is described [here](@ref config_file).

The various objects referenced in the configuration are made up of one
or more beads (or atoms). Atoms have various properties, such as size 
and color and objects interact through their atoms.
The organization of the various atoms to make up objects is described by a 
topology file that is described in detail [here](@ref topology_file).

The interactions between the different atoms is defined by a force field.
This data structure defines the properties of the different atom types, and
their pairwise interactions. The force field is saved in a file that is
described [here](@ref force_field_file).

## Compilation, installation and testing

* Currently the implementation needs the BOOST library and valgrind for the test suite : sudo apt install libboost-all-dev valgrind
* The documentation makes use of doxygen and several Latex utilities to build the pdf reference manual  : sudo apt install doxygen graphviz texlive-latex-base texlive-latex-extra  graphicsmagick-imagemagick-compat
* Running 'make all' in the root directory should compile everything including 
  the documentation.
* Running 'make test' will perform various tests to make sure the programmes 
appear to be running as expected.

## Tutorial

