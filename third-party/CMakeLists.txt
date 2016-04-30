ExternalProject_add(
	libwebsockets

	PREFIX ${DEPS_BUILD_DIR}/lws

	GIT_REPOSITORY "https://github.com/warmcat/libwebsockets.git"
	GIT_TAG "master"

	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	TEST_COMMAND ""

	CMAKE_ARGS
		-DLWS_WITHOUT_CLIENT=ON
		-DLWS_WITHOUT_TESTAPPS=ON
		-DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
)
