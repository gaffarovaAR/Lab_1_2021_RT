#ifndef RT_SAMPLE_NEW_GEOMETRY_H
#define RT_SAMPLE_NEW_GEOMETRY_H

#include "LiteMath.h"
#include "RayTracer.h"
#include "Material.h"

using namespace HydraLiteMath;

//Базовый виртуальный класс для геометрического объекта, с которым может пересекаться луч
class GeoObject
{
public:
  virtual bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const = 0;
  std::shared_ptr<Material> m_ptr;
  explicit GeoObject(Material *a_m) : m_ptr(a_m) {} ;
  virtual ~GeoObject() = default;
};

//Бесконечная плоскость
class Plane : public GeoObject
{
public:
	Plane(const float3 a_point, const float3 a_normal, Material* a_m) : GeoObject(a_m), point(a_point), normal(a_normal) {}
	~Plane() = default;

	bool Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const override;

protected:
	float3 point; //точка, из которой проводим нормаль
	float3 normal; //нормаль к плоскости
};

//Сфера
class Sphere : public GeoObject
{
public:
	Sphere(const float3 a_center, const float a_radius, Material* a_m) : GeoObject(a_m), center(a_center), radius(a_radius), radius_sqr(a_radius* a_radius) {}
	~Sphere() = default;

	bool Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const override;

private:
	float3 center; //центр сферы
	float radius; //радиус сферы
	float radius_sqr; //радиус сферы в кфадрате
};

//Параллелепипед
class Parallelepiped : public GeoObject 
{
public:
	Parallelepiped(const float3& a_min, const float3& a_max, Material* a_m) :
		GeoObject(a_m), t_min(a_min), t_max(a_max) {}
	~Parallelepiped() = default;

	bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
	float3 t_min; // минимальная точка
	float3 t_max; // максимальная точка
};

//Треугольник
class Triangle : public GeoObject {
public:
	Triangle(const float3& a, const float3& b, const float3& c, Material* a_m) : a(a), b(b), c(c), GeoObject(a_m) {}
	~Triangle() = default;

	bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
	float3 a; // вершина a треугольника
	float3 b; // вершина b треугольника
	float3 c; // вершина c треугольника
};

//Квадрат
class Square : public Plane
{
public:
	Square(const float3 a_point, const float& a_l, Material* a_m) :
		Plane(a_point, float3(+0.0f, +0.0f, +1.0f), a_m), l(a_l) {}
	~Square() = default;

	bool Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const override;

private:
	float l; // длина одной стороны квадрата
};

#endif //RT_SAMPLE_NEW_GEOMETRY_H
