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
	for(auto& owner : m_collidables)
	{
		owner.Collisions.clear();
		for(const auto& other : m_collidables)
		{
			if (owner.Object->GetID() == other.Object->GetID())
				continue;

			/************************* Bounding Volume *************************/
			if (!isCollisionDetectedWithBoundingVolumeTest(owner.Object->GetBoundingSphereTransformed(), other.Object->GetBoundingSphereTransformed()))
			continue;

			owner.Collisions.push_back(Collision2D(other.Object));
			Collision2D& collision = owner.Collisions[owner.Collisions.size()-1];
			collision.LastDetectedType = BoundingVolume;

			/************************* AABB *************************/
			if (!isCollisionDetectedWithAABB(owner.Object->GetAxisAlignedBoundingBoxTransformed(), other.Object->GetAxisAlignedBoundingBoxTransformed()))
				continue;
			collision.LastDetectedType = AABB;

			/************************* OBB *************************/
			if (!isCollisionDetectedWithOBB(owner.Object->GetOrientedBoundingBoxTransformed(), other.Object->GetOrientedBoundingBoxTransformed()))
				continue;
			collision.LastDetectedType = OBB;

			/************************* Minkovsky Sum *************************/
			if (!isCollisionDetectedWithMinkovskiDifference(owner.Object->GetVertices3DTransformed(), other.Object->GetVertices3DTransformed()))
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

	//https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
	//https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
	int i, j;
	bool collision = false;
	for (i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++)
	{
		if (((vertices[i].y > 0.f) != (vertices[j].y > 0.f)) && (0.f < (vertices[j].x - vertices[i].x) * (-vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
		{
			collision = !collision;
		}
	}
	return collision;
}
