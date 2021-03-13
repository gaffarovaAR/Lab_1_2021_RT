#include "Geometry.h"


bool Plane::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{
	// t = (a-r0)*n/(d*n), 
	//    a - точка с которой проводится нормаль к плоскости
	//    r0 - точка, в которой находится камера
	//    n - нормаль
	//    d - направление камеры
    surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

    if(surf.t > t_min && surf.t < t_max)
    {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normal;
    surf.m_ptr    = m_ptr;
    return true;
    }

    return false;
}

//Для пересечения со сферой подставляем луч в уравнение сферы и решаем получившееся квадратное уравнение
//Решения этого уравнения - точки пересечения луча со сферой
bool Sphere::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const
{
    // O - точка, в которой находится камера; P - точка на сфере; С - центр сферы
    // <P-C,P-C>=radius^2    P=O+tD, где D - направление вектора   <O+tD-C,O+tD-C>=radius^2    OC=O-C   <OC+tD,OC+tD>=radius^2
    // <tD,tD>+<2*OC,tD>+<OC,OC>-radius^2=0
    // <D,D>*t^2+<OC,D>*2*t+<OC,OC>-radius^2=0- квадратное уравнение с неизвестной t
    // если Дискриминант<0, то пересечений нет, Дискриминант=0 пересечение одно, Дискриминант>0 пересечений два

    float3 k = ray.o - center; // k = OC = О - С

    float k1 = dot(ray.d, ray.d); // k1 = <D,D>
    float k2 = dot(2 * k, ray.d); // k2 = <OC,D>*2
    float k3 = dot(k, k) - radius_sqr; // k3 = <OC,OC>-radius^2

    // k1*t^2 + k2*t + k3 = 0

    float discriminant = k2 * k2 - 4 * k1 * k3;

    if (discriminant < 0) return false; // Пересечений нет 

    // Пересечений есть
    // Первый корень
    surf.t = (-k2 - sqrt(discriminant)) / 2 * k1;
    if (surf.t > t_min && surf.t < t_max)
    {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint - center);
        surf.m_ptr = m_ptr;
        return true;
    }

    // Второй корень
    surf.t = (-k2 + sqrt(discriminant)) / 2 * k1;
    if (surf.t > t_min && surf.t < t_max)
    {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint - center);
        surf.m_ptr = m_ptr;
        return true;
    }

    return false;
}

// Выровнен по осям координат поэтому строим по 2 точкам
bool Parallelepiped::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	float dirfracX = 1.0f / ray.d.x;
	float dirfracY = 1.0f / ray.d.y;
	float dirfracZ = 1.0f / ray.d.z;
	float x1 = (t_min.x - ray.o.x) * dirfracX;
	float x2 = (t_max.x - ray.o.x) * dirfracX;
	float y1 = (t_min.y - ray.o.y) * dirfracY;
	float y2 = (t_max.y - ray.o.y) * dirfracY;
	float z1 = (t_min.z - ray.o.z) * dirfracZ;
	float z2 = (t_max.z - ray.o.z) * dirfracZ;

	float tMin = max(max(min(x1, x2), min(y1, y2)), min(z1, z2));
	float tMax = min(min(max(x1, x2), max(y1, y2)), max(z1, z2));


	surf.t = tMin;
	if (tMin < tMax && tMax > 0 && surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = ray.o + surf.t * ray.d;
		surf.normal = normalize(surf.hitPoint);
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Triangle::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	//Метод Möller - Trumbore
	float3 E1 = b - a;
	float3 E2 = c - a;
	float3 T = ray.o - a;
	float3 P = cross(ray.d, E2);
	float3 Q = cross(T, E1);
	float det = dot(E1, P);

	//Нет пересечения с плоскостью
	if (det < tmin && det > tmax) {
		return false;
	}

	float invDet = 1 / det;
	float u = dot(T, P) * invDet;

	//Вне треугольника

	if (u < 0 || u > 1) {
		return false;
	}

	float v = dot(ray.d, Q) * invDet;

	if (v < 0 || u + v > 1) {
		return false;
	}

	//Если внутри

	surf.t = dot(E2, Q) * invDet;
	if (surf.t > tmin && surf.t < tmax) {
		surf.hit = true;
		surf.hitPoint = float3(surf.t, u, v);
		surf.normal = cross((b - a), (c - a));
		surf.m_ptr = m_ptr;
		return true;
	}
	return false;
}

bool Square::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const
{
	if (!Plane::Intersect(ray, tmin, tmax, surf))
		return false;

	float u = (surf.hitPoint.x - point.x) / l;
	float v = (surf.hitPoint.y - point.y) / l;

	return !(u < 0 || v < 0 || u > 1 || v > 1); //Если вне треугольника, то возвращает ложь
}