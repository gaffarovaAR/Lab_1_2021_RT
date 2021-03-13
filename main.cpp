#include <iostream>
#include <vector>
#include <random>

#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"
#include "LightSource.h"

using namespace HydraLiteMath;

struct Scene {
    std::vector<std::shared_ptr<GeoObject>> objects;
    std::vector<std::shared_ptr<LightSource>> lights;
};

void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const Scene &scene, const Camera &cam, const std::string &filename)
{
    auto  background_color = float3(135/float(255), 206/float(255), 235/float(255));
    auto  film = std::make_unique<Film>(w, h, num_samples);
    auto  tracer = std::make_unique<WhittedRT>(16, background_color);
    float invWidth  = 1.0f / float(w);
    float invHeight = 1.0f / float(h);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float3 pixel_color = float3(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < num_samples; ++s)
            {
                Ray ray = cam.genRay(w, h, x, h - y); //генерируем луч из камеры через текущий пиксель
                pixel_color += tracer->TraceRay(ray, scene.objects, scene.lights, 0); //трассируем луч и получаем цвет
            }
            pixel_color /= film->num_samples;      // усредняем полученные цвета
            pixel_color *= cam.getExposureTime();  // умножаем на время экспонирования сенсора - выдержка виртуальной камеры
            film->SetPixelColor(x, y, pixel_color); //записываем цвет на виртуальный сенсор
        }
    }
    film->SaveImagePPM(filename); //сохраняем картинку
}

void create_scene()
{
    Scene myScene;
    float3        eye   (0.0f, 2.0f, 20.0f);
    float3        lookat(0.0f, 2.0f, 0.0f);
    float3        up    (0.0f, 1.0f, 0.0f);
    float         field_of_view = 90.0f;
    unsigned int  w = 640;
    unsigned int  h =  480;
    Camera        cam(eye, lookat, up, field_of_view, float(w) / float(h));

    // Создание объектов
    //auto plane = std::make_shared<Plane>(float3(+0.0f, +1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(0.3f, 0.3f, 0.3f)));
    auto plane = std::make_shared<Plane>(float3(+0.0f, +0.0f, +0.0f), float3(+0.0f, +1.0f, +0.0f), new Defuse(float3(139 / float(255), 69 / float(255), 19 / float(255))));
    auto sphere1 = std::make_shared<Sphere>(float3(+0.0f, +2.0f, -10.0f), 2.0f, new IdealMirror(float3(255 / float(255), 140 / float(255), 0 / float(255))));
    auto sphere2 = std::make_shared<Sphere>(float3(+3.0f, +3.0f, -15.0f), 3, new Defuse(float3(255 / float(255), 140 / float(255), 0.0f)));
    auto parallelepiped1 = std::make_shared<Parallelepiped>(float3(-3.0f, +0.0f, +10.0f), float3(-2.0f, +3.0f, +15.0f), new IdealMirror(float3(148 / float(255), 0.0f, 211 / float(255))));
    auto parallelepiped2 = std::make_shared<Parallelepiped>(float3(-5.0f, +0.0f, -10.0f), float3(-2.0f, +7.0f, +5.0f), new Defuse(float3(148 / float(255), 0.0f, 211 / float(255))));
    auto triangle1 = std::make_shared<Triangle>(float3(-1.0f, +0.0f, +13.0f), float3(+1.0f, +0.0f, +15.0f), float3(+0.0f, +1.0f, +13.0f), new IdealMirror(float3(0.0f, 0.0f, 128 / float(255))));
    auto triangle2 = std::make_shared<Triangle>(float3(+3.0f, +3.0f, +10.0f), float3(+4.0f, +0.0f, +15.0f), float3(+3.5f, +0.0f, +12.0f), new Defuse(float3(0.0f, 0.0f, 128 / float(255))));
    auto square1 = std::make_shared<Square>(float3(+20.0f, +0.0f, -15.0f), float(+2.0f), new IdealMirror(float3(107 / float(255), 142 / float(255), 35 / float(255))));
    auto square2 = std::make_shared<Square>(float3(+20.0f, +0.0f, -20.0f), float(+5.0f), new Defuse(float3(107 / float(255), 142 / float(255), 35 / float(255))));
    auto lightS1= std::make_shared<Sphere>(float3(+15.0f, +10.0f, +10.0f), 0.1f, new Light(float3(+1.0f, +1.0f, +1.0f)));
    auto lightS2 = std::make_shared<Sphere>(float3(-8.0f, +15.0f, +20.0f), 0.1f, new Light(float3(+1.0f, +1.0f, +1.0f)));

    //Создание источников
    auto light1 = std::make_shared<PointSource>(float3(+15.0f, +10.0f, +10.0f), float3(+1.0f, +1.0f, +1.0f), +1.0f);
    auto light2 = std::make_shared<PointSource>(float3(-8.0f, +15.0f, +20.0f), float3(1.0f, 1.0f, 1.0f), 1.0f);

    // Добавление объектов на сцену
    myScene.objects.push_back(plane);
    myScene.objects.push_back(sphere1);
    myScene.objects.push_back(sphere2);
    myScene.objects.push_back(parallelepiped1);
    myScene.objects.push_back(parallelepiped2);
    myScene.objects.push_back(triangle1);
    myScene.objects.push_back(triangle2);
    myScene.objects.push_back(square1);
    myScene.objects.push_back(square2);

    // Добавление источников на сцену
    myScene.lights.push_back(light1);
    myScene.lights.push_back(light2);


    RenderScene(w, h, 1, myScene, cam,  "basic_scene");
}

int main()
{
  create_scene();

  return 0;
}
