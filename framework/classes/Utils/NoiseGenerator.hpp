# pragma once

/// System includes
# include <cstdlib>

/// Dependencies
# include <glm/glm.hpp>

namespace GE::Utils {
	/**
	 * @brief A simple noise generator for 2D and 3D space.
	 * 
	 * Currently supports :
	 * - 2D Perlin noise
	 * - 3D Perlin noise
	 */
	class	NoiseGenerator {
		public:
			explicit NoiseGenerator(const uint64_t &seed = 0) { setSeed(seed); }
			~NoiseGenerator() = default;

			/// Public methods

			float	perlin2D(const glm::vec2 &v);
			float	perlin3D(const glm::vec3 &v);

			void	setSeed(const uint64_t &seed);

		private:
			uint64_t	_seed;

			inline float	_perlin2DDot(const glm::ivec2 &v1, const glm::vec2 &v2);
			inline float	_perlin2DCubInterpol(const glm::vec2 &v, const float &weight);
			glm::vec2		_perlin2DRandomGradiant(const glm::ivec2 &v);
	};
} // namespace GE::Utils