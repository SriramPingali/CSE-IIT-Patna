/*
    Name of Student: M. Maheeth Reddy
    Roll No.:        1801CS31
*/

-- 1. Find the name of the Suppliers who have supplied more than one Product (in terms of different PIDs)
select Suppliers.SName 
from SP natural join Suppliers 
group by SP.SID 
having count(SP.PID)>1;

-- 2. Find the Supplier ID of the Suppliers who have supplied some specific Product more than five in quantity
select distinct SID from SP where Qty > 5;

-- 3. Find the PID(s) with highest warranty period.
select PID from Products where WarrantyPeriod=(select max(WarrantyPeriod) from Products);

-- 4. Find the name of the suppliers who have not supplied any items.
select SName from Suppliers where SID not in (select SID from SP);

-- 5. Find the total number of suppliers city wise.
select City, count(SName) from Suppliers group by City;

-- 6. Find the city (or cities) with least number of suppliers
select City from (
    select City,count(SName) as 'Suppliers_Count' 
    from Suppliers group by City
) t1 
natural join (
    select min(Suppliers_Count) as 'Suppliers_Count' 
    from (
        select City,count(SName) as 'Suppliers_Count' 
        from Suppliers group by City
    ) b
) t2;

select City from Suppliers group by City
having count(SID) = (
    select min(t1.count1)
    from (
        select City, count(SID) as 'count1'
        from Suppliers group by City
    ) t1
);

select City from Suppliers

-- 7. Check whether the suppliers ‘S001’ and ‘S002’ are from same city or not
select if( (select City=(select City from Suppliers where SID = 'S001') from Suppliers where SID = 'S002'),'true','false') as Same_City;

-- 8. Find the PID of the product which has been supplied atleast 5 in quantity and by atleast 5 number of suppliers.
select PID from SP group by PID having sum(Qty)>=5 and count(SID)>=5;

-- 9. Find the total quantity of each product supplied by supplier ‘S003’
select Products.PName,Qty from SP natural join Products 
where SP.SID='S003';

-- 10. Find out the products with same name but with different warranty periods.
select distinct a.PName from Products a, Products b 
where a.PName=b.PName and a.WarrantyPeriod!=b.WarrantyPeriod;

-- 11. Find out the supplier ID, city, PID and total quantity supplied by each supplier.
select SP.SID, City, SP.PID, SP.Qty from SP natural join Suppliers;

-- 12. Check whether the supplier with name ‘Dharmesh’ has supplied product with product name ‘screw’
select * from (
    select * from SP natural join Products
) as a 
natural join Suppliers 
where SName='Dharmesh' and PName='screw';
