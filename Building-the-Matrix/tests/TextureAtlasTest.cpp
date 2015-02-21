#include "../src/Common.hpp"

#ifdef TEST

#include <Catch/catch.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <utility>


#include "../src/TextureAtlas.hpp"

TEST_CASE("Testing Texture Atlas", "[texture atlas]"){
	TextureAtlas textureAtlas("resources//textures//texture_atlas.png");
	SECTION("Testing grass") {
		std::tuple<float, float, float, float> result = textureAtlas.calculateTextureCoords("grass");
		REQUIRE(std::get<0>(result) == 0.0f);
		REQUIRE(std::get<1>(result) == 0.0f);
	}
	SECTION("Testing sand") {
		std::tuple<float, float, float, float> result = textureAtlas.calculateTextureCoords("sand");
	}
}
#endif 