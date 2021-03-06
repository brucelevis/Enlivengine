#include <Enlivengine/Core/CameraComponent.hpp>

#include <Enlivengine/Core/TransformComponent.hpp>

namespace en
{

CameraComponent::CameraComponent()
	: Camera()
	, mEntity()
{
}

CameraComponent::~CameraComponent()
{
}

CameraComponent::CameraComponent(CameraComponent&& other) noexcept
	: Camera()
	, mEntity(other.mEntity)
{
	other.mEntity = Entity();
}

CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept
{
	mEntity = other.mEntity;
	other.mEntity = Entity();
	return *this;
}

void CameraComponent::Apply() const
{
	Camera::Apply();
	if (GetProjection() == Camera::ProjectionMode::Perspective)
	{
		bgfx::setViewTransform(GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
	else
	{
		bgfx::setViewTransform(GetViewID(), GetViewMatrix().GetData(), GetProjectionMatrix().GetData());
	}
}

Frustum CameraComponent::CreateFrustum() const
{
	return Camera::CreateFrustum().Transform(GetEntityMatrix());
}

const Matrix4f& CameraComponent::GetViewMatrix() const
{
	// TODO : Only if EntityTransformDirty
	if (true)
	{
		const Matrix4f& entityMatrix = GetEntityMatrix();
		const Vector3f position = entityMatrix.TransformPoint(GetPosition());
		const Vector3f direction = entityMatrix.TransformDirection(GetRotation().GetForward());
		mViewMatrix = Matrix4f::LookAt(position, position + direction, ENLIVE_DEFAULT_UP, ENLIVE_DEFAULT_HANDEDNESS);
	}
	return mViewMatrix;
}

Entity CameraComponent::GetEntity() const
{
	return mEntity;
}

World* CameraComponent::GetWorld()
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

const World* CameraComponent::GetWorld() const
{
	if (mEntity.IsValid())
	{
		return &mEntity.GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void CameraComponent::InitializeView(const Vector3f& position, const Matrix3f& rotation)
{
	Camera::InitializeView(position, rotation);
}

void CameraComponent::SetPosition(const Vector3f& position)
{
	Camera::SetPosition(position);
}

const Vector3f& CameraComponent::GetPosition() const
{
	return Camera::GetPosition();
}

void CameraComponent::Move(const Vector3f& movement)
{
	Camera::Move(movement);
}

void CameraComponent::SetRotation(const Matrix3f& rotation)
{
	Camera::SetRotation(rotation);
}

const Matrix3f& CameraComponent::GetRotation() const
{
	return Camera::GetRotation();
}

void CameraComponent::Rotate(const Matrix3f& rotation)
{
	Camera::Rotate(rotation);
}

Matrix4f CameraComponent::GetEntityMatrix() const
{
	if (mEntity.IsValid() && mEntity.Has<TransformComponent>())
	{
		return mEntity.Get<TransformComponent>().GetGlobalMatrix();
	}
	else
	{
		return Matrix4f::Identity();
	}
}

} // namespace en