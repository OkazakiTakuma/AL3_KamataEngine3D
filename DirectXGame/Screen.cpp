#include "Screen.h"
#include "math.h"
#include <algorithm>

using namespace KamataEngine;

void DrawGrid(const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHarfWidth = 2.0f;                               // グリッドの半分の幅
	const int kSubdivision = 10;                                     // グリッドの細分化数
	const float kGridEvery = (kGridHarfWidth * 2.0f) / kSubdivision; // グリッドの間隔
	// 奥から手前への線を引く
	for (int xIndex = 0; xIndex <= kSubdivision; xIndex++) {
		float x = -kGridHarfWidth + (kGridEvery * xIndex); // x座標
		Vector3 a, b;
		a.x = x;
		a.y = 0.0f;
		a.z = -kGridHarfWidth;
		b.x = x;
		b.y = 0.0f;
		b.z = kGridHarfWidth;
		Vector3 ndcVertexA = Transform(a, wvpMatrix);
		Vector3 ndcVertexB = Transform(b, wvpMatrix);
		Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
		Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
		// 線を描画 (実際の描画コードに置き換え)
		PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f,1.0f,1.0f,1.0f});
		
	}
	// 左から右への線を引く
	for (int zIndex = 0; zIndex <= kSubdivision; zIndex++) {
		float z = -kGridHarfWidth + (kGridEvery * zIndex); // z座標
		Vector3 a, b;
		a.x = -kGridHarfWidth;
		a.y = 0.0f;
		a.z = z;
		b.x = kGridHarfWidth;
		b.y = 0.0f;
		b.z = z;
		Vector3 ndcVertexA = Transform(a, wvpMatrix);
		Vector3 ndcVertexB = Transform(b, wvpMatrix);
		Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
		Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
		// 線を描画 (実際の描画コードに置き換え)
		PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f, 1.0f, 1.0f, 1.0f});
	
	}
}

void DrawSegment(const Segment& segment, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 ndcVertexA = Transform(segment.origin, wvpMatrix);
	Vector3 ndcVertexB = Transform(Add(segment.origin, segment.diff), wvpMatrix);
	Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
	Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	// 線を描画 (実際の描画コードに置き換え)
	PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f, 1.0f, 1.0f, 1.0f});
}

void DrawLine(const Line& line, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 ndcVertexA = Transform(line.origin, wvpMatrix);
	Vector3 ndcVertexB = Transform(Add(line.origin, line.diff), wvpMatrix);
	Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
	Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	// 線を描画 (実際の描画コードに置き換え)
	PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f, 1.0f, 1.0f, 1.0f});
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	const float p = 3.1415f;                         // 円周率
	const int kSubdivision = 20;                     // 球の細分化数
	const float kLonEvery = 2.0f * p / kSubdivision; // 経度の間隔
	const float kLatEvery = p / kSubdivision;        // 緯度の間隔
	for (int latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float lat = -p / 2.0f + latIndex * kLatEvery; // 緯度
		for (int lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			float lon = -p + lonIndex * kLonEvery; // 経度
			Vector3 a, b, c;
			a.x = sphere.radius * cosf(lat) * cosf(lon) + sphere.center.x;
			a.y = sphere.radius * sinf(lat) + sphere.center.y;
			a.z = sphere.radius * cosf(lat) * sinf(lon) + sphere.center.z;
			b.x = sphere.radius * cosf(lat + kLatEvery) * cosf(lon) + sphere.center.x;
			b.y = sphere.radius * sinf(lat + kLatEvery) + sphere.center.y;
			b.z = sphere.radius * cosf(lat + kLatEvery) * sinf(lon) + sphere.center.z;
			c.x = sphere.radius * cosf(lat) * cosf(lon + kLonEvery) + sphere.center.x;
			c.y = sphere.radius * sinf(lat) + sphere.center.y;
			c.z = sphere.radius * cosf(lat) * sinf(lon + kLonEvery) + sphere.center.z;
			Vector3 ndcVertexA = Transform(a, wvpMatrix);
			Vector3 ndcVertexB = Transform(b, wvpMatrix);
			Vector3 ndcVertexC = Transform(c, wvpMatrix);
			Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
			Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
			Vector3 screenVerticesC = Transform(ndcVertexC, viewportMatrix);
			// 線を描画 (実際の描画コードに置き換え)
			PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f, 1.0f, 1.0f, 1.0f});
			PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesC, {1.0f, 1.0f, 1.0f, 1.0f});




		}
	}
}

Vector3 Project(const Vector3& v1, Vector3& v2) {
	float v2Length = Length(v2);       // v2を正規化
	float dot = Dot(v1, v2);           // 内積を計算
	float c = dot / v2Length;          // v2を正規化
	Vector3 project = Multiply(c, v2); // 投影ベクトルを計算
	return project;                    // 投影ベクトルを返す
}

