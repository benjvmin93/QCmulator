#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "complex.h"

enum PROJECTOR
{
    ZERO=0,
    ONE,
};

struct Projector
{
    struct Complex **data;
    enum PROJECTOR id;
    int nqubits;
};

struct Projector *init_projector(enum PROJECTOR id);
void free_projector(struct Projector *proj);
int get_nqubits_from_proj_id(enum PROJECTOR proj);
struct Complex **get_data_from_proj_id(enum PROJECTOR proj);

#endif