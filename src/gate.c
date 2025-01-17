#include "headers/gate.h"
#include "headers/alloc.h"
#include "headers/complex.h"

#include <err.h>
#include <math.h>
#include <stdio.h>

struct Complex **I_gate(struct Complex **data)
{
    data[0 * 2 + 0] = init_complex(1., 0.);
    data[0 * 2 + 1] = init_complex(0., 0.);
    data[1 * 2 + 0] = init_complex(0., 0.);
    data[1 * 2 + 1] = init_complex(1., 0.);
    return data;
}

struct Complex **X_gate(struct Complex **data)
{
    data[0 * 2 + 0] = init_complex(0., 0.);
    data[0 * 2 + 1] = init_complex(1., 0.);
    data[1 * 2 + 0] = init_complex(1., 0.);
    data[1 * 2 + 1] = init_complex(0., 0.);
    return data;
}

struct Complex **Y_gate(struct Complex **data)
{
    data[0 * 2 + 0] = init_complex(0., 0.);
    data[0 * 2 + 1] = init_complex(0., -1.);
    data[1 * 2 + 0] = init_complex(0., 1.);
    data[1 * 2 + 1] = init_complex(0., 0.);
    return data;
}

struct Complex **Z_gate(struct Complex **data)
{
    data[0 * 2 + 0] = init_complex(1., 0.);
    data[0 * 2 + 1] = init_complex(0., 0.);
    data[1 * 2 + 0] = init_complex(0., 0.);
    data[1 * 2 + 1] = init_complex(-1., 0.);
    return data;
}

struct Complex **H_gate(struct Complex **data)
{
    double FRAC_SQRT_2 = 1/sqrt(2.);
    data[0 * 2 + 0] = init_complex(FRAC_SQRT_2, 0.);
    data[0 * 2 + 1] = init_complex(FRAC_SQRT_2, 0.);
    data[1 * 2 + 0] = init_complex(FRAC_SQRT_2, 0.);
    data[1 * 2 + 1] = init_complex(-FRAC_SQRT_2, 0.);
    return data;
}

struct Complex **RX_gate(struct Complex **data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RX gate");
    }
    double frac_theta_2 = *theta / 2.;
    data[0 * 2 + 0] = init_complex(cos(frac_theta_2), 0.);
    data[0 * 2 + 1] = init_complex(0., -sin(frac_theta_2));
    data[1 * 2 + 0] = init_complex(0., -sin(frac_theta_2));
    data[1 * 2 + 1] = init_complex(cos(frac_theta_2), 0.);
    return data;
}

struct Complex **RY_gate(struct Complex **data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RY gate");
    }
    const double frac_theta_2 = *theta / 2.;
    data[0 * 2 + 0] = init_complex(cos(frac_theta_2), 0.);
    data[0 * 2 + 1] = init_complex(0., -sin(frac_theta_2));
    data[1 * 2 + 0] = init_complex(0., sin(frac_theta_2));
    data[1 * 2 + 1] = init_complex(cos(frac_theta_2), 0.);
    return data;
}

struct Complex **RZ_gate(struct Complex **data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RZ gate");
    }
    const double frac_theta_2 = sqrt(2.);
    data[0 * 2 + 0] = init_complex(0., exp(-frac_theta_2));
    data[0 * 2 + 1] = init_complex(0., 0.);
    data[1 * 2 + 0] = init_complex(0., 0.);
    data[1 * 2 + 1] = init_complex(0., exp(frac_theta_2));
    return data;
}

struct Complex **CX_gate(struct Complex **data)
{
    data[0 * 4 + 0] = init_complex(1., 0.);
    data[0 * 4 + 1] = init_complex(0., 0.);
    data[0 * 4 + 2] = init_complex(0., 0.);
    data[0 * 4 + 3] = init_complex(0., 0.);
    
    data[1 * 4 + 0] = init_complex(0., 0.);
    data[1 * 4 + 1] = init_complex(1., 0.);
    data[1 * 4 + 2] = init_complex(0., 0.);
    data[1 * 4 + 3] = init_complex(0., 0.);

    data[2 * 4 + 0] = init_complex(0., 0.);
    data[2 * 4 + 1] = init_complex(0., 0.);
    data[2 * 4 + 2] = init_complex(0., 0.);
    data[2 * 4 + 3] = init_complex(1., 0.);