Vector3 ClosestPoint(const Segment& segment, const Vector3& point) {
	Vector3 segmentDiff = Subtract(segment.diff, segment.origin);          // 線分のベクトル
	Vector3 pointDiff = Subtract(point, segment.origin);                   // 点と線分の始点のベクトル
	float t = Dot(pointDiff, segmentDiff) / Dot(segmentDiff, segmentDiff); // tを計算
	if (t < 0.0f) {
		return segment.origin; // 線分の始点を返す
	} else if (t > 1.0f) {
		return Add(segment.origin, segment.diff); // 線分の終点を返す
	}
	Vector3 closestPoint = Add(segment.origin, Multiply(t, segmentDiff)); // 最近接点を計算
	return closestPoint;                                                  // 最近接点を返す
}

bool IsCollisionSphereToSphere(const Sphere& s1, const Sphere& s2) {
	float distance = Length(Vector3(s2.center.x - s1.center.x, s2.center.y - s1.center.y, s2.center.z - s1.center.z));
	if (distance <= s1.radius + s2.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsCollisionSphereToPlane(const Sphere& sphere, const Plane& plane) {
	float distance = sqrtf(((Dot(sphere.center, plane.normal))-plane.distance) * ((Dot(sphere.center, plane.normal))-plane.distance));
	if (distance <= sphere.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsCollisionSegmentToPlane(const Segment& segment, const Plane& plane) {
	float dot = Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	if (t == 1) {
		return true;
	} else if (t == 2) {
		return false;
	}
	return false;
}

bool IsCollisionLineToPlane(const Line& line, const Plane& plane) {
	float dot = Dot(plane.normal, line.diff);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	if (t == 1) {
		return false;
	} else if (t == 2) {
		return true;
	}
	return false;
}

bool IsCollisionRayToPlane(const Ray& ray, const Plane& plane) {

	float dot = Dot(plane.normal, ray.diff);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	if (t == 1) {
		return false;
	} else if (t == 2) {
		return true;
	}
	return false;
}

bool IsCollisionTriangeleToSegment(const Triangle& triangle, const Segment& segment) {
	Vector3 edge01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 edge12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 cross012 = Cross(edge01, edge12);
	Vector3 normal = Normalize(cross012);
	float distnaceTriangle = Dot(normal, triangle.vertices[0]);
	float t = (distnaceTriangle - Dot(normal, segment.origin)) / Dot(normal, segment.diff);
	Vector3 collisionPoint = Multiply(t, segment.diff);
	Vector3 cross01point = Subtract(collisionPoint, triangle.vertices[0]);
	Vector3 cross12point = Subtract(collisionPoint, triangle.vertices[1]);
	Vector3 cross20point = Subtract(collisionPoint, triangle.vertices[2]);

	if (Dot(cross01point, normal) >= 0.0f && Dot(cross12point, normal) >= 0.0f && Dot(cross20point, normal) >= 0.0f) {
		return true;
	} else {
		return false;
	}
}

bool IsCollisionSphereToAABB(const Sphere& sphere, const AABB& aabb) {
	Vector3 closestPoint{std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)};
	float distance = Length(Vector3(closestPoint.x - sphere.center.x, closestPoint.y - sphere.center.y, closestPoint.z - sphere.center.z));
	if (distance <= sphere.radius) {
		return true;
	} else {
		return false;
	}
}
bool IsCollisionAABBtoSegment(const AABB& aabb, const Segment& segment) {
	Vector3 min = aabb.min;
	Vector3 max = aabb.max;

	Vector3 segmentMin = segment.origin;
	Vector3 segmentMax = Add(segment.origin, segment.diff);
	if (segmentMin.x > segmentMax.x) {
		std::swap(segmentMin.x, segmentMax.x);
	}
	if (segmentMin.y > segmentMax.y) {
		std::swap(segmentMin.y, segmentMax.y);
	}
	if (segmentMin.z > segmentMax.z) {
		std::swap(segmentMin.z, segmentMax.z);
	}
	if (segmentMin.x > max.x || segmentMax.x < min.x) {
		return false;
	}
	if (segmentMin.y > max.y || segmentMax.y < min.y) {
		return false;
	}
	if (segmentMin.z > max.z || segmentMax.z < min.z) {
		return false;
	}
	if (segmentMin.x < min.x && segmentMax.x > max.x) {
		return true;
	}
	if (segmentMin.y < min.y && segmentMax.y > max.y) {
		return true;
	}
	if (segmentMin.z < min.z && segmentMax.z > max.z) {
		return true;
	}

	return false;
}
bool IsCollisionAABBtoLine(const AABB& aabb, const Line& line) {

	Vector3 min = aabb.min;
	Vector3 max = aabb.max;
	Vector3 lineMin = line.origin;
	Vector3 lineMax = Add(line.origin, line.diff);
	if (lineMin.x > lineMax.x) {
		std::swap(lineMin.x, lineMax.x);
	}
	if (lineMin.y > lineMax.y) {
		std::swap(lineMin.y, lineMax.y);
	}
	if (lineMin.z > lineMax.z) {
		std::swap(lineMin.z, lineMax.z);
	}
	if (lineMin.x > max.x || lineMax.x < min.x) {
		return false;
	}
	if (lineMin.y > max.y || lineMax.y < min.y) {
		return false;
	}
	if (lineMin.z > max.z || lineMax.z < min.z) {
		return false;
	}
	if (lineMin.x < min.x && lineMax.x > max.x) {
		return true; // 線分がAABBを横断している
	}
	if (lineMin.y < min.y && lineMax.y > max.y) {
		return true; // 線分がAABBを横断している
	}
	if (lineMin.z < min.z && lineMax.z > max.z) {
		return true; // 線分がAABBを横断している
	}
	// 線分がAABBの内部にある場合

	return false;
}
bool IscoliisionOBBToSphere(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 OBBWorldMatrix = MakeAffineMatrix(Vector3{1.0f, 1.0f, 1.0f}, obb.orientation[0], obb.center);
	Vector3 centrerInOBBLocalSpace = Transform(sphere.center, Inverse(OBBWorldMatrix)); // OBBのローカル空間に変換
	AABB obbAABB = {Vector3(-obb.size.x, -obb.size.y, -obb.size.z), obb.size};          // OBBのAABBを計算
	Sphere sphereInOBBLocalSpace = {centrerInOBBLocalSpace, sphere.radius};             // OBBのローカル空間に変換した球
	if (IsCollisionSphereToAABB(sphereInOBBLocalSpace, obbAABB)) {
		return true; // OBBと球が衝突している
	} else {
		return false; // OBBと球は衝突していない
	}
}
bool IscollisionOBBToLine(const OBB& obb, const Line& line) {

	Matrix4x4 OBBWorldMatrix = MakeAffineMatrix(Vector3{1.0f, 1.0f, 1.0f}, obb.orientation[0], obb.center);
	Vector3 lineOriginInOBBLocalSpace = Transform(line.origin, Inverse(OBBWorldMatrix));                        // OBBのローカル空間に変換
	Vector3 lineDiffInOBBLocalSpace = Transform(Add(line.origin, line.diff), Inverse(OBBWorldMatrix));          // OBBのローカル空間に変換
	AABB obbAABB = {Vector3(-obb.size.x, -obb.size.y, -obb.size.z), obb.size};                                  // OBBのAABBを計算
	Line localline = {lineOriginInOBBLocalSpace, Subtract(lineDiffInOBBLocalSpace, lineOriginInOBBLocalSpace)}; // OBBのローカル空間に変換した線
	if (IsCollisionAABBtoLine(obbAABB, localline)) {
		return true; // OBBと線が衝突している
	} else {
		// OBBと線は衝突していない
		// ここで、線の始点と終点がOBBの内部にあるかどうかをチェックする
		if (IsCollisionSphereToAABB({line.origin, 0.0f}, obbAABB) || IsCollisionSphereToAABB({Add(line.origin, line.diff), 0.0f}, obbAABB)) {
			return true; // 線の始点または終点がOBBの内部にある
		}
	}

	return false;
}
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return Vector3(-vector.y, vector.x, 0.0f);
	}
	return Vector3(0.0f, -vector.z, vector.y);
}

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {

	if (t < 0.0f)
		t = 0.0f; // tが0未満の場合は0に制限
	if (t > 1.0f)
		t = 1.0f; // tが1を超える場合は1に制限
	return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 centor = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = {-perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z};
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = {-perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z};

	Vector3 points[4];
	for (int i = 0; i < 4; i++) {
		Vector3 extend = Multiply(2.0f, perpendiculars[i]);
		Vector3 point = Add(centor, extend);
		points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 ndcVertexA = Transform(triangle.vertices[0], wvpMatrix);
	Vector3 ndcVertexB = Transform(triangle.vertices[1], wvpMatrix);
	Vector3 ndcVertexC = Transform(triangle.vertices[2], wvpMatrix);
	Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
	Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	Vector3 screenVerticesC = Transform(ndcVertexC, viewportMatrix);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {

	Vector3 vertices[8];
	vertices[0] = {aabb.min.x, aabb.min.y, aabb.min.z};
	vertices[1] = {aabb.max.x, aabb.min.y, aabb.min.z};
	vertices[2] = {aabb.max.x, aabb.max.y, aabb.min.z};
	vertices[3] = {aabb.min.x, aabb.max.y, aabb.min.z};
	vertices[4] = {aabb.min.x, aabb.min.y, aabb.max.z};
	vertices[5] = {aabb.max.x, aabb.min.y, aabb.max.z};
	vertices[6] = {aabb.max.x, aabb.max.y, aabb.max.z};
	vertices[7] = {aabb.min.x, aabb.max.y, aabb.max.z};
	// 立方体の各辺を描画
	for (int i = 0; i < 4; i++) {
		Vector3 ndcVertexA = Transform(vertices[i], wvpMatrix);
		Vector3 ndcVertexB = Transform(vertices[(i + 1) % 4], wvpMatrix);
		Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
		Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	}
	for (int i = 4; i < 8; i++) {
		Vector3 ndcVertexA = Transform(vertices[i], wvpMatrix);
		Vector3 ndcVertexB = Transform(vertices[i - 4], wvpMatrix);
		Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
		Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	}
	for (int i = 0; i < 4; i++) {
		Vector3 ndcVertexA = Transform(vertices[i + 4], wvpMatrix);
		Vector3 ndcVertexB = Transform(vertices[(i + 1) % 4 + 4], wvpMatrix);
		Vector3 screenVerticesA = Transform(ndcVertexA, viewportMatrix);
		Vector3 screenVerticesB = Transform(ndcVertexB, viewportMatrix);
	}
}

void DrawOBB(const OBB& obb, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 vertices[8];

	// OBBの8つの頂点を求める
	for (int i = 0; i < 8; ++i) {
		Vector3 offset = {
		    ((i & 1) ? 1.0f : -1.0f) * obb.size.x * obb.orientation[0].x + ((i & 2) ? 1.0f : -1.0f) * obb.size.y * obb.orientation[1].x + ((i & 4) ? 1.0f : -1.0f) * obb.size.z * obb.orientation[2].x,

		    ((i & 1) ? 1.0f : -1.0f) * obb.size.x * obb.orientation[0].y + ((i & 2) ? 1.0f : -1.0f) * obb.size.y * obb.orientation[1].y + ((i & 4) ? 1.0f : -1.0f) * obb.size.z * obb.orientation[2].y,

		    ((i & 1) ? 1.0f : -1.0f) * obb.size.x * obb.orientation[0].z + ((i & 2) ? 1.0f : -1.0f) * obb.size.y * obb.orientation[1].z + ((i & 4) ? 1.0f : -1.0f) * obb.size.z * obb.orientation[2].z};

		vertices[i] = {obb.center.x + offset.x, obb.center.y + offset.y, obb.center.z + offset.z};
	}

	// ワールド変換とビューポート変換を適用
	for (int i = 0; i < 8; ++i) {
		vertices[i] = Transform(vertices[i], wvpMatrix);
		vertices[i] = Transform(vertices[i], viewportMatrix);
	}

	// OBBの辺を描画 (12本)
	static constexpr int indices[12][2] = {
	    {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
        {4, 5},
        {5, 7},
        {7, 6},
        {6, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

	for (auto& edge : indices) {
		Vector3 ndcVertexA = vertices[edge[0]];
		Vector3 ndcVertexB = vertices[edge[1]];
		Vector3 screenVerticesA = ndcVertexA;
		Vector3 screenVerticesB = ndcVertexB;
		// 線を描画 (実際の描画コードに置き換え)
		PrimitiveDrawer::GetInstance()->DrawLine3d(screenVerticesA, screenVerticesB, {1.0f, 1.0f, 1.0f, 1.0f});
	}
}

void DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& wvpMatrix, const Matrix4x4& viewportMatrix) {

	for (float t = 0; t <= 21.0f / 20.0f; t += 1.0f / 20.0f) {
		Vector3 p0p1 = Lerp(p0, p1, t);                              // p0とp1の間の点
		Vector3 p1p2 = Lerp(p1, p2, t);                              // p1とp2の間の点
		Vector3 bezierPoint = Lerp(p0p1, p1p2, t);                   // ベジェ曲線上の点
		Vector3 ndcVertex = Transform(bezierPoint, wvpMatrix);       // NDC座標に変換
		Vector3 screenVertex = Transform(ndcVertex, viewportMatrix); // スクリーン座標に変換
		// 1個前の点と現在の点を結ぶ線を描画
		if (t > 0.0f) {
			// 前のベジェ点を計算
			Vector3 prevp0p1 = Lerp(p0, p1, t - 1.0f / 20.0f);                    // p0とp1の間の前の点
			Vector3 prevp1p2 = Lerp(p1, p2, t - 1.0f / 20.0f);                    // p1とp2の間の前の点
			Vector3 prevBezierPoint = Lerp(prevp0p1, prevp1p2, t - 1.0f / 20.0f); // 前のベジェ曲線上の点

			Vector3 prevNdcVertex = Transform(prevBezierPoint, wvpMatrix);
			Vector3 prevScreenVertex = Transform(prevNdcVertex, viewportMatrix);
		}
	}
}
