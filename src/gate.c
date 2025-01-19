#include "headers/gate.h"
#include "headers/alloc.h"

#include <err.h>
#include <math.h>
#include <stdio.h>

double complex *ID_gate(double complex *data)
{
    data[0 * 2 + 0] = 1;
    data[0 * 2 + 1] = 0;
    data[1 * 2 + 0] = 0;
    data[1 * 2 + 1] = 1;
    return data;
}

double complex *X_gate(double complex *data)
{
    data[0 * 2 + 0] = 0;
    data[0 * 2 + 1] = 1;
    data[1 * 2 + 0] = 1;
    data[1 * 2 + 1] = 0;
    return data;
}

double complex *Y_gate(double complex *data)
{
    data[0 * 2 + 0] = 0;
    data[0 * 2 + 1] = -1 * I;
    data[1 * 2 + 0] = I;
    data[1 * 2 + 1] = 0;
    return data;
}

double complex *Z_gate(double complex *data)
{
    data[0 * 2 + 0] = 1;
    data[0 * 2 + 1] = 0;
    data[1 * 2 + 0] = 0;
    data[1 * 2 + 1] = -1;
    return data;
}

double complex *H_gate(double complex *data)
{
    double FRAC_SQRT_2 = 1/sqrt(2.);
    data[0 * 2 + 0] = FRAC_SQRT_2;
    data[0 * 2 + 1] = FRAC_SQRT_2;
    data[1 * 2 + 0] = FRAC_SQRT_2;
    data[1 * 2 + 1] = -FRAC_SQRT_2;
    return data;
}

double complex *RX_gate(double complex *data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RX gate");
    }
    double frac_theta_2 = *theta / 2.;
    data[0 * 2 + 0] = cos(frac_theta_2);
    data[0 * 2 + 1] = -sin(frac_theta_2) * I;
    data[1 * 2 + 0] = -sin(frac_theta_2) * I;
    data[1 * 2 + 1] = cos(frac_theta_2);
    return data;
}

double complex *RY_gate(double complex *data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RY gate");
    }
    const double frac_theta_2 = *theta / 2.;
    data[0 * 2 + 0] = cos(frac_theta_2);
    data[0 * 2 + 1] = -sin(frac_theta_2);
    data[1 * 2 + 0] = sin(frac_theta_2);
    data[1 * 2 + 1] = cos(frac_theta_2);
    return data;
}

double complex *RZ_gate(double complex *data, double *theta)
{
    if (!theta)
    {
        err(MATH_ERREXCEPT, "Angle is not defined for RZ gate");
    }
    const double frac_theta_2 = sqrt(2.);
    data[0 * 2 + 0] = exp(-frac_theta_2) * I;
    data[0 * 2 + 1] = 0;
    data[1 * 2 + 0] = 0;
    data[1 * 2 + 1] = exp(frac_theta_2) * I;
    return data;
}

double complex *CX_gate(double complex *data)
{
    data[0 * 4 + 0] = 1;
    data[0 * 4 + 1] = 0;
    data[0 * 4 + 2] = 0;
    data[0 * 4 + 3] = 0;
    
    data[1 * 4 + 0] = 0;
    data[1 * 4 + 1] = 1;
    data[1 * 4 + 2] = 0;
    data[1 * 4 + 3] = 0;

    data[2 * 4 + 0] = 0;
    data[2 * 4 + 1] = 0;
    data[2 * 4 + 2] = 0;
    data[2 * 4 + 3] = 1;

    data[3 * 4 + 0] = 0;
    data[3 * 4 + 1] = 0;
    data[3 * 4 + 2] = 1;
    data[3 * 4 + 3] = 0;

    return data;
}

double complex *CZ_gate(double complex *data)
{
    data[0 * 4 + 0] = 1;
    data[0 * 4 + 1] = 0;
    data[0 * 4 + 2] = 0;
    data[0 * 4 + 3] = 0;
    
    data[1 * 4 + 0] = 0;
    data[1 * 4 + 1] = 1;
    data[1 * 4 + 2] = 0;
    data[1 * 4 + 3] = 0;

    data[2 * 4 + 0] = 0;
    data[2 * 4 + 1] = 0;
    data[2 * 4 + 2] = 1;
    data[2 * 4 + 3] = 0;

    data[3 * 4 + 0] = 0;
    data[3 * 4 + 1] = 0;
    data[3 * 4 + 2] = 0;
    data[3 * 4 + 3] = -1;

    return data;
}

