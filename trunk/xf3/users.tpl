{{#create}}
<span style="background-color: yellow;">
	{{message}}
</span>
<form method=post>
	<table>
		<tr><th align=left>Email<td><input name=e type=text value="{{e:h}}" {{#e_error}}style="background-color: yellow;"{{/e_error}}>
		<tr><th><td><input type=submit value="Send Password">
	</table>
</form>
<form method=post>
	<table>
		<tr><th align=left>Name<td><input name=n type=text value="{{n:h}}" {{#n_error}}style="background-color: yellow;"{{/n_error}}>
		<tr><th align=left>Password<td><input name=p type=password {{#p_error}}style="background-color: yellow;"{{/p_error}}>
		<tr><th><td><input type=submit value="Create">
	</table>
</form>
{{/create}}
{{#recover}}
<span style="background-color: yellow;">
	{{message}}
</span>
<form method=post>
	<table>
		<tr><th align=left>Email<td><input name=e type=text value="{{e:h}}" {{#e_error}}style="background-color: yellow;"{{/e_error}}>
		<tr><th><td><input type=submit value="Send Password">
	</table>
</form>
{{/recover}}
{{#user}}
<table>
	<tr><th align=left>Name<td>{{name:h}}
	<tr><th align=left>Group<td><a href="{{glink}}">{{gname:h}}</a>
	<tr><th align=left>Posts<td>{{posts_count}}
	<tr><th align=left>Modified<td>{{mtime}}
	<tr><th align=left>Created<td>{{ctime}}
	{{#can_edit_user}}
	<tr><th><td><form method=post><input type=submit name=edit value="Edit"></form>
	{{/can_edit_user}}
</table>
{{>topics_table}}
{{/user}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Name<td><input name=name type=text value="{{name:h}}">
		<tr><th align=left>Group<td><select name=gid>{{#group}}<option value={{gid}} {{selected}}>{{name:h}}</option>{{/group}}</select>
		<tr><th align=left>Posts<td>{{posts_count}}
		<tr><th align=left>Modified<td>{{mtime}}
		<tr><th align=left>Created<td>{{ctime}}
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{>users_table}}
