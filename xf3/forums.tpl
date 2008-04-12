{{#create}}
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=title type=text>
		<tr><th><td><input type=submit value="Create">
	</table>
</form>
{{/create}}
{{#forum}}
<a href="..">Up</a> | <a href="_create/">New Topic</a>
{{#can_edit_forum}}
{{BI_SPACE}}| <form action="{{link}}" method=post><input type=hidden name=edit><input type=submit value="Edit"></form>
{{/can_edit_forum}}
{{>topics_table}}
<a href="..">Up</a> | <a href="_create/">New Topic</a>
{{/forum}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=title type=text value="{{title:h}}">
		<tr><th align=left>Description<td><textarea name=description cols=80 rows=10>{{description:p}}</textarea>
		<tr><th align=left>Modified<td>{{mtime}}
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{#forums}}
{{#can_create_forum}}
<a href="_create/">New Forum</a>
{{/can_create_forum}}
<table class=forums_table>
	<tr>
		<th align=left>Title
		<th align=right>Topics
		<th align=right>Posts
		<th align=left>Modified
	{{#row}}
	<tr>
		<td><a href="{{link}}">{{title:h}}</a>
		<td align=right>{{topics_count}}
		<td align=right>{{posts_count}}
		<td>{{mtime}}
	{{/row}}
</table>
{{/forums}}
