{{#create}}
<span style="background-color: yellow;">
	{{message}}
</span>
<form method=post>
	<table>
		<tr><th align=left>Name<td><input name=n type=text>
		<tr><th><td><input type=submit value="Create">
	</table>
</form>
{{/create}}
{{#group}}
<table>
	<tr><th align=left>Name<td>{{name:h}}
	<tr><th align=left>Description<td>{{description}}
	<tr><th align=left>Users<td>{{users_count}}
	<tr><th align=left>Created<td>{{ctime}}
	{{#can_edit_group}}
	<tr><th align=left>Privileges<td>{{#privilege}}{{n:h}}<br>{{/privilege}}
	<tr><th><td><form method=post><input type=submit name=edit value="Edit"></form>
	{{/can_edit_group}}
</table>
{{>users_table}}
{{/group}}
{{#edit}}
<form method=post>
	<table>
		<tr><th align=left>Name<td><input name=name type=text value="{{name:h}}">
		<tr><th align=left>Description<td><textarea name=description cols=80 rows=10>{{description:p}}</textarea>
		<tr><th align=left>Created<td>{{ctime}}
		<tr><th align=left>Can Create Labels<td><input type=checkbox name=can_create_labels {{can_create_labels}}>
		<tr><th align=left>Can Download Torrents<td><input type=checkbox name=can_download_torrents {{can_download_torrents}}>
		<tr><th align=left>Can Edit All Labels<td><input type=checkbox name=can_edit_all_labels {{can_edit_all_labels}}>
		<tr><th align=left>Can Edit All Torrents<td><input type=checkbox name=can_edit_all_torrents {{can_edit_all_torrents}}>
		<tr><th align=left>Can Post Torrent Comments<td><input type=checkbox name=can_post_torrent_comments {{can_post_torrent_comments}}>
		<tr><th align=left>Can Upload Torrents<td><input type=checkbox name=can_upload_torrents {{can_upload_torrents}}>
		<tr><th align=left>Can View Site<td><input type=checkbox name=can_view_site {{can_view_site}}>
		<tr><th align=left>Delete<td><input type=checkbox name=delete>
		<tr><th><td><input type=submit name=edit value="Edit">
	</table>
</form>
{{/edit}}
{{#groups}}
<table>
	<tr>
		<th align=left>Name
		<th align=left>Description
		<th align=right>Users
	{{#row}}
	<tr>
		<td><a href="{{link}}">{{name:h}}</a>
		<td>{{description}}
		<td align=right>{{users_count}}
	{{/row}}
</table>
{{/groups}}
