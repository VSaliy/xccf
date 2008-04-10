{{#create}}
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=title type=text value="{{title:h}}" style="width: 100%; {{#title_error}}background-color: yellow;{{/title_error}}">
		<tr><th align=left>Message<td><textarea name=body cols=80 rows=20 style="width: 100%; {{#body_error}}background-color: yellow;{{/body_error}}">{{body:p}}</textarea>
		<tr><th><td><input type=submit value="Post">
	</table>
</form>
{{/create}}
{{#topic}}
<a href="..">Up</a>
{{#can_edit_topic}}
{{BI_SPACE}}| <form action="{{link}}" method=post><input type=hidden name=edit><input type=submit value="Edit"></form>
{{/can_edit_topic}}
{{>posts_table}}
<a href="..">Up</a>
{{/topic}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Title<td><input name=title type=text value="{{title:h}}">
		<tr><th align=left>Forum<td><select name=fid>{{#forum}}<option value={{fid}} {{selected}}>{{title:h}}</option>{{/forum}}</select>
		<tr><th align=left>Modified<td>{{mtime}}
		<tr><th align=left>Created<td>{{ctime}}
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{>topics_table}}
