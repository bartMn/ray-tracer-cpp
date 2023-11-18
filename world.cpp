// In your cpp file, say World.cpp
#include "World.h"
#include "Material.h"


Ray World::compute_reflected_ray(Ray& r, HitRecord& rec) {
    vec3 reflected_direction = reflect(r.get_normalized(), rec.normal);
    vec3 reflected_origin = rec.p + 0.01 * rec.normal; // Add a small epsilon to avoid self-intersection
    return Ray(reflected_origin, reflected_direction, vec3(255, 255, 255), r.getDepth() + 1);
}

vec3 World::reflect(const vec3& v, const vec3& normal) {
    return v - 2 * vec3::dot(v, normal) * normal;
}

bool World::hit(Ray& r, double t_min, double t_max, HitRecord& rec, int depth) {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;

            // Lambertian shading (replace this with your shading model)
            vec3 shading_diffuse = temp_rec.material.getDiffuseColor();

            // Initialize specular color
            vec3 shading_specular(0, 0, 0);

            closest_so_far = temp_rec.t;

            // Include the recursive call for reflections
            if (depth < maxBounces) {
                Ray reflected_ray = compute_reflected_ray(r, temp_rec);
                HitRecord reflected_rec;
                if (hit(reflected_ray, t_min, t_max, reflected_rec, depth + 1)) {
                    // Combine specular reflection with shading
                    shading_specular = reflected_rec.material.getSpecularColor();
                }
            }

            r.setColor(r.getColor() + shading_diffuse + shading_specular);
            rec = temp_rec;
        }
    }

    return hit_anything;
}



void World::createAndAddSphere(const nlohmann::json& jsonInput){    
    
    vec3 position = vec3(jsonInput["center"][0],
                         jsonInput["center"][1],
                         jsonInput["center"][2]);
    double radius = jsonInput["radius"];

    Sphere newSphere(position, radius);
    if (jsonInput.contains("material"))
    {
        newSphere.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
    }

    World::addHittable(std::make_shared<Sphere>(newSphere));
}

void World::createAndAddTriangle(const nlohmann::json& jsonInput)//vec3 vertex1, vec3 vertex2, vec3 vertex3)
{

    vec3 vertex1 = vec3(jsonInput["v0"][0],
                        jsonInput["v0"][1],
                        jsonInput["v0"][2]);

    vec3 vertex2 = vec3(jsonInput["v1"][0],
                        jsonInput["v1"][1],
                        jsonInput["v1"][2]);

    vec3 vertex3 = vec3(jsonInput["v2"][0],
                        jsonInput["v2"][1],
                        jsonInput["v2"][2]);    

    Triangle newTriangle(vertex1,
                         vertex2,
                         vertex3);

    if (jsonInput.contains("material"))
    {
        newTriangle.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
    }
                         
    World::addHittable(std::make_shared<Triangle>(newTriangle));
}

void World::createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3)
{    Triangle newTriangle(vertex1,
                          vertex2,
                          vertex3);
    World::addHittable(std::make_shared<Triangle>(newTriangle));
}



void World::createAndAddCylinder(const nlohmann::json& jsonInput)//vec3 bottomCenter, double radius, double height, vec3 normalVector)
{
    double height = jsonInput["height"];
    double radius = jsonInput["radius"];    

    vec3 normalVector = vec3(jsonInput["axis"][0],
                             jsonInput["axis"][1],
                             jsonInput["axis"][2]).return_unit();

    vec3 bottomCenter = vec3(jsonInput["center"][0],
                             jsonInput["center"][1],
                             jsonInput["center"][2]) - normalVector*(0.5*height);


    vec3 upperCenter = bottomCenter + normalVector*(0.5*height);
    Circle topCircle (upperCenter, radius, normalVector);
    Circle bottomCircle(bottomCenter, radius, normalVector*(-1));

    Cylinder cylinder(bottomCenter, //center of the bottom circle 
                      radius,             //radious
                      height,            //height
                      normalVector);


    if (jsonInput.contains("material"))
    {
        cylinder.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
        topCircle.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
        bottomCircle.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
    }

    World::addHittable(std::make_shared<Cylinder>(cylinder));
    World::addHittable(std::make_shared<Circle>(topCircle));
    World::addHittable(std::make_shared<Circle>(bottomCircle));
}

void World::createAndAddFloor(vec3 floorCenter, double floorSize){

    double halfSize = floorSize/2;
    createAndAddTriangle(floorCenter + vec3(-halfSize, -halfSize, 0),
                         floorCenter + vec3(halfSize, -halfSize, 0),
                         floorCenter + vec3(halfSize, halfSize, 0));
    createAndAddTriangle(floorCenter + vec3(-halfSize, -halfSize, 0),
                         floorCenter + vec3(halfSize, halfSize, 0),
                         floorCenter + vec3(-halfSize, halfSize, 0));
}


void World::loadScene(const std::string& filename, Camera& camera) {
    std::ifstream file(filename);
    nlohmann::json sceneJson;
    file >> sceneJson;

    // Extract camera information
    objects.clear();
    camera.setupFromJson(sceneJson["camera"], sceneJson["rendermode"]);
    
    if (sceneJson.contains("nbounces"))
    {
        maxBounces = int(sceneJson["nbounces"]);
    }
    else{
        maxBounces = 0;
    }

    std::cout << maxBounces <<std::endl;
    // Extract world information
    const nlohmann::json& sceneInfo = sceneJson["scene"];
    //world.setBackgroundColor(sceneInfo["backgroundcolor"]);

    // Extract shapes information
    const nlohmann::json& shapesInfo = sceneInfo["shapes"];
    for (const auto& shapeInfo : shapesInfo) {
        std::string type = shapeInfo["type"];
        if (type == "sphere") {
            createAndAddSphere(shapeInfo);
        } else if (type == "cylinder") {
            createAndAddCylinder(shapeInfo);
        } else if (type == "triangle") {
            createAndAddTriangle(shapeInfo);
        }
        // Add more shape types as needed
    }
}

