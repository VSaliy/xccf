{{#index}}
<span style="background-color: yellow;">
	{{message}}
</span>
<form method=post>
	<table>
		<tr><th align=left>Email<td><input name=e type=text size=60 value="{{e:h}}" {{#e_error}}style="background-color: yellow;"{{/e_error}}>
		<tr><th align=left>Password<td><input name=p type=password size=60 {{#p_error}}style="background-color: yellow;"{{/p_error}}>
		<tr><th><td><input type=submit name=update value="Send Email Address Verification Code">
	</table>
</form>
<form method=post>
	<table>
		<tr><th align=left>Email Address Verification Code<td><input name=eavc type=text size=60 {{#eavc_error}}style="background-color: yellow;"{{/eavc_error}}>
		<tr><th><td><input type=submit name=update value="Update">
	</table>
</form>
<form method=post>
	<table>
		<tr><th align=left>Old Password<td><input name=password0 type=password size=60 {{#p0_error}}style="background-color: yellow;"{{/p0_error}}>
		<tr><th align=left>New Password<td><input name=password1 type=password size=60 {{#p1_error}}style="background-color: yellow;"{{/p1_error}}>
		<tr><th align=left>Confirm New Password<td><input name=password2 type=password size=60 {{#p2_error}}style="background-color: yellow;"{{/p2_error}}>
		<tr><th><td><input type=submit name=update value="Update">
	</table>
</form>
{{/index}}