#ifdef DOUBLE_PRECISION
    typedef double data_t;
#else
    typedef float data_t;
#endif

#include <unistd.h>

#include "param.hpp"
#include "operator.hpp"
#include "IO.hpp"
#include "seplib.h"

typedef vecReg<data_t> vec;
typedef cvecReg<data_t> cvec;
typedef axis<data_t> ax;
typedef hypercube<data_t> hyper;

void printdoc(){
    std::string doc = "\nDescription:\n"
    "   Compute spectra using Fourier transform along the fast axis (often time).\n"
    "\nInput/Output:\n"
    "   Provide input as 'input=file.H' or '< file.H' and output as 'output=file.H' or '> file.H'.\n"
    "\nParameters:\n"
    "   type - string - ['amplitude']:\n\t\toptions: 'amplitude', 'power', 'phase', 'real', 'imag'.\n"
    "\nExample:\n"
    "   FX.x < infile.H type=power > oufile.H.\n"
    "\n";
    fprintf(stderr,doc.c_str());
}

int main(int argc, char **argv){

    if (argc == 1 && isatty(STDIN_FILENO)==true) {printdoc(); return 0;}

	initpar(argc,argv);

    std::string input_file="in", output_file="out", type="amplitude";
    readParam<std::string>(argc, argv, "input", input_file);
	readParam<std::string>(argc, argv, "output", output_file);
    readParam<std::string>(argc, argv, "type", type);

    successCheck(input_file!="none",__FILE__,__LINE__,"Input file is not provided\n");
    
    std::shared_ptr<vec> input = sepRead<data_t>(input_file);

    fxTransform fx(*input->getHyper());
    std::shared_ptr<cvec> fxvec = std::make_shared<cvec> (*fx.getRange());
    fxvec->zero();
    fx.forward(false,input,fxvec);

    std::shared_ptr<vec> output;
    if(type=="power") output = fxvec->modulus2();
    else if (type=="phase") output = fxvec->arg();
    else if (type=="real") output = fxvec->real();
    else if (type=="imag") output = fxvec->imag();
    else output = fxvec->modulus();

    output->setHyper(*fx.getRange());
     
    if (output_file!="none") sepWrite<data_t>(output, output_file);

    return 0;
}