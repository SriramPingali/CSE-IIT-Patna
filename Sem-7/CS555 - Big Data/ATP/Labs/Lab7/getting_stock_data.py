# visit yahoo finance to get more info
from pandas_datareader import data as pdr 
from datetime import datetime 
#download data 
ibm = pdr.DataReader('IBM', 'yahoo', start=datetime(2010, 8, 1), end=datetime(2016, 11, 30))  
aapl = pdr.DataReader('AAPL', 'yahoo', start=datetime(2010, 8, 1), end=datetime(2016, 11, 30))  
fb = pdr.DataReader('FB', 'yahoo', start=datetime(2010, 8, 1), end=datetime(2016, 11, 30))  
googl = pdr.DataReader('GOOGL', 'yahoo', start=datetime(2010, 8, 1), end=datetime(2016, 11, 30)) 
 
#print first few lines of data 
print(ibm.head()) 
print(aapl.head()) 
print(fb.head()) 
print(googl.head()) 
 
#export and save as csv files 
ibm.to_csv('IBM_stock.csv', sep=',') 
aapl.to_csv('Apple_stock.csv', sep=',') 
fb.to_csv('Facebook_stock.csv', sep=',') 
googl.to_csv('Google_stock.csv', sep=',') 