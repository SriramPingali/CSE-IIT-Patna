''' Name: Chandrawanshi Mangesh Shivaji
    Assignment 2 : Polymorphic Virus 
    CS547 : Foundation of Computer Security
    Roll No. : 1801CS16 '''

import os

#signature of virus
Decrypt_Function= "fernet.decrypt"

def detect_virus(path):

    infected_files = []
    filelist = os.listdir(path)
    
    for filename in filelist:
        
        #Extend search in case of a folder
        if os.path.isdir(path+"/"+filename):
            infected_files.extend(detect_virus(path+"/"+filename))

        #If it is a python script, check for possible infection	
        elif filename[-3:] == ".py" and filename!="detect_virus.py":
           
            infected=False  #true if signature exists

            for line in open(path+"/"+filename):
                if Decrypt_Function in line:
                    infected=True

            #If both signatures are present, virus detected    
            if infected==True:
                infected_files.append(path+"/"+filename)

    return infected_files

#Get and Print all infected files
infected_files=detect_virus("./")

if len(infected_files)==0:
    print("VIRUS NOT FOUND!")
else:
    print("VIRUS FOUND!")
    print("INFECTED FILES : ")
    print(infected_files)