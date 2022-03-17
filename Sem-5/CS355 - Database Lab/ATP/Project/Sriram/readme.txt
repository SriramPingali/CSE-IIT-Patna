				CS355 Mini Project
Name: P. V. Sriram
Roll No. : 1801CS37

Project Video Demo link: https://drive.google.com/drive/folders/1aZB8BmIR8W1Kjwc2e6JLtkwWyHEPK3w7?usp=sharing
Above folder link contains: 1) 1801CS37_CS355 Project_demo.mp4 - Main project demo
		     	    2) Extra Features.mp4 - Demo for additional functionalities on database
			    3) 8 csv files, used for tables in the database
********************************************************************************************

Database Description

Tables:
1) Customers (Data of the customers who used the company’s service)

Attributes:
	1) Cust_ID (Unique ID for each customer)(Primary Key)
	2) Cust_Name (Name of the customer)
	3) Phone_No (Contact Details of the customer)
	4) Membership (Member or not)
	5) Amt_Owed (Amount due from the customer)
	6) H_No (House Number of the customer)
	7) Street (Street of the customer)
	8) City (City of the customer)

2) Packages (Data about the packages shipped by the company)

Attributes:
	1) Pkg _ID (Unique ID for each package)(Primary key)
	2) Pkg_desc (Description of the package, to map to a service)
	3) Weight (Weight of the package in kgs)
	4) Mode_of_pymnt (Mode of payment) 

3) Shipments (Data about the past and present shipment)

Attributes:
	1) Pkg_ID (Unique ID of the package)(MUL)
	2) From_cust (Customer ID of the person shipping the package)(MUL)
	3) To_cust (Customer ID of the person recieving the package)(MUL)
	4) Deliv_charge (Ampunt charged to the sender)
	5) DoS (Date of shipment)
	6) DoD (Date of Delivery)
	7) Expt_DoD (Expected Date of delivery)

4) Tracking (Data about shipments throughout the whole process)

Attributes:
	1) Pkg_ID (Unique ID of the package)(MUL)
	2) Location_ID (Unique ID of the place holding the package)(MUL)
	3) Location_type (Warehouse / Truck / Airplane / Truck / Staff)

5) Logistics (Data about all the vehicles for intercity travel used by the company)

Attributes:
	1) ID (Unique ID for each location)(Primary Key)
	2) Type (Warehouse / Truck / Airplane / Truck / Staff)
	3) Contact_No (Phone number of the person of contact)
	4) License (Registeration details for the location)
	5) City (Location of the vehicle)

6) Warehouses (Data about the storage and warehouse facilities of the company)

Attributes:
	1) ID (Unique ID for each warehouse)(Primary Key)
	2) City (City where the warehouse is present)
	3) Contact_No (Phone number for the person of Contact)

7) Staff (Data about the delivery staff who work in the company)

Attributes:
	1) Staff_ID (Unique ID for each delivery agent)(Primary Key)
	2) Staff_Name (Name of the agent)
	3) Phone_No (Contact details of the agent)
	4) City (City where the agent delivers)
	5) Rating (Service rating from customers)
	6) Deliveries (Number of deliveries completed by the agent)

8) Services (Data about the type of services offered by the company)

Attributes:
	1) Pkg_desc (TYpe of service)(Primary Key)
	2) Cost (Base charge of the service)