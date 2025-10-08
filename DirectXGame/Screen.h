#pragma once
#include "KamataEngine.h"
#include "Matrix.h"
#include "Vector.h"


void DrawGrid(const KamataEngine::Matrix4x4& viewProjectionMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawSegment(const Segment& segment, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawLine(const Line& line, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawPlane(const Plane& plane, const KamataEngine::Matrix4x4& viewProjectionMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawTriangle(const Triangle& triangle, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawAABB(const AABB& aabb, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);
void DrawOBB(const OBB& obb, const KamataEngine::Matrix4x4& wvpMatrix, const KamataEngine::Matrix4x4& viewportMatrix);

KamataEngine::Vector3 ClosestPoint(const Segment& segment, const KamataEngine::Vector3& point);
KamataEngine::Vector3 Perpendicular(const KamataEngine::Vector3& vector);
KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& a, const KamataEngine::Vector3& b, float t);

bool IsCollisionSphereToSphere(const Sphere& s1, const Sphere& s2);
bool IsCollisionSphereToPlane(const Sphere& sphere, const Plane& plane);
bool IsCollisionSegmentToPlane(const Segment& segment, const Plane& plane);
bool IsCollisionLineToPlane(const Line& line, const Plane& plane);
bool IsCollisionRayToPlane(const Ray& ray, const Plane& plane);
bool IsCollisionTriangeleToSegment(const Triangle& triangle, const Segment& segment);
bool IsCollisionSphereToAABB(const Sphere& sphere, const AABB& aabb);
bool IsCollisionAABBtoSegment(const AABB& aabb, const Segment& segment);
bool IsCollisionAABBtoLine(const AABB& aabb, const Line& line);
bool IscoliisionOBBToSphere(const OBB& obb, const Sphere& sphere);
bool IscollisionOBBToLine(const OBB& obb, const Line& line);