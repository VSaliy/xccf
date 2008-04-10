{{#create}}
<form method=post>
	<table>
		<tr><th align=left>Message<td><textarea name=m cols=80 rows=20>{{message:p}}</textarea>
		<tr><th><td><input type=submit value="Post">
	</table>
</form>
{{/create}}
{{#post}}
<table>
	<tr><th align=left>Message<td>{{message:h}}
	<tr><th align=left>Modified<td>{{mtime}}
	<tr><th align=left>Created<td>{{ctime}}
	{{#can_edit_post}}
	<tr><th><td><form method=post><input type=submit name=edit value="Edit"></form>
	{{/can_edit_post}}
</table>
{{/post}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Message<td><textarea name=m cols=80 rows=20>{{message:p}}</textarea>
		<tr><th align=left>Modified<td>{{mtime}}
		<tr><th align=left>Created<td>{{ctime}}
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{>posts_table}}
