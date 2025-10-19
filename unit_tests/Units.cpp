// unit_tests/test_Camera.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include <spdlog/spdlog.h>

#include "Camera.hpp"

#define DEBUG_LEVEL spdlog::level::debug

using Catch::Approx;

// Helper for approximate comparison of mat4
bool mat4Equals(const glm::mat4& a, const glm::mat4& b, float epsilon = 1e-5f) {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (fabs(a[i][j] - b[i][j]) > epsilon)
				return false;
	return true;
}

TEST_CASE("Camera functionality", "[camera]") {
	spdlog::set_level(DEBUG_LEVEL); // or spdlog::level::debug

	SECTION("Initial state") {
		Camera cam;
		spdlog::info("Camera initial position: {}, {}, {}", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
		spdlog::info("Camera initial front: {}, {}, {}", cam.getFront().x, cam.getFront().y, cam.getFront().z);

		glm::mat4 expectedView = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 4), glm::vec3(0, 1, 0));
		spdlog::debug("Expected view matrix: ");
		for (int i = 0; i < 4; i++) spdlog::debug("{}, {}, {}, {}", expectedView[i][0], expectedView[i][1], expectedView[i][2], expectedView[i][3]);

		REQUIRE(mat4Equals(cam.getView(), expectedView));

		glm::mat4 expectedProj = glm::perspective(glm::radians(cam.fovDeg), 800.0f / 600.0f, cam.nearPlane, cam.farPlane);
		REQUIRE(mat4Equals(cam.getProjection(), expectedProj));
	}

	SECTION("Move updates position and view matrix") {
		Camera cam;
		cam.setPosition({ 0,0,0 });
		spdlog::info("Camera position before move: {}, {}, {}", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);

		cam.Move({ 1,2,3 });
		spdlog::info("Camera position after move: {}, {}, {}", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);

		const glm::mat4 view = cam.getView();
		spdlog::debug("Camera view matrix after move:");
		for (int i = 0; i < 4; i++)
			spdlog::debug("{}, {}, {}, {}", view[i][0], view[i][1], view[i][2], view[i][3]);

		glm::vec3 expectedPos = { 1,2,3 };
		REQUIRE(view[3][0] == Catch::Approx(-expectedPos.x));
		REQUIRE(view[3][1] == Catch::Approx(-expectedPos.y));
		REQUIRE(-view[3][2] == Catch::Approx(-expectedPos.z)); // Since front is {0,0,-1}
	}

	SECTION("Rotate updates front vector") {
		Camera cam;
		cam.setDirection({ 0,0,-1 }, { 0,1,0 });
		spdlog::info("Front before rotation: {}, {}, {}", cam.getFront().x, cam.getFront().y, cam.getFront().z);

		cam.Rotate({ 0, 90, 0 });
		spdlog::info("Front after rotation: {}, {}, {}", cam.getFront().x, cam.getFront().y, cam.getFront().z);

		glm::vec3 expectedFront = glm::normalize(glm::vec3(1, 0, 0));
		const glm::vec3& front = cam.getFront();
		REQUIRE(front.x == Catch::Approx(expectedFront.x).epsilon(0.001f));
		REQUIRE(front.y == Catch::Approx(expectedFront.y).epsilon(0.001f));
		REQUIRE(front.z == Catch::Approx(expectedFront.z).epsilon(0.001f));
	}

	SECTION("lookAtTarget sets front correctly") {
		Camera cam;
		cam.setPosition({ 0,0,0 });
		cam.lookAtTarget({ 0,0,-1 });

		spdlog::info("Front after lookAtTarget: {}, {}, {}", cam.getFront().x, cam.getFront().y, cam.getFront().z);

		glm::mat4 expectedView = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), { 0,1,0 });
		REQUIRE(mat4Equals(cam.getView(), expectedView));
	}

	SECTION("Projection updates correctly") {
		Camera cam;
		cam.setPerspective(60.0f, 0.1f, 100.0f);
		cam.setViewportSize(800, 600);

		spdlog::info("Perspective fov: {}, near: {}, far: {}", cam.fovDeg, cam.nearPlane, cam.farPlane);

		glm::mat4 expectedProj = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		REQUIRE(mat4Equals(cam.getProjection(), expectedProj));
	}

	SECTION("setDirection updates front, right and up vectors") {
		Camera cam;
		cam.setDirection({ 0,0,-1 }, { 0,1,0 });

		spdlog::info("Front: {}, {}, {}", cam.getFront().x, cam.getFront().y, cam.getFront().z);
		spdlog::info("Right: {}, {}, {}", cam.getRight().x, cam.getRight().y, cam.getRight().z);
		spdlog::info("Up: {}, {}, {}", cam.getUp().x, cam.getUp().y, cam.getUp().z);

		glm::vec3 expectedFront = glm::normalize(glm::vec3(0, 0, -1));
		REQUIRE(cam.getFront().x == Catch::Approx(expectedFront.x));
		REQUIRE(cam.getFront().y == Catch::Approx(expectedFront.y));
		REQUIRE(cam.getFront().z == Catch::Approx(expectedFront.z));

		glm::vec3 expectedRight = glm::normalize(glm::cross(expectedFront, glm::vec3(0, 1, 0)));
		glm::vec3 expectedUp = glm::normalize(glm::cross(expectedRight, expectedFront));

		REQUIRE(cam.getRight().x == Catch::Approx(expectedRight.x));
		REQUIRE(cam.getRight().y == Catch::Approx(expectedRight.y));
		REQUIRE(cam.getRight().z == Catch::Approx(expectedRight.z));

		REQUIRE(cam.getUp().x == Catch::Approx(expectedUp.x));
		REQUIRE(cam.getUp().y == Catch::Approx(expectedUp.y));
		REQUIRE(cam.getUp().z == Catch::Approx(expectedUp.z));
	}
}

