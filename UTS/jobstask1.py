def assign_jobs(cost_matrix, people, jobs):
    assignments = []
    for _ in range(len(people)):
        min_cost = min(min(row) for row in cost_matrix)
        for i, row in enumerate(cost_matrix):
            if min_cost in row:
                j = row.index(min_cost)
                break
        assignments.append((people.pop(i), jobs.pop(j), min_cost))
        cost_matrix = [row[:j] + row[j+1:] for row in (cost_matrix[:i] + cost_matrix[i+1:])]
    return assignments

def print_assignments(assignments):
    print("Penugasan menggunakan strategi greedy:")
    for person, job, cost in assignments:
        print(f"{person} -> {job} (biaya: {cost})")
    print(f"Total biaya penugasan: {sum(cost for _, _, cost in assignments)}")

# Matriks biaya
cost_matrix = [[9, 2, 7, 8], [6, 4, 3, 7], [5, 8, 1, 4], [7, 6, 9, 4]]
people = ['a', 'b', 'c', 'd']
jobs = ['Job 1', 'Job 2', 'Job 3', 'Job 4']

assignments = assign_jobs(cost_matrix, people, jobs)
print_assignments(assignments)