%module tapedelay_libtapedelay
%include <od/glue/mod.cpp.swig>

%{

#undef SWIGLUA
#include <TapeDelay.h>
#define SWIGLUA

%}

%include <TapeDelay.h>
