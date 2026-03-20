// The module name is configured in the Makefile.
%module wf259_libwf259

// Include the necessary boiler-plate for C++ mods.
%include <od/glue/mod.cpp.swig>

// Code between %{ and %} is inserted verbatim into the wrapper.
%{

#undef SWIGLUA

// Add header files necessary for compilation of the interfaces added further down.
#include <dsp/wf259.h>

#define SWIGLUA

%}

// Here we define what classes should have lua interfaces generated.
// Usually just %include'ing the header file is enough.
// Code bracked by #ifndef SWIGLUA will be ignored here.

%include <dsp/wf259.h>
