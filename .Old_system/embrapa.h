#ifndef EMBRAPA_H_INCLUDED
#define EMBRAPA_H_INCLUDED
#include "data.h"

class Embrapa{
public:
	static void load_pta(Sample *samples, string ptaFile);
	static Sample* remove_points_without_pta(Sample *samples, int verbose);
	static Sample* correct_pta(Sample *samples, int option);
	static int generate_classes(Sample *samples, int option);
};

#endif
