from itertools import permutations

# Matriks produktivitas
prod_matrix = [
    [8, 7, 4, 9, 5],
    [4, 2, 4, 5, 7],
    [9, 5, 8, 7, 8],
    [4, 3, 7, 6, 7],
    [6, 7, 4, 6, 5]
]

# Membuat list dari P1 hingga P5
persons = ['P1', 'P2', 'P3', 'P4', 'P5']
jobs = ['Job1', 'Job2', 'Job3', 'Job4', 'Job5']

# Membuat semua kemungkinan penugasan
assignments = list(permutations(persons))

# Inisialisasi produktivitas maksimum dan penugasan terbaik
max_productivity = 0
best_assignment = None

# Melakukan iterasi untuk setiap kemungkinan penugasan
for assignment in assignments:
    # Menghitung produktivitas untuk penugasan ini
    productivity = sum(prod_matrix[i][persons.index(assignment[i])] for i in range(5))
    
    # Jika produktivitas ini lebih baik dari yang sebelumnya, simpan penugasan ini dan produktivitasnya
    if productivity > max_productivity:
        max_productivity = productivity
        best_assignment = assignment

# Mencetak penugasan terbaik dan produktivitas maksimum
print("Penugasan terbaik adalah:")
for i in range(5):
    print(f'{best_assignment[i]} -> {jobs[i]} (produktivitas: {prod_matrix[i][persons.index(best_assignment[i])]})')
print("Produktivitas maksimum adalah:", max_productivity)
