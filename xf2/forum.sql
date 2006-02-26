CREATE TABLE xf_config
(
  name varchar(255) NOT NULL,
  value varchar(255) NOT NULL
);

CREATE TABLE xf_guests
(
  aid int NOT NULL auto_increment,
  name varchar(255) NOT NULL,
  PRIMARY KEY (aid),
  KEY name (name)
);

CREATE TABLE xf_languages
(
  lid int NOT NULL auto_increment,
  fname varchar(255) NOT NULL,
  name varchar(255) NOT NULL,
  PRIMARY KEY (lid),
  UNIQUE KEY name (name)
);

CREATE TABLE xf_layouts
(
  lid int NOT NULL auto_increment,
  fname varchar(255) NOT NULL,
  name varchar(255) NOT NULL,
  PRIMARY KEY (lid),
  UNIQUE KEY name (name)
);

CREATE TABLE xf_messages
(
  mid int NOT NULL auto_increment,
  aid int NOT NULL,
  tid int NOT NULL,
  uid int NOT NULL,
  subject varchar(255) NOT NULL,
  body text NOT NULL,
  signature text NOT NULL,
  pid int NOT NULL,
  ipa int NOT NULL,
  ipa1 int NOT NULL,
  flags int NOT NULL,
  type int NOT NULL,
  mtime int NOT NULL,
  ctime int NOT NULL,
  thread_size int NOT NULL,
  PRIMARY KEY (mid),
  KEY aid (aid),
  KEY tid (tid),
  KEY uid (uid),
  KEY mtime (mtime)
);

CREATE TABLE xf_messages_read
(
  uid int NOT NULL,
  mid int NOT NULL,
  ctime int NOT NULL,
  PRIMARY KEY (uid, mid)
);

CREATE TABLE xf_moderators
(
  mid int,
  uid int,
  KEY mid (mid),
  KEY uid (uid)
);

CREATE TABLE xf_smilies
(
  sid int NOT NULL auto_increment,
  fname varchar(255) NOT NULL,
  name varchar(255) NOT NULL,
  PRIMARY KEY (sid),
  UNIQUE KEY name (name)
);

CREATE TABLE xf_strings
(
  lid int NOT NULL,
  i int NOT NULL,
  value varchar(255) NOT NULL,
  PRIMARY KEY (lid,i)
);

CREATE TABLE xf_styles
(
  sid int NOT NULL auto_increment,
  link varchar(255) NOT NULL,
  name varchar(255) NOT NULL,
  PRIMARY KEY (sid),
  UNIQUE KEY name (name)
);

CREATE TABLE xf_templates
(
  lid int NOT NULL,
  i int NOT NULL,
  value blob NOT NULL,
  PRIMARY KEY (lid,i)
);

CREATE TABLE xf_users
(
  uid int NOT NULL auto_increment,
  name varchar(255) NOT NULL,
  password varchar(255) NOT NULL,
  private_mail varchar(255),
  public_mail varchar(255),
  signature text,
  icq_id int,
  link_title varchar(255),
  link varchar(255),
  custom_css varchar(255),
  language int,
  layout int,
  style int,
  field_height int,
  field_length int,
  ipa int NOT NULL,
  ipa1 int NOT NULL,
  rows_per_page int,
  time_offset int,
  flags int,
  mtime int NOT NULL,
  ctime int NOT NULL,
  PRIMARY KEY (uid),
  UNIQUE KEY name (name)
);
