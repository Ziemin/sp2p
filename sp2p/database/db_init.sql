create table users(
    id serial primary key not null,
    login char(30) not null unique,
    password char(30) not null
);

create table networks(
    id serial primary key not null,
    name char(30) not null unique,
    public boolean not null default false,
    visible boolean not null default false,
    participable boolean not null default false,
    creator_email varchar(40),
    protocol_name varchar(40),
    unique(user_id, network_id)
);

create table servers(
    id serial primary key not null,
    user_id integer references users(id) not null,
    network_id integer references networks(id) not null,
    ip char(20) not null,
    port integer not null,
    public boolean not null default false,
    ttl integer not null default 0,             -- time from 1970 (Node time) 
    unique(user_id, network_id)             -- after which server is considered dead
);

create table invitations(
    id serial primary key not null,
    user_id integer references users(id) not null,
    network_id integer references networks(id) not null
);

create table publicKey(
    id serial primary key not null,
    user_id integer references users(id) not null,
    network_id integer references networks(id) not null,
    key text not null,
    unique(user_id, network_id)
);

insert into networks (name) values('root_network');