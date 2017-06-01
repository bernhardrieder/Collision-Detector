#pragma once

namespace CollisionDetection
{
	static void GetMinAndMaxVertices(const std::vector<DirectX::SimpleMath::Vector3>& inVertices, DirectX::SimpleMath::Vector3& outMinX, DirectX::SimpleMath::Vector3& outMaxX, DirectX::SimpleMath::Vector3& outMinY, DirectX::SimpleMath::Vector3& outMaxY, DirectX::SimpleMath::Vector3& outMinZ, DirectX::SimpleMath::Vector3& outMaxZ)
	{
		outMinX = outMaxX = outMinY = outMaxY = outMinZ = outMaxZ = DirectX::SimpleMath::Vector3::Zero;

		for (auto& vertex : inVertices)
		{
			if (vertex.x < outMinX.x)
				outMinX = vertex;

			if (vertex.x > outMaxX.x)
				outMaxX = vertex;

			if (vertex.y < outMinY.y)
				outMinY = vertex;

			if (vertex.y > outMaxY.y)
				outMaxY = vertex;

			if (vertex.z < outMinZ.z)
				outMinZ = vertex;

			if (vertex.z > outMaxZ.z)
				outMaxZ = vertex;
		}
	}
	
	static std::vector<DirectX::SimpleMath::Vector3> GetMinkowkiDifference(const std::vector<DirectX::SimpleMath::Vector3>& lhs, const std::vector<DirectX::SimpleMath::Vector3> & rhs)
	{
		std::vector<DirectX::SimpleMath::Vector3> minkowskiDifference;

		for (const DirectX::SimpleMath::Vector3& left : lhs)
		{
			for (const DirectX::SimpleMath::Vector3& right : rhs)
			{
				minkowskiDifference.push_back(left + (-right));
			}
		}

		return minkowskiDifference;
	}

}