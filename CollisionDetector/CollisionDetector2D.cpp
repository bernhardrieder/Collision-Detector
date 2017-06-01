#include "pch.h"
#include "CollisionDetector2D.h"

using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

CollisionDetector2D::CollisionDetector2D()
{
}


CollisionDetector2D::~CollisionDetector2D()
{
}

void CollisionDetector2D::RegisterCollidable(Collideable2D* const collidable)
{
	m_collidables.push_back(CollisionObject(collidable));
}

void CollisionDetector2D::DeregisterCollidable(Collideable2D* const collidable)
{
	//todo
}

void CollisionDetector2D::DetectAndUpdateCollisionsOnAllRegisteredObjects()
{
#pragma omp parallel for
	for(int i = 0; i < m_collidables.size(); ++i)
	{
		m_collidables[i].Collisions.clear();
		for(const auto& other : m_collidables)
		{
			if (m_collidables[i].Object->GetID() == other.Object->GetID())
				continue;

			/************************* Bounding Volume *************************/
			if (!isCollisionDetectedWithBoundingVolumeTest(m_collidables[i].Object->GetBoundingSphereTransformed(), other.Object->GetBoundingSphereTransformed()))
			continue;

			m_collidables[i].Collisions.push_back(Collision2D(other.Object));
			Collision2D& collision = m_collidables[i].Collisions[m_collidables[i].Collisions.size()-1];
			collision.LastDetectedType = BoundingVolume;

			/************************* AABB *************************/
			if (!isCollisionDetectedWithAABB(m_collidables[i].Object->GetAxisAlignedBoundingBoxTransformed(), other.Object->GetAxisAlignedBoundingBoxTransformed()))
				continue;
			collision.LastDetectedType = AABB;

			/************************* OBB *************************/
			if (!isCollisionDetectedWithOBB(m_collidables[i].Object->GetOrientedBoundingBoxTransformed(), other.Object->GetOrientedBoundingBoxTransformed()))
				continue;
			collision.LastDetectedType = OBB;

			/************************* Minkovsky Sum *************************/
			if (!isCollisionDetectedWithMinkovskiDifference(m_collidables[i].Object->GetVertices3DTransformed(), other.Object->GetVertices3DTransformed()))
				continue;
			collision.LastDetectedType = MinkovskiDifference;
		}
	}
}


bool CollisionDetector2D::isCollisionDetectedWithBoundingVolumeTest(const BoundingSphere& lhs, const BoundingSphere& rhs)
{
	return lhs.Intersect(rhs);
}

bool CollisionDetector2D::isCollisionDetectedWithAABB(const AxisAlignedBoundingBox& lhs, const AxisAlignedBoundingBox& rhs)
{
	return lhs.Intersect(rhs);
}

bool CollisionDetector2D::isCollisionDetectedWithOBB(const OrientedBoundingBox& lhs, const OrientedBoundingBox& rhs)
{
	return lhs.Intersect(rhs);
}

bool CollisionDetector2D::isCollisionDetectedWithMinkovskiDifference(const std::vector<DirectX::SimpleMath::Vector3>& lhs, const std::vector<DirectX::SimpleMath::Vector3>& rhs)
{
	auto vertices = CollisionDetection::GetMinkowkiDifference(lhs, rhs);

	JarvisMarch jarvis;
	std::vector<Vector2> vertices2d;
	for (const auto& vec : vertices)
		vertices2d.push_back({ vec.x, vec.y });
	auto convexHull = jarvis.GetConvexHull(vertices2d);

	//https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
	int i, j;
	bool collision = false;
	for (i = 0, j = convexHull.size()-1; i < convexHull.size(); j = i++)
	{
		if (((convexHull[i].y > 0.f) != (convexHull[j].y > 0.f)) && (0.f < (convexHull[j].x - convexHull[i].x) * (-convexHull[i].y) / (convexHull[j].y - convexHull[i].y) + convexHull[i].x))
		{
			collision = !collision;
		}
	}
	return collision;
}
