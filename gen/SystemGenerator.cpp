#include "SystemGenerator.h"

#define MIN_ORB_RADIUS 10.0
#define MAX_ORB_RADIUS 100.0
#define MIN_PL_RADIUS 0.6
#define MAX_PL_RADIUS 2.0
#define MIN_SUN_RADIUS 3.0
#define MAX_SUN_RADIUS 6.0
#define MIN_NUM_PLANETS 4
#define MAX_NUM_PLANETS 12

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
    QList<float> axial_rot = QList<float>();
    QList<float> orb_rot = QList<float>();
    QList<float> orb_tilt = QList<float>();
    QList<Vector3> axis = QList<Vector3>();
    Vector3 v;
    for (int i = 0; i < num_planets; i++) {
        pl_radius.push_back(randf(MIN_PL_RADIUS, MAX_PL_RADIUS));
        axial_rot.push_back(randf(0,M_2PI));
        orb_rot.push_back(randf(0,M_2PI));
        orb_tilt.push_back(randf(-M_PI/8.0,M_PI/8.0));
        v = Vector3(randf(-0.2,0.2),1.0,randf(-0.2,0.2));
        v = v.normalize();
        axis.push_back(v);
    }
    QList<float> orb_radius = QList<float>();
    for (int i = 0; i < num_planets; i++) {
        bool good = true;
        float r;
        do {
            r = randf(MIN_ORB_RADIUS,MAX_ORB_RADIUS);
            for (int j = 0; j < orb_radius.size(); j++) {
                good = !(pl_radius[i] + r > pl_radius[j] - orb_radius[j] && pl_radius[i] - r < pl_radius[j] + orb_radius[j]);
            }
        } while (!good);
        push_back(r);
    }
    QList<Planet> planets = QList<Planet>();
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
            Planet planet;
            planet.setDetail(LOW);
            planet.setOrbit(Orbit(orb_radius[idx], orb_tilt[idx], orb_radius[idx]));
            planet.setRadius(pl_radius[idx]);
            planet.setAxis(axis[idx]);
            planet.setAxialRotation(axial_rot[idx]);
            planet.setOrbitalRotation(orb_rot[idx]);
            // lol textchers
            planet.setTexture("/course/cs123/data/image/terrain/grass.JPG", 0);
            planet.setTexture("/course/cs123/data/image/terrain/dirt.JPG", 1);
            planet.setTexture("/course/cs123/data/image/terrain/snow.JPG", 2);
            planet.setTexture("/course/cs123/data/image/terrain/rock.JPG", 3);
            planets.push_back(planet);
        }
    }
}
