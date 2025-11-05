
file = open("CLOPE_Loginom\mushrooms.txt","r")
sp = []
for item in file.readlines():
    item = item.replace('\n','').split(',')
    sp.append(item)
print(sp[:20])