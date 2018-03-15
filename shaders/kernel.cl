float get_random_0_1_ul(unsigned int* seed)
{
  *seed = ((*seed) * 16807 ) % 2147483647;
  return  (float)(*seed) * 4.6566129e-10;
}

__kernel void clinit(float deltaTime, float4 cursor, __global float4 *lpos, __global float4 *lvel)
{
  int global_id = get_global_id(0);
  unsigned int seed = (unsigned int)global_id;
  float4 pos = lpos[global_id];
  float4 vel = lvel[global_id];

  if (cursor.w == 0.0f){
    float radius = 1.0f;
    float x = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
    float y = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
    float z = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;

    int iter_max = 10;
    while ((x * x) + (y * y) + (z * z) > (radius * radius) && iter_max > -1){
      x = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      y = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      z = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      iter_max = iter_max - 1;
    }
    pos.x = x;
    pos.y = y;
    pos.z = z;
  } else {
    float radius = 6.0f;
    float x = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
    float y = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
    float z = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;

    int iter_max = 10;
    while ((x * x) + (y * y) + (z * z) > (radius * radius) && iter_max > -1){
      x = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      y = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      z = ((float)get_random_0_1_ul(&seed) * 2.0f) - 1.0f;
      iter_max = iter_max - 1;
    }
    pos.x = x;
    pos.y = y;
    pos.z = z;
  }

  pos.w = 0.0f;
  vel.x = 0.0f;
  vel.y = 0.0f;
  vel.z = 0.0f;
  vel.w = 0.0f;
  lpos[global_id] = pos;
  lvel[global_id] = vel;
}

__kernel void clpart(float deltaTime, float4 cursor, __global float4 *lpos, __global float4 *lvel)
{
  int global_id = get_global_id(0);
  float4 pos = lpos[global_id];
  float4 vel = lvel[global_id];

  if (cursor.w != -1.0f) {
    float m = 1.0f;
    pos.w = 0.0f;
    float4 force = cursor - pos;
    force = normalize(force);
    vel += force * deltaTime;
    pos += vel;
  }
  lpos[global_id] = pos;
  lvel[global_id] = vel;
}
