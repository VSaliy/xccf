set(CMAKE_BUILD_TYPE release)
include_directories(. ../xbt/misc ../xf2/misc)
add_executable(
	xcc_forum
	../xbt/misc/sha1.cpp
	../xf2/misc/sql/database.cpp
	../xf2/misc/sql/sql_query.cpp
	../xf2/misc/sql/sql_result.cpp
	../xf2/misc/virtual_binary.cpp
	../xf2/misc/xcc_z.cpp
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
	users_request.cpp
	"XCC Forum.cpp"
	xf_misc.cpp
	xf_request.cpp
)
target_link_libraries(xcc_forum boost_program_options-mt ctemplate mysqlclient)
