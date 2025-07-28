#include "math.h"

// Modern constants with better precision
constexpr float PI_RAD = 57.295779513082f;
constexpr float PI = 3.14159265358979323846f;
constexpr float PI_2 = 1.57079632679489661923f;
constexpr float DEG2RAD_MULTIPLIER = PI / 180.0f;
constexpr float RAD2DEG_MULTIPLIER = 180.0f / PI;

namespace big
{
	// Enhanced math functions with bounds checking and safety
	float radians_to_degrees(float radians)
	{
		// Add bounds checking for extreme values
		if (!std::isfinite(radians))
		{
			return 0.0f;
		}
		
		// Normalize radians to prevent overflow
		while (radians > PI) radians -= 2.0f * PI;
		while (radians < -PI) radians += 2.0f * PI;
		
		return radians * RAD2DEG_MULTIPLIER;
	}

	float degrees_to_radians(float degrees)
	{
		// Add bounds checking for extreme values
		if (!std::isfinite(degrees))
		{
			return 0.0f;
		}
		
		// Normalize degrees to prevent overflow
		while (degrees > 360.0f) degrees -= 360.0f;
		while (degrees < -360.0f) degrees += 360.0f;
		
		return degrees * DEG2RAD_MULTIPLIER;
	}

	float get_fov_radius(float fov_degrees, float screen_width, float screen_height)
	{
		// Enhanced bounds checking and validation
		if (!std::isfinite(fov_degrees) || !std::isfinite(screen_width) || !std::isfinite(screen_height))
		{
			return 0.0f;
		}
		
		// Clamp FOV to reasonable range
		fov_degrees = std::clamp(fov_degrees, 1.0f, 179.0f);
		
		// Prevent division by zero
		if (screen_height <= 0.0f)
		{
			return 0.0f;
		}
		
		float aspect_ratio = screen_width / screen_height;
		float fov_radians = degrees_to_radians(fov_degrees);
		
		// Prevent invalid tan calculation
		if (fov_radians >= PI)
		{
			return screen_height;
		}
		
		return (screen_height / 2.0f) * tan(fov_radians / 2.0f);
	}

	float get_abs_delta_at_given_points(const Vector2& src, const Vector2& dst)
	{
		// Enhanced bounds checking
		if (!std::isfinite(src.x) || !std::isfinite(src.y) || 
			!std::isfinite(dst.x) || !std::isfinite(dst.y))
		{
			return 0.0f;
		}
		
		Vector2 out = { src.x - dst.x, src.y - dst.y };
		return fabsf(out.Length());
	}

	void normalize_angle(Vector2& angle)
	{
		// Enhanced angle normalization with bounds checking
		if (!std::isfinite(angle.x) || !std::isfinite(angle.y))
		{
			angle = Vector2(0.0f, 0.0f);
			return;
		}
		
		// Normalize X component
		while (angle.x <= -PI)
			angle.x += 2.0f * PI;
		while (angle.x > PI)
			angle.x -= 2.0f * PI;
			
		// Normalize Y component with bounds checking
		while (angle.y <= -PI_2)
			angle.y += PI;
		while (angle.y > PI_2)
			angle.y -= PI;
			
		// Final bounds check
		angle.x = std::clamp(angle.x, -PI, PI);
		angle.y = std::clamp(angle.y, -PI_2, PI_2);
	}

	void cerp_angle(Vector2& from, Vector2& to, float step_x, float step_y)
	{
		// Enhanced cubic interpolation with bounds checking
		if (!std::isfinite(step_x) || !std::isfinite(step_y))
		{
			return;
		}
		
		// Clamp steps to valid range
		step_x = std::clamp(step_x, 0.0f, 1.0f);
		step_y = std::clamp(step_y, 0.0f, 1.0f);
		
		float CubicStepX = (1.0f - cos(step_x * PI)) / 2.0f;
		float CubicStepY = (1.0f - cos(step_y * PI)) / 2.0f;

		Vector2 Delta = (to - from);
		normalize_angle(Delta);

		to.x = (from.x + CubicStepX * Delta.x);
		to.y = (from.y + CubicStepY * Delta.y);

		normalize_angle(to);
	}

