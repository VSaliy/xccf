{{>pager}}
<table class=posts_table>
	{{#row}}
	<tr id="{{pid}}">
		<th align=left>
			<a href="{{owner_link}}">{{owner:h}}</a>
		<th align=left>
			<a href="#{{pid}}">#{{pid}} {{ctime}}</a>
		<th align=right>
			{{#can_edit_post}}
			<form action="{{link}}" method=post><input type=hidden name=edit><input type=submit value="Edit"></form> |{{BI_SPACE}}
			{{/can_edit_post}}
			<a href="_create/?pid={{pid}}">Reply</a>
	<tr>
		<td>
		<td colspan=9>{{message}}
	{{/row}}
</table>
{{>pager}}
