#extension GL_EXT_gpu_shader4 : enable

float curve(float x) {
    return x * x * (3.0 - 2.0*x);
}

vec3 randomVector(int x, int y, int z, int seed) {
    x = (x << 13) ^ (x + seed);
    y = (y << 7) ^ (y + seed);
    z = (z << 11) ^ (z + seed);

    vec3 random =  vec3( ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0),
                         ( 1.0 - ( (y * (y * y * 18637 + 829463) + 1265124733) & 0x7fffffff) / 1073741824.0),
                         ( 1.0 - ( (z * (z * z * 14737 + 758629) + 1220927501) & 0x7fffffff) / 1073741824.0));

    return normalize(random);
}

float interpolatedNoise(vec3 pos, int seed) {
    vec3 posScaled = 2.0 * (pos+vec3(0.5,0.5,0.5));

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

    float tv_1 = mix(dot(vec3(fx, fy, fz), v_1), dot(vec3(fx, fy, fz-1), v_2), cz);
    float tv_2 = mix(dot(vec3(fx, fy-1, fz), v_3), dot(vec3(fx, fy-1, fz-1), v_4), cz);
    float t_3 = mix(tv_1, tv_2, cy);

    tv_1 = mix(dot(vec3(fx-1, fy, fz), v_5), dot(vec3(fx-1, fy, fz-1), v_6), cz);
    tv_2 = mix(dot(vec3(fx-1, fy-1, fz), v_7), dot(vec3(fx-1, fy-1, fz-1), v_8), cz);
    float t_4 = mix(tv_1, tv_2, cy);

    return mix(t_3, t_4, cx);
}

float perlinNoise(vec3 pos, int seed, int octaves) {
    float offset = 0;
    float frequency, amplitude;
    for (int i = 0; i < octaves; i++) {
        frequency = 2 << i;
        amplitude = 1 / sqrt(frequency);
        offset += interpolatedNoise(frequency * pos, seed) * amplitude;
    }

    return offset;
}

void main() {
    vec3 perturbedVertex = gl_Vertex.xyz + .05 * perlinNoise(gl_Vertex.xyz, 12345, 10) * gl_Normal;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(perturbedVertex, 1);
}

