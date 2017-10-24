#include "math3d.hpp"

const t_matrix IDENTITY_MATRIX = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
const t_matrix NULL_MATRIX = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

float       degToRad(float deg)
{
    return deg * (M_PI / 180);
}

float       radToDeg(float rad)
{
    return rad * (180 / M_PI);
}

void	*ft_memcpy(void *dst, const void *src, int n)
{
	int	i;
	char	*dst2;
	char	*src2;

	dst2 = (char *)dst;
	src2 = (char *)src;
	i = 0;
	while (i < n)
	{
		*dst2 = *src2;
		dst2++;
		src2++;
		i++;
	}
	return (dst);
}

t_matrix    multiplyMatrix(t_matrix *m1, t_matrix *m2)
{
    t_matrix ret = IDENTITY_MATRIX;
    int i;
    int j;

    i = 0;
    while (i < 4)
    {
        j = 0;
        while (j < 4)
        {
            ret.m[i * 4 + j] = 
            (m1->m[i * 4] * m2->m[j + 0]) +
            (m1->m[i * 4 + 1] * m2->m[j + 4]) +
            (m1->m[i * 4 + 2] * m2->m[j + 8]) +
            (m1->m[i * 4 + 3] * m2->m[j + 12]);
            j++; 
        }
        i++;
    }
    return ret;
}

void        scaleMatrix(t_matrix *m, float x, float y, float z)
{
    t_matrix scale = IDENTITY_MATRIX;

    scale.m[0] = x;
    scale.m[5] = y;
    scale.m[10] = z;

    ft_memcpy(m->m, multiplyMatrix(m, &scale).m, sizeof(m->m));
}

void        translateMatrix(t_matrix *m, float x, float y, float z)
{
    t_matrix translation = IDENTITY_MATRIX;

    translation.m[12] = x;
    translation.m[13] = y;
    translation.m[14] = z;

    ft_memcpy(m->m, multiplyMatrix(m, &translation).m, sizeof(m->m));
}

void        rotateMatrixOnX(t_matrix *m, float angle)
{
    t_matrix rotation = IDENTITY_MATRIX;
    float sinus = sin(angle);
    float cosinus = cos(angle);

    rotation.m[5] = cosinus;
    rotation.m[6] = -sinus;
    rotation.m[9] = sinus;
    rotation.m[10] = cosinus;

    ft_memcpy(m->m, multiplyMatrix(m, &rotation).m, sizeof(m->m));
}

void        rotateMatrixOnY(t_matrix *m, float angle)
{
    t_matrix rotation = IDENTITY_MATRIX;
    float sinus = sin(angle);
    float cosinus = cos(angle);

    rotation.m[0] = cosinus;
    rotation.m[8] = sinus;
    rotation.m[2] = -sinus;
    rotation.m[10] = cosinus;

    ft_memcpy(m->m, multiplyMatrix(m, &rotation).m, sizeof(m->m));
}

void        rotateMatrixOnZ(t_matrix *m, float angle)
{
    t_matrix rotation = IDENTITY_MATRIX;
    float sinus = sin(angle);
    float cosinus = cos(angle);

    rotation.m[0] = cosinus;
    rotation.m[1] = -sinus;
    rotation.m[4] = sinus;
    rotation.m[5] = cosinus;

    ft_memcpy(m->m, multiplyMatrix(m, &rotation).m, sizeof(m->m));
}

t_matrix createProjectionMatrix(float fov, float ratio, float nearPlane, float farPlane)
{
    t_matrix ret;
    float yScale;
    float xScale;
    float frustrum;
    
    ret = NULL_MATRIX;
    yScale = 1.0f / tan(degToRad(fov / 2));
    xScale = yScale / ratio;
    frustrum = farPlane - nearPlane;

    ret.m[0] = xScale;
    ret.m[5] = yScale;
    ret.m[10] = -((farPlane + nearPlane) / frustrum);
    ret.m[11] = -1;
    ret.m[14] = -((2 * farPlane * nearPlane) / frustrum);
    return ret;
}