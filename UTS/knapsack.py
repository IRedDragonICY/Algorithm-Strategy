from itertools import combinations

# List berat obyek
weights = [5, 12, 15, 18, 20, 25]

# Kapasitas kotak
capacity = 30

# Menyimpan kombinasi terbaik dan beratnya
best_combination = []
best_weight = 0

# Mencari semua kombinasi obyek
for r in range(1, len(weights) + 1):
    for combination in combinations(weights, r):
        weight = sum(combination)
        # Jika berat kombinasi lebih baik dan tidak melebihi kapasitas
        if best_weight < weight <= capacity:
            best_weight = weight
            best_combination = combination

print("Kombinasi terbaik adalah: ", best_combination)
print("Dengan total berat: ", best_weight)
