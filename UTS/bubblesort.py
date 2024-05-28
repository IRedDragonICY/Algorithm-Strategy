def bubble_sort(arr):
    n = len(arr)    
    for i in range(n-1):
        print(f"Iterasi ke-{i+1}")
        swapped = False
        for j in range(n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]
                swapped = True
                print(f"j = {j+1}: {arr}")
            print(f"j = {j+1}: {arr} (Tidak ada pertukaran)")
        if not swapped:
            break
    
    return arr

# Contoh penggunaan
arr = [7, 10, 16, 13, 4, 12, 3, 81, 75, 26]
print("Array awal:", arr)
sorted_arr = bubble_sort(arr)
print("Array terurut:", sorted_arr)