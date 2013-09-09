create table users(
    id serial primary key not null,
    login char(30) not null unique,
    password char(30) not null
);

create table networks(
    id serial primary key not null,
    owner_id integer references users(id) ON DELETE CASCADE not null,
    name char(30) not null unique,
    public boolean not null default false,
    visible boolean not null default false,
    participable boolean not null default false,
    creator_email varchar(40),
    protocol_name varchar(40)
);

create table servers(
    id serial primary key not null,
    user_id integer references users(id) ON DELETE CASCADE not null,
    network_id integer references networks(id)  ON DELETE CASCADE not null,
    ip char(20) not null,
    port integer not null,
    ttl integer not null default 0, -- time from 1970 (Node time)
    unique(user_id, network_id) -- after which server is considered dead
);

create table invitations(
    id serial primary key not null,
    user_id integer references users(id) ON DELETE CASCADE not null,
    network_id integer references networks(id) ON DELETE CASCADE not null 
);

create table publicKey(
    id serial primary key not null,
    user_id integer references users(id) ON DELETE CASCADE not null,
    network_id integer references networks(id) ON DELETE CASCADE not null,
    key text not null,
    unique(user_id, network_id)
);



CREATE OR REPLACE FUNCTION clean(T integer)
    returns void as
$$
declare
    curr integer;
begin
    curr = extract(epoch FROM now())::int; 
    DELETE FROM servers WHERE ttl < curr - T;
end;
$$
language plpgsql;
