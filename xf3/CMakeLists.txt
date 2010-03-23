cmake_minimum_required(VERSION 2.4)
set(CMAKE_BUILD_TYPE release)
include_directories(. ../xbt/misc ../xf2/misc)
add_executable(
	xcc_forum
	../xbt/misc/sha1.cpp
	../xbt/misc/sql/database.cpp
	../xbt/misc/sql/sql_query.cpp
	../xbt/misc/sql/sql_result.cpp
	../xbt/misc/virtual_binary.cpp
	../xbt/misc/xcc_z.cpp
	admin_control_panel_request.cpp
	config.cpp
	connection.cpp
	database.cpp
	forums_request.cpp
	groups_request.cpp
	http_cookie.cpp
	posts_request.cpp
	request.cpp
	request_handler.cpp
	server.cpp
	topics_request.cpp
	user_control_panel_request.cpp
	users_request.cpp
	"XCC Forum.cpp"
	xf_misc.cpp
	xf_request.cpp
)
target_link_libraries(xcc_forum boost_program_options-mt boost_system-mt ctemplate mysqlclient)
