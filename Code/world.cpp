// In your cpp file, say World.cpp
#include "world.h"
#include "Material.h"

Ray World::compute_reflected_ray(Ray& r, HitRecord& rec) {
    vec3 reflected_direction = reflect(r.get_normalized(), rec.normal);
    vec3 reflected_origin = rec.p + 0.01 * rec.normal; // Add a small epsilon to avoid self-intersection
    return Ray(reflected_origin, reflected_direction, vec3(0, 0, 0), r.getDepth() + 1);
}

vec3 World::reflect(const vec3& v, const vec3& normal) {
    return (v - 2 * vec3::dot(v, normal) * normal).return_unit();
}

bool World::hit(Ray& r, double t_min, double t_max, HitRecord& rec, int depth) {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    int numOfNotReachingLights = 0;

    for (const auto& object : objects) 
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) 
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
        }
    }
    // Lambertian shading (replace this with your shading model)
    vec3 diffuse_colour = temp_rec.material.getDiffuseColor();
    vec3 ambient_part = diffuse_colour;
    // Initialize specular color
    vec3 collected_colour = vec3(0,0,0);
    vec3 colour_shading = vec3(0,0,0);
    float kd ; 
    float ks ;
    float specularexponent ; 

    vec3 diffuse_part ;
    vec3 specular_part ; 
    vec3 light ;
    // Include the recursive call for reflections
    
        

    // Combine specular reflection with shading
    //collected_colour += phongRayShading();
    for (const auto& lightSource : lightSources)
    {
        HitRecord temp_rec_shading;
        bool hit_anything_shading = false;
        double closest_so_far_shading = t_max;
        Ray r_shading(temp_rec.p, lightSource->getPosition(), vec3(0, 0, 0), 0);
        for (const auto& object : objects) 
        {   
            if (object->hit(r_shading, t_min, closest_so_far_shading, temp_rec_shading)) 
            {
                hit_anything_shading = true;
                numOfNotReachingLights += 1;
            }
        }

        if (hit_anything_shading) {continue;}
        
        vec3 normalLightVector = (lightSource->getPosition() - temp_rec.p).return_unit();
        vec3 normalViewVector = (camPtr->getPosition() - temp_rec.p).return_unit();
        vec3 normalReflectedVector = 2*vec3::dot(normalLightVector, temp_rec.normal)*temp_rec.normal - normalLightVector;
        kd = temp_rec.material.getKd(); 
        ks = temp_rec.material.getKs();
        specularexponent = temp_rec.material.getSpecularexponent(); 


        double diffuseDot = std::max(0.0, vec3::dot(normalLightVector, temp_rec.normal));
        double specularDot = std::max(0.0, vec3::dot(normalReflectedVector, normalViewVector));
        double expoResult = std::pow(specularDot, specularexponent);
        diffuse_part = kd * diffuseDot * temp_rec.material.getDiffuseColor() * lightSource->getLightColour();
        specular_part = ks * expoResult * temp_rec.material.getSpecularColor() * lightSource->getLightColour();
        //light = reflected_ray.getColor();
        colour_shading += diffuse_part + specular_part;
    }
                

    HitRecord reflected_rec;
    int numSamples;
    if (depth == 0) {numSamples = 15;}
    else if (depth < 2) {numSamples = 2;}
    else            {numSamples = 1;}
    // In your hit function:
    if (depth < maxBounces) {
        // Declare stddev and generator outside the loop
        //double stddev = 0.1; // Adjust this value as needed
        //std::default_random_engine generator;

        for (int i = 0; i < numSamples; ++i) {
            // Sample random direction on the hemispherendom_double
            Ray reflected_ray = compute_reflected_ray(r, temp_rec);
            vec3 sampledDirection = randomUnitVector(temp_rec.normal);
            reflected_ray.setColor(vec3(0,0,0));
            if (temp_rec.material.getIsreflective()) {
                HitRecord reflected_rec;
                vec3 updatedDirection = temp_rec.material.getReflectivity()*reflected_ray.getDirection() + (1.0-temp_rec.material.getReflectivity())*sampledDirection;
                reflected_ray.setDirection(updatedDirection);
                if (depth < maxBounces) {
                    HitRecord reflected_rec;
                    if (hit(reflected_ray, t_min, t_max, reflected_rec, depth + 1)) {
                            double dotPrd = std::max(0.0, vec3::dot(temp_rec.normal, (-1)*reflected_rec.normal));
                            collected_colour +=  (1.0/numSamples)*dotPrd*temp_rec.material.getSpecularColor() * reflected_ray.getColor();
                    }
                }  
            }
            
            else{
                Ray reflected_ray = compute_reflected_ray(r, temp_rec);
                reflected_ray.setColor(vec3(0,0,0));
                reflected_ray.setDirection(sampledDirection);
            // Create a ray in the sampled direction

            // Recursive call to trace the sampled ray
                HitRecord sampledRec;
                if (hit(reflected_ray, t_min, t_max, sampledRec, depth + 1)) {
                    double dotPrd = std::max(0.0, vec3::dot(temp_rec.normal, (-1) * sampledRec.normal));
                    collected_colour += (1.0/numSamples)*dotPrd * temp_rec.material.getSpecularColor() * sampledRec.material.getDiffusecolor();
                    
                }
            }
        }

        // Average the sampled colors
        //collected_colour /= numSamples;
        //collected_colour /= numOfNotReachingLights+1;
    }

    //r.setColor(r.getColor() + ambient_part + collected_colour);
    r.setColor(ambient_part + collected_colour + colour_shading);
    rec = temp_rec;    

    return hit_anything;
}


