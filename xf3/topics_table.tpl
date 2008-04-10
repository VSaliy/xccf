{{>pager}}
<table class=topics_table>
	<tr>
		<th align=left>Title
		<th align=left>Topic Starter
		<th align=right>Replies
		<th align=left>Modified
	{{#row}}
	<tr>
		<td><a href="{{link}}">{{title:h}}</a>
		<td>{{owner}}
		<td align=right>{{reply_count}}
		<td>{{mtime}}
	{{/row}}
</table>
{{>pager}}