#pragma OPENCL EXTENSION cl_khr_gl_event : enable

__kernel void init_cube(__global float4* pos, __global float4* vel)
{
    const int idx = get_global_id(0);
    const int size = cbrt((float)get_global_size(0));

    const int step_x = idx % size;
    const int step_y = (idx / size) % size;
    const int step_z = idx / (size * size);

    pos[idx].x = (((float)((step_x) * 8.f) / size) - 4.f);
    pos[idx].y = (((float)((step_y) * 8.f) / size) - 4.f);
    pos[idx].z = (((float)((step_z) * 8.f) / size) - 4.f);

    vel[idx].x = 0;
    vel[idx].y = 0;
    vel[idx].z = 0;
}

__kernel void init_sphere(__global float4* pos, __global float4* vel)
{
    uint const		idx = get_global_id(0);
	uint const		global_size = get_global_size(0);

	float offset = 2.f / global_size;
    float k = 2.f;
    offset *= k;
	pos[idx].y = ((idx * offset * k) - 1.f * k) + (offset / 2.f);
	float r = sqrt(k * k - pow(pos[idx].y, 2));
	float phi = (idx % global_size) * M_PI * (2.f - sqrtf(5.f));
    pos[idx].x = cos(phi) * r;
    pos[idx].z = sin(phi) * r;
    pos[idx].w = 1.0;

    vel[idx].x = 0;
    vel[idx].y = 0;
    vel[idx].z = 0;
}

__kernel void init_line(__global float4* pos, __global float4* vel)
{
    uint const		idx = get_global_id(0);

    pos[idx].x = sin((float)idx) * 5;
    pos[idx].y = sin((float)idx) * 5;
    pos[idx].z = sin((float)idx) * 5;

    vel[idx].x = 0;
    vel[idx].y = 0;
    vel[idx].z = 0;
}

__kernel void gravity(__global float4* pos, __global float4* vel, float4 mouse)
{
    const int idx = get_global_id(0);
    float4 position = pos[idx];
    float4 velocity = vel[idx];

    float4 d = mouse - position;
    float dist = fast_distance(mouse, position);
    if (dist < 5)
       dist = 5;
    else if (dist > 10)
        dist = 10;

    float forceMagnitude = 0.5 / (dist * dist);

    float4 force = (d / dist) * forceMagnitude;

    vel[idx] += force;
    pos[idx] += velocity + force;
}