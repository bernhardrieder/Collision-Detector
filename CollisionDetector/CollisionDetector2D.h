#pragma once	

namespace CollisionDetection
{
	enum CollisionType
	{
		None = 0,
		BoundingVolume,
		AABB,
		OBB,
		MinkovskiDifference
	};

	struct Collision2D
	{
		Collision2D(Collideable2D* const partner) : Partner(partner) {};

		Collideable2D* const Partner = nullptr;
		CollisionType LastDetectedType = None;
	};

	struct CollisionObject
	{
		CollisionObject(Collideable2D* const object) : Object(object) {};

		Collideable2D* const Object = nullptr;
		std::vector<Collision2D> Collisions;

		bool HasCollision() const { return Collisions.size() > 0; }
	};

	class CollisionDetector2D
	{
	public:
		CollisionDetector2D();
		~CollisionDetector2D();

		void RegisterCollidable(Collideable2D* const collidable);
		void DeregisterCollidable(Collideable2D* const collidable);

		void DetectAndUpdateCollisionsOnAllRegisteredObjects();
		auto GetAllRegisteredCollisionObjects() const -> const std::vector<CollisionObject>& {return m_collidables; };

	protected:
		std::vector<CollisionObject> m_collidables;

		static bool isCollisionDetectedWithBoundingVolumeTest(const BoundingSphere& lhs, const BoundingSphere& rhs);
		static bool isCollisionDetectedWithAABB(const AxisAlignedBoundingBox& lhs, const AxisAlignedBoundingBox& rhs);
		static bool isCollisionDetectedWithOBB(const OrientedBoundingBox& lhs, const OrientedBoundingBox& rhs);
		static bool isCollisionDetectedWithMinkovskiDifference(const std::vector<DirectX::SimpleMath::Vector3>& lhs, const std::vector<DirectX::SimpleMath::Vector3>& rhs);
	};
}