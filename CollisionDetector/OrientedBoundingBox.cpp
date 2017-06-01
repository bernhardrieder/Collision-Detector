#include "pch.h"
#include "OrientedBoundingBox.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

OrientedBoundingBox::OrientedBoundingBox()
{
}


OrientedBoundingBox::~OrientedBoundingBox()
{
}

void OrientedBoundingBox::CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points)
{
	Vector3 mu = Vector3::Zero;

	// find the mean point location
	for (const auto& point : points)
	{
		mu += point / static_cast<float>(points.size());
	}

	// build the covariance matrix
	float cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;
	for (const auto& point : points)
	{
		cxx += point.x * point.x - mu.x * mu.x;
		cxy += point.x * point.y - mu.x * mu.y;
		cxz += point.x * point.z - mu.x * mu.z;
		cyy += point.y * point.y - mu.y * mu.y;
		cyz += point.y * point.z - mu.y * mu.z;
		czz += point.z * point.z - mu.z * mu.z;
	}

	// extract the eigenvectors from covariance matrix
	XMVECTOR eigenVectors[3];
	Internal::CalculateEigenVectorsFromCovarianceMatrix(cxx, cyy, czz, cxy, cxz, cyz, &eigenVectors[0], &eigenVectors[1], &eigenVectors[2]);

	m_right = eigenVectors[0];
	m_right.Normalize();
	m_localAxes[0] = &m_right;

	m_up = eigenVectors[1];
	m_up.Normalize();
	m_localAxes[1] = &m_up;

	m_forward = eigenVectors[2];
	m_forward.Normalize();
	m_localAxes[2] = &m_forward;

	//rotation matrix using the eigvenvectors
	Matrix rotation = Matrix(m_right, m_up, m_forward).Transpose();
	Matrix rotationInverse = rotation.Invert();

	// build the bounding box extents in the rotated frame
	Vector3 min = Vector3::TransformNormal(points[0], rotationInverse);
	Vector3 max = min;
	for (Vector3 point : points)
	{
		point = Vector3::TransformNormal(point, rotationInverse);
		min = Vector3::Min(min, point);
		max = Vector3::Max(max, point);
	}

	m_center = Vector3::TransformNormal((max + min) * 0.5, rotation);
	m_extents = (max - min) * 0.5;
}

void OrientedBoundingBox::Transform(OrientedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& translationMatrix, const DirectX::SimpleMath::Matrix& scaleMatrix, const DirectX::SimpleMath::Matrix& rotationMatrix)
{
	outBV.m_extents = Vector3::Transform(m_extents, scaleMatrix);
	outBV.m_center = Vector3::Transform(m_center, scaleMatrix*rotationMatrix*translationMatrix);
	outBV.m_right = Vector3::Transform(m_right, rotationMatrix);
	outBV.m_up = Vector3::Transform(m_up, rotationMatrix);
	outBV.m_forward = Vector3::Transform(m_forward, rotationMatrix);
	outBV.m_localAxes[0] = &outBV.m_right;
	outBV.m_localAxes[1] = &outBV.m_up;
	outBV.m_localAxes[2] = &outBV.m_forward;
}

