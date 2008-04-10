<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<link rel=stylesheet href="/xcc.css">
<title>{{title:h}}</title>
<table>
	<tr>
		<td><form action="{{form_action}}"><input name=q type=text> <input type=submit value="Search"></form>
		<td><a href="/forums/">Forums</a>
		<td><a href="/groups/">Groups</a>
		<td><a href="/users/">Users</a>
		{{#logout}}
		{{#can_create_forum}}
		<td><a href="/forums/_create/">Create Forum</a>
		{{/can_create_forum}}
		{{#can_create_group}}
		<td><a href="/groups/_create/">Create Group</a>
		{{/can_create_group}}
		{{#can_create_label}}
		<td><a href="/labels/_create/">Create Label</a>
		{{/can_create_label}}
		{{#can_upload_torrent}}
		<td><a href="/torrents/_create/">Upload Torrent</a>
		{{/can_upload_torrent}}
		{{#is_administrator}}
		<td><a href="/admin_control_panel/">Admin Control Panel</a>
		{{/is_administrator}}
		<td><a href="/control_panel/">Control Panel</a>
		<td><form method=post><input type=hidden name=a value=logout><input type=submit value="Logout"></form>
		{{/logout}}
	{{#login}}
		<td><a href="/users/_create/">Create User</a>
		<td><a href="/users/_recover/">Recover User</a>
	<tr>
		<td colspan=9>
			<form method=post>
				Name: <input name=n type=text value="{{n:h}}" {{#n_error}}style="background-color: yellow;"{{/n_error}}>{{BI_SPACE}}
				Password: <input name=p type=password {{#p_error}}style="background-color: yellow;"{{/p_error}}>{{BI_SPACE}}
				<input type=hidden name=a value=login>
				<input type=submit value="Login">
			</form>
	{{/login}}
</table>
<h1>{{title:h}}</h1>
