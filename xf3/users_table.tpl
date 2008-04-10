{{count}} users
{{>pager}}
<table>
	<tr>
		<th align=left>Name
		<th align=left>Group
		<th align=left>Modified
		<th align=left>Created
	{{#row}}
	<tr>
		<td><a href="{{link}}">{{name:h}}</a>
		<td><a href="{{glink}}">{{gname:h}}</a>
		<td>{{mtime}}
		<td>{{ctime}}
	{{/row}}
</table>
{{>pager}}