bool OrientedBoundingBox::Intersect(const OrientedBoundingBox& other) const
{
	// see christer ericson collision detection book page 103
	float ra, rb;
	Matrix R = Matrix::Identity;
	Matrix AbsR = R;

	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R.m[i][j] = m_localAxes[i]->Dot(*(other.m_localAxes[j]));

	// Compute translation vector ts
	Vector3 t = other.m_center - m_center;

	// Bring translation into a’s coordinate frame
	Matrix rotationMatrix = Matrix(m_right, m_up, m_forward).Transpose();
	t = Vector3::TransformNormal(t, rotationMatrix);

	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR.m[i][j] = std::abs(R.m[i][j]) + std::numeric_limits<float>::epsilon();

	const float* const a_extents[3] = {&m_extents.x, &m_extents.y, &m_extents.z};
	const float* const b_extents[3] = {&other.m_extents.x, &other.m_extents.y, &other.m_extents.z};
	const float* const t_arr[3] = {&t.x, &t.y, &t.z};

	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++)
	{
		ra = *a_extents[i];
		rb = other.m_extents.x * AbsR.m[i][0] + other.m_extents.y * AbsR.m[i][1] + other.m_extents.z * AbsR.m[i][2];
		if (std::abs(*t_arr[i]) > ra + rb)
			return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		ra = m_extents.x * AbsR.m[0][i] + m_extents.y * AbsR.m[1][i] + m_extents.z * AbsR.m[2][i];
		rb = *b_extents[i];
		if (std::abs(t.x * R.m[0][i] + t.y * R.m[1][i] + t.z * R.m[2][i]) > ra + rb)
			return false;
	}

	// Test axis L = A0 x B0
	ra = m_extents.y * AbsR.m[2][0] + m_extents.z * AbsR.m[1][0];
	rb = other.m_extents.y * AbsR.m[0][2] + other.m_extents.z * AbsR.m[0][1];
	if (std::abs(t.z * R.m[1][0] - t.y * R.m[2][0]) > ra + rb)
		return false;

	// Test axis L = A0 x B1
	ra = m_extents.y * AbsR.m[2][1] + m_extents.z * AbsR.m[1][1];
	rb = other.m_extents.x * AbsR.m[0][2] + other.m_extents.z * AbsR.m[0][0];
	if (std::abs(t.z * R.m[1][1] - t.y * R.m[2][1]) > ra + rb)
		return false;

	// Test axis L = A0 x B2
	ra = m_extents.y * AbsR.m[2][2] + m_extents.z * AbsR.m[1][2];
	rb = other.m_extents.x * AbsR.m[0][1] + other.m_extents.y * AbsR.m[0][0];
	if (std::abs(t.z * R.m[1][2] - t.y * R.m[2][2]) > ra + rb)
		return false;

	// Test axis L = A1 x B0
	ra = m_extents.x * AbsR.m[2][0] + m_extents.z * AbsR.m[0][0];
	rb = other.m_extents.y * AbsR.m[1][2] + other.m_extents.z * AbsR.m[1][1];
	if (std::abs(t.x * R.m[2][0] - t.z * R.m[0][0]) > ra + rb)
		return false;

	// Test axis L = A1 x B1
	ra = m_extents.x * AbsR.m[2][1] + m_extents.z * AbsR.m[0][1];
	rb = other.m_extents.x * AbsR.m[1][2] + other.m_extents.z * AbsR.m[1][0];
	if (std::abs(t.x * R.m[2][1] - t.z * R.m[0][1]) > ra + rb)
		return false;

	// Test axis L = A1 x B2
	ra = m_extents.x * AbsR.m[2][2] + m_extents.z * AbsR.m[0][2];
	rb = other.m_extents.x * AbsR.m[1][1] + other.m_extents.y * AbsR.m[1][0];
	if (std::abs(t.x * R.m[2][2] - t.z * R.m[0][2]) > ra + rb)
		return false;

	// Test axis L = A2 x B0
	ra = m_extents.x * AbsR.m[1][0] + m_extents.y * AbsR.m[0][0];
	rb = other.m_extents.y * AbsR.m[2][2] + other.m_extents.z * AbsR.m[2][1];
	if (std::abs(t.y * R.m[0][0] - t.x * R.m[1][0]) > ra + rb)
		return false;

	// Test axis L = A2 x B1
	ra = m_extents.x * AbsR.m[1][1] + m_extents.y * AbsR.m[0][1];
	rb = other.m_extents.x * AbsR.m[2][2] + other.m_extents.z * AbsR.m[2][0];
	if (std::abs(t.y * R.m[0][1] - t.x * R.m[1][1]) > ra + rb)
		return false;

	// Test axis L = A2 x B2
	ra = m_extents.x * AbsR.m[1][2] + m_extents.y * AbsR.m[0][2];
	rb = other.m_extents.x * AbsR.m[2][1] + other.m_extents.y * AbsR.m[2][0];
	if (std::abs(t.y * R.m[0][2] - t.x * R.m[1][2]) > ra + rb)
		return false;

	// Since no separating axis is found, the OBBs must be intersecting
	return true;
}

const DirectX::SimpleMath::Vector3& OrientedBoundingBox::GetCenter() const
{
	return m_center;
}

const DirectX::SimpleMath::Vector3& OrientedBoundingBox::GetExtents() const
{
	return m_extents;
}