double complex *CCX_gate(double complex *data)
{
    data[0 * 8 + 0] = 1;
    data[0 * 8 + 1] = 0;
    data[0 * 8 + 2] = 0;
    data[0 * 8 + 3] = 0;
    data[0 * 8 + 4] = 0;
    data[0 * 8 + 5] = 0;
    data[0 * 8 + 6] = 0;
    data[0 * 8 + 7] = 0;
    
    data[1 * 8 + 0] = 0;
    data[1 * 8 + 1] = 1;
    data[1 * 8 + 2] = 0;
    data[1 * 8 + 3] = 0;
    data[1 * 8 + 4] = 0;
    data[1 * 8 + 5] = 0;
    data[1 * 8 + 6] = 0;
    data[1 * 8 + 7] = 0;

    data[2 * 8 + 0] = 0;
    data[2 * 8 + 1] = 0;
    data[2 * 8 + 2] = 1;
    data[2 * 8 + 3] = 0;
    data[2 * 8 + 4] = 0;
    data[2 * 8 + 5] = 0;
    data[2 * 8 + 6] = 0;
    data[2 * 8 + 7] = 0;

    data[3 * 8 + 0] = 0;
    data[3 * 8 + 1] = 0;
    data[3 * 8 + 2] = 0;
    data[3 * 8 + 3] = 1;
    data[3 * 8 + 4] = 0;
    data[3 * 8 + 5] = 0;
    data[3 * 8 + 6] = 0;
    data[3 * 8 + 7] = 0;
    
    data[4 * 8 + 0] = 0;
    data[4 * 8 + 1] = 0;
    data[4 * 8 + 2] = 0;
    data[4 * 8 + 3] = 0;
    data[4 * 8 + 4] = 1;
    data[4 * 8 + 5] = 0;
    data[4 * 8 + 6] = 0;
    data[4 * 8 + 7] = 0;

    data[5 * 8 + 0] = 0;
    data[5 * 8 + 1] = 0;
    data[5 * 8 + 2] = 0;
    data[5 * 8 + 3] = 0;
    data[5 * 8 + 4] = 1;
    data[5 * 8 + 5] = 0;
    data[5 * 8 + 6] = 0;
    data[5 * 8 + 7] = 0;

    data[6 * 8 + 0] = 0;
    data[6 * 8 + 1] = 0;
    data[6 * 8 + 2] = 0;
    data[6 * 8 + 3] = 0;
    data[6 * 8 + 4] = 0;
    data[6 * 8 + 5] = 0;
    data[6 * 8 + 6] = 0;
    data[6 * 8 + 7] = 1;

    data[7 * 8 + 0] = 0;
    data[7 * 8 + 1] = 0;
    data[7 * 8 + 2] = 0;
    data[7 * 8 + 3] = 0;
    data[7 * 8 + 4] = 0;
    data[7 * 8 + 5] = 0;
    data[7 * 8 + 6] = 1;
    data[7 * 8 + 7] = 0;

    return data;
}

struct Gate *get_nqubits_from_gate_id(struct Gate* gate)
{
    unsigned char nqubits = 0;
    switch (gate->id)
    {
    case ID:
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
    case CCX:
        nqubits = 3;
    default:
        break;
    }

    gate->nqubits = nqubits;
    return gate;   
    
}

struct Gate *get_data_from_gate_id(struct Gate *gate, double *theta)
{
    size_t dim = 1 << gate->nqubits;
    double complex *data = xmalloc(dim * dim * sizeof(double complex));

    switch (gate->id)
    {
    case ID:
        data = ID_gate(data);
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
    case CCX:
        data = CCX_gate(data);
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
    free(gate->data);
    free(gate);
}

char *gate_id_to_str(enum GATE id)
{
    char *str = "";
    switch (id)
    {
    case ID:
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
    case CCX:
        str = "CCX";
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

        printf("%.1f%+.1fi", creal(gate->data[idx]), cimag(gate->data[idx]));
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