	Vector2 clamp_magnitude(const Vector2& v, float maxLength)
	{
		// Enhanced magnitude clamping with bounds checking
		if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(maxLength))
		{
			return Vector2(0.0f, 0.0f);
		}
		
		if (maxLength <= 0.0f)
		{
			return Vector2(0.0f, 0.0f);
		}
		
		float sqrMagnitude = v.x * v.x + v.y * v.y;
		
		// Check for overflow
		if (!std::isfinite(sqrMagnitude))
		{
			return Vector2(0.0f, 0.0f);
		}
		
		if (sqrMagnitude > maxLength * maxLength)
		{
			float scale = maxLength / sqrt(sqrMagnitude);
			return Vector2(v.x * scale, v.y * scale);
		}
		return v;
	}

	float vector_dot(const Vector3& v1, const Vector3& v2)
	{
		// Enhanced dot product with bounds checking
		if (!std::isfinite(v1.x) || !std::isfinite(v1.y) || !std::isfinite(v1.z) ||
			!std::isfinite(v2.x) || !std::isfinite(v2.y) || !std::isfinite(v2.z))
		{
			return 0.0f;
		}
		
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector2 get_screen_size()
	{
		// Enhanced screen size retrieval with null pointer protection
		const auto dx_renderer = DxRenderer::GetInstance();
		if (!dx_renderer)
		{
			LOG(WARNING) << "DxRenderer instance is null";
			return Vector2(0.0f, 0.0f);
		}

		const auto screen = dx_renderer->m_pScreen;
		if (!screen)
		{
			LOG(WARNING) << "Screen instance is null";
			return Vector2(0.0f, 0.0f);
		}

		// Validate screen dimensions
		if (screen->m_Width <= 0 || screen->m_Height <= 0)
		{
			LOG(WARNING) << "Invalid screen dimensions: " << screen->m_Width << "x" << screen->m_Height;
			return Vector2(0.0f, 0.0f);
		}

		return Vector2(static_cast<float>(screen->m_Width), static_cast<float>(screen->m_Height));
	}

	// Enhanced matrix to quaternion conversion with bounds checking
	Quaternion matrix_to_quaternion(const Matrix& matrix)
	{
		Quaternion result = {0.0f, 0.0f, 0.0f, 1.0f};

		// Validate matrix elements
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (!std::isfinite(matrix.m[i][j]))
				{
					LOG(WARNING) << "Invalid matrix element detected";
					return result;
				}
			}
		}

		// Extract the diagonal elements
		float trace = matrix._11 + matrix._22 + matrix._33;

		if (trace > 0.0f)
		{
			float S = sqrt(trace + 1.0f) * 2.0f;
			result.w = 0.25f * S;
			result.x = (matrix._23 - matrix._32) / S;
			result.y = (matrix._31 - matrix._13) / S;
			result.z = (matrix._12 - matrix._21) / S;
		}
		else if ((matrix._11 > matrix._22) && (matrix._11 > matrix._33))
		{
			float S = sqrt(1.0f + matrix._11 - matrix._22 - matrix._33) * 2.0f;
			result.w = (matrix._23 - matrix._32) / S;
			result.x = 0.25f * S;
			result.y = (matrix._12 + matrix._21) / S;
			result.z = (matrix._31 + matrix._13) / S;
		}
		else if (matrix._22 > matrix._33)
		{
			float S = sqrt(1.0f + matrix._22 - matrix._11 - matrix._33) * 2.0f;
			result.w = (matrix._31 - matrix._13) / S;
			result.x = (matrix._12 + matrix._21) / S;
			result.y = 0.25f * S;
			result.z = (matrix._23 + matrix._32) / S;
		}
		else
		{
			float S = sqrt(1.0f + matrix._33 - matrix._11 - matrix._22) * 2.0f;
			result.w = (matrix._12 - matrix._21) / S;
			result.x = (matrix._31 + matrix._13) / S;
			result.y = (matrix._23 + matrix._32) / S;
			result.z = 0.25f * S;
		}

		return result;
	}
}