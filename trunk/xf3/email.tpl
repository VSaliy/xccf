To: {{to}}
Subject: {{site_title}} - Email Address Verification
{{BI_NEWLINE}}
{{#users_create}}
Password: {{password}}
{{/users_create}}
{{#users_recover}}
Name: {{name}}
Password: {{password}}
{{/users_recover}}
{{#users_update_email_address}}
Email Address Verification Code: {{password}}
{{/users_update_email_address}}
