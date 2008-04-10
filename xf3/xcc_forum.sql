create table if not exists xf_config
(
	name varchar(255) not null,
	value varchar(255) not null
);

create table if not exists xf_email_verification
(
	email varchar(255) not null,
	pass binary(20) not null,
	ctime int not null,
	unique (pass)
);

create table if not exists xf_forums
(
	fid int not null auto_increment,
	title varchar(255) not null,
	mtime int not null,
	primary key (fid)
);

create table if not exists xf_groups
(
	gid int not null auto_increment,
	name varchar(255) not null,
	description text not null,
	privileges int not null,
	users_count int not null,
	ctime int not null,
	primary key (gid),
	fulltext (name, description)
);

create table if not exists xf_posts
(
	pid int not null auto_increment,
	tid int not null,
	uid int not null,
	message text not null,
	mtime int not null,
	ctime int not null,
	primary key (pid),
	key (tid),
	key (uid),
	fulltext key (message)
);

create table if not exists xf_sessions
(
	sid int not null auto_increment,
	uid int not null,
	pass binary(20) not null,
	mtime int not null,
	ctime int not null,
	primary key (sid)
);

create table if not exists xf_topics
(
	tid int not null auto_increment,
	fid int not null,
	uid int not null,
	title varchar(255) not null,
	posts_count int not null,
	mtime int not null,
	ctime int not null,
	primary key (tid),
	key (fid),
	key (uid),
	fulltext key (title)
);

create table if not exists xf_users
(
	uid int not null auto_increment,
	gid int not null,
	name varchar(255) not null,
	pass binary(20) not null,
	email varchar(255) not null,
	posts_count int not null,
	mtime int not null,
	ctime int not null,
	fulltext (name),
	unique (name),
	unique (email),
	primary key (uid)
);

drop trigger if exists xf_posts_insert;
drop trigger if exists xf_posts_update;
drop trigger if exists xf_posts_delete;
drop trigger if exists xf_users_insert;
drop trigger if exists xf_users_update;
drop trigger if exists xf_users_delete;

delimiter ;;

create trigger xf_posts_insert after insert on xf_posts for each row
	update xf_topics set posts_count = posts_count + 1 where tid = new.tid;;

create trigger xf_posts_update after update on xf_posts for each row
	begin
		update xf_topics set posts_count = posts_count - 1 where tid = old.tid;
		update xf_topics set posts_count = posts_count + 1, mtime = unix_timestamp() where tid = new.tid;
	end;;

create trigger xf_posts_delete after delete on xf_posts for each row
	update xf_topics set posts_count = posts_count - 1 where tid = old.tid;;

create trigger xf_users_insert after insert on xf_users for each row
	update xf_groups set users_count = users_count + 1 where gid = new.gid;;

create trigger xf_users_update after update on xf_users for each row
	begin
		update xf_groups set users_count = users_count - 1 where gid = old.gid;
		update xf_groups set users_count = users_count + 1 where gid = new.gid;
	end;;

create trigger xf_users_delete after delete on xf_users for each row
	update xf_groups set users_count = users_count - 1 where gid = old.gid;;

delimiter ;

insert into xf_groups (gid, name, privileges, ctime) values
(1, 'Administrators', -1, unix_timestamp()),
(2, 'Guests', 0, unix_timestamp()),
(3, 'Users', 0, unix_timestamp());