double random_doubleW(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;  // Mersenne Twister PRNG
    return distribution(generator);
}
// Function to generate a random unit vector
//vec3 World::randomUnitVector(std::default_random_engine& generator) {
vec3 World::randomUnitVector(const vec3& normal) {
    double x=  random_doubleW(0.0, 1.0);
    double y = random_doubleW(0.0, 1.0);
    double z = random_doubleW(0.0, 1.0);

    // Generate a random vector in the XY plane
    vec3 randomVec(x, y, z);

    // Reflect the vector about the normal to ensure it's on one side of the reflection plane
    double dotProduct = vec3::dot(randomVec.return_unit(), normal);
    vec3 reflectedVec;
    if (dotProduct <0){
       reflectedVec = (-1)*randomVec;
    }
    else{
        reflectedVec = randomVec;
    }
    return reflectedVec.return_unit();
}


void World::createAndLight(const nlohmann::json& jsonInput){    
    
    vec3 position = vec3(jsonInput["position"][0],
                         jsonInput["position"][1],
                         jsonInput["position"][2]);
    double radius = 1;
    vec3 intensity = vec3(jsonInput["intensity"][0],
                         jsonInput["intensity"][1],
                         jsonInput["intensity"][2]);

    Sphere newSphere(position, radius);
    newSphere.setLightColour(intensity);
    World::addLightSource(std::make_shared<Sphere>(newSphere));
    //World::addHittable(std::make_shared<Sphere>(newSphere));
}

void World::createAndAddSphere(const nlohmann::json& jsonInput, const std::string& pathToTextures){    
    #ifdef _WIN32
    std::string os_sep = "\\";
    #else
    std::string os_sep = "/";
    #endif

    vec3 position = vec3(jsonInput["center"][0],
                         jsonInput["center"][1],
                         jsonInput["center"][2]);
    double radius = jsonInput["radius"];

    Sphere newSphere(position, radius);
    if (jsonInput.contains("material"))
    {
        newSphere.setMaterial(Material::getMaterialFromJson(jsonInput["material"]));
    }
    if (jsonInput.contains("texture"))
    {
        std::cout << "setting texture" << std::endl;
        std::string loc = pathToTextures +os_sep;
        std::string ppmString = jsonInput["texture"];
        newSphere.setTexture(loc + ppmString);
    }

    World::addHittable(std::make_shared<Sphere>(newSphere));
}

