from clopi import Cluster
# Пробуем прочитать файл
file = open("CLOPE_Loginom\\mushrooms.txt","r") #хехе, грибы
Database = []
for item in file.readlines():
    item = item.replace('\n','').split(',')
    Database.append(item)

# Ща посмотрим как это выглядит
C1 = Cluster()
C1.add_transaction(['a','b'])
C1.add_transaction(['a','b','c'])
C1.add_transaction(['a','c','d'])
print(C1.transactions)
print(f'Площадь = {C1.area} \nШирина = {C1.width} \nГрадиент = {C1.gradient}')

C1.visualize_cluster()

C2 = Cluster()
C2.add_transaction(['d','e'])
C2.add_transaction(['d','e','f'])
print(C2.transactions)
print(f'Площадь = {C2.area} \nШирина = {C2.width} \nГрадиент = {C2.gradient}')

C2.visualize_cluster()