global StackData 
global StackPointer
StackData = [0,0,0,0,0,0,0,0,0,0] 
StackPointer = 0


def PrintArray():
 global StackData
 global StackPointer
 print(StackPointer)
 for index in range (0, 10):
    print(StackData[index])

def Push(Data):
   global StackData
   global StackPointer
   if StackPointer == 10:
     return False
   else:
    StackData[StackPointer] = Data
    StackPointer = StackPointer + 1
    return True 
   


for x in range(0, 11):
  TempNumber = int(input("Enter a number"))
  if Push(TempNumber) == True:
    print("Stored")
  else:
    print("Stack full")
PrintArray()   

def Pop():
 global StackData
 global StackPointer
 if StackPointer == 0:
   return -1
 else:
   ReturnData = StackData[StackPointer - 1]
   StackPointer = StackPointer - 1
   return ReturnData 
 
Pop()
 
for index in range (0, 10):
    print(StackData[index])
    