    data[3 * 4 + 0] = init_complex(0., 0.);
    data[3 * 4 + 1] = init_complex(0., 0.);
    data[3 * 4 + 2] = init_complex(1., 0.);
    data[3 * 4 + 3] = init_complex(0., 0.);

    return data;
}

struct Complex **CZ_gate(struct Complex **data)
{
    data[0 * 4 + 0] = init_complex(1., 0.);
    data[0 * 4 + 1] = init_complex(0., 0.);
    data[0 * 4 + 2] = init_complex(0., 0.);
    data[0 * 4 + 3] = init_complex(0., 0.);
    
    data[1 * 4 + 0] = init_complex(0., 0.);
    data[1 * 4 + 1] = init_complex(1., 0.);
    data[1 * 4 + 2] = init_complex(0., 0.);
    data[1 * 4 + 3] = init_complex(0., 0.);

    data[2 * 4 + 0] = init_complex(0., 0.);
    data[2 * 4 + 1] = init_complex(0., 0.);
    data[2 * 4 + 2] = init_complex(1., 0.);
    data[2 * 4 + 3] = init_complex(0., 0.);

    data[3 * 4 + 0] = init_complex(0., 0.);
    data[3 * 4 + 1] = init_complex(0., 0.);
    data[3 * 4 + 2] = init_complex(0., 0.);
    data[3 * 4 + 3] = init_complex(-1., 0.);

    return data;
}

struct Gate *get_nqubits_from_gate_id(struct Gate* gate)
{
    unsigned char nqubits = 0;
    switch (gate->id)
    {
    case I:
    case X:
    case Y:
    case Z:
    case H:
    case RX:
    case RY:
    case RZ:
        nqubits =  1;
        break;
    case CX:
    case CZ:
        nqubits = 2;
        break;
    default:
        break;
    }

    gate->nqubits = nqubits;
    return gate;   
    
}

struct Gate *get_data_from_gate_id(struct Gate *gate, double *theta)
{
    size_t dim = 1 << gate->nqubits;
    struct Complex **data = xmalloc(dim * dim * sizeof(struct Complex*));

    switch (gate->id)
    {
    case I:
        data = I_gate(data);
        break;
    case X:
        data = X_gate(data);
        break;
    case Y:
        data = Y_gate(data);
        break;
    case Z:
        data = Z_gate(data);
        break;
    case H:
        data = H_gate(data);
        break;
    case RX:
        data = RX_gate(data, theta);
        break;
    case RY:
        data = RY_gate(data, theta);
        break;
    case RZ:
        data = RZ_gate(data, theta);
        break;
    case CX:
        data = CX_gate(data);
        break;
    case CZ:
        data = CZ_gate(data);
        break;
    default:
        return NULL;
    }
    gate->data = data;
    return gate;
}

struct Gate *init_gate(enum GATE id, double *alpha)
{
    struct Gate *gate = xmalloc(sizeof(struct Gate));
    gate->id = id;
    gate = get_nqubits_from_gate_id(gate);
    gate = get_data_from_gate_id(gate, alpha);
    return gate;
}

void free_gate(struct Gate *gate)
{
    size_t dim = 1 << gate->nqubits;
    for (size_t i = 0; i < dim * dim; ++i)
    {

        free_complex(gate->data[i]);
    }
    free(gate->data);
    free(gate);
}

char *gate_id_to_str(enum GATE id)
{
    char *str = "";
    switch (id)
    {
    case I:
        str = "I";
        break;
    case X:
        str = "X";
        break;
    case Y:
        str = "Y";
        break;
    case Z:
        str = "Z";
        break;
    case H:
        str = "H";
        break;
    case RX:
        str = "RX";
        break;
    case RY:
        str = "RY";
        break;
    case RZ:
        str = "RZ";
        break;
    case CX:
        str = "CX";
        break;
    case CZ:
        str = "CZ";
        break;
    default:
        break;
    }

    return str;
}

void print_gate(struct Gate *gate)
{
    printf("%s gate:\n", gate_id_to_str(gate->id));
    size_t dim = 1 << gate->nqubits;
    for (size_t idx = 0; idx < dim * dim; ++idx)
    {
        size_t j = idx % dim;

        if (j == 0)
        {
            printf("\t[ ");
        }
        print_complex(gate->data[idx]);

        if (j < dim - 1)
        {
            printf(", ");
        }
        else
        {
            printf(" ]\n");
        }
    }
}