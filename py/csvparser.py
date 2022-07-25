import re
import csv


def get_array_of_cleaned_elements():
    cleaned_elements = []

    with open('../build/CPUprofileRun.txt', 'r') as file:
        data = file.readlines()

    for index, line in enumerate(data):
        if index == 0:
            continue 

        line = line.strip()
        line = re.sub(' +', ' ', line)
        
        elements = line.split(' ')
        if len(elements) == 7:
            elements[5] = f'{elements[5]} {elements[6]}'
            elements.pop()
        cleaned_elements.append(elements) 

    return cleaned_elements


def write_to_csv(cleaned_elements):
    with open("./results/CPUprofileRun.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerows(cleaned_elements)


cleaned_elements = get_array_of_cleaned_elements()
write_to_csv(cleaned_elements)

