#pragma once
#include "LiteMath.h"
#include "RayTracer.h"

using namespace HydraLiteMath;

class LightSource {
public:
	explicit LightSource(const float3 a_position, const float3 a_color, const float a_intensity) : position(a_position), color(a_color), intensity(a_intensity) {};
	virtual ~LightSource() = default;
public:
	float3 position; // расположение источника света
	float3 color; // цвет света
	float intensity;
};

// Фоновый источник света
class AmbientSource : public LightSource {
public:
	AmbientSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~AmbientSource() = default;
	//....для фонового
};

// Точечный источник света
class PointSource : public LightSource {
public:
	PointSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~PointSource() = default;
};

// Прямой источник света
class DirectSource : public LightSource {
public:
	DirectSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~DirectSource() = default;
	//.....для прямого
};

/////