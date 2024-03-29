#include "SystemGenerator.h"

#define MIN_ORB_RADIUS 8.0
#define MIN_PL_RADIUS 0.4
#define MAX_PL_RADIUS 1.6
#define MIN_SUN_RADIUS 3.0
#define MAX_SUN_RADIUS 5.5
#define MIN_NUM_PLANETS 4
#define MAX_NUM_PLANETS 10

#define NUM_LOWTEX 6
#define NUM_MLTEX 7
#define NUM_MHTEX 3
#define NUM_HITEX 2

const char *lowtex[] = {
    "../CS123-Final-Project/textures/unsorted/Rust0141_7_S.jpg",
    "../CS123-Final-Project/textures/unsorted/Rust0143_15_S.jpg",
    "../CS123-Final-Project/textures/unsorted/SoilSand0181_23_S.jpg",
    "../CS123-Final-Project/textures/unsorted/SoilSand0182_11_S.jpg",
    "../CS123-Final-Project/textures/unsorted/SoilCracked0157_7_M.jpg",
    "/course/cs123/data/image/terrain/rock.JPG"
};

const char *medlowtex[] = {
    "../CS123-Final-Project/textures/unsorted/SoilSand0181_23_S.jpg",
    "../CS123-Final-Project/textures/unsorted/SoilSand0182_11_S.jpg",
    "../CS123-Final-Project/textures/unsorted/SoilCracked0157_7_M.jpg",
    // "../CS123-Final-Project/textures/unsorted/Grass0126_2_S.jpg",
    "../CS123-Final-Project/textures/unsorted/grass_genetica.jpg",
    "../CS123-Final-Project/textures/unsorted/grass_genetica_2.jpg",
    "../CS123-Final-Project/textures/unsorted/NatureForests0009_2_S.jpg",
    "../CS123-Final-Project/textures/unsorted/NatureForests0072_12_S.jpg",
    "/course/cs123/data/image/terrain/grass.JPG"
};

const char *medhitex[] = {
    "../CS123-Final-Project/textures/unsorted/Cliffs0143_9_S.jpg",
    "../CS123-Final-Project/textures/unsorted/Cliffs0199_189_S.jpg",
    "/course/cs123/data/image/terrain/dirt.JPG"
};

const char *hitex[] = {
    "../CS123-Final-Project/textures/unsorted/Snow0041_5_S.jpg",
    "../CS123-Final-Project/textures/unsorted/Ice0044_42_S.jpg"
};

int randi(int min, int max) {
    return rand()%(max - min)+min;
}

float randf(float min, float max) {
    float r = (float)rand()/(float)RAND_MAX;
    return r*(max-min)+min;
}

SystemGenerator::SystemGenerator() {}

SystemGenerator::~SystemGenerator() {}

QList<Planet> SystemGenerator::generate() {
    srand(time(NULL));
    int num_planets = randi(MIN_NUM_PLANETS,MAX_NUM_PLANETS);
    QList<float> pl_radius = QList<float>();
    for (int i = 0; i < num_planets; i++) {
        pl_radius.push_back(randf(MIN_PL_RADIUS, MAX_PL_RADIUS));
    }
    QList<float> orb_radius = QList<float>();
    float r = MIN_ORB_RADIUS + randf(0.0,5.0);
    for (int i = 0; i < num_planets; i++) {
        r += pl_radius[i];
        orb_radius.push_back(r);
        r += pl_radius[i] + randf(0.0,5.0);
    }
    QList<Planet> planets = QList<Planet>();
    Vector3 v;
    for (int i = 0; i < num_planets + 1; i++) {
        if (i == 0) { // sun
            Planet sun;
            sun.setDetail(LOW);
            sun.setRadius(randf(MIN_SUN_RADIUS,MAX_SUN_RADIUS));
            // may want different textures depending on size at some point
            sun.setTexture("../CS123-Final-Project/textures/SunTexture.png", 0);
            sun.setIsStar(true);
            planets.push_back(sun);
        } else { // not sun
            int idx = i-1;
            bool far = orb_radius[idx] > 30;
            bool water = far ? false : randi(0,2);

            Planet planet;
            planet.setDetail(LOW);
            planet.setOrbit(Orbit(orb_radius[idx], randf(-M_PI/8.0,M_PI/8.0), orb_radius[idx]));
            planet.setRadius(pl_radius[idx]);
            v = Vector3(randf(-0.2,0.2),1.0,randf(-0.2,0.2));
            v.normalize();
            planet.setAxis(v);
            planet.setAxialRotation(randf(0,M_2PI));
            planet.setOrbitalRotation(randf(0,M_2PI));
            planet.setHasWater(water);
            planet.setTerrainAmplitude(randf(.02, .1));

            // lol textchers
            planet.setTexture((water ? "../CS123-Final-Project/textures/unsorted/WaterPlain0012_2_S.jpg" : lowtex[randi(0,NUM_LOWTEX)]), 0);
            planet.setTexture(medlowtex[randi(0,NUM_MLTEX)], 1);
            planet.setTexture(medhitex[randi(0,NUM_MHTEX)], 2);
            planet.setTexture(hitex[randi(0,NUM_HITEX)], 3);
            planets.push_back(planet);
        }
    }
    return planets;
}
