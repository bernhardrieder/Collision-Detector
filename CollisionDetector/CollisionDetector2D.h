#pragma once	

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
	Collision2D(const Collideable2D* const partner) : Partner(partner) {};

	const Collideable2D* const Partner = nullptr;
	CollisionType LastDetectedType = None;
};

struct CollisionObject
{
	CollisionObject(const Collideable2D* const object) : Object(object) {};

	const Collideable2D* const Object = nullptr;
	std::vector<Collision2D> Collisions;

	bool HasCollision() const { return Collisions.size() > 0; }
};

class CollisionDetector2D
{
public:
	CollisionDetector2D();
	~CollisionDetector2D();

	void RegisterCollidable(const Collideable2D* const collidable);
	void DeregisterCollidable(const Collideable2D* const collidable);

	void DetectAndUpdateCollisionsOnAllRegisteredObjects();
	auto GetAllRegisteredCollisionObjects() const -> const std::vector<CollisionObject>& {return m_collidables; };

protected:
	std::vector<CollisionObject> m_collidables;

	static bool isCollisionDetectedWithBoundingVolumeTest(const DirectX::BoundingSphere& lhs, const DirectX::BoundingSphere& rhs);
	static bool isCollisionDetectedWithAABB(const DirectX::BoundingBox& lhs, const DirectX::BoundingBox& rhs);
	static bool isCollisionDetectedWithOBB(const DirectX::BoundingOrientedBox& lhs, const DirectX::BoundingOrientedBox& rhs);
	bool isCollisionDetectedWithMinkovskiDifference();
};
