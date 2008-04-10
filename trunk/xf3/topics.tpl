{{#create}}
<span style="background-color: yellow;">
	{{message}}
</span>
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=t type=text style="width: 100%">
		<tr><th align=left>Message<td><textarea name=m cols=80 rows=20 style="width: 100%"></textarea>
		<tr><th><td><input type=submit value="Post">
	</table>
</form>
{{/create}}
{{#topic}}
<a href="..">Up</a>
{{>posts_table}}
<a href="..">Up</a>
{{/topic}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=t type=text value="{{title:h}}">
		<tr><th align=left>Modified<td>{{mtime}}
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{>topics_table}}
