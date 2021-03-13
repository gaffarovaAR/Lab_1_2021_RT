#pragma once
#include "LiteMath.h"
#include "RayTracer.h"

using namespace HydraLiteMath;

class LightSource {
public:
	explicit LightSource(const float3 a_position, const float3 a_color, const float a_intensity) : position(a_position), color(a_color), intensity(a_intensity) {};
	virtual ~LightSource() = default;
public:
	float3 position; // ������������ ��������� �����
	float3 color; // ���� �����
	float intensity;
};

// ������� �������� �����
class AmbientSource : public LightSource {
public:
	AmbientSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~AmbientSource() = default;
	//....��� ��������
};

// �������� �������� �����
class PointSource : public LightSource {
public:
	PointSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~PointSource() = default;
};

// ������ �������� �����
class DirectSource : public LightSource {
public:
	DirectSource(const float3 a_position, const float3 a_color, const float a_intensity) : LightSource(a_position, a_color, a_intensity) {};
	~DirectSource() = default;
	//.....��� �������
};

/////