# Data awal
data = [7,10,3,6,2,8]
n = len(data)
print(f"Data awal: {data}")

# Proses sorting
for i in range(n):
    print(f"Iterasi ke-{i+1}")
    for k in range(n-1, i, -1):  
        print(f"k = {k}")
        if data[k] < data[k-1]:
            print(f"{data[k]} < {data[k-1]} TRUE (swap)")
            temp = data[k] 
            data[k] = data[k-1]  
            data[k-1] = temp
        else:
            print(f"{data[k]} < {data[k-1]} FALSE")
        print(f"Data sementara: {data}")
print(f"Data akhir: {data}")  
