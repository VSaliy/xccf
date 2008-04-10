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
