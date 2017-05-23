#pragma once	

enum CollisionType
{
	None,
	BoundingVolume,
	AABB,
	OBB,
	MinkovskySum
};

struct Collision2D
{
	Collision2D(const ICollidable2D* const partner) : Partner(partner) {};

	const ICollidable2D* const Partner = nullptr;
	CollisionType LastDetectedType = None;
};

struct CollisionObject
{
	CollisionObject(const ICollidable2D* const object) : Object(object) {};

	const ICollidable2D* const Object = nullptr;
	std::vector<Collision2D> Collisions;

	bool HasCollision() const { return Collisions.size() > 0; }
};

class CollisionDetector2D
{
public:
	CollisionDetector2D();
	~CollisionDetector2D();

	void RegisterCollidable(const ICollidable2D* const collidable);
	void DeregisterCollidable(const ICollidable2D* const collidable);

	void DetectAndUpdateCollisionsOnAllRegisteredObjects();
	auto GetAllRegisteredCollisionObjects() const -> const std::vector<CollisionObject>& {return m_collidables; };

protected:
	std::vector<CollisionObject> m_collidables;

	bool isCollisionDetectedWithBoundingVolumeTest(const DirectX::BoundingSphere& lhs, const DirectX::BoundingSphere& rhs);
	bool isCollisionDetectedWithAABB();
	bool isCollisionDetectedWithOBB();
	bool isCollisionDetectedWithMinkovskySum();
};