void World::createAndAddTriangle(const nlohmann::json& jsonInput, const std::string& pathToTextures)//vec3 vertex1, vec3 vertex2, vec3 vertex3)
{
    #ifdef _WIN32
    std::string os_sep = "\\";
    #else
    std::string os_sep = "/";
    #endif

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
    if (jsonInput.contains("texture"))
    {
        std::cout << "setting texture" << std::endl;
        std::string loc = pathToTextures +os_sep;
        std::string ppmString = jsonInput["texture"];
        newTriangle.setTexture(loc + ppmString);
    }
                         
    World::addHittable(std::make_shared<Triangle>(newTriangle));
}

void World::createAndAddTriangle(vec3 vertex1, vec3 vertex2, vec3 vertex3)
{    Triangle newTriangle(vertex1,
                          vertex2,
                          vertex3);
    World::addHittable(std::make_shared<Triangle>(newTriangle));
}

void World::createAndAddCylinder(const nlohmann::json& jsonInput, const std::string& pathToTextures)//vec3 bottomCenter, double radius, double height, vec3 normalVector)
{
    #ifdef _WIN32
    std::string os_sep = "\\";
    #else
    std::string os_sep = "/";
    #endif

    double height = jsonInput["height"];
    double radius = jsonInput["radius"];    

    vec3 normalVector = vec3(jsonInput["axis"][0],
                             jsonInput["axis"][1],
                             jsonInput["axis"][2]).return_unit();

    vec3 cylinderCenter = vec3(jsonInput["center"][0],
                               jsonInput["center"][1],
                               jsonInput["center"][2]);

    vec3 bottomCenter = cylinderCenter - normalVector*(0.5*height);


    vec3 upperCenter = bottomCenter + normalVector*(height);
    Circle topCircle (upperCenter, radius, normalVector, height);
    Circle bottomCircle(bottomCenter, radius, normalVector*(-1), height);

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
    if (jsonInput.contains("texture"))
    {
        std::cout << "setting texture" << std::endl;
        std::string loc = pathToTextures +os_sep;
        std::string ppmString =jsonInput["texture"];
        cylinder.setTexture(loc + ppmString);
        topCircle.setTexture(loc + ppmString);
        bottomCircle.setTexture(loc + ppmString);
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


void World::loadScene(const std::string& filename, Camera& camera, const std::string& pathToTextures) {
    
    std::ifstream file(filename);
    nlohmann::json sceneJson;
    file >> sceneJson;

    // Extract camera information
    objects.clear();
    lightSources.clear();
    
    
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
    vec3 background = vec3(sceneInfo["backgroundcolor"][0],
                           sceneInfo["backgroundcolor"][1],
                           sceneInfo["backgroundcolor"][2]);


    camera.setupFromJson(sceneJson["camera"], sceneJson["rendermode"], background);
    camPtr = &camera;
    // Extract shapes information
    const nlohmann::json& shapesInfo = sceneInfo["shapes"];
    for (const auto& shapeInfo : shapesInfo) {
        std::string type = shapeInfo["type"];
        if (type == "sphere") {
            createAndAddSphere(shapeInfo, pathToTextures);
        } else if (type == "cylinder") {
            createAndAddCylinder(shapeInfo, pathToTextures);
        } else if (type == "triangle") {
            createAndAddTriangle(shapeInfo, pathToTextures);
        }
        // Add more shape types as needed
    }

    if (sceneInfo.contains("lightsources")) {
        const nlohmann::json& lightsInfo = sceneInfo["lightsources"];
        for (const auto& lightInfo : lightsInfo) {
            std::string type = lightInfo["type"];
            if (type == "pointlight") {
                createAndLight(lightInfo);
            // Add more shape types as needed
            }
        }
    }
}