// generate float in 0-1 range
float rand(unsigned int* seed) {
  *seed = ((*seed) * 16807 ) % 2147483647;
  return  (float)(*seed) * 4.6566129e-10;
}

__kernel void clinit(float deltaTime, float4 cursor, __global float4 *lpos, __global float4 *lvel) {
  int global_id = get_global_id(0);
  unsigned int seed = (unsigned int)global_id;
  float4 pos = lpos[global_id];

  if (cursor.w == 0.0f) {
    // Sphere init
    float phi = rand(&seed) * (2.0f * M_PI);
    float costheta = (rand(&seed) * 2.0f) - 1.0f;
    float u = rand(&seed);

    float theta = acos(costheta);
    float r = 1.0f * cbrt(u);

    pos.x = r * sin(theta) * cos(phi);
    pos.y = r * sin(theta) * sin(phi);
    pos.z = r * cos(theta);
  } else {
    // Cube init
    pos.x = rand(&seed);
    pos.y = rand(&seed);
    pos.z = rand(&seed);
    pos = (pos * 2.0f) - 1.0f;
  }

  pos.w = 0.0f;
  lpos[global_id] = pos;
  lvel[global_id] = (float4)(0.0f);
}

__kernel void clpart(float deltaTime, float4 cursor, __global float4 *lpos, __global float4 *lvel) {
  int global_id = get_global_id(0);
  float4 pos = lpos[global_id];
  float4 vel = lvel[global_id];

  if (cursor.w != -1.0f) {
    pos.w = 0.0f;
    float4 force = normalize(cursor - pos);
    vel += force * deltaTime;
    pos += vel;
  }
  lpos[global_id] = pos;
  lvel[global_id] = vel;
}

__kernel void clemit(float deltaTime, float4 cursor, __global float4 *lpos, __global float4 *lvel) {
  int global_id = get_global_id(0);
  float4 pos = lpos[global_id];
  float4 vel = lvel[global_id];

  if (cursor.w != -1.0f) {
    pos.w = 0.0f;
    float4 force = normalize(cursor - pos);
    vel += force * deltaTime;
    pos += vel;
  }
  lpos[global_id] = pos;
  lvel[global_id] = vel;
}
