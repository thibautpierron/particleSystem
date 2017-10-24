#ifndef MATH3D_H
# define MATH3D_H

# include <math.h>

typedef struct      s_vertex
{
    float           position[4];
    float           color[4];
}                   t_vertex;

typedef struct      s_matrix
{
    float           m[16];
}                   t_matrix;


float               degToRad(float deg);
float               radToDeg(float rad);

t_matrix            multiplyMatrix(t_matrix *m1, t_matrix *m2);
void                scaleMatrix(t_matrix *m1, float x, float y, float z);
void                translateMatrix(t_matrix *m1, float x, float y, float z);
void                rotateMatrixOnX(t_matrix *m1, float angle);
void                rotateMatrixOnY(t_matrix *m1, float angle);
void                rotateMatrixOnZ(t_matrix *m1, float angle);
t_matrix            createProjectionMatrix(float fov, float ratio, float nearPlane, float farPlane);

#endif