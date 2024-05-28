def quick_sort(arr):
    print(f"Urutkan array: {arr}")
    if len(arr) <= 1:
        print(f"Array berisi 1 atau kurang elemen, kembalikan seperti adanya.")
        return arr
    else:
        pivot = arr[0]
        print(f"Pivot yang dipilih: {pivot}")
        less_than_pivot = [x for x in arr[1:] if x <= pivot]
        print(f"Elemen kurang dari atau sama dengan pivot: {less_than_pivot}")
        greater_than_pivot = [x for x in arr[1:] if x > pivot]
        print(f"Elemen lebih besar dari pivot: {greater_than_pivot}")
        print(f"Menggabungkan {less_than_pivot}, {pivot}, dan {greater_than_pivot}")
        sorted_array = quick_sort(less_than_pivot) + [pivot] + quick_sort(greater_than_pivot)
        print(f"Array yang telah diurutkan: {sorted_array}")
        return sorted_array

arr = [5, 12, 3, 9, 1, 20, 7, 2]
print(quick_sort(arr))