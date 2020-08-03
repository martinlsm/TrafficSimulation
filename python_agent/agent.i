%module agent
%{
  #define SWIG_FILE_WITH_INIT
  #include "agent.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}


%apply (int DIM1, double* INPLACE_ARRAY1) {(int size, double *state)};
%include "agent.h"
