#extension GL_EXT_gpu_shader4 : enable
#define norm_eps 0.00001
#define pi 3.1415926

uniform float global_amp_scale;
uniform float global_pos_scale;
uniform int planet_seed;
uniform int noise_octaves;

varying float intensity;
varying float height;

vec3 roty(vec3 v, float angle) {
    return mat3(cos(angle),0.0,-sin(angle),0.0,1.0,0.0,sin(angle),0.0,cos(angle))*v;
}

vec3 rotx(vec3 v, float angle) {
    return mat3(1.0,0.0,0.0,0.0,cos(angle),sin(angle),0.0,-sin(angle),cos(angle))*v;
}

float curve(float x) {
    return x*x*x*(x*(6.0*x-15.0) + 10.0);
}

/* Value noise implementation
float randomValue(int x, int y, int z, int seed) {
    int n = x * 57 + y * 13 + z + seed;
    n = (n << 13) ^ n;
    return 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

float interpolatedValue(vec3 pos, int seed) {
    vec3 posScaled = global_pos_scale * pos;

    int px = int(posScaled.x);
    int py = int(posScaled.y);
    int pz = int(posScaled.z);

    float fx = fract(posScaled.x);
    float fy = fract(posScaled.y);
    float fz = fract(posScaled.z);

    float cx = curve(fx);
    float cy = curve(fy);
    float cz = curve(fz);

    float v_1 = randomValue(px, py, pz, seed);
    float v_2 = randomValue(px, py, pz+1, seed);
    float v_3 = randomValue(px, py+1, pz, seed);
    float v_4 = randomValue(px, py+1, pz+1, seed);
    float v_5 = randomValue(px+1, py, pz, seed);
    float v_6 = randomValue(px+1, py, pz+1, seed);
    float v_7 = randomValue(px+1, py+1, pz, seed);
    float v_8 = randomValue(px+1, py+1, pz+1, seed);

    float tv_1 = mix(v_1, v_2, cz);
    float tv_2 = mix(v_3, v_4, cz);
    float t_3 = mix(tv_1, tv_2, cy);

    tv_1 = mix(v_5, v_6, cz);
    tv_2 = mix(v_7, v_8, cz);
    float t_4 = mix(tv_1, tv_2, cy);

    return mix(t_3, t_4, cx);
}
*/

vec3 randomVector(int x, int y, int z, int seed) {
    x = 57 * y + 13 * z + x;
    y = 47 * z + 19 * x + y;
    z = 79 * x + 23 * y + z;
    x = (x << 13) ^ (x + seed);
    y = (y << 13) ^ (y + seed);
    z = (z << 13) ^ (z + seed);

    vec3 random =  vec3( ( 1.0 - float( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0),
                         ( 1.0 - float( (y * (y * y * 18637 + 829463) + 1265124733) & 0x7fffffff) / 1073741824.0),
                         ( 1.0 - float( (z * (z * z * 14737 + 758629) + 1220927501) & 0x7fffffff) / 1073741824.0));

    return normalize(random);
}

float interpolatedNoise(vec3 pos, int seed) {
    vec3 posScaled = global_pos_scale * pos;

    int px = int(posScaled.x);
    int py = int(posScaled.y);
    int pz = int(posScaled.z);

    float fx = fract(posScaled.x);
    float fy = fract(posScaled.y);
    float fz = fract(posScaled.z);

    float cx = curve(fx);
    float cy = curve(fy);
    float cz = curve(fz);

    vec3 v_1 = randomVector(px, py, pz, seed);
    vec3 v_2 = randomVector(px, py, pz+1, seed);
    vec3 v_3 = randomVector(px, py+1, pz, seed);
    vec3 v_4 = randomVector(px, py+1, pz+1, seed);
    vec3 v_5 = randomVector(px+1, py, pz, seed);
    vec3 v_6 = randomVector(px+1, py, pz+1, seed);
    vec3 v_7 = randomVector(px+1, py+1, pz, seed);
    vec3 v_8 = randomVector(px+1, py+1, pz+1, seed);

    float tv_1 = mix(dot(vec3(fx, fy, fz), v_1), dot(vec3(fx, fy, fz-1.0), v_2), cz);
    float tv_2 = mix(dot(vec3(fx, fy-1.0, fz), v_3), dot(vec3(fx, fy-1.0, fz-1.0), v_4), cz);
    float t_3 = mix(tv_1, tv_2, cy);

    tv_1 = mix(dot(vec3(fx-1.0, fy, fz), v_5), dot(vec3(fx-1.0, fy, fz-1.0), v_6), cz);
    tv_2 = mix(dot(vec3(fx-1.0, fy-1.0, fz), v_7), dot(vec3(fx-1.0, fy-1.0, fz-1.0), v_8), cz);
    float t_4 = mix(tv_1, tv_2, cy);

    return mix(t_3, t_4, cx);
}

float perlinNoise(vec3 pos, int seed, int octaves) {
    pos += vec3(1.0,1.0,1.0);
    float offset = 0.0;
    float frequency, amplitude;
    for (int i = 0; i < octaves; i++) {
        frequency = float(2 << i);
        amplitude = 1.0 / sqrt(frequency);
        offset += interpolatedNoise(frequency * pos, seed) * amplitude;
    }
    return offset;
}

vec3 perturbedNormal(vec3 pos, vec3 norm, float disp, int seed, int octaves) {
    float u = mod(gl_MultiTexCoord0.s+0.25,1.0);
    float v = 1.0-gl_MultiTexCoord0.t;

    float theta = u * 2.0 * pi;
    float phi = v * pi;

    vec3 ortho_1 = roty(rotx(vec3(-1.0,0.0,0.0),phi),theta);
    vec3 ortho_2 = roty(rotx(vec3(0.0,0.0,1.0),phi),theta);

    vec3 eps_o1 = norm_eps * ortho_1;
    vec3 eps_o2 = norm_eps * ortho_2;

    float n_0 = disp;
    float n_1 = global_amp_scale * perlinNoise(pos+eps_o1,seed,octaves);
    float n_2 = global_amp_scale * perlinNoise(pos-eps_o1,seed,octaves);
    float n_3 = global_amp_scale * perlinNoise(pos+eps_o2,seed,octaves);
    float n_4 = global_amp_scale * perlinNoise(pos-eps_o2,seed,octaves);

    vec3 v_1 = eps_o1+(n_1-n_0)*norm;
    vec3 v_2 = -eps_o1+(n_2-n_0)*norm;
    vec3 v_3 = eps_o2+(n_3-n_0)*norm;
    vec3 v_4 = -eps_o2+(n_4-n_0)*norm;

    return normalize(cross(v_1,v_3)+cross(v_3,v_2)+cross(v_2,v_4)+cross(v_4,v_1));
}

void main() {
    height = global_amp_scale * perlinNoise(gl_Vertex.xyz, planet_seed, noise_octaves);
    vec3 perturbedVertex = gl_Vertex.xyz + height * gl_Normal;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(perturbedVertex, 1);
    vec3 normal = normalize(gl_NormalMatrix * perturbedNormal(gl_Vertex, gl_Normal, height, planet_seed, noise_octaves)).xyz;
    vec3 light = normalize(gl_LightSource[0].position -
                           gl_ModelViewMatrix * vec4(perturbedVertex, 1)).xyz;
    intensity = max(0.0, dot(normal, light));

    gl_TexCoord[0] = gl_MultiTexCoord0;
}

