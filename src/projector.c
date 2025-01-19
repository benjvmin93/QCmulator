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
    size_t size = 1 << proj->nqubits;
    for (size_t i = 0; i < size; ++i)
    {
        free_complex(proj->data[i]);
    }
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

struct Complex **proj_ZERO(void)
{
    size_t size = 1 << 1;
    struct Complex **data = xmalloc(size * size * sizeof(struct Complex *));
    data[0] = init_complex(1., 0.);
    data[1] = init_complex(0., 0.);
    data[2] = init_complex(0., 0.);
    data[3] = init_complex(0., 0.);
    return data;
}

struct Complex **proj_ONE(void)
{
    size_t size = 1 << 1;
    struct Complex **data = xmalloc(size * size * sizeof(struct Complex *));
    data[0] = init_complex(0., 0.);
    data[1] = init_complex(0., 0.);
    data[2] = init_complex(0., 0.);
    data[3] = init_complex(1., 0.);
    return data;
}


struct Complex **get_data_from_proj_id(enum PROJECTOR proj)
{
    size_t size = 1 << get_nqubits_from_proj_id(proj);
    struct Complex **data = NULL;
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