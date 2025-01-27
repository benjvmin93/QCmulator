#include "headers/projector.h"
#include "headers/alloc.h"


struct Projector *init_projector(enum PROJECTOR id)
{
    struct Projector *proj = xmalloc(sizeof(struct Projector));
    proj->data = get_data_from_proj_id(id);
    proj->nqubits = get_nqubits_from_proj_id(id);
    return proj;
}

void free_projector(struct Projector *proj)
{
    free(proj->data);
    free(proj);
}

int get_nqubits_from_proj_id(enum PROJECTOR proj)
{
    switch (proj)
    {
    case ZERO:
        return 1;
    case ONE:
        return 1;
    default:
        return 0;
    }
}

double complex *proj_ZERO(void)
{
    size_t size = 1 << 1;
    double complex *data = xmalloc(size * size * sizeof(double complex));
    data[0] = 1;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    return data;
}

double complex *proj_ONE(void)
{
    size_t size = 1 << 1;
    double complex *data = xmalloc(size * size * sizeof(double complex));
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 1;
    return data;
}


double complex *get_data_from_proj_id(enum PROJECTOR proj)
{
    double complex *data = NULL;
    switch (proj)
    {
    case ZERO:
        data = proj_ZERO();
        break;
    case ONE:
        data = proj_ONE();
        break;
    default:
        break;
    }

    return data;
}