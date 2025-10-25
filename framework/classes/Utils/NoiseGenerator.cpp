
#include "NoiseGenerator.hpp"

namespace GE::Utils {

	# pragma region Public methods

	void	NoiseGenerator::setSeed(const uint64_t &seed) {
		srand(seed);
		_seed = 1 + rand();
	}

	# pragma region Perlin Noise

	float	NoiseGenerator::perlin2D(const glm::vec2 &v) {
		const glm::ivec2	topLeft     = { v.x    , v.y     };
		const glm::ivec2	topRight    = { v.x + 1, v.y     };
		const glm::ivec2	bottomLeft  = { v.x    , v.y + 1 };
		const glm::ivec2	bottomRight = { v.x + 1, v.y + 1 };
		const glm::vec2		wVector     = {
			v.x - topLeft.x,
			v.y - topLeft.y
		};

		return (_perlin2DCubInterpol(glm::vec2{
				_perlin2DCubInterpol(glm::vec2{_perlin2DDot(topLeft, v),
							_perlin2DDot(topRight, v)}, wVector.x),
				_perlin2DCubInterpol(glm::vec2{_perlin2DDot(bottomLeft, v),
							_perlin2DDot(bottomRight, v)}, wVector.x)},
			wVector.y));
	}

	float	NoiseGenerator::perlin3D(const glm::vec3 &v) {
		float	ab = perlin2D({v.x, v.y});
		float	bc = perlin2D({v.y, v.z});
		float	ac = perlin2D({v.x, v.z});

		float	ba = perlin2D({v.y, v.x});
		float	cb = perlin2D({v.z, v.y});
		float	ca = perlin2D({v.z, v.x});

		float	abc = ab + bc + ac + ba + cb + ca;
		return (abc / 6.0f);
	}

	inline float	NoiseGenerator::_perlin2DDot(const glm::ivec2 &v1, const glm::vec2 &v2) {
		return (glm::dot(glm::vec2{v2.x - (float)v1.x, v2.y - (float)v1.y}, _perlin2DRandomGradiant(v1)));
	}

	inline float	NoiseGenerator::_perlin2DCubInterpol(const glm::vec2 &v, const float &weight) {
		return ((v.y - v.x) * (3.0f - weight * 2.0f) * weight * weight + v.x);
	}

	glm::vec2	NoiseGenerator::_perlin2DRandomGradiant(const glm::ivec2 &v) {
		const unsigned int	w = 8 * sizeof(unsigned);
		const unsigned int	s = w / 2; 
		unsigned int		a = v.x + _seed, b = v.y + _seed;
		
		a *= 3284157443;
	
		b ^= a << s | a >> (w - s);
		b *= 1911520717;
	
		a ^= b << s | b >> (w - s);
		a *= 2048419325;
		float	random = a * (M_PI / ~(~0u >> 1)); // in [0, 2*Pi]
		
		return  glm::vec2{sin(random), cos(random)};
	}

	# pragma endregion

	# pragma endregion

} // namespace GE::Utils