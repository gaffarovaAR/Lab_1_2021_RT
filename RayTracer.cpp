#include <limits>
#include "RayTracer.h"
#include "Material.h"
#include "Geometry.h"


float3 WhittedRT::TraceRay(const Ray& ray, const std::vector<std::shared_ptr<GeoObject>>& geo, const std::vector<std::shared_ptr<LightSource>>& ligth, int depth) 
{
	float3 color = float3(1.0f, 1.0f, 1.0f);
	float3 timeColor = float3(1.0f, 1.0f, 1.0f);
	SurfHit surf;
	Ray timeRay = ray;
	
	while (depth < max_ray_depth) { // Если количество отражений меньше максимального
		color *= timeColor;
		float tnear = std::numeric_limits<float>::max();

		int   geoIndex = -1;
		SurfHit surf;

		for (int i = 0; i < geo.size(); ++i)
		{
			SurfHit temp;
			
			if (geo.at(i)->Intersect(timeRay, 0.001, tnear, temp)) // При пересечении
			{
				if (temp.t < tnear)
				{
					tnear = temp.t;
					geoIndex = i;
					surf = temp;
				}
			}
		}
		
		if (geoIndex == -1) // При отсутствии пересечения
		{
			float3 unit_direction = normalize(timeRay.d);
			float t = 0.5f * (unit_direction.y + 1.0f);
			timeColor = (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + t * bg_color;

			depth++;
			break;
		}

		if (dot(timeRay.d, surf.normal) > 0)
		{
			surf.normal = -surf.normal;
		}

		Ray scattered;
		
		if (typeid(*surf.m_ptr) != typeid(Light)) // Если это не источник света
		{

			if ((typeid(*surf.m_ptr).name() == typeid(Defuse).name())) // Если диффузный
			{
				timeColor = float3(0.0f, 0.0f, 0.0f);
				float3 time;
				int countOfLightSourses = 0;

				for (int i = 0; i < ligth.size(); i++) {
					Ray rayIn;
					rayIn.o = ligth.at(i)->position;
					rayIn.d = normalize(rayIn.o - surf.hitPoint);

					Ray shadow(surf.hitPoint + normalize(surf.normal) * 10e-5, rayIn.d);
					
					if (!ShadowRay(shadow, geo)) // Если нет препятствий на пути луча
					{
						surf.m_ptr->Scatter(rayIn, surf, time, scattered);
						timeColor += time * ligth.at(i)->color * ligth.at(i)->intensity;
						++countOfLightSourses;
					}
				}
				break;
			}
			
			else if (surf.m_ptr->Scatter(timeRay, surf, timeColor, scattered)) // Это зеркальный объект
			{
				timeRay = scattered;
				depth++;
			}
			else
			{
				depth++;
				timeColor = float3(0.0f, 0.0f, 0.0f);
			}
		}
		
		else { // Это источник света
			surf.m_ptr->Scatter(timeRay, surf, timeColor, scattered);
			break;
		}
	}
	color *= timeColor;
	return color;

}

bool WhittedRT::ShadowRay(const Ray& ray, const std::vector<std::shared_ptr<GeoObject>>& geo) {
	Ray timeRay = ray;
	float tnear = std::numeric_limits<float>::max();
	int geoIndex = -1;
	SurfHit surf;

	for (int i = 0; i < geo.size(); ++i)
	{
		SurfHit temp;
		if (geo.at(i)->Intersect(timeRay, 0.001, tnear, temp))
		{
			if (temp.t < tnear)
			{
				tnear = temp.t;
				geoIndex = i;
				surf = temp;
			}
			return true;
		}
	}
	return false;
	// возвращается информация о том, есть ли что-то между найденной точки пересечения и источником света 
}

