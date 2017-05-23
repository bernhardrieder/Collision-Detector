#pragma once
class CollisionDetector2D
{
public:
	CollisionDetector2D();
	~CollisionDetector2D();

	void RegisterCollidable(const ICollidable2D* const collidable);


protected:
	std::vector<const ICollidable2D* const> m_collidables;


	bool isCollisionBoundingVolumeTest(const DirectX::BoundingSphere& lhs, const DirectX::BoundingSphere& rhs);
	bool isCollisionAABB();
	bool isCollisionOBB();
	bool isCollisionMinkovskySum();

};

