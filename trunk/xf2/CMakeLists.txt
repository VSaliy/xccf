set(CMAKE_BUILD_TYPE release)
include_directories(. misc)
add_executable(
	forum.cgi
	misc/sql/database.cpp
	misc/sql/sql_query.cpp
	misc/sql/sql_result.cpp
	misc/cgi.cpp
	misc/cookie.cpp
	misc/html.cpp
	misc/html_template.cpp
	misc/multi_line.cpp
	misc/string_conversion.cpp
	misc/virtual_binary.cpp
	misc/web_tools.cpp
	misc/xcc_z.cpp
	misc/xf2_mm.cpp
	config.cpp
	form_base.cpp
	form_config.cpp
	form_ipa_search.cpp
	form_languages.cpp
	form_layouts.cpp
	form_login.cpp
	form_message.cpp
	form_password.cpp
	form_preferences.cpp
	form_profile.cpp
	form_register.cpp
	form_search.cpp
	form_smilies.cpp
	form_styles.cpp
	forum.cpp
	forum_database.cpp
	forum_global.cpp
)
target_link_libraries (forum.cgi mysqlclient)
