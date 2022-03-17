/*
Name of Student: M Maheeth Reddy
        Roll No: 1801CS31
*/

-- list existing databases
show databases;

-- create a database with name dblab
create database dblab;

-- use the newly created database
use dblab;

create table Suppliers( SID varchar(10), SName varchar(10), City varchar(10) );
create table Products( PID varchar(10), PName varchar(10), WarrantyPeriod int );
create table SP ( SID varchar(10), PID varchar(10) );

insert into Suppliers
values ('SU01','Philips','Hyderabad'),
('SU02','Hawkins','Kolkata'),
('SU03','Godrej','Chennai'),
('SU04','Samsung','Delhi'),
('SU05','Voltas','Bengaluru'),
('SU06','V-Guard','Mumbai'),
('SU07','Racold','Tirupati'),
('SU08','Symphony','Kanpur'),
('SU09','Prestige','Guwahati'),
('SU10','Lenovo','Bhopal'),
('SU11','DELL','Mangalore');

insert into Products
values ('PD01','TV',5),
('PD02','Stabilizer',1),
('PD03','Fan',5),
('PD04','AC',10),
('PD05','Cooler',1),
('PD06','Oven',3),
('PD07','Grinder',2),
('PD08','Heater',1),
('PD09','Stove',1),
('PD10','Laptop',3),
('PD11','Cooker',7);

insert into SP
values ('SU01','PD01'),
('SU02','PD11'),
('SU03','PD06'),
('SU04','PD01'),
('SU05','PD04'),
('SU06','PD02'),
('SU07','PD08'),
('SU08','PD05'),
('SU09','PD09'),
('SU10','PD10'),
('SU11','PD10'),
('SU04','PD04'),
('SU09','PD11'),
('SU03','PD03'),
('SU09','PD07');

select * from Suppliers;
select * from Products;
select * from SP;

create user 'scot'@'localhost' identified by 'tiger';
